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


// CPU architecture.
#if defined(__x86_64__) || defined(_M_X64)
	#define AZOTH_CPU_X64 1
	// x86-64 code may use AVX2, FMA, and F16C directly (minimum-hardware-spec.md section 7). MSVC implies FMA and F16C with /arch:AVX2.
	#if !defined(__AVX2__) || ((!defined(__FMA__) || !defined(__F16C__)) && !(defined(_MSC_VER) && !defined(__clang__)))
		#error x86-64 builds require AVX2, FMA, and F16C. Build with -mavx2 -mfma -mf16c or /arch:AVX2 (azoth/cmake/azoth_arch.cmake).
	#endif
#elif defined(__i386) || defined(_M_IX86) || defined(__arm__)
#error 32-bit platforms are not supported.
#elif defined(__aarch64__) || defined(_M_ARM64)
#define AZOTH_CPU_ARM 1
#else
#error Unknown CPU architecture
#endif

#if defined(_WIN32) || defined(_WIN64)
#define AZOTH_OS_WINDOWS 1
#elifdef __APPLE__
#define AZOTH_OS_APPLE 1
#define AZOTH_OS_POSIX 1
#include "TargetConditionals.h"
#if TARGET_OS_OSX
#define AZOTH_OS_MAC 1
#elif TARGET_OS_IPHONE
		#define AZOTH_OS_IOS 1
#else
#error Unknown Apple platform
#endif
#elifdef __ANDROID__
#define AZOTH_OS_ANDROID 1
#define AZOTH_OS_LINUX	 1
#define AZOTH_OS_POSIX	 1
#elifdef __linux__
#define AZOTH_OS_LINUX 1
#define AZOTH_OS_POSIX 1
#elifdef __FreeBSD__
#define AZOTH_OS_FREEBSD 1
#define AZOTH_OS_POSIX	 1
#else
#error Unknown operating system
#endif

#ifdef _MSC_VER
	// ReSharper disable once CppUnusedIncludeDirective
	#include "azoth/base/config/compilers/compiler_msvc.hpp"
#elifdef __clang__
// ReSharper disable once CppUnusedIncludeDirective
#include "azoth/base/config/compilers/compiler_clang.hpp"
#elifdef __GNUC__
// ReSharper disable once CppUnusedIncludeDirective
#include "azoth/base/config/compilers/compiler_gcc.hpp"
#else
#error Unknown compiler
#endif

// Endianness.
// NOLINTNEXTLINE: Don't assume this is always true.
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define AZOTH_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define AZOTH_BIG_ENDIAN 1
#elif defined(_WIN32) || defined(__x86_64__) || defined(__aarch64__) || defined(_M_ARM64)
// Every desktop and console target azoth supports is little endian.
#define AZOTH_LITTLE_ENDIAN 1
#else
#error Unable to determine endianness
#endif

// Inferred from NDEBUG unless the build pins one, so it always agrees with assert in every includer and configuration.
#if !defined(AZOTH_CONFIG_DEBUG) && !defined(AZOTH_CONFIG_RELEASE)
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#define AZOTH_CONFIG_DEBUG
#else
#define AZOTH_CONFIG_RELEASE
#endif
#endif

// These pointer-width flags support #if, where sizeof(void *) is unavailable.
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(__LP64__) || defined(_M_ARM64)
#define AZOTH_64BIT 1
#define AZOTH_32BIT 0
#else
#define AZOTH_64BIT 0
#define AZOTH_32BIT 1
#endif

// Every flag the chains above left unset is off.
#ifndef AZOTH_CPU_X64
#define AZOTH_CPU_X64 0
#endif
#ifndef AZOTH_CPU_ARM
#define AZOTH_CPU_ARM 0
#endif
#ifndef AZOTH_OS_WINDOWS
#define AZOTH_OS_WINDOWS 0
#endif
#ifndef AZOTH_OS_APPLE
#define AZOTH_OS_APPLE 0
#endif
#ifndef AZOTH_OS_MAC
#define AZOTH_OS_MAC 0
#endif
#ifndef AZOTH_OS_IOS
	#define AZOTH_OS_IOS 0
#endif
#ifndef AZOTH_OS_ANDROID
#define AZOTH_OS_ANDROID 0
#endif
#ifndef AZOTH_OS_LINUX
#define AZOTH_OS_LINUX 0
#endif
#ifndef AZOTH_OS_FREEBSD
#define AZOTH_OS_FREEBSD 0
#endif
#ifndef AZOTH_OS_POSIX
#define AZOTH_OS_POSIX 0
#endif
#ifndef AZOTH_COMPILER_MSVC
#define AZOTH_COMPILER_MSVC 0
#endif
#ifndef AZOTH_COMPILER_CLANG
#define AZOTH_COMPILER_CLANG 0
#endif
#ifndef AZOTH_COMPILER_GCC
#define AZOTH_COMPILER_GCC 0
#endif
#ifndef AZOTH_LITTLE_ENDIAN
#define AZOTH_LITTLE_ENDIAN 0
#endif
#ifndef AZOTH_BIG_ENDIAN
#define AZOTH_BIG_ENDIAN 0
#endif


namespace azo::platform
{
    [[nodiscard]] constexpr bool is_windows() noexcept
    {
        return AZOTH_OS_WINDOWS;
    }

    [[nodiscard]] constexpr bool is_apple() noexcept
    {
        return AZOTH_OS_APPLE;
    }

    [[nodiscard]] constexpr bool is_mac() noexcept
    {
        return AZOTH_OS_MAC;
    }

    [[nodiscard]] constexpr bool is_linux() noexcept
    {
        return AZOTH_OS_LINUX;
    }

    [[nodiscard]] constexpr bool is_free_bsd() noexcept
    {
        return AZOTH_OS_FREEBSD;
    }

    [[nodiscard]] constexpr bool is_posix() noexcept
    {
        return AZOTH_OS_POSIX;
    }

    [[nodiscard]] constexpr bool is_64bit() noexcept
    {
        return AZOTH_64BIT;
    }

    [[nodiscard]] constexpr bool is_debug_build() noexcept
    {
#ifdef AZOTH_CONFIG_DEBUG
        return true;
#else
        return false;
#endif
    }
} // namespace azo::platform
