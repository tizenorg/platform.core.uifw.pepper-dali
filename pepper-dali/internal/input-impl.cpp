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

// CLASS HEADER
#include <pepper-dali/internal/input-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/compositor-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <pepper-input-backend.h>
#include <Ecore_Input.h>
#include <xkbcommon/xkbcommon.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperInputLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_INPUT" );
#endif

} // unnamed namespace

InputPtr Input::New( Pepper::Compositor compositor )
{
  InputPtr impl = new Input();

  // Second-phase init of the implementation
  impl->Initialize( compositor );

  return impl;
}

Input::Input()
: mSeat( NULL ),
  mDevice( NULL ),
  mPointer( NULL ),
  mKeyboard( NULL ),
  mTouch( NULL ),
  mContext( NULL ),
  mEcoreEventHandler()
{
}

Input::~Input()
{
  for( std::vector<Ecore_Event_Handler*>::iterator iter = mEcoreEventHandler.begin(), endIter = mEcoreEventHandler.end(); iter != endIter; ++iter )
  {
    ecore_event_handler_del( *iter );
  }

  xkb_context_unref( mContext );

  pepper_seat_remove_input_device( mSeat, mDevice );
  pepper_input_device_destroy( mDevice );

  pepper_seat_destroy( mSeat );
}

void Input::Initialize( Pepper::Compositor& compositor )
{
  const char *defaultName = "seat0";

  mSeat = pepper_compositor_add_seat( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ), defaultName );
  if( !mSeat )
  {
    DALI_LOG_INFO( gPepperInputLogging, Debug::General, "pepper_compositor_add_seat is failed\n" );
    return;
  }

  /* create and add devices. */
  mDevice = pepper_input_device_create( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ),
                                        WL_SEAT_CAPABILITY_TOUCH, NULL, NULL );
//  mDevice = pepper_input_device_create( static_cast< pepper_compositor_t* >( Pepper::GetImplementation( compositor ).GetCompositorHandle() ),
//                                        ( WL_SEAT_CAPABILITY_POINTER | WL_SEAT_CAPABILITY_KEYBOARD | WL_SEAT_CAPABILITY_TOUCH ), NULL, NULL );
  if( !mDevice )
  {
    DALI_LOG_INFO( gPepperInputLogging, Debug::General, "pepper_input_device_create is failed\n" );
    pepper_seat_destroy( mSeat );
    return;
  }

  pepper_seat_add_input_device( mSeat, mDevice );

  /* get device resource from seat */
  mPointer = pepper_seat_get_pointer( mSeat );
  mKeyboard = pepper_seat_get_keyboard( mSeat );
  mTouch = pepper_seat_get_touch( mSeat );

  mCompositor = compositor;

  SetKeymap();

  mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_KEY_DOWN, EcoreEventKeyDown, this ) );
  mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_KEY_UP, EcoreEventKeyUp, this ) );

  DALI_LOG_INFO( gPepperInputLogging, Debug::Verbose, "Input::Initialize: success\n" );
}

void Input::SetKeymap()
{
  struct xkb_keymap* keymap;
  struct xkb_context* context;
  struct xkb_rule_names names;

  names.rules = "evdev";
  names.model = "pc105";
  names.layout = "us";
  names.options = NULL;

  context = xkb_context_new( XKB_CONTEXT_NO_FLAGS );
  if( !context )
  {
    DALI_LOG_INFO( gPepperInputLogging, Debug::General, "failed to create xkb_context\n" );
    return;
  }

  keymap = xkb_map_new_from_names( context, &names, XKB_KEYMAP_COMPILE_NO_FLAGS );
  if( !keymap )
  {
    DALI_LOG_INFO( gPepperInputLogging, Debug::General, "failed to get keymap\n" );
    xkb_context_unref( context );
    return;
  }

  pepper_keyboard_set_keymap( mKeyboard, keymap );
  xkb_map_unref( keymap );

  mContext = context;
}

Eina_Bool Input::EcoreEventKeyDown( void* data, int type, void* event )
{
  Input* input = static_cast< Input* >( data );
  pepper_view_t* view;
  Ecore_Event_Key *keyEvent = static_cast< Ecore_Event_Key* >( event );
  uint32_t keycode;

  if( !( view = pepper_keyboard_get_focus( input->mKeyboard ) ) )
  {
    return ECORE_CALLBACK_RENEW;
  }

  keycode = (keyEvent->keycode - 8);

  pepper_keyboard_send_key( input->mKeyboard, view, keyEvent->timestamp, keycode, WL_KEYBOARD_KEY_STATE_PRESSED );

  return ECORE_CALLBACK_RENEW;
}

Eina_Bool Input::EcoreEventKeyUp( void* data, int type, void* event )
{
  Input* input = static_cast< Input* >( data );
  pepper_view_t* view;
  Ecore_Event_Key *keyEvent = static_cast< Ecore_Event_Key* >( event );
  uint32_t keycode;

  if( !( view = pepper_keyboard_get_focus( input->mKeyboard ) ) )
  {
    return ECORE_CALLBACK_RENEW;
  }

  keycode = (keyEvent->keycode - 8);

  pepper_keyboard_send_key( input->mKeyboard, view, keyEvent->timestamp, keycode, WL_KEYBOARD_KEY_STATE_RELEASED );

  return ECORE_CALLBACK_RENEW;
}

pepper_pointer_t* Input::GetPointer()
{
  return mPointer;
}

pepper_keyboard_t* Input::GetKeyboard()
{
  return mKeyboard;
}

pepper_touch_t* Input::GetTouch()
{
  return mTouch;
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
