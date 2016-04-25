/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
#include <pepper-dali/internal/shell-client-impl.h>
#include <pepper-dali/internal/object-view-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/images/buffer-image.h>
#include <dali/public-api/events/touch-event.h>
#include <dali/public-api/adaptor-framework/native-image-source.h>
#include <wayland-tbm-server.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperObjectLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_OBJECT" );
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
  mTbmSurface( 0 ),
  mSurfaceDestroyListener( NULL ),
  mBufferDestroyListener( NULL ),
  mWidth( 0 ),
  mHeight( 0 )
{
}

Object::~Object()
{
  if( mBuffer )
  {
    pepper_buffer_unreference( mBuffer );
    pepper_event_listener_remove( mBufferDestroyListener );
  }

  if( mSurfaceDestroyListener )
  {
    pepper_event_listener_remove( mSurfaceDestroyListener );
  }
}

void Object::Initialize( pepper_surface_t* surface, Pepper::Input input )
{
  mSurface = surface;

  mPointer = input.GetPointer();
  mKeyboard = input.GetKeyboard();
  mTouch = input.GetTouch();

  mSurfaceDestroyListener = pepper_object_add_event_listener( (pepper_object_t*)surface, PEPPER_EVENT_OBJECT_DESTROY, 0, Object::OnDestroySurface, this );

  DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::Initialize: success\n" );
}

bool Object::AttchBuffer( int* width, int* height )
{
  pepper_buffer_t* buffer;
  wl_resource* bufferResource;
  wl_shm_buffer* shmBuffer = NULL;
  tbm_surface_h tbmSurface = NULL;
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
  mBufferDestroyListener = pepper_object_add_event_listener( (pepper_object_t*)buffer, PEPPER_EVENT_OBJECT_DESTROY, 0, Object::OnDestroyBuffer, this );

  bufferResource = pepper_buffer_get_resource( buffer );

  if( !mObjectView )
  {
    mObjectView = Pepper::ObjectView::New();

    mObjectView.TouchedSignal().Connect( this, &Object::OnTouched );

    // Set surface
    Pepper::GetImplementation( mObjectView ).SetSurface( mSurface );

    // TODO: support multi touch, focus in/out, mouse in/out

    // TODO: resize callback

    Pepper::Object handle( this );
    mObjectViewAddedSignal.Emit( handle, mObjectView );
  }

  shmBuffer = wl_shm_buffer_get( bufferResource );
  if( shmBuffer )
  {
    bufWidth = wl_shm_buffer_get_width( shmBuffer );
    bufHeight = wl_shm_buffer_get_height( shmBuffer );

    if( shmBuffer != mShmBuffer )
    {
      // create a new image
      PixelBuffer* pixelBuffer = static_cast< PixelBuffer* >( wl_shm_buffer_get_data( shmBuffer ) );
      mBufferImage = BufferImage::New( pixelBuffer, bufWidth, bufHeight );

      mObjectView.SetImage( mBufferImage );

      mShmBuffer = shmBuffer;
    }
  }
  else
  {
    tbmSurface = wayland_tbm_server_get_surface( NULL, bufferResource );
    if( tbmSurface )
    {
      bufWidth = tbm_surface_get_width( tbmSurface );
      bufHeight = tbm_surface_get_height( tbmSurface );

      if ( tbmSurface != mTbmSurface )
      {
        // create a new image
        NativeImageSourcePtr nativeImageSource = NativeImageSource::New( tbmSurface );
        mNativeImage = NativeImage::New( *nativeImageSource );

        mObjectView.SetImage( mNativeImage );

        mTbmSurface = tbmSurface;
      }
    }
    else
    {
      DALI_LOG_INFO( gPepperObjectLogging, Debug::General, "Object::AttchBuffer: Failed to get buffer\n" );
      return false;
    }
  }

  if( ( mWidth != bufWidth ) || ( mHeight != bufHeight ) )
  {
    mWidth = bufWidth;
    mHeight = bufHeight;

    // resize actor
    mObjectView.SetSize( mWidth, mHeight );
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

Pepper::Object::ObjectSignalType& Object::ObjectViewAddedSignal()
{
  return mObjectViewAddedSignal;
}

Pepper::Object::ObjectSignalType& Object::ObjectViewDeletedSignal()
{
  return mObjectViewDeletedSignal;
}

bool Object::OnTouched( Actor actor, const TouchEvent& touchEvent )
{
  const TouchPoint& point = touchEvent.GetPoint(0);

  Pepper::Internal::ShellClientPtr shellClient = reinterpret_cast< Pepper::Internal::ShellClient* >( pepper_object_get_user_data( reinterpret_cast< pepper_object_t* >( mSurface ), pepper_surface_get_role( mSurface ) ) );
  if( !shellClient )
  {
    DALI_LOG_INFO( gPepperObjectLogging, Debug::General, "Object::OnTouched: shellClient is null. mSurface = %p\n", mSurface );
    return false;
  }

  switch( point.state )
  {
    case TouchPoint::Down:
    {
      pepper_touch_add_point( mTouch, point.deviceId, point.local.x, point.local.y );
      pepper_touch_send_down( mTouch, shellClient->GetView(), touchEvent.time, point.deviceId, point.local.x, point.local.y );

      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Down (%.2f, %.2f) device = %d surface = %p\n", point.local.x, point.local.y, point.deviceId, mSurface );
      return true;
    }
    case TouchPoint::Up:
    {
      pepper_touch_send_up( mTouch, shellClient->GetView(), touchEvent.time, point.deviceId );
      pepper_touch_remove_point( mTouch, point.deviceId );

      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Up (%.2f, %.2f) surface = %p\n", point.local.x, point.local.y, mSurface );
      return true;
    }
    case TouchPoint::Motion:
    {
      pepper_touch_send_motion( mTouch, shellClient->GetView(), touchEvent.time, point.deviceId, point.local.x, point.local.y );

//      DALI_LOG_INFO( gPepperObjectLogging, Debug::Verbose, "Object::OnTouched: Touch Motion (%.2f, %.2f)\n", point.local.x, point.local.y );
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
  object->mObjectViewDeletedSignal.Emit( handle, object->mObjectView );

  object->mSurface = NULL;
  object->mTbmSurface = NULL;

  if( object->mSurfaceDestroyListener )
  {
    pepper_event_listener_remove( object->mSurfaceDestroyListener );
  }
}

void Object::OnDestroyBuffer( pepper_event_listener_t* listener, pepper_object_t* pepperObject, uint32_t id, void* info, void* data )
{
  Object* object = static_cast< Object* >( data );

  if( object->mBuffer )
  {
    pepper_buffer_unreference( object->mBuffer );
    pepper_event_listener_remove( object->mBufferDestroyListener );
  }

#if 0
  if( object->mShmBuffer )
  {
    void* shmData = wl_shm_buffer_get_data( object->mShmBuffer );

    object->mBufferImage = BufferImage::New( object->mWidth, object->mHeight );

    PixelBuffer* buffer = object->mBufferImage.GetBuffer();
    memcpy(buffer, shmData, object->mWidth * object->mHeight * 4);

    object->mBufferImage.Update();

    object->mObjectView.SetImage( object->mBufferImage );
  }
  else if( object->mTbmSurface )
  {
  }
#endif
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
