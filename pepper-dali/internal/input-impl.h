#ifndef __PEPPER_DALI_INTERNAL_INPUT_H__
#define __PEPPER_DALI_INTERNAL_INPUT_H__

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
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/base-object.h>

// EXTERNAL INCLUDES
#include <Ecore.h>

struct xkb_context;

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Input;
typedef IntrusivePtr<Input> InputPtr;

class Input : public BaseObject
{
public:

  static InputPtr New( Pepper::Compositor compositor );

  static Eina_Bool EcoreEventKeyDown( void* data, int type, void* event );
  static Eina_Bool EcoreEventKeyUp( void* data, int type, void* event );

  pepper_pointer_t* GetPointer();
  pepper_keyboard_t* GetKeyboard();
  pepper_touch_t* GetTouch();

private:

  /**
   * Construct a new Input.
   */
  Input();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~Input();

  /**
   * Second-phase constructor. Must be called immediately after creating a new Input;
   */
  void Initialize( Pepper::Compositor& compositor );

  void SetKeymap();

private:

  // Undefined
  Input( const Input& );

  // Undefined
  Input& operator=( const Input& rhs );

private: // Data

  Pepper::Compositor mCompositor;

  pepper_seat_t* mSeat;
  pepper_input_device_t* mDevice;
  pepper_pointer_t* mPointer;
  pepper_keyboard_t* mKeyboard;
  pepper_touch_t* mTouch;

  xkb_context* mContext;

  std::vector<Ecore_Event_Handler*> mEcoreEventHandler;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::Input& GetImplementation( Pepper::Input& input )
{
  DALI_ASSERT_ALWAYS( input && "Input handle is empty" );

  BaseObject& handle = input.GetBaseObject();

  return static_cast< Internal::Input& >( handle );
}

inline const Pepper::Internal::Input& GetImplementation( const Pepper::Input& input )
{
  DALI_ASSERT_ALWAYS( input && "Input handle is empty" );

  const BaseObject& handle = input.GetBaseObject();

  return static_cast< const Internal::Input& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __PEPPER_DALI_INTERNAL_INPUT_H__
