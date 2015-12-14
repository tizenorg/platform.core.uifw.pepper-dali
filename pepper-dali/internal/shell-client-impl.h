#ifndef __DALI_PEPPER_INTERNAL_SHELL_CLIENT_H__
#define __DALI_PEPPER_INTERNAL_SHELL_CLIENT_H__

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

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell-client.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/base-object.h>
#include <wayland-server.h>
#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class ShellClient;
typedef IntrusivePtr<ShellClient> ShellClientPtr;

class ShellClient : public BaseObject
{
public:

  typedef void (*ConfigureCallback)( void* data, int width, int height );

  static ShellClientPtr New( Pepper::Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id );

  void Configure( int width, int height, ConfigureCallback callback, void* data );
  void AckConfigure();

  pepper_view_t* GetView();
  void GetSurface( wl_client* client, unsigned int id, wl_resource* surfaceResource );

  void SetTitle( const std::string title );
  std::string GetTitle() const;

  void SetAppId( const std::string appId );
  std::string GetAppId() const;

private:

  /**
   * Construct a new ShellClient.
   */
  ShellClient();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~ShellClient();

  /**
   * Second-phase constructor. Must be called immediately after creating a new ShellClient;
   */
  void Initialize( Pepper::Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id );

  static void SurfaceResourceDestroy( struct wl_resource* resource );
  static void OnSurfaceDestroy( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data );
  static void OnSurfaceCommit( pepper_event_listener_t* listener, pepper_object_t* surface, uint32_t id, void* info, void* data );

private:

  // Undefined
  ShellClient( const ShellClient& );

  // Undefined
  ShellClient& operator=( const ShellClient& rhs );

private: // Data

  Pepper::Compositor mCompositor;

  wl_resource* mClient;
  wl_resource* mSurface;

  pepper_view_t* mView;

  pepper_event_listener_t* mSurfaceDestroyListener;
  pepper_event_listener_t* mSurfaceCommitListener;

  std::string mTitle;
  std::string mAppId;

  bool mSurfaceMapped;
  bool mAckConfigure;

  ConfigureCallback mConfigureCallback;
  void* mConfigureCallbackData;
  int mWidth;
  int mHeight;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::ShellClient& GetImplementation( Pepper::ShellClient& shellClient )
{
  DALI_ASSERT_ALWAYS( shellClient && "ShellClient handle is empty" );

  BaseObject& handle = shellClient.GetBaseObject();

  return static_cast< Internal::ShellClient& >( handle );
}

inline const Pepper::Internal::ShellClient& GetImplementation( const Pepper::ShellClient& shellClient )
{
  DALI_ASSERT_ALWAYS( shellClient && "ShellClient handle is empty" );

  const BaseObject& handle = shellClient.GetBaseObject();

  return static_cast< const Internal::ShellClient& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_INTERNAL_SHELL_CLIENT_H__
