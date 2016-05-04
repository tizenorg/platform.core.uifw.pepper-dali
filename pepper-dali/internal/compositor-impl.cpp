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
#include <pepper-dali/internal/compositor-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <wayland-server.h>
#include <tizen-extension-client-protocol.h>
#include <Ecore_Wayland.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperCompositorLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_COMPOSITOR" );
#endif

static void EmbeddedCompositorGetSocket( void* data, struct tizen_embedded_compositor* embeddedCompositor, int fd )
{
  int* socketFd = (int*)data;

  *socketFd = fd;
  return;
}

static const struct tizen_embedded_compositor_listener tizenEmbeddedCompositorListener =
{
  EmbeddedCompositorGetSocket
};

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
  mTbmServer( NULL ),
  mShell( NULL ),
  mInput( NULL ),
  mOutput( NULL ),
  mSocketName()
{
}

Compositor::~Compositor()
{
  if( mTbmServer )
  {
    wayland_tbm_server_deinit( mTbmServer );
  }

  if( mCompositor )
  {
    pepper_compositor_destroy( mCompositor );
  }

  if( mFdHandler )
  {
    ecore_main_fd_handler_del( mFdHandler );
  }
}

const std::string& Compositor::GetName() const
{
  return mSocketName;
}

void* Compositor::GetCompositorHandle()
{
  return static_cast< void* >( mCompositor );
}

void Compositor::Initialize( Application application, const std::string& name )
{
  // Get socket fd from server
  int socketFd = GetSocketFdFromServer();
  if( socketFd == -1 )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: socket fd is invalid.\n" );
    return;
  }

  // create compositor
  mCompositor = pepper_compositor_create_fd( name.c_str(), socketFd );
  if( !mCompositor )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: pepper_compositor_create_fd is failed. [%d]\n", socketFd );
    return;
  }

  mDisplay = pepper_compositor_get_display( mCompositor );

  // create shell
  mShell = Pepper::Shell::New( Pepper::Compositor( this ) );
  if( !mShell )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: Fail to create shell.\n" );
    pepper_compositor_destroy( mCompositor );
    mCompositor = NULL;
    return;
  }

  mTbmServer = wayland_tbm_server_init( mDisplay, NULL, -1, 0 );
  if( !mTbmServer )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: wayland_tbm_server_init is failed.\n" );
    pepper_compositor_destroy( mCompositor );
    mCompositor = NULL;
    return;
  }

  // create input
  mInput = Pepper::Input::New( Pepper::Compositor( this ) );
  if( !mInput )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: Fail to create input.\n" );
    wayland_tbm_server_deinit( mTbmServer );
    mTbmServer = NULL;
    pepper_compositor_destroy( mCompositor );
    mCompositor = NULL;
    return;
  }

  mSocketName = pepper_compositor_get_socket_name( mCompositor );

  mEventLoop = wl_display_get_event_loop( mDisplay );

  int loopFd = wl_event_loop_get_fd( mEventLoop );

  mFdHandler = ecore_main_fd_handler_add( loopFd, (Ecore_Fd_Handler_Flags)(ECORE_FD_READ | ECORE_FD_ERROR), Compositor::FdReadCallback, this, NULL, NULL );

  ecore_main_fd_handler_prepare_callback_set( mFdHandler, Compositor::FdPrepareCallback, this );

  // create output
  mOutput = Pepper::Output::New( Pepper::Compositor( this ), application, mInput );
  if( !mOutput )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::Initialize: Fail to create output.\n" );

    ecore_main_fd_handler_del( mFdHandler );
    mFdHandler = NULL;
    wayland_tbm_server_deinit( mTbmServer );
    mTbmServer = NULL;
    pepper_compositor_destroy( mCompositor );
    mCompositor = NULL;
    return;
  }

  mOutput.ObjectViewAddedSignal().Connect( this, &Compositor::OnObjectViewAdded );
  mOutput.ObjectViewDeletedSignal().Connect( this, &Compositor::OnObjectViewDeleted );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::Initialize: success. socket name = %s\n", mSocketName.c_str() );
}

int Compositor::GetSocketFdFromServer()
{
  Eina_Inlist* list;
  Eina_Inlist* tmp;
  Ecore_Wl_Global* global;
  struct tizen_embedded_compositor* embeddedCompositor = NULL;
  int fd = -1;

  list = ecore_wl_globals_get();
  if( !list )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::GetSocketFdFromServer: ecore_wl_globals_get returns NULL.\n" );
    return -1;
  }

  EINA_INLIST_FOREACH_SAFE( list, tmp, global )
  {
    if( !strcmp( global->interface, "tizen_embedded_compositor" ) )
    {
      embeddedCompositor = static_cast< struct tizen_embedded_compositor* >( wl_registry_bind( ecore_wl_registry_get(), global->id, &tizen_embedded_compositor_interface, 1 ) );

      tizen_embedded_compositor_add_listener( embeddedCompositor, &tizenEmbeddedCompositorListener, &fd );
      break;
    }
  }

  if( !embeddedCompositor )
  {
    DALI_LOG_INFO( gPepperCompositorLogging, Debug::General, "Compositor::GetSocketFdFromServer: tizen_embedded_compositor is not supported.\n" );
    return -1;
  }

  tizen_embedded_compositor_get_socket( embeddedCompositor );
  ecore_wl_sync();

  tizen_embedded_compositor_destroy( embeddedCompositor );

  return fd;
}

Pepper::Compositor::CompositorSignalType& Compositor::ObjectViewAddedSignal()
{
  return mObjectViewAddedSignal;
}

Pepper::Compositor::CompositorSignalType& Compositor::ObjectViewDeletedSignal()
{
  return mObjectViewDeletedSignal;
}

void Compositor::OnObjectViewAdded( Pepper::Output output, Pepper::ObjectView objectView )
{
  Pepper::Compositor handle( this );
  mObjectViewAddedSignal.Emit( handle, objectView );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::ObjectViewAddedSignal: ObjectView is added!\n" );
}

void Compositor::OnObjectViewDeleted( Pepper::Output output, Pepper::ObjectView objectView )
{
  Pepper::Compositor handle( this );
  mObjectViewDeletedSignal.Emit( handle, objectView );

  DALI_LOG_INFO( gPepperCompositorLogging, Debug::Verbose, "Compositor::ObjectViewDeletedSignal: ObjectView is deleted!\n" );
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

} // namespace Internal

} // namespace Pepper

} // namespace Dali
