#ifndef __PEPPER_DALI_INTERNAL_SHELL_H__
#define __PEPPER_DALI_INTERNAL_SHELL_H__

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
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/base-object.h>

// EXTERNAL INCLUDES

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Shell;
typedef IntrusivePtr<Shell> ShellPtr;

class Shell : public BaseObject
{
public:

  static ShellPtr New( Pepper::Compositor compositor );

  void Configure( int width, int height, Pepper::Shell::ConfigureCallback callback, void* data );

  void CreateClient( wl_client* client, unsigned int version, unsigned int id );
  void GetSurface( wl_client* client, unsigned int id, wl_resource* surfaceResource );

  void OnSurfaceDestroy( pepper_object_t *surface );
  void OnSurfaceCommit();

  void SetTitle( const char* title );
  void AckConfigure();

  pepper_view_t* GetView();

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

private:

  // Undefined
  Shell( const Shell& );

  // Undefined
  Shell& operator=( const Shell& rhs );

private: // Data

  Pepper::Compositor mCompositor;

  wl_resource* mClient;
  wl_resource* mSurface;

  pepper_view_t* mView;

  pepper_event_listener_t* mSurfaceDestroyListener;
  pepper_event_listener_t* mSurfaceCommitListener;

  std::string mTitle;

  bool mSurfaceMapped;
  bool mAckConfigure;

  Pepper::Shell::ConfigureCallback mConfigureCallback;
  void* mConfigureCallbackData;
  int mWidth;
  int mHeight;
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

#endif // __PEPPER_DALI_INTERNAL_SHELL_H__
