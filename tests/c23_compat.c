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

#include "azoth/base/c23_compat.h"

static_assert(sizeof(bool) == 1);
static_assert(true && !false, "true and false");
static_assert(alignof(int) == sizeof(int));

struct aligned_block
{
	alignas(16) unsigned char bytes[16];
};

static thread_local int counter;

AZO_C23_NODISCARD static int next(void)
{
	return ++counter;
}

static int classify(int value)
{
	switch (value)
	{
	case 0:	 value = 1; AZO_C23_FALLTHROUGH;
	case 1:	 return value;
	default: return -1;
	}
}

int main(void)
{
	AZO_C23_MAYBE_UNUSED struct aligned_block block = { { 0 } };
#if AZOTH_C23_HAS_TYPEOF
	typeof(counter) copy = next();
#else
	int copy = next();
#endif
	return (copy == 1 && classify(0) == 1 && classify(7) == -1) ? 0 : 1;
}
