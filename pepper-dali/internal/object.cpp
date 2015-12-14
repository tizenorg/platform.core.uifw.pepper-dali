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
#include <pepper-dali/internal/object.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/object-impl.h>

namespace Dali
{

namespace Pepper
{

Object Object::New( pepper_surface_t* surface, Input input )
{
  Internal::ObjectPtr internal = Internal::Object::New( surface, input );

  return Object( internal.Get() );
}

Object Object::DownCast( BaseHandle handle )
{
  return Object( dynamic_cast< Internal::Object* >( handle.GetObjectPtr() ) );
}

Object::Object()
{
}

Object::Object( const Object& handle )
: BaseHandle(handle)
{
}

Object& Object::operator=( const Object& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

Object::~Object()
{
}

Object::Object( Internal::Object* implementation )
: BaseHandle( implementation )
{
}

bool Object::AttchBuffer( int* width, int* height )
{
  return GetImplementation( *this ).AttchBuffer( width, height );
}

Object::ObjectSignalType& Object::AddedSignal()
{
  return GetImplementation(*this).AddedSignal();
}

Object::ObjectSignalType& Object::DeletedSignal()
{
  return GetImplementation(*this).DeletedSignal();
}

} // namespace Toolkit

} // namespace Dali
