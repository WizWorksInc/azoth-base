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

// Example usage:
//     printf("Line: %s", AZO_STRINGIFY(__LINE__));
//
// Stringification of an argument is a preprocessor operation, no function can perform it.
#ifndef AZO_STRINGIFY
#define AZO_STRINGIFY(x)     AZO_STRINGIFY_IMPL(x)
#define AZO_STRINGIFY_IMPL(x) #x
#endif
