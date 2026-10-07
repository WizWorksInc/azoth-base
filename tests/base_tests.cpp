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

#include "azoth/base/attr/likely.hpp"
#include "azoth/base/attr/unlikely.hpp"
#include "azoth/base/config/platform.hpp"
#include "azoth/base/config/threading.hpp"
#include "azoth/base/pp/discard.hpp"
#include "azoth/base/pp/join.hpp"
#include "azoth/base/pp/stringify.hpp"
#include "azoth/base/sanitizers.hpp"

#include <cstring>
#include <iostream>

static_assert(AZOTH_CPU_X64 + AZOTH_CPU_ARM == 1);
static_assert(AZOTH_COMPILER_MSVC + AZOTH_COMPILER_CLANG + AZOTH_COMPILER_GCC == 1);
static_assert(AZOTH_LITTLE_ENDIAN + AZOTH_BIG_ENDIAN == 1);
static_assert(AZOTH_64BIT == 1 && AZOTH_32BIT == 0);
static_assert(AZOTH_OS_WINDOWS + AZOTH_OS_APPLE + AZOTH_OS_LINUX + AZOTH_OS_FREEBSD == 1);
static_assert(AZOTH_OS_APPLE == AZOTH_OS_MAC + AZOTH_OS_IOS);
static_assert(AZOTH_OS_POSIX + AZOTH_OS_WINDOWS == 1);
static_assert(AZOTH_WIN32_THREADS + AZOTH_POSIX_THREADS == 1);
static_assert(azo::platform::is_windows() == (AZOTH_OS_WINDOWS == 1));
static_assert(azo::platform::is_posix() == (AZOTH_OS_POSIX == 1));
static_assert(azo::platform::is_64bit());
static_assert(sizeof(void *) == 8);

#define AZOTH_BASE_TEST_VALUE 42
static_assert(AZO_PP_JOIN(AZOTH_BASE_TEST_, VALUE) == 42);

int main()
{
	int failures = 0;

	if (std::strcmp(AZO_STRINGIFY(AZOTH_BASE_TEST_VALUE), "42") != 0)
	{
		std::cout << "AZO_STRINGIFY did not expand its argument\n";
		++failures;
	}

	const bool debug =
#ifdef NDEBUG
		false;
#else
		true;
#endif
	if (azo::platform::is_debug_build() != debug)
	{
		std::cout << "is_debug_build disagrees with NDEBUG\n";
		++failures;
	}

	if (AZO_UNLIKELY(failures != 0) || !AZO_LIKELY(failures == 0))
	{
		return 1;
	}

	int marker = 0;
	azo::sanitizer::happens_before(&marker);
	azo::sanitizer::happens_after(&marker);
	{
		azo::sanitizer::ScopedIgnoreThread ignore;
		AZO_DISCARD(ignore);
	}
	return 0;
}
