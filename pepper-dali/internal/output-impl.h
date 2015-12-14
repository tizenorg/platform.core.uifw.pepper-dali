#ifndef __PEPPER_DALI_INTERNAL_OUTPUT_H__
#define __PEPPER_DALI_INTERNAL_OUTPUT_H__

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
#include <pepper-dali/internal/output.h>
#include <pepper-dali/internal/object.h>
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/signals/connection-tracker.h>

// EXTERNAL INCLUDES
#include <pepper-output-backend.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Output;
typedef IntrusivePtr<Output> OutputPtr;

class Output : public BaseObject, public ConnectionTracker
{
public:

  static OutputPtr New( Pepper::Compositor compositor, Application application, Pepper::Input input );

  Vector2 GetSize();

  void OnResize( Application& application );

  void OnDestroy();
  void OnAssignPlanes( const pepper_list_t* viewList );
  void OnStartRepaintLoop();
  void OnAttachSurface( pepper_surface_t* surface, int* width, int* height );

public: //Signals

  Pepper::Output::OutputSignalType& ObjectAddedSignal();
  Pepper::Output::OutputSignalType& ObjectDeletedSignal();

private:

  /**
   * Construct a new Output.
   */
  Output();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~Output();

  /**
   * Second-phase constructor. Must be called immediately after creating a new Output;
   */
  void Initialize( Pepper::Compositor& compositor, Application application, Pepper::Input input );

  void OnObjectAdded( Pepper::Object object, Toolkit::ImageView imageView );
  void OnObjectDeleted( Pepper::Object object, Toolkit::ImageView imageView );

private:

  // Undefined
  Output( const Output& );

  // Undefined
  Output& operator=( const Output& rhs );

private: // Data

  Pepper::Compositor mCompositor;

  Vector2 mSize;

  Pepper::Input mInput;
  std::vector<Pepper::Object> mObjectList;

  pepper_output_t* mOutput;
  pepper_plane_t* mPrimaryPlane;

  // Signals
  Pepper::Output::OutputSignalType mObjectAddedSignal;
  Pepper::Output::OutputSignalType mObjectDeletedSignal;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::Output& GetImplementation( Pepper::Output& output )
{
  DALI_ASSERT_ALWAYS( output && "Output handle is empty" );

  BaseObject& handle = output.GetBaseObject();

  return static_cast< Internal::Output& >( handle );
}

inline const Pepper::Internal::Output& GetImplementation( const Pepper::Output& output )
{
  DALI_ASSERT_ALWAYS( output && "Output handle is empty" );

  const BaseObject& handle = output.GetBaseObject();

  return static_cast< const Internal::Output& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __PEPPER_DALI_INTERNAL_OUTPUT_H__
