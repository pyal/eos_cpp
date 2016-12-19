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
        cpp/Urs/test/urs_2phase.cpp
        cpp/Urs/test/urs_curve.cpp
        cpp/Urs/urs_std_curve.cpp
        cpp/Urs/test/urs_2phase.h
        cpp/Urs/test/urs_curve.h
        cpp/Urs/test/urs_curve_calculator.cpp
        cpp/Urs/test/urs_curve_calculator.h
        cpp/URS/urs_opac_curve.cpp
        cpp/URS/urs_opac_curve.h
        cpp/Urs/urs_std_curve.h
        cpp/URS/curve_forcelnk.cpp
        cpp/URS/curve_forcelnk.h
        )
add_library(urs_curve_lib STATIC ${URS_CURVE_LIB_SOURCE_FILES})
add_dependencies(urs_curve_lib version)
if(RAPIDCHECK_LIB)
    add_dependencies(urs_curve_lib ${RAPIDCHECK_LIB})
endif()



set(QUAD_PACK_SOURCE_FILES
        cpp/F2c/quadpack/C/dgtsl.c
        cpp/F2c/quadpack/C/dqag.c
        cpp/F2c/quadpack/C/dqage.c
        cpp/F2c/quadpack/C/dqagi.c
        cpp/F2c/quadpack/C/dqagie.c
        cpp/F2c/quadpack/C/dqagp.c
        cpp/F2c/quadpack/C/dqagpe.c
        cpp/F2c/quadpack/C/dqags.c
        cpp/F2c/quadpack/C/dqagse.c
        cpp/F2c/quadpack/C/dqawc.c
        cpp/F2c/quadpack/C/dqawce.c
        cpp/F2c/quadpack/C/dqawf.c
        cpp/F2c/quadpack/C/dqawfe.c
        cpp/F2c/quadpack/C/dqawo.c
        cpp/F2c/quadpack/C/dqawoe.c
        cpp/F2c/quadpack/C/dqaws.c
        cpp/F2c/quadpack/C/dqawse.c
        cpp/F2c/quadpack/C/dqc25c.c
        cpp/F2c/quadpack/C/dqc25f.c
        cpp/F2c/quadpack/C/dqc25s.c
        cpp/F2c/quadpack/C/dqcheb.c
        cpp/F2c/quadpack/C/dqelg.c
        cpp/F2c/quadpack/C/dqk15.c
        cpp/F2c/quadpack/C/dqk15i.c
        cpp/F2c/quadpack/C/dqk15w.c
        cpp/F2c/quadpack/C/dqk21.c
        cpp/F2c/quadpack/C/dqk31.c
        cpp/F2c/quadpack/C/dqk41.c
        cpp/F2c/quadpack/C/dqk51.c
        cpp/F2c/quadpack/C/dqk61.c
        cpp/F2c/quadpack/C/dqmomo.c
        cpp/F2c/quadpack/C/dqng.c
        cpp/F2c/quadpack/C/dqpsrt.c
        cpp/F2c/quadpack/C/dqwgtc.c
        cpp/F2c/quadpack/C/dqwgtf.c
        cpp/F2c/quadpack/C/dqwgts.c
        cpp/F2c/quadpack/C/fmin.c
        cpp/F2c/quadpack/C/FZERO.c
        cpp/F2c/quadpack/C/qag.c
        cpp/F2c/quadpack/C/qage.c
        cpp/F2c/quadpack/C/qagi.c
        cpp/F2c/quadpack/C/qagie.c
        cpp/F2c/quadpack/C/qagp.c
        cpp/F2c/quadpack/C/qagpe.c
        cpp/F2c/quadpack/C/qags.c
        cpp/F2c/quadpack/C/qagse.c
        cpp/F2c/quadpack/C/qawc.c
        cpp/F2c/quadpack/C/qawce.c
        cpp/F2c/quadpack/C/qawf.c
        cpp/F2c/quadpack/C/qawfe.c
        cpp/F2c/quadpack/C/qawo.c
        cpp/F2c/quadpack/C/qawoe.c
        cpp/F2c/quadpack/C/qaws.c
        cpp/F2c/quadpack/C/qawse.c
        cpp/F2c/quadpack/C/qc25c.c
        cpp/F2c/quadpack/C/qc25f.c
        cpp/F2c/quadpack/C/qc25s.c
        cpp/F2c/quadpack/C/qcheb.c
        cpp/F2c/quadpack/C/qelg.c
        cpp/F2c/quadpack/C/qk15.c
        cpp/F2c/quadpack/C/qk15i.c
        cpp/F2c/quadpack/C/qk15w.c
        cpp/F2c/quadpack/C/qk21.c
        cpp/F2c/quadpack/C/qk31.c
        cpp/F2c/quadpack/C/qk41.c
        cpp/F2c/quadpack/C/qk51.c
        cpp/F2c/quadpack/C/qk61.c
        cpp/F2c/quadpack/C/qmomo.c
        cpp/F2c/quadpack/C/qng.c
        cpp/F2c/quadpack/C/qpsrt.c
        cpp/F2c/quadpack/C/qwgtc.c
        cpp/F2c/quadpack/C/qwgtf.c
        cpp/F2c/quadpack/C/qwgts.c
        cpp/F2c/quadpack/C/r1_mach.c
        cpp/F2c/quadpack/C/sgtsl.c
        )
