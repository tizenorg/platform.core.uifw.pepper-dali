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
#include <pepper-dali/internal/shell-client.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell-client-impl.h>

namespace Dali
{

namespace Pepper
{

ShellClient ShellClient::New( Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id )
{
  Internal::ShellClientPtr internal = Internal::ShellClient::New( compositor, client, version, id );

  return ShellClient( internal.Get() );
}

ShellClient ShellClient::DownCast( BaseHandle handle )
{
  return ShellClient( dynamic_cast< Internal::ShellClient* >( handle.GetObjectPtr() ) );
}

ShellClient::ShellClient()
{
}

ShellClient::ShellClient( const ShellClient& handle )
: BaseHandle(handle)
{
}

ShellClient& ShellClient::operator=( const ShellClient& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

ShellClient::~ShellClient()
{
}

ShellClient::ShellClient( Internal::ShellClient* implementation )
: BaseHandle( implementation )
{
}

} // namespace Toolkit

} // namespace Dali
