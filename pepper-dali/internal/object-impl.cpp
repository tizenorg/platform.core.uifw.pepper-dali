/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// CLASS HEADER
#include <pepper-dali/internal/object-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/images/buffer-image.h>
#include <dali/public-api/events/touch-event.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperObjectLogging  = Integration::Log::Filter::New( Debug::NoLogging, false, "LOG_PEPPER_OBJECT" );
#endif

} // unnamed namespace

ObjectPtr Object::New( pepper_surface_t* surface, Pepper::Input input )
{
  ObjectPtr impl = new Object();

  // Second-phase init of the implementation
  impl->Initialize( surface, input );

  return impl;
}

Object::Object()
: mSurface( NULL ),
  mBuffer( NULL ),
  mPointer( NULL ),
  mKeyboard( NULL ),
  mTouch( NULL ),
  mShmBuffer( NULL ),
  mSurfaceDestroyListener( NULL ),
  mBufferDestroyListener( NULL ),
  mWidth( 0 ),
  mHeight( 0 )
{
}

Object::~Object()
{
}

void Object::Initialize( pepper_surface_t* surface, Pepper::Input input )
{
  mSurface = surface;

  mPointer = input.GetPointer();
  mKeyboard = input.GetKeyboard();
  mTouch = input.GetTouch();

  mSurfaceDestroyListener = pepper_object_add_event_listener( (pepper_object_t*)surface, PEPPER_EVENT_OBJECT_DESTROY, 0, OnDestroySurface, this );

  DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::Initialize: success\n" );
}

bool Object::AttchBuffer( int* width, int* height )
{
  pepper_buffer_t* buffer;
  wl_resource* bufferResource;
  wl_shm_buffer* shmBuffer;
  int bufWidth, bufHeight;

  buffer = pepper_surface_get_buffer( mSurface );
  if( !buffer )
  {
    DALI_LOG_INFO( gPepperObjectLogging, Debug::General, "Object::AttchBuffer: pepper_surface_get_buffer is failed\n" );
    return false;
  }

  if( mBuffer )
  {
    pepper_buffer_unreference( mBuffer );
    pepper_event_listener_remove( mBufferDestroyListener );
  }

  pepper_buffer_reference( buffer );

  mBuffer = buffer;
  mBufferDestroyListener = pepper_object_add_event_listener( (pepper_object_t*)buffer, PEPPER_EVENT_OBJECT_DESTROY, 0, OnDestroyBuffer, this );

  bufferResource = pepper_buffer_get_resource( buffer );

  shmBuffer = wl_shm_buffer_get( bufferResource );
  if( !shmBuffer )
  {
    DALI_LOG_INFO( gPepperObjectLogging, Debug::General, "Object::AttchBuffer: wl_shm_buffer_get is failed\n" );
    return false;
  }

  bufWidth = wl_shm_buffer_get_width( shmBuffer );
  bufHeight = wl_shm_buffer_get_height( shmBuffer );

  if( !mImageView )
  {
    mImageView = Toolkit::ImageView::New();

    mImageView.TouchedSignal().Connect( this, &Object::OnTouched );

    Pepper::Object handle( this );
    mAddedSignal.Emit( handle, mImageView );

    // TODO: temp
    mImageView.SetName("Client object");
  }

//  if( shmBuffer != mShmBuffer )
  {
    // create a new image
    PixelBuffer* pixelBuffer = static_cast< PixelBuffer* >( wl_shm_buffer_get_data( shmBuffer ) );
    mBufferImage = BufferImage::New( pixelBuffer, bufWidth, bufHeight );

    mImageView.SetImage( mBufferImage );

    mShmBuffer = shmBuffer;
  }

  if( ( mWidth != bufWidth ) || ( mHeight != bufHeight ) )
  {
    mWidth = bufWidth;
    mHeight = bufHeight;

    // resize actor
    mImageView.SetSize( mWidth, mHeight );
  }

  if( width )
  {
    *width = bufWidth;
  }

  if( height )
  {
    *height = bufHeight;
  }

  DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::AttchBuffer: this = %p, width = %d, height = %d", this, bufWidth, bufHeight );

  return true;
}

Pepper::Object::ObjectSignalType& Object::AddedSignal()
{
  return mAddedSignal;
}

Pepper::Object::ObjectSignalType& Object::DeletedSignal()
{
  return mDeletedSignal;
}

bool Object::OnTouched( Actor actor, const TouchEvent& touchEvent )
{
  const TouchPoint& point = touchEvent.GetPoint(0);

  Pepper::Internal::ShellPtr shell = reinterpret_cast< Pepper::Internal::Shell* >( pepper_object_get_user_data( reinterpret_cast< pepper_object_t* >( mSurface ), pepper_surface_get_role( mSurface ) ) );
  if( !shell )
  {
    DALI_LOG_INFO( gPepperObjectLogging, Debug::General, "Object::OnTouched: shell is null\n" );
    return false;
  }

  switch( point.state )
  {
    case TouchPoint::Down:
    {
      pepper_touch_add_point( mTouch, point.deviceId, point.local.x, point.local.y );
      pepper_touch_send_down( mTouch, shell->GetView(), touchEvent.time, point.deviceId, point.local.x, point.local.y );

      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Down (%.2f, %2.f) device = %d\n", point.local.x, point.local.y, point.deviceId );
      return true;
    }
    case TouchPoint::Up:
    {
      pepper_touch_send_up( mTouch, shell->GetView(), touchEvent.time, point.deviceId );
      pepper_touch_remove_point( mTouch, point.deviceId );

      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Up (%.2f, %2.f)\n", point.local.x, point.local.y );
      return true;
    }
    case TouchPoint::Motion:
    {
      pepper_touch_send_motion( mTouch, shell->GetView(), touchEvent.time, point.deviceId, point.local.x, point.local.y );

//      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Motion (%.2f, %2.f)\n", point.local.x, point.local.y );
      return true;
    }
    default:
    {
      return false;
    }
  }

  return false;
}

void Object::OnDestroySurface( pepper_event_listener_t* listener, pepper_object_t* pepperObject, uint32_t id, void* info, void* data )
{
  Object* object = static_cast< Object* >( data );

  Pepper::Object handle( object );
  object->mDeletedSignal.Emit( handle, object->mImageView );

  object->mSurface = NULL;

  if( object->mSurfaceDestroyListener )
  {
    pepper_event_listener_remove( object->mSurfaceDestroyListener );
  }
}

void Object::OnDestroyBuffer( pepper_event_listener_t* listener, pepper_object_t* pepperObject, uint32_t id, void* info, void* data )
{
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
