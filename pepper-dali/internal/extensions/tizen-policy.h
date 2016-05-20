#ifndef __DALI_PEPPER_INTERNAL_EXTENSIONS_TIZEN_POLICY_H__
#define __DALI_PEPPER_INTERNAL_EXTENSIONS_TIZEN_POLICY_H__

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

#include <pepper.h>

namespace Dali
{

namespace Pepper
{

namespace Internal
{

namespace Extension
{

enum VisibilityType
{
   UNOBSCURED = 0,
   PARTIALLY_OBSCURED,
   FULLY_OBSCURED
};

bool TizenPolicyInit( pepper_compositor_t* compositor );
void TizenPolicyShutdown();
bool TizenPolicySetVisibility( pepper_surface_t* psurf, VisibilityType type );

} // namespace Extension

} // namespace Internal

} // namespace Pepper

} // namespace Dali

#endif // __DALI_PEPPER_INTERNAL_EXTENSIONS_TIZEN_POLICY_H__
