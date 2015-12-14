#ifndef __PEPPER_DALI_INTERNAL_COMPOSITOR_H__
#define __PEPPER_DALI_INTERNAL_COMPOSITOR_H__

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
#include <pepper-dali/public-api/compositor/compositor.h>
#include <pepper-dali/internal/shell.h>
#include <pepper-dali/internal/input.h>
#include <pepper-dali/internal/output.h>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/signals/connection-tracker.h>

// EXTERNAL INCLUDES
#include <pepper.h>
#include <Ecore.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Compositor;
typedef IntrusivePtr<Compositor> CompositorPtr;

class Compositor : public BaseObject, public ConnectionTracker
{
public:

  /**
   * @copydoc Dali::Pepper::Compositor::New()
   */
  static CompositorPtr New( Application application, const std::string& name );

  void* GetCompositorHandle();

public: //Signals

  Pepper::Compositor::CompositorSignalType& ObjectAddedSignal();
  Pepper::Compositor::CompositorSignalType& ObjectDeletedSignal();

private:

  /**
   * Construct a new Compositor.
   */
  Compositor();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~Compositor();

  /**
   * Second-phase constructor. Must be called immediately after creating a new Compositor;
   */
  void Initialize( Application application, const std::string& name );

  void OnObjectAdded( Pepper::Output output, Toolkit::ImageView imageView );
  void OnObjectDeleted( Pepper::Output output, Toolkit::ImageView imageView );

  static Eina_Bool FdReadCallback( void* data, Ecore_Fd_Handler* handler );
  static void FdPrepareCallback( void* data, Ecore_Fd_Handler* handler );

private:

  // Undefined
  Compositor( const Compositor& );

  // Undefined
  Compositor& operator=( const Compositor& rhs );

private: // Data

  pepper_compositor_t* mCompositor;
  wl_display* mDisplay;
  wl_event_loop* mEventLoop;
  Ecore_Fd_Handler* mFdHandler;

  Pepper::Shell mShell;
  Pepper::Input mInput;
  Pepper::Output mOutput;

  std::string mSocketName;

  // Signals
  Pepper::Compositor::CompositorSignalType mObjectAddedSignal;
  Pepper::Compositor::CompositorSignalType mObjectDeletedSignal;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::Compositor& GetImplementation( Pepper::Compositor& compositor )
{
  DALI_ASSERT_ALWAYS( compositor && "Compositor handle is empty" );

  BaseObject& handle = compositor.GetBaseObject();

  return static_cast< Internal::Compositor& >( handle );
}

inline const Pepper::Internal::Compositor& GetImplementation( const Pepper::Compositor& compositor )
{
  DALI_ASSERT_ALWAYS( compositor && "Compositor handle is empty" );

  const BaseObject& handle = compositor.GetBaseObject();

  return static_cast< const Internal::Compositor& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __PEPPER_DALI_INTERNAL_COMPOSITOR_H__
