#ifndef __PEPPER_DALI_INPUT_H__
#define __PEPPER_DALI_INPUT_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/object/base-handle.h>

// EXTERNAL INCLUDES
#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class Input;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class DALI_IMPORT_API Input : public BaseHandle
{
public:

  /**
   * @brief Create Input.
   *
   * @since_tizen 3.0
   * @return A handle to Input.
   */
  static Input New( Pepper::Compositor compositor );

  /**
   * @brief Downcast a handle to Input handle.
   *
   * If the BaseHandle points is a Input the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a Input or an empty handle
   */
  static Input DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  Input();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  Input( const Input& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  Input& operator=( const Input& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~Input();

  pepper_pointer_t* GetPointer();
  pepper_keyboard_t* GetKeyboard();
  pepper_touch_t* GetTouch();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Pepper::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The Input implementation.
   */
  explicit DALI_INTERNAL Input( Internal::Input* implementation );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __PEPPER_DALI_INPUT_H__
