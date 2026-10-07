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

#ifdef AZOTH_CONFIG_VALGRIND
#include <valgrind/helgrind.h>
#include <valgrind/valgrind.h>

	#include <utility>

	#define AZO_VALGRIND_ID					 unsigned m_stackId{ 0 };
	#define AZO_VALGRIND_REGISTER(s, e)		 m_stackId = VALGRIND_STACK_REGISTER(s, e)
	#define AZO_VALGRIND_DEREGISTER()		 VALGRIND_STACK_DEREGISTER(m_stackId)
	#define AZO_VALGRIND_SWAP(first, second) std::swap((first).m_stackId, (second).m_stackId)

	#define AZO_VALGRIND_HG_DISABLE_CHECKING(s, e) VALGRIND_HG_DISABLE_CHECKING(s, e)
#else
#define AZO_VALGRIND_ID
#define AZO_VALGRIND_REGISTER(s, e)
#define AZO_VALGRIND_DEREGISTER()
#define AZO_VALGRIND_SWAP(first, second)

#define AZO_VALGRIND_HG_DISABLE_CHECKING(s, e)
#endif
