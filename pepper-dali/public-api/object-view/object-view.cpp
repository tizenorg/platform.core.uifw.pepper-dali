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
#include <pepper-dali/public-api/object-view/object-view.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/object-view-impl.h>

namespace Dali
{

namespace Pepper
{

ObjectView ObjectView::New()
{
  return Internal::ObjectView::New();
}

ObjectView ObjectView::DownCast( BaseHandle handle )
{
  return Toolkit::Control::DownCast<ObjectView, Internal::ObjectView>( handle );
}

ObjectView::ObjectView()
{
}

ObjectView::ObjectView( const ObjectView& handle )
: Toolkit::Control( handle )
{
}

ObjectView& ObjectView::operator=( const ObjectView& handle )
{
  if( &handle != this )
  {
    Control::operator=( handle );
  }
  return *this;
}

ObjectView::~ObjectView()
{
}

void ObjectView::SetImage( Image image )
{
  Dali::Pepper::GetImplementation( *this ).SetImage( image );
}

pid_t ObjectView::GetPid() const
{
  return Dali::Pepper::GetImplementation( *this ).GetPid();
}

std::string ObjectView::GetTitle() const
{
  return Dali::Pepper::GetImplementation( *this ).GetTitle();
}

std::string ObjectView::GetAppId() const
{
  return Dali::Pepper::GetImplementation( *this ).GetAppId();
}

bool ObjectView::CancelTouchEvent()
{
  return Dali::Pepper::GetImplementation( *this ).CancelTouchEvent();
}

void ObjectView::Show()
{
  Dali::Pepper::GetImplementation( *this ).Show();
}

void ObjectView::Hide()
{
  Dali::Pepper::GetImplementation( *this ).Hide();
}

void ObjectView::SetVisibilityType( VisibilityType type )
{
  Dali::Pepper::GetImplementation( *this ).SetVisibilityType( type );
}

ObjectView::ObjectView( Internal::ObjectView& implementation )
: Control( implementation )
{
}

ObjectView::ObjectView( Dali::Internal::CustomActor* internal )
: Control( internal )
{
  VerifyCustomActorPointer<Internal::ObjectView>( internal );
}

} // namespace Pepper

} // namespace Dali