add_library(quad_pack STATIC ${QUAD_PACK_SOURCE_FILES})
add_dependencies(quad_pack version)


set(MATH_SOURCE_FILES
        cpp/Mat/Cmatrics.cpp
        cpp/Mat/DAT_FAC.cpp
        cpp/Mat/Dat_mani.cpp
        cpp/Mat/Eigen_ma.cpp
        cpp/Mat/fun_fac.CPP
        cpp/Mat/mat_gaus.CPP
        cpp/Mat/Matrics.cpp
        cpp/Mat/Mtfunc.cpp
        cpp/Mat/Mtmin.cpp
        cpp/Mat/Mtquad.cpp
        cpp/Mat/Mtrnd.cpp
        cpp/Mat/mtrung.cpp
        cpp/Mat/multimin.CPP
        cpp/Mat/Myappr.cpp
        cpp/Mat/NdimMin.cpp
        cpp/MAT/OneVarCurves.cpp
        cpp/Mat/quad.cpp
        cpp/MAT/SpecFunc.cpp
        cpp/Mat/Spl_fac.cpp
        cpp/Mat/Spline.cpp
        cpp/Mat/Turnmat.cpp
        cpp/Mat/Uralgol.cpp
        cpp/Mat/Cmatrics.h
#        cpp/MAT/CurveAnalyzer.cpp
#        cpp/MAT/CurveAnalyzer.h
        cpp/Mat/Dat_fac.h
        cpp/Mat/Dat_mani.h
        cpp/Mat/Dat_reg.h
        cpp/Mat/Eigen_ma.h
        cpp/Mat/fun_fac.h
        cpp/Mat/mat_gaus.H
        cpp/Mat/Matrics.h
        cpp/Mat/Mtfunc.h
        cpp/Mat/Mtrnd.h
        cpp/Mat/mtrung.h
        cpp/Mat/multimin.H
        cpp/Mat/Myappr.h
        cpp/Mat/NdimMin.h
        cpp/MAT/OneVarCurves.h
        cpp/MAT/OpacityClc.cpp
        cpp/MAT/OpacityClc.h
        cpp/Mat/quad.h
        cpp/MAT/SpecFunc.H
        cpp/Mat/Spl_fac.h
        cpp/Mat/Spline.h
        cpp/Mat/Turnmat.h
        cpp/Mat/Uralgol.h
#        cpp/Mat/Spl_ex.cpp
        cpp/Mat/Mtmin.h
        cpp/Mat/Mtquad.h
        cpp/Mat/Mtzero.cpp
        cpp/Mat/mtzero.h
        cpp/Mat/Zero.cpp
        cpp/Mat/Zero.h
        cpp/MAT/spline/func_cvt.cpp
        cpp/MAT/spline/func_cvt.h
        cpp/MAT/spline/spl_1d.cpp
        cpp/MAT/spline/spl_1d.h
        cpp/MAT/spline/spl_2d.cpp
        cpp/MAT/spline/spl_2d.h
        cpp/MAT/spline/spl_encode.cpp
        cpp/MAT/spline/spl_encode.h
        cpp/MAT/spline/spl_inc.cpp
        cpp/MAT/spline/spl_inc.h
        cpp/MAT/spline/spl_interface.h
        )
