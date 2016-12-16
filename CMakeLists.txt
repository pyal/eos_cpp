cmake_minimum_required(VERSION 3.2)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project(eos_cpp)
include(ExternalProject)
enable_testing()
#unset(WCPP_NET)
#add_definitions(-DWCPP_NET)
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -Wl,-z,relro,-z,now")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wno-deprecated-declarations")

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]" OR CMAKE_BUILD_TYPE STREQUAL "")
    message("!!! DEBUG BUILD !!!")
    set(CMAKE_BUILD_TYPE "DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
elseif(CMAKE_BUILD_TYPE MATCHES "[Rr][Ee][Ll][Ee][Aa][Ss][Ee]")
    message("!!! RELEASE BUILD !!!")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
else()
    message(FATAL_ERROR "!!! NEITHER DEBUG NOR RELEASE BUILD !!!")
endif()

#include(re2c)
#include(ragel)

find_package(Git)
#file(COPY ${CMAKE_SOURCE_DIR}/useful/commit-msg DESTINATION ${CMAKE_SOURCE_DIR}/.git/hooks/)
file(WRITE ${CMAKE_BINARY_DIR}/version.h.in "\#define VERSION \"@VERSION@\"\n")

#find_package(CURL REQUIRED)
#if(NOT CURL_FOUND)
#    message(FATAL_ERROR "Could not find the CURL library and development files")
#endif()
#
#include_directories(${CURL_INCLUDE_DIR})
#
#find_package(OpenSSL)
#if(NOT OPENSSL_FOUND)
#    set(APPLE_OPENSSL "/usr/local/opt/openssl")
#    if(APPLE AND EXISTS "${APPLE_OPENSSL}" AND IS_DIRECTORY "${APPLE_OPENSSL}")
#        message(WARNING "Setting up OpenSSL paths with OpenSSL_ROOT_DIR=${APPLE_OPENSSL}")
#        set(OpenSSL_ROOT_DIR "${APPLE_OPENSSL}")
#        set(OpenSSL_LIBRARIES "${APPLE_OPENSSL}/lib")
#        set(OpenSSL_INCLUDE_DIR "${APPLE_OPENSSL}/include")
#    else()
#        message(FATAL_ERROR "Could not find the OpenSSL library and development files")
#    endif()
#endif()
#
#include_directories(${OpenSSL_INCLUDE_DIR})

option(ASAN "Enable Address Sanitizer (http://clang.llvm.org/docs/AddressSanitizer.html)" OFF)
if(ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

# Download Rapidcheck.
# (clang (or very new gcc) is required to compile)
# Usage: `cmake -DCMAKE_CXX_COMPILER=clang++ . && make`
if(CMAKE_CXX_COMPILER MATCHES "/clang[+][+]$")
    set(RAPIDCHECK_DIR "${PROJECT_SOURCE_DIR}/deps/src/Rapidcheck")

    ExternalProject_Add(Rapidcheck
        PREFIX "deps"
        DOWNLOAD_DIR "deps"
        BINARY_DIR ${RAPIDCHECK_DIR}
        GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
        GIT_TAG "f5d3afa4f387ecf147faf98d96710a6edfa420f1"
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
    )

    add_library(rapidcheck STATIC IMPORTED)
    add_dependencies(rapidcheck Rapidcheck)
    set_property(TARGET rapidcheck PROPERTY IMPORTED_LOCATION
        ${RAPIDCHECK_DIR}/librapidcheck.a)
    include_directories(${RAPIDCHECK_DIR}/include)

    # Conditionally enabling dependency on Rapidcheck
    set(RAPIDCHECK_LIB rapidcheck)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_RAPIDCHECK")
endif()

## Use Intel ISA-L library to accelerate compression.
#find_program(NASM_EXE nasm)
#if(NOT "${NASM_EXE}" STREQUAL "NASM_EXE-NOTFOUND")
#    set(ISAL_DIR "${PROJECT_SOURCE_DIR}/deps/src/ISA_L")
#    ExternalProject_Add(ISA_L
#        PREFIX "deps"
#        DOWNLOAD_DIR "deps"
#        GIT_REPOSITORY https://github.com/01org/isa-l.git
#        GIT_TAG "489db7e9130a0260a2a0bc9721353a5b3a286d69"
#        CONFIGURE_COMMAND bash -c "./autogen.sh && ./configure --prefix=${ISAL_DIR} --disable-shared CFLAGS=-DLARGE_WINDOW"
#        BUILD_COMMAND make
#        BUILD_IN_SOURCE 1
#        INSTALL_COMMAND ""
#        UPDATE_COMMAND ""
#        )
#
#    add_library(libisal STATIC IMPORTED)
#    add_dependencies(libisal ISA_L)
#    set_property(TARGET libisal PROPERTY IMPORTED_LOCATION
#        ${ISAL_DIR}/.libs/libisal.a)
#    include_directories(${ISAL_DIR}/include)
#
#    # Conditionally enabling dependency on ISA-L
#    set(ISAL_LIB libisal)
#    # Do not use ISA-L code just yet.
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_ISAL_not_yet -DLARGE_WINDOW")
#endif()


file(WRITE ${CMAKE_BINARY_DIR}/version.cmake
        "EXECUTE_PROCESS(
             COMMAND ${GIT_EXECUTABLE} describe --long --dirty --always --tags
             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
             OUTPUT_VARIABLE VERSION
             OUTPUT_STRIP_TRAILING_WHITESPACE
         )
         CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
        ")
include_directories(${CMAKE_BINARY_DIR})
add_custom_target(
        version
        ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/version.h.in
        -D DST=${CMAKE_BINARY_DIR}/version.h
        -P ${CMAKE_BINARY_DIR}/version.cmake
)

include_directories( cpp/ )

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
#find_package(CURL)

set(STD_SOURCE_FILES
        cpp/lib/std/CONST.H
        cpp/lib/std/DEFFUNC.H
        cpp/lib/std/EX_OUT.CPP
        cpp/lib/std/EX_OUT.H
        cpp/lib/std/F2C.H
        cpp/lib/std/MY_COMP.CPP
        cpp/lib/std/MY_COMP.H
        cpp/lib/std/Util.cpp
        cpp/lib/std/Util.h
        cpp/lib/std/stdexception.h
        cpp/F2C/libf2c/math.h
    )
#rebuild_re2c(RTPLATFORM_SOURCE_FILES)
add_library(std STATIC ${STD_SOURCE_FILES})
#target_link_libraries(rtplatform Threads::Threads)
#target_link_libraries(rtplatform ${CURL_LIBRARIES})
#target_link_libraries(rtplatform z)    # zlib, for compression
#target_link_libraries(rtplatform ${ISAL_LIB})    # Intel ISA-L, for compression
add_dependencies(std version)
if(RAPIDCHECK_LIB)
    add_dependencies(std ${RAPIDCHECK_LIB})
endif()

set(REF_SOURCE_FILES
        cpp/lib/Ref/CommandLine.cpp
#        cpp/lib/Ref/NamedRegistry.cpp
        cpp/lib/Ref/Stroka.cpp
        cpp/lib/Ref/class.cpp
        cpp/lib/Ref/class_sav.cpp
        cpp/lib/Ref/data_manip.cpp
        cpp/lib/Ref/exenv.cpp
        cpp/lib/Ref/file_manip.cpp
        cpp/lib/Ref/ref.cpp
        cpp/lib/Ref/refmanip.cpp
        cpp/lib/Ref/str_manip.cpp
        cpp/lib/Ref/TEST/unit_test.cpp)
add_library(ref STATIC ${REF_SOURCE_FILES})
add_dependencies(ref version)
if(RAPIDCHECK_LIB)
    add_dependencies(ref ${RAPIDCHECK_LIB})
endif()

set(SET1GRPH_SOURCE_FILES
    )
add_executable(set1grph cpp/UTIL/SET1GRPH.CPP ${SET1GRPH_SOURCE_FILES})
target_link_libraries(set1grph std)
add_dependencies(set1grph version)

set(UNIT_TESTS_SOURCE_FILES
        ${REF_SOURCE_FILES}
        ${STD_SOURCE_FILES}
        cpp/lib/Ref/TEST/unit_test.cpp
        cpp/lib/Ref/TEST/reftest.cpp
        cpp/lib/Ref/TEST/reftestx.cpp
        cpp/lib/Ref/TEST/classtest.cpp
        cpp/lib/Ref/TEST/class_sav_test.cpp
        cpp/lib/Ref/TEST/class_sav_lib.cpp
        )
add_executable(unit_tests cpp/lib/Ref/TEST/unit_test_driver.cpp ${UNIT_TESTS_SOURCE_FILES})
target_link_libraries(ref std)
add_dependencies(unit_tests version)


include_directories( cpp/  )
set(URS_CURVE_LIB_SOURCE_FILES
        cpp/URS/curve_forcelnk.cpp
        cpp/Urs/test/urs_curve_calculator.cpp
        cpp/Urs/test/urs_curve.cpp
        cpp/Urs/test/urs_2phase.cpp
        cpp/Urs/test/urs_2phase.h
        cpp/Urs/test/urs_curve.h
        cpp/Urs/test/urs_curve_calculator.h
        cpp/URS/urs_opac_curve.cpp
        cpp/URS/urs_opac_curve.h
        cpp/Urs/urs_std_curve.h
        )
add_library(urs_curve_lib STATIC ${URS_CURVE_LIB_SOURCE_FILES})
add_dependencies(urs_curve_lib version)
if(RAPIDCHECK_LIB)
    add_dependencies(urs_curve_lib ${RAPIDCHECK_LIB})
endif()
