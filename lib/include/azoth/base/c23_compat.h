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

#ifndef AZOTH_BASE_C23_COMPAT_H
#define AZOTH_BASE_C23_COMPAT_H

// AZOTH_CONFIG_C23_NO_EXTENSIONS disables compiler-specific fallbacks.
#ifdef __cplusplus
	#define AZOTH_C23_HAS_NATIVE_BOOL	1
	#define AZOTH_C23_HAS_NULLPTR		1
	#define AZOTH_C23_HAS_CONSTEXPR		1
	#define AZOTH_C23_HAS_AUTO			1
	#define AZOTH_C23_HAS_THREAD_LOCAL	1
	#define AZOTH_C23_HAS_BITINT		0
	#define AZOTH_C23_HAS_TYPEOF		0
	#define AZOTH_C23_HAS_TYPEOF_UNQUAL 0
#else
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201710L
#error This header requires C17 or newer.
#endif

#ifdef __is_identifier
#define AZOTH_C23_DETAIL_NATIVE(keyword) (!__is_identifier(keyword))
#elif __STDC_VERSION__ >= 202311L
#define AZOTH_C23_DETAIL_NATIVE(keyword) 1
#else
#define AZOTH_C23_DETAIL_NATIVE(keyword) 0
#endif

#ifdef __is_identifier
#if __is_identifier(bool)
#define AZOTH_C23_HAS_NATIVE_BOOL 0
#else
#define AZOTH_C23_HAS_NATIVE_BOOL 1
#endif
#elif __STDC_VERSION__ >= 202311L
#define AZOTH_C23_HAS_NATIVE_BOOL 1
#else
#define AZOTH_C23_HAS_NATIVE_BOOL 0
#endif

#if !AZOTH_C23_HAS_NATIVE_BOOL
#ifndef bool
#define bool _Bool
#endif
#endif

// C17 true and false retain the integer type used by stdbool.h.
#ifndef true
#if !AZOTH_C23_DETAIL_NATIVE(true)
#define true 1
#endif
#endif

#ifndef false
#if !AZOTH_C23_DETAIL_NATIVE(false)
#define false 0
#endif
#endif

#if !AZOTH_C23_DETAIL_NATIVE(alignas)
#ifndef alignas
#define alignas _Alignas
#endif
#endif

#if !AZOTH_C23_DETAIL_NATIVE(alignof)
#ifndef alignof
#define alignof _Alignof
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__) && _MSC_VER < 1938
#ifdef AZOTH_CONFIG_C23_NO_EXTENSIONS
#define AZOTH_C23_HAS_THREAD_LOCAL 0
#else
#define AZOTH_C23_HAS_THREAD_LOCAL 1
#ifndef thread_local
#define thread_local __declspec(thread)
#endif
#endif
#else
#define AZOTH_C23_HAS_THREAD_LOCAL 1
#endif

#ifndef thread_local
#if AZOTH_C23_HAS_THREAD_LOCAL && !AZOTH_C23_DETAIL_NATIVE(thread_local)
#define thread_local _Thread_local
#endif
#endif

#if __STDC_VERSION__ == 201710L
#define AZOTH_C23_DETAIL_ASSERT_ADAPTER 1
#elif defined(__is_identifier) // NOLINT(readability-use-concise-preprocessor-directives)
#define AZOTH_C23_DETAIL_ASSERT_ADAPTER __is_identifier(static_assert)
#else
#define AZOTH_C23_DETAIL_ASSERT_ADAPTER (__STDC_VERSION__ < 202311L)
#endif

#if AZOTH_C23_DETAIL_ASSERT_ADAPTER
#include <assert.h>
#ifdef static_assert
#undef static_assert
#endif
#define AZOTH_C23_DETAIL_ASSERT_1(condition)						 _Static_assert(condition, #condition)
#define AZOTH_C23_DETAIL_ASSERT_2(condition, message)				 _Static_assert(condition, message)
#define AZOTH_C23_DETAIL_ASSERT_SELECT(first, second, selected, ...) selected
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif
#define static_assert(...) AZOTH_C23_DETAIL_ASSERT_SELECT(__VA_ARGS__, AZOTH_C23_DETAIL_ASSERT_2, AZOTH_C23_DETAIL_ASSERT_1, unused)(__VA_ARGS__)
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
#undef AZOTH_C23_DETAIL_ASSERT_ADAPTER

#if AZOTH_C23_DETAIL_NATIVE(nullptr)
#define AZOTH_C23_HAS_NULLPTR 1
#else
#define AZOTH_C23_HAS_NULLPTR 0
#endif

#if AZOTH_C23_DETAIL_NATIVE(constexpr)
#define AZOTH_C23_HAS_CONSTEXPR 1
#else
#define AZOTH_C23_HAS_CONSTEXPR 0
#endif

// Draft C23 modes may still lack type inference.
#if __STDC_VERSION__ >= 202311L
#define AZOTH_C23_HAS_AUTO 1
#else
#define AZOTH_C23_HAS_AUTO 0
#endif

#if __STDC_VERSION__ >= 202000L && defined(__BITINT_MAXWIDTH__)
#define AZOTH_C23_HAS_BITINT 1
#else
#define AZOTH_C23_HAS_BITINT 0
#endif

