cmake_minimum_required(VERSION 3.10)

project(pky CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE
    LIB_FILES
    src/*.cpp
    include/*.h
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pky STATIC ${LIB_FILES})
target_include_directories(pky PUBLIC include thirdparty/minunit)
target_include_directories(pky PUBLIC include thirdparty/doctest)

option(PKY_BUILD_PLAYGROUND "Build playground executable" ON)

if (PKY_BUILD_PLAYGROUND)
    add_executable(pky_test
        tests/main.cpp
    )
    target_link_libraries(pky_test PRIVATE pky)
endif()
