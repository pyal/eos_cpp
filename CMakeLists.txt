cmake_minimum_required(VERSION 3.2)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project(eos_cpp)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLibraryFiles.txt)
enable_testing()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -Wl,-z,relro,-z,now")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wno-deprecated-declarations -Wno-deprecated-register")

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

find_package(Git)
file(WRITE ${CMAKE_BINARY_DIR}/version.h.in "\#define VERSION \"@VERSION@\"\n")


option(ASAN "Enable Address Sanitizer (http://clang.llvm.org/docs/AddressSanitizer.html)" OFF)
if(ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()


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

add_definitions(-DMAC=yes)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories( cpp/ )

#set(STD_SOURCE_FILES
#        cpp/lib/std/CONST.H
#        cpp/lib/std/DEFFUNC.H
#        cpp/lib/std/EX_OUT.CPP
#        cpp/lib/std/EX_OUT.H
#        cpp/lib/std/F2C.H
#        cpp/lib/std/MY_COMP.CPP
#        cpp/lib/std/MY_COMP.H
#        cpp/lib/std/Util.cpp
#        cpp/lib/std/Util.h
#        cpp/lib/std/stdexception.h
#        cpp/F2C/libf2c/math.h
#    )
add_library(stdlib STATIC ${STDLIB_SOURCE_FILES})
add_dependencies(stdlib version)
if(RAPIDCHECK_LIB)
    add_dependencies(stdlib ${RAPIDCHECK_LIB})
endif()

add_library(reflib STATIC ${REFLIB_SOURCE_FILES})
add_dependencies(reflib version)







add_library(urs_curve_lib STATIC ${URS_CURVE_LIB_SOURCE_FILES})
add_dependencies(urs_curve_lib version)

add_library(quad_pack_lib STATIC ${QUAD_PACK_LIB_SOURCE_FILES})
add_dependencies(quad_pack_lib version)

add_library(mathlib STATIC ${MATHLIB_SOURCE_FILES})
add_dependencies(mathlib version)

add_library(waveletlib STATIC ${WAVELETLIB_SOURCE_FILES})
add_dependencies(waveletlib version)

add_library(calclib STATIC ${CALCLIB_SOURCE_FILES})
add_dependencies(calclib version)


add_library(lex_anal_lib STATIC ${LEX_ANAL_LIB_SOURCE_FILES})
add_dependencies(lex_anal_lib version)

add_library(eoslib STATIC ${EOSLIB_SOURCE_FILES})
add_dependencies(eoslib version)

add_library(libf2c STATIC ${LIBF2C_SOURCE_FILES})
add_dependencies(libf2c version)

add_library(multiminlib STATIC ${MULTIMINLIB_SOURCE_FILES})
add_dependencies(multiminlib version)

add_library(datalib STATIC ${DATALIB_SOURCE_FILES})
add_dependencies(datalib version)

add_library(regridlib STATIC ${REGRIDLIB_SOURCE_FILES})
add_dependencies(regridlib version)

add_library(math_func_lib STATIC ${MATH_FUNC_LIB_SOURCE_FILES})
add_dependencies(math_func_lib version)







set(CALCULATOR_SOURCE_FILES
        cpp/Util/code_gen/calculator/calculator_std.cpp
        cpp/UTIL/code_gen/calculator/calc_forcelnk.cpp
        )
add_executable(calculator ${CALCULATOR_SOURCE_FILES})
target_link_libraries(calculator reflib stdlib calclib lex_anal_lib)
add_dependencies(calculator version)




add_executable(set1grph cpp/UTIL/SET1GRPH.CPP)
target_link_libraries(set1grph stdlib)
add_dependencies(set1grph version)

add_executable(unit_tests cpp/lib/Ref/TEST/unit_test_driver.cpp ${UNIT_TESTS_SOURCE_FILES})
target_link_libraries(unit_tests reflib stdlib)
add_dependencies(unit_tests version)


set(URS_CURVE_SOURCE_FILES
        cpp/Urs/curve_forcelnk.cpp
        cpp/Urs/curve_forcelnk.h
        cpp/Urs/urs_forcelnk.h
        cpp/Urs/urs_forcelnk.cpp
        cpp/UTIL/code_gen/calculator/calc_forcelnk.h
        cpp/UTIL/code_gen/calculator/calc_forcelnk.cpp
        )
add_executable(urs_curve cpp/Urs/urs_curve_main.cpp ${URS_CURVE_SOURCE_FILES})
target_link_libraries(urs_curve  reflib stdlib urs_curve_lib quad_pack_lib mathlib
        waveletlib calclib lex_anal_lib regridlib datalib multiminlib libf2c eoslib math_func_lib)
add_dependencies(urs_curve version)

