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
#include <pepper-dali/internal/shell-client-impl.h>

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
Integration::Log::Filter* gPepperShellClientLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_SHELL_CLIENT" );
#endif

static void XdgShellClientDestroy( struct wl_client* client, struct wl_resource* resource)
{
}

static void XdgShellClientUseUnstableVersion( struct wl_client* client, struct wl_resource* resource, int32_t version )
{
}

static void XdgShellClientGetSurface( struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* surfaceResource )
{
  ShellClient* shellClient = static_cast< ShellClient* >( wl_resource_get_user_data( resource ) );

  shellClient->GetSurface( client, id, surfaceResource );
}

static void XdgShellClientGetPopup( struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* surfaceResource,
                              struct wl_resource* parentResource, struct wl_resource* seatResource, uint32_t serial, int32_t x, int32_t y )
{
}

static void XdgShellClientPong( struct wl_client* client, struct wl_resource* resource, uint32_t serial )
{
}

static const struct xdg_shell_interface xdgImpl = {
  XdgShellClientDestroy,
  XdgShellClientUseUnstableVersion,
  XdgShellClientGetSurface,
  XdgShellClientGetPopup,
  XdgShellClientPong
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
  ShellClient* shellClient = static_cast< ShellClient* >( wl_resource_get_user_data( resource ) );

  shellClient->SetTitle( title );
}

static void XdgSurfaceSetAppId( struct wl_client* client, struct wl_resource* resource, const char* appId )
{
  ShellClient* shellClient = static_cast< ShellClient* >( wl_resource_get_user_data( resource ) );

  shellClient->SetAppId( appId );
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
  ShellClient* shellClient = static_cast< ShellClient* >( wl_resource_get_user_data( resource ) );

  shellClient->AckConfigure();
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

ShellClientPtr ShellClient::New( Pepper::Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id )
{
  ShellClientPtr impl = new ShellClient();

  // Second-phase init of the implementation
  impl->Initialize( compositor, client, version, id );

  return impl;
}

ShellClient::ShellClient()
: mClient( NULL ),
  mSurface( NULL ),
  mView( NULL ),
  mSurfaceDestroyListener( NULL ),
  mSurfaceCommitListener( NULL ),
  mTitle(),
  mAppId(),
  mSurfaceMapped( false ),
  mAckConfigure( false ),
  mConfigureCallback( NULL ),
  mConfigureCallbackData( NULL ),
  mWidth( 0 ),
  mHeight( 0 )
{
}

ShellClient::~ShellClient()
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

void ShellClient::Initialize( Pepper::Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id )
{
  mCompositor = compositor;

  mClient = wl_resource_create( client, &xdg_shell_interface, version, id );
  if( !mClient )
  {
    DALI_LOG_INFO( gPepperShellClientLogging, Debug::General, "ShellClient::Initialize: wl_resource_create is failed\n" );
    return;
  }

  wl_resource_set_implementation( mClient, &xdgImpl, this, NULL );

  DALI_LOG_INFO( gPepperShellClientLogging, Debug::Verbose, "ShellClient::Initialize: success\n" );
}

void ShellClient::Configure( int width, int height, ConfigureCallback callback, void* data )
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

void ShellClient::AckConfigure()
{
  mAckConfigure = true;

  if( mConfigureCallback )
  {
    mConfigureCallback( mConfigureCallbackData, mWidth, mHeight );

    mConfigureCallback = NULL;
    mConfigureCallbackData = NULL;
  }
}

pepper_view_t* ShellClient::GetView()
{
  return mView;
}

void ShellClient::GetSurface( wl_client* client, unsigned int id, wl_resource* surfaceResource )
{
  pepper_surface_t* surface = static_cast< pepper_surface_t* >( wl_resource_get_user_data( surfaceResource ) );

  mSurface = wl_resource_create( client, &xdg_surface_interface, 1, id );

  mView = pepper_compositor_add_view( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( mCompositor ).GetCompositorHandle() ) );
  if( !mView )
  {
    DALI_LOG_INFO( gPepperShellClientLogging, Debug::General, "pepper_compositor_add_view is failed\n" );
    wl_client_post_no_memory( client );
    return;
  }

  if( !pepper_view_set_surface( mView, surface ) )
  {
    DALI_LOG_INFO( gPepperShellClientLogging, Debug::General, "pepper_view_set_surface is failed\n" );
    wl_client_post_no_memory( client );
    return;
  }

  wl_resource_set_implementation( mSurface, &xdgSurfaceImplementation, this, ShellClient::SurfaceResourceDestroy );

  mSurfaceDestroyListener = pepper_object_add_event_listener( reinterpret_cast< pepper_object_t* >( surface ), PEPPER_EVENT_OBJECT_DESTROY, 0, ShellClient::OnSurfaceDestroy, this );

  mSurfaceCommitListener = pepper_object_add_event_listener( reinterpret_cast< pepper_object_t* >( surface ), PEPPER_EVENT_SURFACE_COMMIT, 0, ShellClient::OnSurfaceCommit, this );

  mSurfaceMapped = false;

  pepper_surface_set_role( surface, "xdg_surface" );

  pepper_object_set_user_data( reinterpret_cast< pepper_object_t* >( surface ), pepper_surface_get_role( surface ), this, NULL );

  DALI_LOG_INFO( gPepperShellClientLogging, Debug::Verbose, "ShellClient::GetSurface: success. surface = %p\n", surface );
}

void ShellClient::SetTitle( const std::string title )
{
  mTitle = title;
}

std::string ShellClient::GetTitle() const
{
  return mTitle;
}

void ShellClient::SetAppId( const std::string appId )
{
  mAppId = appId;
}

std::string ShellClient::GetAppId() const
{
  return mAppId;
}

void ShellClient::SurfaceResourceDestroy( struct wl_resource* resource )
{
  ShellClient* shellClient = static_cast< ShellClient* >( wl_resource_get_user_data( resource ) );

  shellClient->mSurface = NULL;
}

void ShellClient::OnSurfaceDestroy( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data )
{
  ShellClient* shellClient = static_cast< ShellClient* >( data );

  pepper_event_listener_remove( shellClient->mSurfaceDestroyListener );

  if( shellClient->mSurface )
  {
    wl_resource_destroy( shellClient->mSurface );
    shellClient->mSurface = NULL;
  }

  if( !shellClient->mTitle.empty() )
  {
    shellClient->mTitle.clear();
  }

  if( !shellClient->mAppId.empty() )
  {
    shellClient->mAppId.clear();
  }

  pepper_object_set_user_data( surface, pepper_surface_get_role( reinterpret_cast< pepper_surface_t* >( surface ) ), NULL, NULL );

  DALI_LOG_INFO( gPepperShellClientLogging, Debug::Verbose, "ShellClient::OnSurfaceDestroy: success. surface = %p\n", surface );
}

void ShellClient::OnSurfaceCommit( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data )
{
  ShellClient* shellClient = static_cast< ShellClient* >( data );

  if( !shellClient->mSurfaceMapped )
  {
    pepper_view_map( shellClient->mView );

    shellClient->mSurfaceMapped = true;
  }
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