add_library(math STATIC ${MATH_SOURCE_FILES})
add_dependencies(math version)


set(WAVELET_SOURCE_FILES
        cpp/EXT/wave++/source/ArrayTree.cpp
        cpp/EXT/wave++/source/ArrayTree.h
        cpp/EXT/wave++/source/BinTree.h
        cpp/EXT/wave++/source/common.cpp
        cpp/EXT/wave++/source/common.h
        cpp/EXT/wave++/source/ConvDec.cpp
        cpp/EXT/wave++/source/ConvDec.h
        cpp/EXT/wave++/source/fft.c
        cpp/EXT/wave++/source/fft.h
        cpp/EXT/wave++/source/FFTGaborMP.cpp
        cpp/EXT/wave++/source/FFTGaborMP.h
        cpp/EXT/wave++/source/Gabor.cpp
        cpp/EXT/wave++/source/Gabor.h
        cpp/EXT/wave++/source/Hedge.cpp
        cpp/EXT/wave++/source/Hedge.h
        cpp/EXT/wave++/source/InOutUtil.cpp
        cpp/EXT/wave++/source/InOutUtil.h
        cpp/EXT/wave++/source/Interval.cpp
        cpp/EXT/wave++/source/Interval.h
        cpp/EXT/wave++/source/libw.h
        cpp/EXT/wave++/source/oqfs.h
        cpp/EXT/wave++/source/Partition.cpp
        cpp/EXT/wave++/source/Partition.h
        cpp/EXT/wave++/source/QMF.cpp
        cpp/EXT/wave++/source/QMF.h
        cpp/EXT/wave++/source/ShiftGaborMP.cpp
        cpp/EXT/wave++/source/ShiftGaborMP.h
        cpp/EXT/wave++/source/Utility.cpp
        cpp/EXT/wave++/source/Utility.h
        cpp/EXT/wave++/source/WavePacket.cpp
        cpp/EXT/wave++/source/WavePacket.h
        cpp/EXT/wave++/source/WaveTrans.cpp
        cpp/EXT/wave++/source/WaveTrans.h
        cpp/EXT/wave++/source/Wavelet.cpp
        cpp/EXT/wave++/source/Wavelet.h
        )
add_library(wavelet STATIC ${WAVELET_SOURCE_FILES})
add_dependencies(wavelet version)



set(CALCLIB_SOURCE_FILES
        cpp/Util/code_gen/calculator/calc_prg.cpp
        cpp/Util/code_gen/calculator/calc_prg_types.cpp
        cpp/Util/code_gen/calculator/zubr_impl.cpp
        cpp/Util/code_gen/calculator/calc_prg.h
        cpp/Util/code_gen/calculator/calc_prg_types.h
        cpp/Util/code_gen/calculator/calculator_std.h
        cpp/Util/code_gen/calculator/zubr_impl.h
        cpp/Util/code_gen/calculator/zubr_import.h
        cpp/Util/code_gen/calculator/zubr_calculator_tab.cpp
        cpp/Util/code_gen/calculator/zubr_calculator_tab.h
#        cpp/Util/code_gen/calculator/calculator.y
#        cpp/Util/code_gen/calculator/calculator_std.cpp
#        cpp/Util/code_gen/calculator/ToDo.txt
#        cpp/UTIL/code_gen/calculator/calc_forcelnk.h
        )
add_library(calclib STATIC ${CALCLIB_SOURCE_FILES})
add_dependencies(calclib version)

set(CALCULATOR_SOURCE_FILES
        cpp/Util/code_gen/calculator/calculator_std.cpp
        cpp/UTIL/code_gen/calculator/calc_forcelnk.cpp)
add_executable(calculator ${CALCULATOR_SOURCE_FILES})
target_link_libraries(calculator ref std calclib lex_anal)
add_dependencies(calculator version)


