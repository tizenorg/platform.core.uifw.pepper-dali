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
#include <pepper-dali/internal/shell.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell-impl.h>

namespace Dali
{

namespace Pepper
{

Shell Shell::New( Compositor compositor )
{
  Internal::ShellPtr internal = Internal::Shell::New( compositor );

  return Shell( internal.Get() );
}

Shell Shell::DownCast( BaseHandle handle )
{
  return Shell( dynamic_cast< Internal::Shell* >( handle.GetObjectPtr() ) );
}

Shell::Shell()
{
}

Shell::Shell( const Shell& handle )
: BaseHandle(handle)
{
}

Shell& Shell::operator=( const Shell& handle )
{
  BaseHandle::operator=(handle);
  return *this;
}

Shell::~Shell()
{
}

Shell::Shell( Internal::Shell* implementation )
: BaseHandle( implementation )
{
}

} // namespace Toolkit

} // namespace Dali
