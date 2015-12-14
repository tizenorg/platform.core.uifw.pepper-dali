#ifndef __PEPPER_DALI_COMPOSITOR_H__
#define __PEPPER_DALI_COMPOSITOR_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/object/base-handle.h>
#include <dali/public-api/adaptor-framework/application.h>
#include <dali-toolkit/public-api/controls/image-view/image-view.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class Compositor;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class DALI_IMPORT_API Compositor : public BaseHandle
{
public:

  /**
   * @brief Create compositer.
   *
   * @since_tizen 3.0
   * @return A handle to Compositor.
   */
  static Compositor New( Application application, const std::string& name );

  /**
   * @brief Downcast a handle to Compositor handle.
   *
   * If the BaseHandle points is a Compositor the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a Compositor or an empty handle
   */
  static Compositor DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  Compositor();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  Compositor( const Compositor& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  Compositor& operator=( const Compositor& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~Compositor();

public: //Signals

  typedef Signal< void ( Compositor, Toolkit::ImageView ) > CompositorSignalType;

  CompositorSignalType& ObjectAddedSignal();
  CompositorSignalType& ObjectDeletedSignal();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Pepper::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The Compositor implementation.
   */
  explicit DALI_INTERNAL Compositor( Internal::Compositor* implementation );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __PEPPER_DALI_COMPOSITOR_H__
