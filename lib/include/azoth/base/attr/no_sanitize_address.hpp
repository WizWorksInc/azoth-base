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

#ifdef __clang__
#define AZO_ATTR_NO_SANITIZE_ADDRESS __attribute__((no_sanitize("address")))
#elif defined(_MSC_VER) && defined(__SANITIZE_ADDRESS__)
#define AZO_ATTR_NO_SANITIZE_ADDRESS __declspec(no_sanitize_address)
#elifdef __GNUC__
#define AZO_ATTR_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
#define AZO_ATTR_NO_SANITIZE_ADDRESS
#endif
