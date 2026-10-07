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

#if !defined(AZOTH_CONFIG_NO_FORCE_INLINE) && __has_cpp_attribute(gnu::always_inline) // NOLINT(*-avoid-unconditional-preprocessor-if)
#define AZO_FORCE_INLINE_LAMBDA [[gnu::always_inline]]
#else
#define AZO_FORCE_INLINE_LAMBDA
#endif
