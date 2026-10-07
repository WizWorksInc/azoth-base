# Copyright 2026 Ian Pike
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.24)

set(AZOTH_BASE_BUILD_VERSION 1.0.0)

project(AzothBase
        VERSION ${AZOTH_BASE_BUILD_VERSION}
        DESCRIPTION "Platform detection, compiler attributes and preprocessor helpers shared by the Azoth libraries"
        HOMEPAGE_URL "https://github.com/WizWorksInc/azoth-base"
        LANGUAGES CXX
)

set(AZOTH_BASE_IS_TOP_LEVEL ${PROJECT_IS_TOP_LEVEL})

option(AZOTH_BASE_BUILD_TESTS "Build the test suite" ${AZOTH_BASE_IS_TOP_LEVEL})
option(AZOTH_BASE_INSTALL "Generate the install and export rules" ${AZOTH_BASE_IS_TOP_LEVEL})
option(AZOTH_BASE_ENABLE_CLANG_TIDY "Run clang-tidy over the headers while building the tests" OFF)

include(GNUInstallDirs)

set(AZOTH_BASE_HEADERS
        c23_compat.h
        sanitizers.hpp
        valgrind.hpp
        attr/align_func.hpp
        attr/export.hpp
        attr/flag_enum.hpp
        attr/force_inline.hpp
        attr/force_inline_lambda.hpp
        attr/import.hpp
        attr/indeterminate.hpp
        attr/lifetime_bound.hpp
        attr/likely.hpp
        attr/no_inline.hpp
        attr/no_sanitize_address.hpp
        attr/no_sanitize_undefined.hpp
        attr/no_unique_address.hpp
        attr/packed.hpp
        attr/pure.hpp
        attr/restrict.hpp
        attr/unlikely.hpp
        attr/ide/rs_format.hpp
        attr/ide/rs_guard.hpp
        attr/ide/rs_has_side_effects.hpp
        attr/ide/rs_pass_by_value.hpp
        attr/ide/rs_used_implicitly.hpp
        config/platform.hpp
        config/threading.hpp
        config/compilers/compiler_clang.hpp
        config/compilers/compiler_gcc.hpp
        config/compilers/compiler_msvc.hpp
        pp/discard.hpp
        pp/helpers.hpp
        pp/join.hpp
        pp/pack.hpp
        pp/stringify.hpp
        pp/warning_clang.hpp
        pp/warning_gcc.hpp
        pp/warning_msvc.hpp
)
list(TRANSFORM AZOTH_BASE_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/azoth/base/")

add_library(AzothBase INTERFACE)
add_library(azoth::base ALIAS AzothBase)
set_target_properties(AzothBase PROPERTIES EXPORT_NAME base)
target_compile_features(AzothBase INTERFACE cxx_std_23)
target_include_directories(AzothBase INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_sources(AzothBase INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/include
        FILES ${AZOTH_BASE_HEADERS}
)

# config/platform.hpp refuses x86-64 without AVX2, FMA and F16C.
if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    set(AZOTH_BASE_TARGET_ARCHS ${CMAKE_OSX_ARCHITECTURES})
else()
    set(AZOTH_BASE_TARGET_ARCHS ${CMAKE_SYSTEM_PROCESSOR})
endif()
list(LENGTH AZOTH_BASE_TARGET_ARCHS _arch_count)
foreach(_arch IN LISTS AZOTH_BASE_TARGET_ARCHS)
    if(NOT _arch MATCHES "^(x86_64|AMD64|amd64)$")
        continue()
    endif()
    if(MSVC)
        target_compile_options(AzothBase INTERFACE /arch:AVX2)
    elseif(_arch_count GREATER 1)
        target_compile_options(AzothBase INTERFACE
                "SHELL:-Xarch_${_arch} -mavx2" "SHELL:-Xarch_${_arch} -mfma" "SHELL:-Xarch_${_arch} -mf16c")
    else()
        target_compile_options(AzothBase INTERFACE -mavx2 -mfma -mf16c)
    endif()
endforeach()

if(AZOTH_BASE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(AZOTH_BASE_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS AzothBase
            EXPORT AzothBaseTargets
            FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT AzothBaseTargets
            FILE AzothBaseTargets.cmake
            NAMESPACE azoth::
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/AzothBase
    )

    configure_package_config_file(
            cmake/AzothBaseConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/AzothBaseConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/AzothBase
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/AzothBaseConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMinorVersion
            ARCH_INDEPENDENT
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/AzothBaseConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/AzothBaseConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/AzothBase
    )

    install(FILES LICENSE
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/AzothBase
    )
endif()
