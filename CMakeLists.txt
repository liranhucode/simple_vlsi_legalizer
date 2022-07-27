cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set (PROJECT_NAME "Legalizer" CXX)
project(${PROJECT_NAME})
#set(Boost_INCLUDE_DIR /usr/local/src/boost_1_46_1)
#set(Boost_LIBRARY_DIR /usr/local/src/boost_1_46_1/stage/lib)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB SRC_FILES *.cpp)

#find boost lib
find_package (Boost REQUIRED COMPONENTS)
if (Boost_FOUND)
    message(STATUS "Boost_DIR = ${Boost_DIR}")
    message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBS = ${Boost_LIBS}")
    message(STATUS "Boost_LIBS = ${Boost_LIBRARIES}")
    include_directories (SYSTEM ${Boost_INCLUDE_DIRS})
endif ()

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
