#ifndef __DALI_PEPPER_INTERNAL_OBJECT_VIEW_H__
#define __DALI_PEPPER_INTERNAL_OBJECT_VIEW_H__

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
#include <pepper-dali/public-api/object-view/object-view.h>

// EXTERNAL INCLUDES
#include <dali/public-api/images/buffer-image.h>
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/public-api/controls/image-view/image-view.h>
#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class ObjectView : public Toolkit::Internal::Control
{
public:

  /**
   * @copydoc Dali::Pepper::ObjectView::New
   */
  static Dali::Pepper::ObjectView New();

  /**
   * @copydoc Dali::Pepper::ObjectView::SetImage
   */
  void SetImage( Image image );

  /**
   * @copydoc Dali::Pepper::ObjectView::GetPid
   */
  pid_t GetPid() const;

  /**
   * @copydoc Dali::Pepper::ObjectView::GetTitle
   */
  std::string GetTitle() const;

  /**
   * @copydoc Dali::Pepper::ObjectView::GetAppId
   */
  std::string GetAppId() const;

  /**
   * @copydoc Dali::Pepper::ObjectView::CancelTouchEvent
   */
  bool CancelTouchEvent();

  /**
   * @copydoc Dali::Pepper::ObjectView::Show
   */
  void Show();

  /**
   * @copydoc Dali::Pepper::ObjectView::Hide
   */
  void Hide();

  /**
   * Set pepper surface
   */
  void SetSurface( pepper_surface_t* surface );

  /**
   * Set pepper input modules
   */
  void SetInput( pepper_pointer_t* pointer, pepper_keyboard_t* keyboard, pepper_touch_t* touch );

protected:

  /**
   * Construct a new ObjectView.
   */
  ObjectView();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~ObjectView();

private: // From Control

  /**
   * @copydoc Dali::Toolkit::Control::OnInitialize()
   */
  virtual void OnInitialize();

private: // From CustomActorImpl

  /**
   * @copydoc Dali::CustomActorImpl::OnTouchEvent( const TouchEvent& event )
   */
  virtual bool OnTouchEvent( const TouchEvent& event );

  /**
   * @copydoc CustomActorImpl::OnSizeSet( const Vector3& targetSize )
   */
  virtual void OnSizeSet( const Vector3& targetSize );

private:

  // Undefined
  ObjectView( const ObjectView& );

  // Undefined
  ObjectView& operator= ( const ObjectView& );

  static void OnConfigureCallback( void* data, int width, int height );

private:

  Toolkit::ImageView mImageView;
  BufferImage mBufferImage;

  int mWidth;
  int mHeight;

  bool mTouchDown;

  pepper_surface_t* mSurface;
  pepper_pointer_t* mPointer;
  pepper_keyboard_t* mKeyboard;
  pepper_touch_t* mTouch;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::ObjectView& GetImplementation( Pepper::ObjectView& objectView )
{
  DALI_ASSERT_ALWAYS( objectView );

  Dali::RefObject& handle = objectView.GetImplementation();

  return static_cast<Internal::ObjectView&>( handle );
}

inline const Pepper::Internal::ObjectView& GetImplementation( const Pepper::ObjectView& objectView )
{
  DALI_ASSERT_ALWAYS( objectView );

  const Dali::RefObject& handle = objectView.GetImplementation();

  return static_cast<const Internal::ObjectView&>( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_INTERNAL_OBJECT_VIEW_H__