set(LEX_ANAL_SOURCE_FILES
        cpp/Util/code_gen/analyse_base/lex_anal.cpp
        cpp/Util/code_gen/analyse_base/stream_buf.cpp
        cpp/Util/code_gen/analyse_base/lex_anal.h
        cpp/Util/code_gen/analyse_base/stream_buf.h
        cpp/UTIL/code_gen/analyse_base/lex_anal_test.cpp
        cpp/UTIL/code_gen/analyse_base/lex_anal_test.h
        )
add_library(lex_anal STATIC ${LEX_ANAL_SOURCE_FILES})
add_dependencies(lex_anal version)

set(EOS_SOURCE_FILES
        cpp/Urs/Empi_equ.cpp
        cpp/Urs/FRE_bus.cpp
        cpp/Urs/fre_dis.cpp
        cpp/URS/fre_dis2.cpp
        cpp/Urs/Fre_fac.cpp
        cpp/URS/fre_ion.cpp
        cpp/URS/fre_mixt.cpp
        cpp/Urs/fre_multi.cpp
        cpp/Urs/FRE_ros.cpp
        cpp/Urs/Fre_url.cpp
        cpp/URS/FreeE_Electron.cpp
        cpp/Urs/H2_liqui.cpp
        cpp/Urs/Hisch.cpp
        cpp/Urs/mat_fre_spl.cpp
        cpp/Urs/matt_2phase.cpp
        cpp/URS/matt_caloric_spl.cpp
        cpp/Urs/Matt_exp.cpp
        cpp/Urs/Matt_fac.cpp
        cpp/Urs/Matt_h2.cpp
        cpp/Urs/Matt_his.cpp
        cpp/Urs/matt_internal.cpp
        cpp/Urs/Matt_spl.cpp
        cpp/Urs/ROS_bin.cpp
        cpp/Urs/Ros_equ.cpp
        cpp/Urs/spl_2phase.cpp
        cpp/Urs/Url_equ.cpp
        cpp/Urs/Empi_equ.h
        cpp/Urs/FRE_bus.H
        cpp/Urs/fre_dis.h
        cpp/URS/fre_dis2.h
        cpp/Urs/Fre_fac.h
        cpp/Urs/fre_ion.h
        cpp/URS/fre_mixt.h
        cpp/Urs/fre_multi.h
        cpp/Urs/FRE_ros.H
        cpp/Urs/Fre_url.h
        cpp/URS/FreeE_Electron.h
        cpp/Urs/H2_liqui.h
        cpp/Urs/Hisch.h
        cpp/Urs/mat_fre_spl.h
        cpp/Urs/matt_2phase.h
        cpp/URS/matt_caloric_spl.h
        cpp/Urs/Matt_cfg.h
        cpp/Urs/Matt_exp.h
        cpp/Urs/Matt_fac.h
        cpp/Urs/Matt_h2.h
        cpp/Urs/Matt_his.h
        cpp/Urs/matt_internal.h
        cpp/URS/MATT_ISPL.CPP
        cpp/URS/matt_ispl.h
        cpp/Urs/Matt_spl.h
        cpp/Urs/matt_tools.h
        cpp/Urs/ROS_bin.H
        cpp/Urs/Ros_equ.h
        cpp/Urs/spl_2phase.h
        cpp/Urs/Url_equ.h
        cpp/Urs/Adi_dou.cpp
        cpp/URS/clc_anis_deby.cpp
        cpp/URS/clc_anis_deby.h
        cpp/URS/fre_anis_deby.cpp
        cpp/URS/fre_anis_deby.h
        cpp/Urs/Free_fun.cpp
        cpp/Urs/Free_fun.h
        cpp/Urs/urs_forcelnk.cpp
        cpp/Urs/urs_forcelnk.h
#        cpp/perl/ClcGasd.pm
#        cpp/perl/resource/gasd/script_data/MATERIAL.CFG
#        cpp/perl/resource/gasd/script/test_urs_main.pl
        )
add_library(eos STATIC ${EOS_SOURCE_FILES})
add_dependencies(eos version)

