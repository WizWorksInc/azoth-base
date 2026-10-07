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

#include "azoth/base/attr/ide/rs_guard.hpp"

#include <cstddef>

#ifdef __has_feature
#if __has_feature(thread_sanitizer) && !defined(AZOTH_TSAN_ENABLED)
#define AZOTH_TSAN_ENABLED 1
#endif
#if __has_feature(address_sanitizer) && !defined(AZOTH_ASAN_ENABLED)
#define AZOTH_ASAN_ENABLED 1
#endif
#endif

#if defined(__SANITIZE_THREAD__) && !defined(AZOTH_TSAN_ENABLED)
#define AZOTH_TSAN_ENABLED 1
#endif

#if defined(__SANITIZE_ADDRESS__) && !defined(AZOTH_ASAN_ENABLED)
#define AZOTH_ASAN_ENABLED 1
#endif

#ifndef AZOTH_TSAN_ENABLED
#define AZOTH_TSAN_ENABLED 0
#endif
#ifndef AZOTH_ASAN_ENABLED
#define AZOTH_ASAN_ENABLED 0
#endif

#if AZOTH_TSAN_ENABLED
#include <sanitizer/tsan_interface.h>

// Some TSan runtimes export these functions without declaring them in the public header.
extern "C" {
void __tsan_ignore_thread_begin(void);
void __tsan_ignore_thread_end(void);
}
#endif

#if AZOTH_ASAN_ENABLED
#include <sanitizer/asan_interface.h>
#endif

namespace azo::sanitizer
{
	/**
	 * \brief TSan release edge. Pair with happens_after on the same address when custom synchronization publishes earlier work.
	 */
	inline void happens_before([[maybe_unused]] void* addr)
    {
#if AZOTH_TSAN_ENABLED
        __tsan_release(addr);
#endif
	}

	/**
	 * \brief TSan acquire edge. Pair with happens_before on the same address when observing another thread's work.
	 */
	inline void happens_after([[maybe_unused]] void* addr)
    {
#if AZOTH_TSAN_ENABLED
        __tsan_acquire(addr);
#endif
    }

    inline void acquire([[maybe_unused]] void* addr)
    {
#if AZOTH_TSAN_ENABLED
        __tsan_acquire(addr);
#endif
    }

    inline void release([[maybe_unused]] void* addr)
    {
#if AZOTH_TSAN_ENABLED
        __tsan_release(addr);
#endif
    }

    inline void poison_memory([[maybe_unused]] const void* addr, [[maybe_unused]] std::size_t size)
    {
#if AZOTH_ASAN_ENABLED
        __asan_poison_memory_region(addr, size);
#endif
    }

    inline void unpoison_memory([[maybe_unused]] const void* addr, [[maybe_unused]] std::size_t size)
    {
#if AZOTH_ASAN_ENABLED
        __asan_unpoison_memory_region(addr, size);
#endif
    }

    /**
     * \brief Suppress TSan reports for this thread while in scope. Prefer acquire/release annotations for real synchronization.
     */
    class AZO_IDE_ATTR_GUARD ScopedIgnoreThread
    {
    public:
        ScopedIgnoreThread() noexcept
        {
#if AZOTH_TSAN_ENABLED
            __tsan_ignore_thread_begin();
#endif
        }

        ~ScopedIgnoreThread()
        {
#if AZOTH_TSAN_ENABLED
            __tsan_ignore_thread_end();
#endif
        }

        ScopedIgnoreThread(const ScopedIgnoreThread&) = delete;
        ScopedIgnoreThread& operator=(const ScopedIgnoreThread&) = delete;
        ScopedIgnoreThread(ScopedIgnoreThread&&) = delete;
        ScopedIgnoreThread& operator=(ScopedIgnoreThread&&) = delete;
    };
} // namespace azo::sanitizer
