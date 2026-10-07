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

// Suppresses ReSharper's pass by const reference inspection for a class or struct.
#ifdef __RESHARPER__
#define AZO_IDE_ATTR_PASS_BY_VALUE [[rscpp::pass_by_value]]
#else
#define AZO_IDE_ATTR_PASS_BY_VALUE
#endif
