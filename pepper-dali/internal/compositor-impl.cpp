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
Integration::Log::Filter* gPepperCompositorLogging  = Integration::Log::Filter::New( Debug::Verbose, true, "LOG_PEPPER_COMPOSITOR" );
#endif

} // unnamed namespace

CompositorPtr Compositor::New( Application application, const std::string& name )
{
  CompositorPtr impl = new Compositor();

  // Second-phase init of the implementation
  impl->Initialize( application, name );

  return impl;
}

Compositor::Compositor()
: mCompositor( NULL ),
  mDisplay( NULL ),
  mEventLoop( NULL ),
  mFdHandler( NULL ),
  mShell( NULL ),
  mInput( NULL ),
  mOutput( NULL ),
  mSocketName()
{
}

Compositor::~Compositor()
{
  if( mCompositor )
  {
    pepper_compositor_destroy( mCompositor );
  }
}

void* Compositor::GetCompositorHandle()
{
  return static_cast< void* >( mCompositor );
}

void Compositor::Initialize( Application application, const std::string& name )
{
  // create compositor
  mCompositor = pepper_compositor_create( name.c_str() );
//  mCompositor = pepper_compositor_create( NULL );

  // create shell
  mShell = Pepper::Shell::New( Pepper::Compositor( this ) );

  // create input
  mInput = Pepper::Input::New( Pepper::Compositor( this ) );

  mSocketName = pepper_compositor_get_socket_name( mCompositor );
  mDisplay = pepper_compositor_get_display( mCompositor );

  mEventLoop = wl_display_get_event_loop( mDisplay );

  int loopFd = wl_event_loop_get_fd( mEventLoop );

  mFdHandler = ecore_main_fd_handler_add( loopFd, (Ecore_Fd_Handler_Flags)(ECORE_FD_READ | ECORE_FD_ERROR), FdReadCallback, this, NULL, NULL );

  ecore_main_fd_handler_prepare_callback_set( mFdHandler, FdPrepareCallback, this );

  // create output
  mOutput = Pepper::Output::New( Pepper::Compositor( this ), application, mInput );

  mOutput.ObjectAddedSignal().Connect( this, &Compositor::OnObjectAdded );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::Initialize: success. socket name = %s\n", mSocketName.c_str() );

  // TODO: temp
  setenv("WAYLAND_DISPLAY", mSocketName.c_str(), 1);
}

Eina_Bool Compositor::FdReadCallback( void* data, Ecore_Fd_Handler* handler )
{
  Compositor* compositor = static_cast< Compositor* >( data );

  wl_event_loop_dispatch( compositor->mEventLoop, 0 );

  return ECORE_CALLBACK_RENEW;
}

void Compositor::FdPrepareCallback( void* data, Ecore_Fd_Handler* handler )
{
  Compositor* compositor = static_cast< Compositor* >( data );

  wl_display_flush_clients( compositor->mDisplay );
}

Pepper::Compositor::CompositorSignalType& Compositor::ObjectAddedSignal()
{
  return mObjectAddedSignal;
}

Pepper::Compositor::CompositorSignalType& Compositor::ObjectDeletedSignal()
{
  return mObjectDeletedSignal;
}

void Compositor::OnObjectAdded( Pepper::Output output, Toolkit::ImageView imageView )
{
  Pepper::Compositor handle( this );
  mObjectAddedSignal.Emit( handle, imageView );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::ObjectAddedSignal: Object is added!\n" );
}

void Compositor::OnObjectDeleted( Pepper::Output output, Toolkit::ImageView imageView )
{
  Pepper::Compositor handle( this );
  mObjectDeletedSignal.Emit( handle, imageView );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::ObjectDeletedSignal: Object is deleted!\n" );
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
