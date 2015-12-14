#ifndef __DALI_PEPPER_SHELL_H__
#define __DALI_PEPPER_SHELL_H__

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
#include <pepper-dali/public-api/compositor/compositor.h>

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class Shell;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class Shell : public BaseHandle
{
public:

  /**
   * @brief Create Shell.
   *
   * @since_tizen 3.0
   * @return A handle to Shell.
   */
  static Shell New( Compositor compositor );

  /**
   * @brief Downcast a handle to Shell handle.
   *
   * If the BaseHandle points is a Shell the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a Shell or an empty handle
   */
  static Shell DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  Shell();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  Shell( const Shell& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  Shell& operator=( const Shell& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~Shell();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Pepper::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The Shell implementation.
   */
  explicit DALI_INTERNAL Shell( Internal::Shell* implementation );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_SHELL_H__
