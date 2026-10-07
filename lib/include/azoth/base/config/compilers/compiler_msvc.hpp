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

#ifndef _MSC_VER
#error This file should only be included on MSVC.
#endif

#define AZOTH_COMPILER_MSVC	   1
#define AZOTH_COMPILER_VERSION (_MSC_VER)

// MSVC reports a stale __cplusplus unless /Zc:__cplusplus is set, so gate on _MSVC_LANG instead.
#if _MSVC_LANG < 202302L
#error Azoth requires C++23.
#endif

#if defined(_CPPUNWIND)
#define AZOTH_COMPILER_EXCEPTIONS 1
#else
#define AZOTH_COMPILER_EXCEPTIONS 0
#endif
#if defined(_CPPRTTI)
#define AZOTH_COMPILER_RTTI 1
#else
#define AZOTH_COMPILER_RTTI 0
#endif
