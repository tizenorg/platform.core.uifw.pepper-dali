#ifndef __DALI_PEPPER_OBJECT_VIEW_H__
#define __DALI_PEPPER_OBJECT_VIEW_H__

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

// EXTERNAL INCLUDES
#include <dali-toolkit/public-api/controls/control.h>

namespace Dali
{

namespace Pepper
{

namespace Internal DALI_INTERNAL
{
class ObjectView;
}

/**
 * @addtogroup dali_pepper_compositor
 * @{
 */

class DALI_IMPORT_API ObjectView : public Toolkit::Control
{
public:

  /**
   * @brief Create object view.
   *
   * @since_tizen 3.0
   * @return A handle to ObjectView.
   */
  static ObjectView New();

  /**
   * @brief Downcast a handle to ObjectView handle.
   *
   * If the BaseHandle points is a ObjectView the downcast returns a valid handle.
   * If not the returned handle is left empty.
   *
   * @since_tizen 3.0
   * @param[in] handle Handle to an object
   * @return handle to a ObjectView or an empty handle
   */
  static ObjectView DownCast( BaseHandle handle );

  /**
   * @brief Creates an empty handle.
   * @since_tizen 3.0
   */
  ObjectView();

  /**
   * @brief Copy constructor.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   */
  ObjectView( const ObjectView& handle );

  /**
   * @brief Assignment operator.
   *
   * @since_tizen 3.0
   * @param[in] handle The handle to copy from.
   * @return A reference to this.
   */
  ObjectView& operator=( const ObjectView& handle );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   * @since_tizen 3.0
   */
  ~ObjectView();

  /**
   * @brief Sets this ObjectView from an Image.
   *
   * If the handle is empty, ObjectView will display nothing.
   * @since_tizen 3.0
   * @param[in] image The Image to display.
   */
  void SetImage( Image image );

  /**
   * @brief Gets pid of the client.
   *
   * @since_tizen 3.0
   * @return pid on success, otherwise 0.
   */
  pid_t GetPid() const;

  /**
   * @brief Gets title of the client.
   *
   * @since_tizen 3.0
   * @return title on success, otherwise empty string.
   */
  std::string GetTitle() const;

  /**
   * @brief Gets app id of the client.
   *
   * @since_tizen 3.0
   * @return app id on success, otherwise empty string.
   */
  std::string GetAppId() const;

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the ObjectView::Internal implementation.
   *
   * @since_tizen 3.0
   * @param[in] implementation The ObjectView implementation.
   */
  DALI_INTERNAL ObjectView( Internal::ObjectView& implementation );

  /**
   * @brief Allows the creation of this control from an Internal::CustomActor pointer.
   *
   * @since_tizen 3.0
   * @param[in] internal A pointer to the internal CustomActor.
   */
  DALI_INTERNAL ObjectView( Dali::Internal::CustomActor* internal );
};

/**
 * @}
 */
} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_OBJECT_VIEW_H__
