#ifndef __DALI_PEPPER_SHELL_CLIENT_H__
#define __DALI_PEPPER_SHELL_CLIENT_H__

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
#include <pepper-dali/public-api/compositor/compositor.h>

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>
#include <wayland-server.h>
#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class ShellClient;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class ShellClient : public BaseHandle
{
public:

  /**
   * @brief Create ShellClient.
   *
   * @since_tizen 3.0
   * @return A handle to ShellClient.
   */
  static ShellClient New( Compositor compositor, struct wl_client* client, uint32_t version, uint32_t id );

  /**
   * @brief Downcast a handle to ShellClient handle.
   *
   * If the BaseHandle points is a ShellClient the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a ShellClient or an empty handle
   */
  static ShellClient DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  ShellClient();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  ShellClient( const ShellClient& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  ShellClient& operator=( const ShellClient& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~ShellClient();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Pepper::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The ShellClient implementation.
   */
  explicit DALI_INTERNAL ShellClient( Internal::ShellClient* implementation );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_SHELL_CLIENT_H__
