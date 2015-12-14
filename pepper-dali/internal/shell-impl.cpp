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
#include <pepper-dali/internal/shell-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/compositor-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <xdg-shell-server-protocol.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperShellLogging  = Integration::Log::Filter::New( Debug::NoLogging, false, "LOG_PEPPER_SHELL" );
#endif

static void XdgShellDestroy( struct wl_client* client, struct wl_resource* resource)
{
}

static void XdgShellUseUnstableVersion( struct wl_client* client, struct wl_resource* resource, int32_t version )
{
}

static void XdgShellGetSurface( struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* surfaceResource )
{
  Shell* shell = static_cast< Shell* >( wl_resource_get_user_data( resource ) );

  shell->GetSurface( client, id, surfaceResource );
}

static void XdgShellGetPopup( struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* surfaceResource,
                              struct wl_resource* parentResource, struct wl_resource* seatResource, uint32_t serial, int32_t x, int32_t y )
{
}

static void XdgShellPong( struct wl_client* client, struct wl_resource* resource, uint32_t serial )
{
}

static const struct xdg_shell_interface xdgImpl = {
  XdgShellDestroy,
  XdgShellUseUnstableVersion,
  XdgShellGetSurface,
  XdgShellGetPopup,
  XdgShellPong
};

static void XdgSurfaceDestroy( struct wl_client* client, struct wl_resource* resource )
{
  wl_resource_destroy( resource );
}

static void XdgSurfaceSetParent( struct wl_client* client, struct wl_resource* resource, struct wl_resource* parentResource )
{
}

static void XdgSurfaceSetTitle( struct wl_client* client, struct wl_resource* resource, const char* title )
{
  Shell* shell = static_cast< Shell* >( wl_resource_get_user_data( resource ) );

  shell->SetTitle( title );
}

static void XdgSurfaceSetAppId( struct wl_client* client, struct wl_resource* resource, const char* appId )
{
}

static void XdgSurfaceShowWindowMenu( struct wl_client* client, struct wl_resource* surfaceResource, struct wl_resource* seatResource, uint32_t serial, int32_t x, int32_t y )
{
}

static void XdgSurfaceMove( struct wl_client* client, struct wl_resource* resource, struct wl_resource* seatResource, uint32_t serial )
{
}

static void XdgSurfaceResize( struct wl_client* client, struct wl_resource* resource, struct wl_resource* seatResource, uint32_t serial, uint32_t edges )
{
}

static void XdgSurfaceAckConfigure( struct wl_client* client, struct wl_resource* resource, uint32_t serial )
{
  Shell* shell = static_cast< Shell* >( wl_resource_get_user_data( resource ) );

  shell->AckConfigure();
}

static void XdgSurfaceSetWindowGeometry( struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height )
{
}

static void XdgSurfaceSetMaximized( struct wl_client* client, struct wl_resource* resource )
{
}

static void XdgSurfaceUnsetMaximized( struct wl_client* client, struct wl_resource* resource )
{
}

static void XdgSurfaceSetFullScreen( struct wl_client* client, struct wl_resource* resource, struct wl_resource* outputResource )
{
}

static void XdgSurfaceUnsetFullScreen( struct wl_client* client, struct wl_resource* resource )
{
}

static void XdgSurfaceSetMinimized( struct wl_client* client, struct wl_resource* resource )
{
}

static const struct xdg_surface_interface xdgSurfaceImplementation =
{
   XdgSurfaceDestroy,
   XdgSurfaceSetParent,
   XdgSurfaceSetTitle,
   XdgSurfaceSetAppId,
   XdgSurfaceShowWindowMenu,
   XdgSurfaceMove,
   XdgSurfaceResize,
   XdgSurfaceAckConfigure,
   XdgSurfaceSetWindowGeometry,
   XdgSurfaceSetMaximized,
   XdgSurfaceUnsetMaximized,
   XdgSurfaceSetFullScreen,
   XdgSurfaceUnsetFullScreen,
   XdgSurfaceSetMinimized,
};

} // unnamed namespace

ShellPtr Shell::New( Pepper::Compositor compositor )
{
  ShellPtr impl = new Shell();

  // Second-phase init of the implementation
  impl->Initialize( compositor );

  return impl;
}

Shell::Shell()
: mClient( NULL ),
  mSurface( NULL ),
  mView( NULL ),
  mSurfaceDestroyListener( NULL ),
  mSurfaceCommitListener( NULL ),
  mTitle(),
  mSurfaceMapped( false ),
  mAckConfigure( false ),
  mConfigureCallback( NULL ),
  mConfigureCallbackData( NULL ),
  mWidth( 0 ),
  mHeight( 0 )
{
}

Shell::~Shell()
{
  if( mClient )
  {
    wl_resource_destroy( mClient );
  }

  if( mSurface )
  {
    wl_resource_destroy( mSurface );
  }

  if( mView )
  {
    pepper_view_destroy( mView );
  }
}

