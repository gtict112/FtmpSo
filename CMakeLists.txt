cmake_minimum_required(VERSION 3.4.1) #cmake最低版本

project(ftmgp)

add_library(
        ftmgp

        SHARED

        ftmgp.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


link_directories(${PROJECT_SOURCE_DIR}/lib) #添加substrate静态库

include_directories(${PROJECT_SOURCE_DIR}/include) #添加头文件

target_link_libraries(ftmgp
        ${PROJECT_SOURCE_DIR}/lib/libsubstrate.a
        ${log-lib}
        )