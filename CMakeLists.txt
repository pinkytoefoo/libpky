cmake_minimum_required(VERSION 3.10)

project(pky CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
    src/*.h
)

add_executable(pky ${SOURCE_FILES})

include_directories(src)

target_link_libraries(pky)