set(LIBF2C_SOURCE_FILES
        cpp/F2C/libf2c/abort_.c
        cpp/F2C/libf2c/arith.h
        cpp/F2C/libf2c/backspac.c
        cpp/F2C/libf2c/c_abs.c
        cpp/F2C/libf2c/c_cos.c
        cpp/F2C/libf2c/c_div.c
        cpp/F2C/libf2c/c_exp.c
        cpp/F2C/libf2c/c_log.c
        cpp/F2C/libf2c/c_sin.c
        cpp/F2C/libf2c/c_sqrt.c
        cpp/F2C/libf2c/cabs.c
        cpp/F2C/libf2c/close.c
        cpp/F2C/libf2c/d_abs.c
        cpp/F2C/libf2c/d_acos.c
        cpp/F2C/libf2c/d_asin.c
        cpp/F2C/libf2c/d_atan.c
        cpp/F2C/libf2c/d_atn2.c
        cpp/F2C/libf2c/d_cnjg.c
        cpp/F2C/libf2c/d_cos.c
        cpp/F2C/libf2c/d_cosh.c
        cpp/F2C/libf2c/d_dim.c
        cpp/F2C/libf2c/d_exp.c
        cpp/F2C/libf2c/d_imag.c
        cpp/F2C/libf2c/d_int.c
        cpp/F2C/libf2c/d_lg10.c
        cpp/F2C/libf2c/d_log.c
        cpp/F2C/libf2c/d_mod.c
        cpp/F2C/libf2c/d_nint.c
        cpp/F2C/libf2c/d_prod.c
        cpp/F2C/libf2c/d_sign.c
        cpp/F2C/libf2c/d_sin.c
        cpp/F2C/libf2c/d_sinh.c
        cpp/F2C/libf2c/d_sqrt.c
        cpp/F2C/libf2c/d_tan.c
        cpp/F2C/libf2c/d_tanh.c
        cpp/F2C/libf2c/derf_.c
        cpp/F2C/libf2c/derfc_.c
        cpp/F2C/libf2c/dfe.c
        cpp/F2C/libf2c/dolio.c
        cpp/F2C/libf2c/dtime_.c
        cpp/F2C/libf2c/due.c
        cpp/F2C/libf2c/ef1asc_.c
        cpp/F2C/libf2c/ef1cmc_.c
        cpp/F2C/libf2c/endfile.c
        cpp/F2C/libf2c/erf_.c
        cpp/F2C/libf2c/erfc_.c
        cpp/F2C/libf2c/err.c
        cpp/F2C/libf2c/etime_.c
        cpp/F2C/libf2c/exit_.c
#        cpp/F2C/libf2c/f2c.h
        cpp/lib/std/f2c.h
        cpp/F2C/libf2c/f77_aloc.c
        cpp/F2C/libf2c/f77vers.c
        cpp/F2C/libf2c/fmt.c
        cpp/F2C/libf2c/fmtlib.c
        cpp/F2C/libf2c/ftell_.c
        cpp/F2C/libf2c/getarg_.c
        cpp/F2C/libf2c/getenv_.c
        cpp/F2C/libf2c/h_abs.c
        cpp/F2C/libf2c/h_dim.c
        cpp/F2C/libf2c/h_dnnt.c
        cpp/F2C/libf2c/h_indx.c
        cpp/F2C/libf2c/h_len.c
        cpp/F2C/libf2c/h_mod.c
        cpp/F2C/libf2c/h_nint.c
        cpp/F2C/libf2c/h_sign.c
        cpp/F2C/libf2c/hl_ge.c
        cpp/F2C/libf2c/hl_gt.c
        cpp/F2C/libf2c/hl_le.c
        cpp/F2C/libf2c/hl_lt.c
        cpp/F2C/libf2c/i77vers.c
        cpp/F2C/libf2c/i_abs.c
        cpp/F2C/libf2c/i_dim.c
        cpp/F2C/libf2c/i_dnnt.c
        cpp/F2C/libf2c/i_indx.c
        cpp/F2C/libf2c/i_len.c
        cpp/F2C/libf2c/i_mod.c
        cpp/F2C/libf2c/i_nint.c
        cpp/F2C/libf2c/i_sign.c
        cpp/F2C/libf2c/iargc_.c
        cpp/F2C/libf2c/iio.c
        cpp/F2C/libf2c/ilnw.c
        cpp/F2C/libf2c/inquire.c
        cpp/F2C/libf2c/l_ge.c
        cpp/F2C/libf2c/l_gt.c
        cpp/F2C/libf2c/l_le.c
        cpp/F2C/libf2c/l_lt.c
        cpp/F2C/libf2c/lbitbits.c
        cpp/F2C/libf2c/lbitshft.c
        cpp/F2C/libf2c/lread.c
        cpp/F2C/libf2c/lwrite.c
        cpp/F2C/libf2c/main.c
        cpp/F2C/libf2c/math.h
        cpp/F2C/libf2c/open.c
        cpp/F2C/libf2c/pow_ci.c
        cpp/F2C/libf2c/pow_dd.c
        cpp/F2C/libf2c/pow_di.c
        cpp/F2C/libf2c/pow_hh.c
        cpp/F2C/libf2c/pow_ii.c
        cpp/F2C/libf2c/pow_ri.c
        cpp/F2C/libf2c/pow_zi.c
        cpp/F2C/libf2c/pow_zz.c
        cpp/F2C/libf2c/r_abs.c
        cpp/F2C/libf2c/r_acos.c
        cpp/F2C/libf2c/r_asin.c
        cpp/F2C/libf2c/r_atan.c
        cpp/F2C/libf2c/r_atn2.c
        cpp/F2C/libf2c/r_cnjg.c
        cpp/F2C/libf2c/r_cos.c
        cpp/F2C/libf2c/r_cosh.c
        cpp/F2C/libf2c/r_dim.c
        cpp/F2C/libf2c/r_exp.c
        cpp/F2C/libf2c/r_imag.c
        cpp/F2C/libf2c/r_int.c
        cpp/F2C/libf2c/r_lg10.c
        cpp/F2C/libf2c/r_log.c
        cpp/F2C/libf2c/r_mod.c
        cpp/F2C/libf2c/r_nint.c
        cpp/F2C/libf2c/r_sign.c
        cpp/F2C/libf2c/r_sin.c
        cpp/F2C/libf2c/r_sinh.c
        cpp/F2C/libf2c/r_sqrt.c
        cpp/F2C/libf2c/r_tan.c
        cpp/F2C/libf2c/r_tanh.c
        cpp/F2C/libf2c/rdfmt.c
        cpp/F2C/libf2c/rewind.c
        cpp/F2C/libf2c/rsfe.c
        cpp/F2C/libf2c/rsli.c
        cpp/F2C/libf2c/rsne.c
        cpp/F2C/libf2c/s_cat.c
        cpp/F2C/libf2c/s_cmp.c
        cpp/F2C/libf2c/s_copy.c
        cpp/F2C/libf2c/s_paus.c
        cpp/F2C/libf2c/s_rnge.c
        cpp/F2C/libf2c/s_stop.c
        cpp/F2C/libf2c/sfe.c
        cpp/F2C/libf2c/sig_die.c
        cpp/F2C/libf2c/signal1.h
        cpp/F2C/libf2c/signal_.c
        cpp/F2C/libf2c/sue.c
        cpp/F2C/libf2c/sysdep1.h
        cpp/F2C/libf2c/system_.c
        cpp/F2C/libf2c/typesize.c
        cpp/F2C/libf2c/uio.c
        cpp/F2C/libf2c/uninit.c
        cpp/F2C/libf2c/util.c
        cpp/F2C/libf2c/wref.c
        cpp/F2C/libf2c/wrtfmt.c
        cpp/F2C/libf2c/wsfe.c
        cpp/F2C/libf2c/wsle.c
        cpp/F2C/libf2c/wsne.c
        cpp/F2C/libf2c/xwsne.c
        cpp/F2C/libf2c/z_abs.c
        cpp/F2C/libf2c/z_cos.c
        cpp/F2C/libf2c/z_div.c
        cpp/F2C/libf2c/z_exp.c
        cpp/F2C/libf2c/z_log.c
        cpp/F2C/libf2c/z_sin.c
        cpp/F2C/libf2c/z_sqrt.c
        )
