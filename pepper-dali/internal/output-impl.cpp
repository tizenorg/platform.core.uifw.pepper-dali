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
#include <pepper-dali/internal/output-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/compositor-impl.h>

// EXTERNAL INCLUDES
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
Integration::Log::Filter* gPepperOutputLogging  = Integration::Log::Filter::New( Debug::Verbose, true, "LOG_PEPPER_OUTPUT" );
#endif

static void OnDestroy( void* data )
{
  Output* output = static_cast< Output* >( data );

  output->OnDestroy();
}

static int32_t OnGetSubpixelOrder( void* data )
{
  return WL_OUTPUT_SUBPIXEL_UNKNOWN;
}

static const char* OnGetMakerName( void* data )
{
  return "PePPer DALi";
}

static const char* OnGetModelName( void* data )
{
  return "PePPer DALi";
}

static int OnGetModeCount( void* data )
{
  return 1;
}

static void OnGetMode( void* data, int index, pepper_output_mode_t* mode )
{
  Output* output = static_cast< Output* >( data );

  if( index != 0 )
  {
    DALI_LOG_INFO( gPepperOutputLogging, Debug::General, "OnGetMode: wrong index [%d]\n", index );
    return;
  }

  if( mode )
  {
    Vector2 size = output->GetSize();

    mode->flags = WL_OUTPUT_MODE_CURRENT | WL_OUTPUT_MODE_PREFERRED;
    mode->w = (int32_t)size.width;
    mode->h = (int32_t)size.height;
    mode->refresh = 60000;
  }
}

static pepper_bool_t OnSetMode( void* data, const pepper_output_mode_t* mode )
{
  return PEPPER_FALSE;
}

static void OnAssignPlanes( void* data, const pepper_list_t* viewList )
{
  Output* output = static_cast< Output* >( data );

  output->OnAssignPlanes( viewList );
}

static void OnStartRepaintLoop( void* data )
{
  Output* output = static_cast< Output* >( data );

  output->OnStartRepaintLoop();
}

static void OnRepaint( void* data, const pepper_list_t* planeList )
{
  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnRepaint\n" );

  // TODO: repaint
}

static void OnAttachSurface( void* data, pepper_surface_t* surface, int* width, int* height )
{
  Output* output = static_cast< Output* >( data );

  output->OnAttachSurface( surface, width, height );
}

static void OnFlushSurface( void* data, pepper_surface_t* surface, pepper_bool_t* keepBuffer )
{
}

static const struct pepper_output_backend outputInterface =
{
   OnDestroy,

   OnGetSubpixelOrder,
   OnGetMakerName,
   OnGetModelName,

   OnGetModeCount,
   OnGetMode,
   OnSetMode,

   OnAssignPlanes,
   OnStartRepaintLoop,
   OnRepaint,
   OnAttachSurface,
   OnFlushSurface
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
  mPrimaryPlane( NULL )
{
}

Output::~Output()
{
}

void Output::Initialize( Pepper::Compositor& compositor, Application application, Pepper::Input input )
{
  mCompositor = compositor;
  mInput = input;

  // TODO: use application info
  mSize.width = 720.0f;
  mSize.height = 1280.0f;

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

  // TODO: render post?

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::Initialize: success\n" );
}

Vector2 Output::GetSize()
{
  return mSize;
}

void Output::OnResize( Application& application )
{
  // TODO
  // Can't get a new size!
}

void Output::OnDestroy()
{
  if( mPrimaryPlane )
  {
    pepper_plane_destroy( mPrimaryPlane );
    mPrimaryPlane = NULL;
  }
}

void Output::OnAssignPlanes( const pepper_list_t* viewList )
{
  pepper_list_t* list;

  pepper_list_for_each_list( list, viewList )
  {
    pepper_view_t* view = static_cast< pepper_view_t* >( list->item );
    pepper_view_assign_plane( view, mOutput, mPrimaryPlane );
  }
}

void Output::OnStartRepaintLoop()
{
  struct timespec ts;

  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnStartRepaintLoop\n" );

  pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ), &ts );
  pepper_output_finish_frame( mOutput, &ts );
}

void Output::OnAttachSurface( pepper_surface_t* surface, int* width, int* height )
{
  DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnAttachSurface: surface = %p\n", surface );

  long index = reinterpret_cast< long >( pepper_object_get_user_data( (pepper_object_t*)surface, this ) );

  if( index == 0 )
  {
    Pepper::Object object = Pepper::Object::New( surface, mInput );

    object.AddedSignal().Connect( this, &Output::OnObjectAdded );

    index = mObjectList.size() + 1;
    mObjectList.push_back( object );

    pepper_object_set_user_data( (pepper_object_t*)surface, this, reinterpret_cast< void* >( index ), NULL );

    DALI_LOG_INFO( gPepperOutputLogging, Debug::Verbose, "Output::OnAttachSurface: object index = %d\n", index );

    if( !object.AttchBuffer( width, height ) )
    {
      width = 0;
      height = 0;
    }
  }
  else
  {
    Pepper::Object object = mObjectList[index - 1];

    if( !object.AttchBuffer( width, height ) )
    {
      width = 0;
      height = 0;
    }
  }

  // TODO: temp
  struct timespec ts;

  pepper_compositor_get_time( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ), &ts );
  pepper_output_finish_frame( mOutput, &ts );
}

Pepper::Output::OutputSignalType& Output::ObjectAddedSignal()
{
  return mObjectAddedSignal;
}

Pepper::Output::OutputSignalType& Output::ObjectDeletedSignal()
{
  return mObjectDeletedSignal;
}

void Output::OnObjectAdded( Pepper::Object object, Toolkit::ImageView imageView )
{
  Pepper::Output handle( this );
  mObjectAddedSignal.Emit( handle, imageView );
}

void Output::OnObjectDeleted( Pepper::Object object, Toolkit::ImageView imageView )
{
  Pepper::Output handle( this );
  mObjectDeletedSignal.Emit( handle, imageView );

  for ( std::vector< Pepper::Object >::iterator iter = mObjectList.begin(); mObjectList.end() != iter; ++iter )
  {
    if( *iter == object )
    {
      mObjectList.erase( iter );
    }
  }
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
