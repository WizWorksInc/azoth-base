// Copyright 2026 Ian Pike
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "azoth/base/pp/stringify.hpp"

#if defined(__GNUC__) && !defined(__clang__)
#define AZO_DISABLE_GCC_WARNING(warningOption)		   _Pragma("GCC diagnostic push") _Pragma(AZO_STRINGIFY(GCC diagnostic ignored warningOption))
#define AZO_RESTORE_GCC_WARNING()					   _Pragma("GCC diagnostic pop")
#define AZO_ENABLE_GCC_WARNING_AS_ERROR(warningOption) _Pragma("GCC diagnostic push") _Pragma(AZO_STRINGIFY(GCC diagnostic error warningOption))
#define AZO_DISABLE_GCC_WARNING_AS_ERROR()			   _Pragma("GCC diagnostic pop")
#else
#define AZO_DISABLE_GCC_WARNING(warningOption)
#define AZO_RESTORE_GCC_WARNING()
#define AZO_ENABLE_GCC_WARNING_AS_ERROR(warningOption)
#define AZO_DISABLE_GCC_WARNING_AS_ERROR()
#endif