add_library(libf2c STATIC ${LIBF2C_SOURCE_FILES})
add_dependencies(libf2c version)

set(MULTIMIN_SOURCE_FILES
        cpp/F2c/Multi_min/Na6/C/dtesti.c
        cpp/F2c/Multi_min/Mainc.c
        cpp/F2c/Multi_min/Maincb.c
        cpp/F2c/Multi_min/Mainfc.c
        cpp/F2c/Multi_min/Mains.c
        cpp/F2c/Multi_min/Mainsb.c
        cpp/F2c/Multi_min/Na6/C/stesti.c
        cpp/F2c/Multi_min/Blas.c
        cpp/F2c/Multi_min/Tn.c
        cpp/F2c/Multi_min/Na6/C/d1mach.c
        cpp/F2c/Multi_min/Na6/C/dadinf.c
        cpp/F2c/Multi_min/Na6/C/dchinf.c
        cpp/F2c/Multi_min/Na6/C/dexinf.c
        cpp/F2c/Multi_min/Na6/C/dgainf.c
        cpp/F2c/Multi_min/Na6/C/dgaint.c
        cpp/F2c/Multi_min/Na6/C/dqainf.c
        cpp/F2c/Multi_min/Na6/C/dtrint.c
        cpp/F2c/Multi_min/Na6/C/r1mach.c
        cpp/F2c/Multi_min/Na6/C/sadinf.c
        cpp/F2c/Multi_min/Na6/C/schinf.c
        cpp/F2c/Multi_min/Na6/C/sexinf.c
        cpp/F2c/Multi_min/Na6/C/sgainf.c
        cpp/F2c/Multi_min/Na6/C/sgaint.c
        cpp/F2c/Multi_min/Na6/C/sqainf.c
        cpp/F2c/Multi_min/Na6/C/strint.c
        )
