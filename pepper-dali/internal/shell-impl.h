#ifndef __DALI_PEPPER_INTERNAL_SHELL_H__
#define __DALI_PEPPER_INTERNAL_SHELL_H__

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

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell.h>
#include <pepper-dali/internal/shell-client.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/base-object.h>
#include <wayland-server.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Shell;
typedef IntrusivePtr<Shell> ShellPtr;

struct ShellClientInfo
{
  ShellPtr mShell;
  Pepper::ShellClient mShellClient;
  struct wl_listener mDestroyListner;
};

class Shell : public BaseObject
{
public:

  static ShellPtr New( Pepper::Compositor compositor );

  void OnClientDestroyed( Pepper::ShellClient shellClient );

private:

  /**
   * Construct a new Shell.
   */
  Shell();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~Shell();

  /**
   * Second-phase constructor. Must be called immediately after creating a new Shell;
   */
  void Initialize( Pepper::Compositor& compositor );

  static void OnBind( struct wl_client* client, void* data, uint32_t version, uint32_t id );

private:

  // Undefined
  Shell( const Shell& );

  // Undefined
  Shell& operator=( const Shell& rhs );

private: // Data

  Pepper::Compositor mCompositor;

  std::vector<Pepper::ShellClient> mShellClientList;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::Shell& GetImplementation( Pepper::Shell& shell )
{
  DALI_ASSERT_ALWAYS( shell && "Shell handle is empty" );

  BaseObject& handle = shell.GetBaseObject();

  return static_cast< Internal::Shell& >( handle );
}

inline const Pepper::Internal::Shell& GetImplementation( const Pepper::Shell& shell )
{
  DALI_ASSERT_ALWAYS( shell && "Shell handle is empty" );

  const BaseObject& handle = shell.GetBaseObject();

  return static_cast< const Internal::Shell& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_INTERNAL_SHELL_H__
