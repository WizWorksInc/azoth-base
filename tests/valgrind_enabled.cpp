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

#include "azoth/base/valgrind.hpp"

#include <array>
#include <memory>

namespace
{
	struct FiberStack
	{
		AZO_VALGRIND_ID

		FiberStack()							   = default;
		FiberStack(const FiberStack &)			   = delete;
		FiberStack & operator=(const FiberStack &) = delete;
		FiberStack(FiberStack &&)				   = delete;
		FiberStack & operator=(FiberStack &&)	   = delete;

		void register_stack(void * start, void * end)
		{
			AZO_VALGRIND_REGISTER(start, end);
			AZO_VALGRIND_HG_DISABLE_CHECKING(start, 1);
		}

		void swap(FiberStack & other) noexcept
		{
			AZO_VALGRIND_SWAP(*this, other);
		}

		~FiberStack()
		{
			AZO_VALGRIND_DEREGISTER();
		}
	};

	[[maybe_unused]] void exercise_fiber_stack()
	{
		std::array<char, 64> memory{};
		FiberStack first;
		FiberStack second;
		first.register_stack(memory.data(), std::to_address(memory.end()));
		first.swap(second);
	}
} // namespace
