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

// Marks an automatic variable as intentionally indeterminate.
#if __has_cpp_attribute(indeterminate) && ((defined(_MSVC_LANG) && _MSVC_LANG > 202302L) || __cplusplus > 202302L)
#define AZO_ATTR_INDETERMINATE [[indeterminate]]
#elif __has_cpp_attribute(clang::uninitialized) || (defined(__RESHARPER__) && defined(__clang__))
#define AZO_ATTR_INDETERMINATE [[clang::uninitialized]]
#elifdef __GNUC__
#if __has_attribute(uninitialized)
#define AZO_ATTR_INDETERMINATE __attribute__((uninitialized))
#else
#define AZO_ATTR_INDETERMINATE
#endif
#else
#define AZO_ATTR_INDETERMINATE
#endif
