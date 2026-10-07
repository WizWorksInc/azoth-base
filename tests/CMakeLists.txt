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

enable_language(C)

set(CMAKE_CXX_EXTENSIONS OFF)

# Headers have no translation units of their own, so tidy runs over the tests, one of which includes each header.
if(AZOTH_BASE_ENABLE_CLANG_TIDY)
    find_program(AZOTH_BASE_CLANG_TIDY_EXE
            NAMES clang-tidy
            HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
            DOC "Path to clang-tidy"
    )
    if(NOT AZOTH_BASE_CLANG_TIDY_EXE)
        message(FATAL_ERROR "AZOTH_BASE_ENABLE_CLANG_TIDY is ON but clang-tidy was not found. Install LLVM, for example with brew install llvm.")
    endif()
    # The per-header sources are generated in the build tree, where clang-tidy would find no .clang-tidy.
    set(_tidy_args "--config-file=${PROJECT_SOURCE_DIR}/.clang-tidy" "--header-filter=${PROJECT_SOURCE_DIR}/lib/include/azoth/base/")
    if(APPLE)
        set(_sdk_path "${CMAKE_OSX_SYSROOT}")
        if(NOT _sdk_path)
            execute_process(COMMAND xcrun --show-sdk-path OUTPUT_VARIABLE _sdk_path OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        endif()
        if(_sdk_path)
            list(APPEND _tidy_args "--extra-arg=-isysroot" "--extra-arg=${_sdk_path}")
        endif()
    endif()
    set(CMAKE_CXX_CLANG_TIDY "${AZOTH_BASE_CLANG_TIDY_EXE}" ${_tidy_args})
    message(STATUS "AzothBase: clang-tidy enabled on the tests (${AZOTH_BASE_CLANG_TIDY_EXE})")
endif()

set(_include_root "${PROJECT_SOURCE_DIR}/lib/include")
set(_generated "${CMAKE_CURRENT_BINARY_DIR}/self_contained")

set(_self_contained_sources)
foreach(_header IN LISTS AZOTH_BASE_HEADERS)
    file(RELATIVE_PATH _rel "${_include_root}" "${_header}")
    if(_rel MATCHES "^azoth/base/config/compilers/")
        continue()
    endif()
    string(MAKE_C_IDENTIFIER "${_rel}" _id)
    set(_source "${_generated}/${_id}.cpp")
    file(CONFIGURE OUTPUT "${_source}" CONTENT "#include \"${_rel}\"\n#include \"${_rel}\"\n" @ONLY)
    list(APPEND _self_contained_sources "${_source}")
endforeach()

add_library(azoth_base_self_contained OBJECT ${_self_contained_sources})
if(AZOTH_BASE_ENABLE_CLANG_TIDY)
    # Each source includes its header twice and uses nothing from it, by design.
    set_target_properties(azoth_base_self_contained PROPERTIES
            CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--checks=-misc-include-cleaner,-readability-duplicate-include")
endif()
target_link_libraries(azoth_base_self_contained PRIVATE azoth::base)

add_executable(azoth_base_tests base_tests.cpp)
target_link_libraries(azoth_base_tests PRIVATE azoth::base)
add_test(NAME azoth_base_tests COMMAND azoth_base_tests)

add_library(azoth_base_valgrind OBJECT valgrind_enabled.cpp)
target_link_libraries(azoth_base_valgrind PRIVATE azoth::base)
target_include_directories(azoth_base_valgrind PRIVATE stubs)
target_compile_definitions(azoth_base_valgrind PRIVATE AZOTH_CONFIG_VALGRIND)

foreach(_std 17 23)
    add_executable(azoth_base_c${_std} c23_compat.c)
    target_link_libraries(azoth_base_c${_std} PRIVATE azoth::base)
    set_target_properties(azoth_base_c${_std} PROPERTIES C_STANDARD ${_std} C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
    add_test(NAME azoth_base_c${_std} COMMAND azoth_base_c${_std})
endforeach()
