#ifndef __DALI_PEPPER_INTERNAL_OBJECT_H__
#define __DALI_PEPPER_INTERNAL_OBJECT_H__

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

// INTERNAL INCLUDES
#include <pepper-dali/internal/object.h>
#include <pepper-dali/public-api/object-view/object-view.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/images/buffer-image.h>
#include <dali/public-api/images/native-image.h>
#include <tbm_surface.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

class Object;
typedef IntrusivePtr<Object> ObjectPtr;

class Object : public BaseObject, public ConnectionTracker
{
public:

  static ObjectPtr New( pepper_surface_t* surface, Pepper::Input input );

  bool AttchBuffer( int* width, int* height );

public: //Signals

  Pepper::Object::ObjectSignalType& ObjectViewAddedSignal();
  Pepper::Object::ObjectSignalType& ObjectViewDeletedSignal();

private:

  /**
   * Construct a new Object.
   */
  Object();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~Object();

  /**
   * Second-phase constructor. Must be called immediately after creating a new Object;
   */
  void Initialize( pepper_surface_t* surface, Pepper::Input input );

  bool OnTouched( Actor actor, const TouchEvent& touchEvent );

  static void OnDestroySurface( pepper_event_listener_t* listener, pepper_object_t* object, uint32_t id, void* info, void* data );
  static void OnDestroyBuffer( pepper_event_listener_t* listener, pepper_object_t* object, uint32_t id, void* info, void* data );

private:

  // Undefined
  Object( const Object& );

  // Undefined
  Object& operator=( const Object& rhs );

private: // Data

  pepper_surface_t* mSurface;
  pepper_buffer_t* mBuffer;

  pepper_pointer_t* mPointer;
  pepper_keyboard_t* mKeyboard;
  pepper_touch_t* mTouch;

  wl_shm_buffer* mShmBuffer;
  tbm_surface_h mTbmSurface;

  pepper_event_listener_t* mSurfaceDestroyListener;
  pepper_event_listener_t* mBufferDestroyListener;

  Pepper::ObjectView mObjectView;
  BufferImage mBufferImage;
  NativeImage mNativeImage;

  int mWidth;
  int mHeight;

  // Signals
  Pepper::Object::ObjectSignalType mObjectViewAddedSignal;
  Pepper::Object::ObjectSignalType mObjectViewDeletedSignal;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Pepper::Internal::Object& GetImplementation( Pepper::Object& object )
{
  DALI_ASSERT_ALWAYS( object && "Object handle is empty" );

  BaseObject& handle = object.GetBaseObject();

  return static_cast< Internal::Object& >( handle );
}

inline const Pepper::Internal::Object& GetImplementation( const Pepper::Object& object )
{
  DALI_ASSERT_ALWAYS( object && "Object handle is empty" );

  const BaseObject& handle = object.GetBaseObject();

  return static_cast< const Internal::Object& >( handle );
}

} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_INTERNAL_OBJECT_H__
