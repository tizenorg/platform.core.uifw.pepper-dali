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
#include <pepper-dali/public-api/compositor/compositor.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/compositor-impl.h>

namespace Dali
{

namespace Pepper
{

Compositor Compositor::New( Application application, const std::string& name )
{
  Internal::CompositorPtr internal = Internal::Compositor::New( application, name );

  return Compositor( internal.Get() );
}

Compositor Compositor::DownCast( BaseHandle handle )
{
  return Compositor( dynamic_cast< Internal::Compositor* >( handle.GetObjectPtr() ) );
}

Compositor::Compositor()
{
}

Compositor::Compositor( const Compositor& handle )
: BaseHandle(handle)
{
}

Compositor& Compositor::operator=( const Compositor& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

Compositor::~Compositor()
{
}

Compositor::Compositor( Internal::Compositor* implementation )
: BaseHandle( implementation )
{
}

Compositor::CompositorSignalType& Compositor::ObjectViewAddedSignal()
{
  return GetImplementation(*this).ObjectViewAddedSignal();
}

Compositor::CompositorSignalType& Compositor::ObjectViewDeletedSignal()
{
  return GetImplementation(*this).ObjectViewDeletedSignal();
}

} // namespace Toolkit

} // namespace Dali
