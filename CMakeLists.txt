cmake_minimum_required(VERSION 3.14)
project(cudAlignator LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 11)
#set(SPARSELM_DIR "/home/uermel/Programs/sparselm-1.3" CACHE PATH "Path to sparseLM")
#set(CUDA_DIR "/usr/local/cuda-10.0/" CACHE PATH "Path to cuda")
#set(LAPACKBLAS_DIR "/usr/lib" CACHE PATH "Path to lapack/blas libraries")

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#set(LIB_SPLM /home/uermel/Programs/sparselm-1.3/build/libsplm.a)
#SET(SUITESPARSE_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu/" CACHE PATH "The SuiteSparse libraries")
#find_library(LIB_BLAS blas HINTS ${LAPACKBLAS_DIR})
#find_library(LIB_LAPACK lapack HINTS ${LAPACKBLAS_DIR})
find_library(LIB_LM m)
#find_library(LIB_CSPARSE libcxsparse.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_CHOLMOD libcholmod.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_LDL libldl.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_UMFPACK libumfpack.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_CCOLAMD libccolamd.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_CAMD libcamd.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_AMD libamd.so HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_METIS libmetis.so.5 HINTS ${SUITESPARSE_LIBRARY_PATH} NO_DEFAULT_PATH)
#find_library(LIB_CERES libceres.a HINTS /usr/local/lib/ NO_DEFAULT_PATH)

# Add executable



#set(LIBS ${LIB_SPLM} ${LIB_LM} ${LIB_BLAS} ${LIB_LAPACK} ${LIB_CSPARSE} ${LIB_CHOLMOD} ${LIB_LDL}
#        ${LIB_UMFPACK} ${LIB_CCOLAMD} ${LIB_CAMD} ${LIB_AMD} ${LIB_METIS} ${LIB_CERES} /usr/lib/x86_64-linux-gnu/libglog.a
#        )
#set(LIBS cudart;cublas;cusolver;blas;lapack)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/FileIO)
include_directories(${CMAKE_SOURCE_DIR}/FilterGraph)
include_directories(${CMAKE_SOURCE_DIR}/MKLog)
include_directories(${CMAKE_SOURCE_DIR}/getoptpp)
#include_directories(${CMAKE_SOURCE_DIR}/cub-1.8.0)
include_directories(${CMAKE_SOURCE_DIR}/Basics)
#include_directories(${SPARSELM_DIR})
#include_directories(/usr/local/include/ceres)
#include_directories(/usr/include/eigen3/Eigen)
#include_directories(${CUDA_DIR}/include)

#link_directories(${LAPACKBLAS_DIR})
#link_directories(${CUDA_DIR}/lib64)

#add_executable(cudAlignator
#        main.cpp
#
#        FileIO/MarkerFile.cpp FileIO/ind2sub.cpp
#        FileIO/EmFile.cpp FileIO/File.cpp FileIO/FileIOException.cpp
#        FileIO/ImodFiducialFile.cpp FileIO/FileReader.cpp FileIO/FileWriter.cpp
#        FilterGraph/Matrix.cpp FilterGraph/PointF.cpp MKLog/MKLog.cpp FileIO/solver.cpp
#        )
set( CMAKE_VERBOSE_MAKEFILE on )

find_package(Ceres REQUIRED)
include_directories(/usr/include)
include_directories(${CERES_INCLUDE_DIRS})
set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)

#message(${CERES_INCLUDE_DIRS})

add_executable(cudAlignator
        main.cpp

        FileIO/MarkerFile.cpp
        FileIO/EmFile.cpp FileIO/File.cpp FileIO/FileIOException.cpp
        FileIO/ImodFiducialFile.cpp FileIO/FileReader.cpp FileIO/FileWriter.cpp
        FilterGraph/Matrix.cpp FilterGraph/PointF.cpp MKLog/MKLog.cpp
        Minimization/AlignmentProblem.cpp Minimization/Solver.cpp
        getoptpp/getopt_pp.cpp
        )
#FileIO/sparse.cpp
#FileIO/rms_kernels.cu
#${LEVMARDIR}/lm.c ${LEVMARDIR}/Axb.c ${LEVMARDIR}/misc.c
#${LEVMARDIR}/levmar.h ${LEVMARDIR}/misc.h ${LEVMARDIR}/compiler.h

#target_compile_options(cudAlignator PRIVATE -std=c++11 -Wall -O0 )
#-funroll-loops
target_link_libraries(cudAlignator ${CERES_LIBRARIES} ${LIB_LM})
