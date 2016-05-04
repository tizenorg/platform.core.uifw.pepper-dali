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
#include <pepper-dali/internal/output-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/compositor-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/adaptors/adaptor.h>
#include <dali/devel-api/adaptor-framework/render-surface.h>
#include <dali/integration-api/debug.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperOutputLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_OUTPUT" );
#endif

static const struct pepper_output_backend outputInterface =
{
  Output::OnDestroy,

  Output::OnGetSubpixelOrder,
  Output::OnGetMakerName,
  Output::OnGetModelName,

  Output::OnGetModeCount,
  Output::OnGetMode,
  Output::OnSetMode,

  Output::OnAssignPlanes,
  Output::OnStartRepaintLoop,
  Output::OnRepaint,
  Output::OnAttachSurface,
  Output::OnFlushSurface
};

} // unnamed namespace

OutputPtr Output::New( Pepper::Compositor compositor, Application application, Pepper::Input input )
{
  OutputPtr impl = new Output();

  // Second-phase init of the implementation
  impl->Initialize( compositor, application, input );

  return impl;
}

Output::Output()
: mSize(),
  mOutput( NULL ),
  mPrimaryPlane( NULL ),
  mRepaintRequest( false )
{
}

Output::~Output()
{
}

void Output::Initialize( Pepper::Compositor& compositor, Application application, Pepper::Input input )
{
  mCompositor = compositor;
  mInput = input;

  RenderSurface& surface = Adaptor::Get().GetSurface();
  PositionSize positionSize = surface.GetPositionSize();

  mSize.width = positionSize.width;
  mSize.height = positionSize.height;

  mOutput = pepper_compositor_add_output( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ), &outputInterface, "dali", this, WL_OUTPUT_TRANSFORM_NORMAL, 1 );
  if( !mOutput )
  {
    DALI_LOG_INFO( gPepperOutputLogging, Debug::General, "Output::Initialize: pepper_compositor_add_output is failed\n" );
    return;
  }

  mPrimaryPlane = pepper_output_add_plane( mOutput, NULL );
  if( !mPrimaryPlane )
  {
    DALI_LOG_INFO( gPepperOutputLogging, Debug::General, "Output::Initialize: pepper_output_add_plane is failed\n" );
    pepper_output_destroy( mOutput );
    return;
  }

  application.ResizeSignal().Connect( this, &Output::OnResize );

  // Set the thread-synchronization interface on the render-surface
  surface.SetThreadSynchronization( *this );

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::Initialize: success [width = %.2f height = %.2f]\n", mSize.width, mSize.height );
}

void Output::OnDestroy( void* data )
{
  Output* output = static_cast< Output* >( data );

  if( output->mPrimaryPlane )
  {
    pepper_plane_destroy( output->mPrimaryPlane );
    output->mPrimaryPlane = NULL;
  }
}

int32_t Output::OnGetSubpixelOrder( void* data )
{
  return WL_OUTPUT_SUBPIXEL_UNKNOWN;
}

const char* Output::OnGetMakerName( void* data )
{
  return "PePPer DALi";
}

const char* Output::OnGetModelName( void* data )
{
  return "PePPer DALi";
}

int Output::OnGetModeCount( void* data )
{
  return 1;
}

void Output::OnGetMode( void* data, int index, pepper_output_mode_t* mode )
{
  Output* output = static_cast< Output* >( data );

  if( index != 0 )
  {
    DALI_LOG_INFO( gPepperOutputLogging, Debug::General, "Output::OnGetMode: wrong index [%d]\n", index );
    return;
  }

  if( mode )
  {
    mode->flags = WL_OUTPUT_MODE_CURRENT | WL_OUTPUT_MODE_PREFERRED;
    mode->w = (int32_t)output->mSize.width;
    mode->h = (int32_t)output->mSize.height;
    mode->refresh = 60000;
  }
}

pepper_bool_t Output::OnSetMode( void* data, const pepper_output_mode_t* mode )
{
  return PEPPER_FALSE;
}

void Output::OnAssignPlanes( void* data, const pepper_list_t* viewList )
{
  Output* output = static_cast< Output* >( data );
  pepper_list_t* list;

  pepper_list_for_each_list( list, viewList )
  {
    pepper_view_t* view = static_cast< pepper_view_t* >( list->item );
    pepper_view_assign_plane( view, output->mOutput, output->mPrimaryPlane );
  }
}

