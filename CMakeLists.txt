cmake_minimum_required(VERSION 3.14)
project(cAligner LANGUAGES CXX)

find_library(LIB_LM m)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/FileIO)
include_directories(${CMAKE_SOURCE_DIR}/FilterGraph)
include_directories(${CMAKE_SOURCE_DIR}/MKLog)
include_directories(${CMAKE_SOURCE_DIR}/getoptpp)
include_directories(${CMAKE_SOURCE_DIR}/Basics)

set( CMAKE_VERBOSE_MAKEFILE on )

find_package(Ceres REQUIRED)
include_directories(/usr/include)
include_directories(${CERES_INCLUDE_DIRS})
set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)


add_executable(cAligner
        main.cpp

        FileIO/MarkerFile.cpp
        FileIO/EmFile.cpp FileIO/File.cpp FileIO/FileIOException.cpp
        FileIO/ImodFiducialFile.cpp FileIO/FileReader.cpp FileIO/FileWriter.cpp
        FilterGraph/Matrix.cpp FilterGraph/PointF.cpp MKLog/MKLog.cpp
        Minimization/AlignmentProblem.cpp Minimization/Solver.cpp
        getoptpp/getopt_pp.cpp
        )

target_link_libraries(cAligner ${CERES_LIBRARIES} ${LIB_LM})
