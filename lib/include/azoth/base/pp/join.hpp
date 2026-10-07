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

// Expand both arguments before token joining (C++98 16.3.1).
#ifndef AZO_PP_JOIN
	#define AZO_PP_JOIN(a, b)  AZO_PP_JOIN1(a, b)
	#define AZO_PP_JOIN1(a, b) AZO_PP_JOIN2(a, b)
	#define AZO_PP_JOIN2(a, b) a##b
#endif