void Output::OnStartRepaintLoop( void* data )
{
  Output* output = static_cast< Output* >( data );
  struct timespec ts;

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnStartRepaintLoop\n" );

  pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( output->mCompositor ).GetCompositorHandle() ), &ts );
  pepper_output_finish_frame( output->mOutput, &ts );
}

void Output::OnRepaint( void* data, const pepper_list_t* planeList )
{
  Output* output = static_cast< Output* >( data );

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnRepaint\n" );

  output->mRepaintRequest = true;

  // TODO: temp
  if( !output->mRenderFinishTimer )
  {
    output->mRenderFinishTimer= Timer::New(1);
    output->mRenderFinishTimer.TickSignal().Connect( output, &Output::OnRenderFinishTimerTick );
  }

  output->mRenderFinishTimer.Start();
}

void Output::OnAttachSurface( void* data, pepper_surface_t* surface, int* width, int* height )
{
  Output* output = static_cast< Output* >( data );
  Pepper::Object object;

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnAttachSurface: surface = %p\n", surface );

  long index = reinterpret_cast< long >( pepper_object_get_user_data( (pepper_object_t*)surface, output ) );
  if( index == 0 )
  {
    object = Pepper::Object::New( surface, output->mInput );

    object.ObjectViewAddedSignal().Connect( output, &Output::OnObjectViewAdded );
    object.ObjectViewDeletedSignal().Connect( output, &Output::OnObjectViewDeleted );

    index = output->mObjectList.size() + 1;
    output->mObjectList.push_back( object );

    pepper_object_set_user_data( (pepper_object_t*)surface, output, reinterpret_cast< void* >( index ), NULL );

    DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnAttachSurface: object index = %d\n", index );
  }
  else
  {
    object = output->mObjectList[index - 1];
  }

  if( !object.AttchBuffer( width, height ) )
  {
    *width = 0;
    *height = 0;
  }

  // TODO: temp
  struct timespec ts;

  pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( output->mCompositor ).GetCompositorHandle() ), &ts );
  pepper_output_finish_frame( output->mOutput, &ts );
}

void Output::OnFlushSurface( void* data, pepper_surface_t* surface, pepper_bool_t* keepBuffer )
{
}

Pepper::Output::OutputSignalType& Output::ObjectViewAddedSignal()
{
  return mObjectViewAddedSignal;
}

Pepper::Output::OutputSignalType& Output::ObjectViewDeletedSignal()
{
  return mObjectViewDeletedSignal;
}

void Output::OnResize( Application& application )
{
  RenderSurface& surface = Adaptor::Get().GetSurface();
  PositionSize positionSize = surface.GetPositionSize();

  mSize.width = positionSize.width;
  mSize.height = positionSize.height;

  pepper_output_update_mode( mOutput );
}

void Output::OnObjectViewAdded( Pepper::Object object, Pepper::ObjectView objectView )
{
  Pepper::Output handle( this );
  mObjectViewAddedSignal.Emit( handle, objectView );
}

void Output::OnObjectViewDeleted( Pepper::Object object, Pepper::ObjectView objectView )
{
  Pepper::Output handle( this );
  mObjectViewDeletedSignal.Emit( handle, objectView );

  for ( std::vector< Pepper::Object >::iterator iter = mObjectList.begin(); mObjectList.end() != iter; ++iter )
  {
    if( *iter == object )
    {
      mObjectList.erase( iter );
      break;
    }
  }
}

void Output::PostRenderComplete()
{
  if( mRepaintRequest )
  {
    struct timespec ts;

    DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::PostRenderComplete" );

    pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ), &ts );
    pepper_output_finish_frame( mOutput, &ts );

    mRepaintRequest = false;
  }
}

void Output::PostRenderStarted()
{
  // Do nothing
}

void Output::PostRenderWaitForCompletion()
{
  // Do nothing
}

// TODO: temp
bool Output::OnRenderFinishTimerTick()
{
  struct timespec ts;

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnRenderFinishTimerTick\n" );

  pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ), &ts );
  pepper_output_finish_frame( mOutput, &ts );

  return false;
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
