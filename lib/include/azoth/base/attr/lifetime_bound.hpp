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

// Marks referents that must outlive the returned or constructed value.
#if __has_cpp_attribute(clang::lifetimebound) || (defined(__RESHARPER__) && defined(__clang__))
#define AZO_ATTR_LIFETIME_BOUND [[clang::lifetimebound]]
#elif __has_cpp_attribute(msvc::lifetimebound) || (defined(__RESHARPER__) && defined(_MSC_VER))
#define AZO_ATTR_LIFETIME_BOUND [[msvc::lifetimebound]]
#else
#define AZO_ATTR_LIFETIME_BOUND
#endif