#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 193933428
#define AZOTH_C23_DETAIL_MSVC_TYPEOF 1
#else
#define AZOTH_C23_DETAIL_MSVC_TYPEOF 0
#endif

#if AZOTH_C23_DETAIL_NATIVE(typeof)
#define AZOTH_C23_HAS_TYPEOF 1
#elif !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && (defined(__GNUC__) || defined(__clang__) || AZOTH_C23_DETAIL_MSVC_TYPEOF)
#define AZOTH_C23_HAS_TYPEOF 1
#ifndef typeof
#define typeof __typeof__
#endif
#else
#define AZOTH_C23_HAS_TYPEOF 0
#endif

#if AZOTH_C23_DETAIL_NATIVE(typeof_unqual)
#define AZOTH_C23_HAS_TYPEOF_UNQUAL 1
#elif !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && defined(__clang__)
#if !__is_identifier(__typeof_unqual__)
#define AZOTH_C23_HAS_TYPEOF_UNQUAL 1
#ifndef typeof_unqual
#define typeof_unqual __typeof_unqual__
#endif
#else
#define AZOTH_C23_HAS_TYPEOF_UNQUAL 0
#endif
#elif !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && ((defined(__GNUC__) && __GNUC__ >= 13) || AZOTH_C23_DETAIL_MSVC_TYPEOF)
#define AZOTH_C23_HAS_TYPEOF_UNQUAL 1
#ifndef typeof_unqual
#define typeof_unqual __typeof_unqual__
#endif
#else
#define AZOTH_C23_HAS_TYPEOF_UNQUAL 0
#endif

#undef AZOTH_C23_DETAIL_NATIVE
#undef AZOTH_C23_DETAIL_MSVC_TYPEOF
#endif

#if defined(__cplusplus) && defined(__has_cpp_attribute)
	#define AZOTH_C23_DETAIL_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#elif !defined(__cplusplus) && __STDC_VERSION__ >= 202000L && defined(__has_c_attribute)
#define AZOTH_C23_DETAIL_ATTRIBUTE(attribute) __has_c_attribute(attribute)
#else
#define AZOTH_C23_DETAIL_ATTRIBUTE(attribute) 0
#endif

#if !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && defined(__has_attribute)
	#define AZOTH_C23_DETAIL_GNU_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#define AZOTH_C23_DETAIL_GNU_ATTRIBUTE(attribute) 0
#endif

#if AZOTH_C23_DETAIL_ATTRIBUTE(nodiscard)
#define AZO_C23_NODISCARD [[nodiscard]]
#elif AZOTH_C23_DETAIL_GNU_ATTRIBUTE(warn_unused_result)
#define AZO_C23_NODISCARD __attribute__((warn_unused_result))
#else
#define AZO_C23_NODISCARD
#endif

#if AZOTH_C23_DETAIL_ATTRIBUTE(maybe_unused)
#define AZO_C23_MAYBE_UNUSED [[maybe_unused]]
#elif AZOTH_C23_DETAIL_GNU_ATTRIBUTE(unused)
#define AZO_C23_MAYBE_UNUSED __attribute__((unused))
#else
#define AZO_C23_MAYBE_UNUSED
#endif

#if AZOTH_C23_DETAIL_ATTRIBUTE(deprecated)
	#define AZO_C23_DEPRECATED(message) [[deprecated(message)]]
#elif AZOTH_C23_DETAIL_GNU_ATTRIBUTE(deprecated)
#define AZO_C23_DEPRECATED(message) __attribute__((deprecated(message)))
#elif !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && defined(_MSC_VER)
#define AZO_C23_DEPRECATED(message) __declspec(deprecated(message))
#else
#define AZO_C23_DEPRECATED(message)
#endif

#if AZOTH_C23_DETAIL_ATTRIBUTE(fallthrough)
#define AZO_C23_FALLTHROUGH [[fallthrough]]
#elif AZOTH_C23_DETAIL_GNU_ATTRIBUTE(fallthrough)
#define AZO_C23_FALLTHROUGH __attribute__((fallthrough))
#else
#define AZO_C23_FALLTHROUGH ((void)0)
#endif

#if AZOTH_C23_DETAIL_ATTRIBUTE(noreturn)
#define AZO_C23_NORETURN [[noreturn]]
#elif defined(__cplusplus) // NOLINT(readability-use-concise-preprocessor-directives)
#define AZO_C23_NORETURN [[noreturn]]
#else
#define AZO_C23_NORETURN _Noreturn
#endif

#if !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
		#define AZO_C23_UNREACHABLE() __builtin_unreachable()
#endif
#endif

#ifndef AZO_C23_UNREACHABLE
#if !defined(AZOTH_CONFIG_C23_NO_EXTENSIONS) && defined(_MSC_VER)
#define AZO_C23_UNREACHABLE() __assume(0)
#else
#include <stdlib.h>
#define AZO_C23_UNREACHABLE() abort()
#endif
#endif

#undef AZOTH_C23_DETAIL_ATTRIBUTE
#undef AZOTH_C23_DETAIL_GNU_ATTRIBUTE

#endif
