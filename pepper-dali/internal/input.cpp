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

// CLASS HEADER
#include <pepper-dali/internal/input.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/input-impl.h>

namespace Dali
{

namespace Pepper
{

Input Input::New( Pepper::Compositor compositor )
{
  Internal::InputPtr internal = Internal::Input::New( compositor );

  return Input( internal.Get() );
}

Input Input::DownCast( BaseHandle handle )
{
  return Input( dynamic_cast< Internal::Input* >( handle.GetObjectPtr() ) );
}

Input::Input()
{
}

Input::Input( const Input& handle )
: BaseHandle(handle)
{
}

Input& Input::operator=( const Input& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

Input::~Input()
{
}

Input::Input( Internal::Input* implementation )
: BaseHandle( implementation )
{
}

pepper_pointer_t* Input::GetPointer()
{
  return GetImplementation(*this).GetPointer();
}

pepper_keyboard_t* Input::GetKeyboard()
{
  return GetImplementation(*this).GetKeyboard();
}

pepper_touch_t* Input::GetTouch()
{
  return GetImplementation(*this).GetTouch();
}

} // namespace Toolkit

} // namespace Dali