add_library(multimin STATIC ${MULTIMIN_SOURCE_FILES})
add_dependencies(multimin version)

set(DATA_SOURCE_FILES
        cpp/lib/Data/DataArr.cpp
        cpp/lib/Data/DataTyp.cpp
        cpp/lib/Data/DataVec.cpp
        cpp/lib/Data/NamedObject.cpp
        cpp/lib/Data/SparceArray.cpp
        cpp/lib/Data/VecRes.cpp
        cpp/lib/Data/data_forcelink.h
        cpp/lib/Data/DataArr.h
        cpp/lib/Data/DataTyp.h
        cpp/lib/Data/DataVec.h
        cpp/lib/Data/NamedObject.h
        cpp/lib/Data/SparceArray.h
        cpp/lib/Data/VecRes.h
        )
add_library(data STATIC ${DATA_SOURCE_FILES})
add_dependencies(data version)

set(REGRID_SOURCE_FILES
        cpp/F2c/Regrid/Bispev.c
        cpp/F2c/Regrid/Curfit.c
        cpp/F2c/Regrid/Fpback.c
        cpp/F2c/Regrid/Fpbisp.c
        cpp/F2c/Regrid/Fpbspl.c
        cpp/F2c/Regrid/Fpchec.c
        cpp/F2c/Regrid/Fpcurf.c
        cpp/F2c/Regrid/Fpdisc.c
        cpp/F2c/Regrid/Fpgivs.c
        cpp/F2c/Regrid/Fpgrre.c
        cpp/F2c/Regrid/Fpknot.c
        cpp/F2c/Regrid/Fprati.c
        cpp/F2c/Regrid/Fpregr.c
        cpp/F2c/Regrid/Fprota.c
        cpp/F2C/REGRID/gcvspl_genev.c
        cpp/F2c/Regrid/Regrid.c
        cpp/F2c/Regrid/Splev.c
        )
add_library(regrid STATIC ${REGRID_SOURCE_FILES})
add_dependencies(regrid version)



set(URS_CURVE_EXE_SOURCE_FILES
        cpp/Urs/curve_forcelnk.cpp
        cpp/Urs/curve_forcelnk.h
        cpp/Urs/urs_forcelnk.h
        cpp/Urs/urs_forcelnk.cpp
        cpp/UTIL/code_gen/calculator/calc_forcelnk.h
        cpp/UTIL/code_gen/calculator/calc_forcelnk.cpp
        )
add_executable(urs_curve cpp/Urs/urs_curve_main.cpp ${URS_CURVE_SOURCE_FILES})
target_link_libraries(urs_curve  ref std urs_curve_lib quad_pack math wavelet calclib lex_anal regrid data multimin libf2c eos)
add_dependencies(urs_curve version)

