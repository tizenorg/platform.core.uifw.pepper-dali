#ifndef __DALI_PEPPER_OBJECT_H__
#define __DALI_PEPPER_OBJECT_H__

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
#include <pepper-dali/internal/input.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/object/base-handle.h>
#include <dali-toolkit/public-api/controls/image-view/image-view.h>
#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class Object;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class Object : public BaseHandle
{
public:

  /**
   * @brief Create Object.
   *
   * @since_tizen 3.0
   * @return A handle to Object.
   */
  static Object New( pepper_surface_t* surface, Input input );

  /**
   * @brief Downcast a handle to Object handle.
   *
   * If the BaseHandle points is a Object the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a Object or an empty handle
   */
  static Object DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  Object();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  Object( const Object& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  Object& operator=( const Object& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~Object();

  bool AttchBuffer( int* width, int* height );

public: //Signals

  typedef Signal< void ( Object, ObjectView ) > ObjectSignalType;

  ObjectSignalType& ObjectViewAddedSignal();
  ObjectSignalType& ObjectViewDeletedSignal();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Pepper::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The Object implementation.
   */
  explicit DALI_INTERNAL Object( Internal::Object* implementation );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_OBJECT_H__
