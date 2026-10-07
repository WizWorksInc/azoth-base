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

// __builtin_expect must land on the branch condition itself,
// a function wrapper loses the hint wherever it is not inlined.
#if defined(__clang__) || defined(__GNUC__)
#define AZO_UNLIKELY(condition) __builtin_expect(!!(condition), 0)
#else
#define AZO_UNLIKELY(condition) (condition)
#endif