void Shell::Initialize( Pepper::Compositor& compositor )
{
  struct wl_display* display;

  mCompositor = compositor;

  display = pepper_compositor_get_display( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ) );

  if( !wl_global_create( display, &xdg_shell_interface, 1, this, Shell::OnCreateClient ) )
  {
    DALI_LOG_INFO( gPepperShellLogging, Debug::General, "wl_global_create is failed\n" );
  }

  DALI_LOG_INFO( gPepperShellLogging, Debug::Verbose, "Shell::Initialize: success\n" );
}

void Shell::Configure( int width, int height, Pepper::Shell::ConfigureCallback callback, void* data )
{
  struct wl_display* display;
  struct wl_array states;
  uint32_t *state;
  uint32_t serial;

  wl_array_init( &states );

  state = (uint32_t*)wl_array_add( &states, sizeof( uint32_t ) );
  *state = XDG_SURFACE_STATE_ACTIVATED; // this is arbitary.

  display = pepper_compositor_get_display( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ) );
  serial = wl_display_next_serial( display );

  xdg_surface_send_configure( mSurface, width, height, &states, serial );

  wl_array_release( &states );

  mAckConfigure = false;

  mWidth = width;
  mHeight = height;
  mConfigureCallback = callback;
  mConfigureCallbackData = data;
}

void Shell::AckConfigure()
{
  mAckConfigure = true;

  if( mConfigureCallback )
  {
    mConfigureCallback( mConfigureCallbackData, mWidth, mHeight );

    mConfigureCallback = NULL;
    mConfigureCallbackData = NULL;
  }
}

pepper_view_t* Shell::GetView()
{
  return mView;
}

void Shell::GetSurface( wl_client* client, unsigned int id, wl_resource* surfaceResource )
{
  pepper_surface_t* surface = static_cast< pepper_surface_t* >( wl_resource_get_user_data( surfaceResource ) );

  mSurface = wl_resource_create( client, &xdg_surface_interface, 1, id );

  mView = pepper_compositor_add_view( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ) );
  if( !mView )
  {
    DALI_LOG_INFO( gPepperShellLogging, Debug::General, "pepper_compositor_add_view is failed\n" );
    wl_client_post_no_memory( client );
    return;
  }

  if( !pepper_view_set_surface( mView, surface ) )
  {
    DALI_LOG_INFO( gPepperShellLogging, Debug::General, "pepper_view_set_surface is failed\n" );
    wl_client_post_no_memory( client );
    return;
  }

  wl_resource_set_implementation( mSurface, &xdgSurfaceImplementation, this, Shell::SurfaceResourceDestroy );

  mSurfaceDestroyListener = pepper_object_add_event_listener( reinterpret_cast< pepper_object_t* >( surface ), PEPPER_EVENT_OBJECT_DESTROY, 0, Shell::OnSurfaceDestroy, this );

  mSurfaceCommitListener = pepper_object_add_event_listener( reinterpret_cast< pepper_object_t* >( surface ), PEPPER_EVENT_SURFACE_COMMIT, 0, Shell::OnSurfaceCommit, this );

  mSurfaceMapped = false;

  pepper_surface_set_role( surface, "xdg_surface" );

  pepper_object_set_user_data( reinterpret_cast< pepper_object_t* >( surface ), pepper_surface_get_role( surface ), this, NULL );
}

void Shell::SetTitle( const char* title )
{
  mTitle = title;
}

void Shell::OnCreateClient( struct wl_client* client, void* data, uint32_t version, uint32_t id )
{
  Shell* shell = static_cast< Shell* >( data );

  shell->mClient = wl_resource_create( client, &xdg_shell_interface, version, id );
  if( !shell->mClient )
  {
    DALI_LOG_INFO( gPepperShellLogging, Debug::General, "wl_resource_create is failed\n" );
    return;
  }

  wl_resource_set_implementation( shell->mClient, &xdgImpl, shell, NULL );

}

void Shell::SurfaceResourceDestroy( struct wl_resource* resource )
{
  Shell* shell = static_cast< Shell* >( wl_resource_get_user_data( resource ) );

  shell->mSurface = NULL;
}

void Shell::OnSurfaceDestroy( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data )
{
  Shell* shell = static_cast< Shell* >( data );

  pepper_event_listener_remove( shell->mSurfaceDestroyListener );

  if( shell->mSurface )
  {
    wl_resource_destroy( shell->mSurface );
    shell->mSurface = NULL;
  }

  if( !shell->mTitle.empty() )
  {
    shell->mTitle.clear();
  }

  pepper_object_set_user_data( surface, pepper_surface_get_role( reinterpret_cast< pepper_surface_t* >( surface ) ), NULL, NULL );

}

void Shell::OnSurfaceCommit( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data )
{
  Shell* shell = static_cast< Shell* >( data );

  if( !shell->mSurfaceMapped )
  {
    pepper_view_map( shell->mView );

    shell->mSurfaceMapped = true;
  }
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
