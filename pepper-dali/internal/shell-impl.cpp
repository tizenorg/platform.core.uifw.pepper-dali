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
Integration::Log::Filter* gPepperShellLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_SHELL" );
#endif

static void ClientDestroyed( struct wl_listener* listener, void* data )
{
  ShellClientInfo* shellClientInfo = wl_container_of( listener, shellClientInfo, mDestroyListner );

  shellClientInfo->mShell->OnClientDestroyed( shellClientInfo->mShellClient );
}

} // unnamed namespace

ShellPtr Shell::New( Pepper::Compositor compositor )
{
  ShellPtr impl = new Shell();

  // Second-phase init of the implementation
  impl->Initialize( compositor );

  return impl;
}

Shell::Shell()
: mShellClientList()
{
}

Shell::~Shell()
{
  mShellClientList.clear();
}

void Shell::Initialize( Pepper::Compositor& compositor )
{
  struct wl_display* display;

  mCompositor = compositor;

  display = pepper_compositor_get_display( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ) );

  if( !wl_global_create( display, &xdg_shell_interface, 1, this, Shell::OnBind ) )
  {
    DALI_LOG_INFO( gPepperShellLogging, Debug::General, "wl_global_create is failed\n" );
  }

  DALI_LOG_INFO( gPepperShellLogging, Debug::Verbose, "Shell::Initialize: success\n" );
}

void Shell::OnBind( struct wl_client* client, void* data, uint32_t version, uint32_t id )
{
  Shell* shell = static_cast< Shell* >( data );

  Pepper::ShellClient shellClient = Pepper::ShellClient::New( shell->mCompositor, client, version, id );

  shell->mShellClientList.push_back( shellClient );

  ShellClientInfo* shellClientInfo = new ShellClientInfo;

  shellClientInfo->mShell = shell;
  shellClientInfo->mShellClient = shellClient;
  shellClientInfo->mDestroyListner.notify = ClientDestroyed;

  wl_client_add_destroy_listener( client, &shellClientInfo->mDestroyListner );

  DALI_LOG_INFO( gPepperShellLogging, Debug::Verbose, "Shell::OnBind: success\n" );
}

void Shell::OnClientDestroyed( Pepper::ShellClient shellClient )
{
  for ( std::vector< Pepper::ShellClient >::iterator iter = mShellClientList.begin(); mShellClientList.end() != iter; ++iter )
  {
    if( *iter == shellClient )
    {
      mShellClientList.erase( iter );
    }
  }

  DALI_LOG_INFO( gPepperShellLogging, Debug::Verbose, "Shell::OnClientDestroyed: success\n" );
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
