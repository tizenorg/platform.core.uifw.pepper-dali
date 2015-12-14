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
#include <pepper-dali/internal/output.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/output-impl.h>

namespace Dali
{

namespace Pepper
{

Output Output::New( Compositor compositor, Application application, Input input )
{
  Internal::OutputPtr internal = Internal::Output::New( compositor, application, input );

  return Output( internal.Get() );
}

Output Output::DownCast( BaseHandle handle )
{
  return Output( dynamic_cast< Internal::Output* >( handle.GetObjectPtr() ) );
}

Output::Output()
{
}

Output::Output( const Output& handle )
: BaseHandle(handle)
{
}

Output& Output::operator=( const Output& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

Output::~Output()
{
}

Output::Output( Internal::Output* implementation )
: BaseHandle( implementation )
{
}

Output::OutputSignalType& Output::ObjectViewAddedSignal()
{
  return GetImplementation(*this).ObjectViewAddedSignal();
}

Output::OutputSignalType& Output::ObjectViewDeletedSignal()
{
  return GetImplementation(*this).ObjectViewDeletedSignal();
}

} // namespace Toolkit

} // namespace Dali
