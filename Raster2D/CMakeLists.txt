cmake_minimum_required(VERSION 3.22)
project(FH_CG)

set(CMAKE_CXX_STANDARD 14)

add_executable(FH_CG simple.cpp  simple.h render.h ppm.h transformation.h)
target_link_libraries(FH_CG -lfreeGLUT -lopengl32 )
