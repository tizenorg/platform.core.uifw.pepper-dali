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
#include <pepper-dali/internal/object-view-impl.h>

// INTERNAL INCLUDES
#include <pepper-dali/internal/shell-client-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gPepperObjectViewLogging  = Integration::Log::Filter::New( Debug::Verbose, false, "LOG_PEPPER_OBJECT_VIEW" );
#endif

} // unnamed namespace

Dali::Pepper::ObjectView ObjectView::New()
{
  // Create the implementation, temporarily owned on stack
  IntrusivePtr< ObjectView > internalObjectView = new ObjectView();

  // Pass ownership to CustomActor
  Dali::Pepper::ObjectView objectView( *internalObjectView );

  // Second-phase init of the implementation
  // This can only be done after the CustomActor connection has been made...
  internalObjectView->Initialize();

  return objectView;
}

ObjectView::ObjectView()
: Control( ControlBehaviour( REQUIRES_TOUCH_EVENTS ) ),
  mWidth( 0 ),
  mHeight( 0 ),
  mSurface( NULL )
{
}

ObjectView::~ObjectView()
{
}

void ObjectView::SetImage( Image image )
{
  mImageView.SetImage( image );

  DALI_LOG_INFO( gPepperObjectViewLogging, Debug::General, "ObjectView::SetImage: size = %d * %d\n", image.GetWidth(), image.GetHeight() );
}

pid_t ObjectView::GetPid() const
{
  pid_t pid = 0;
  struct wl_client* client;

  if( mSurface )
  {
    client = wl_resource_get_client( pepper_surface_get_resource( mSurface ) );
    if( client )
    {
      wl_client_get_credentials( client, &pid, NULL, NULL );
    }
  }

  DALI_LOG_INFO( gPepperObjectViewLogging, Debug::General, "ObjectView::GetPid: pid = %d\n", pid );

  return pid;
}

std::string ObjectView::GetTitle() const
{
  std::string title;

  if( mSurface )
  {
    Pepper::Internal::ShellClientPtr shellClient = reinterpret_cast< Pepper::Internal::ShellClient* >( pepper_object_get_user_data( reinterpret_cast< pepper_object_t* >( mSurface ), pepper_surface_get_role( mSurface ) ) );
    if( shellClient )
    {
      title = shellClient->GetTitle();
    }
  }

  DALI_LOG_INFO( gPepperObjectViewLogging, Debug::General, "ObjectView::GetTitle: title = %s\n", title.c_str() );

  return title;
}

std::string ObjectView::GetAppId() const
{
  std::string appId;

  if( mSurface )
  {
    Pepper::Internal::ShellClientPtr shellClient = reinterpret_cast< Pepper::Internal::ShellClient* >( pepper_object_get_user_data( reinterpret_cast< pepper_object_t* >( mSurface ), pepper_surface_get_role( mSurface ) ) );
    if( shellClient )
    {
      appId = shellClient->GetAppId();
    }
  }

  DALI_LOG_INFO( gPepperObjectViewLogging, Debug::General, "ObjectView::GetAppId: app id = %s\n", appId.c_str() );

  return appId;
}

void ObjectView::SetSurface( pepper_surface_t* surface )
{
  mSurface = surface;
}

void ObjectView::OnInitialize()
{
  mImageView = Toolkit::ImageView::New();
  mImageView.SetParentOrigin( ParentOrigin::CENTER );
  mImageView.SetAnchorPoint( AnchorPoint::CENTER );

  Self().Add( mImageView );
  Self().SetResizePolicy( ResizePolicy::FIT_TO_CHILDREN, Dimension::ALL_DIMENSIONS );
}

} // namespace Internal

} // namespace Pepper

} // namespace Dali
