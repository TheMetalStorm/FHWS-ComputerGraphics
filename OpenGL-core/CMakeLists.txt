cmake_minimum_required(VERSION 3.22)
project(FH_CG)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(GLEW_STATIC)
#add_compile_definitions(FREEGLUT_STATIC)




add_executable(FH_CG shader.cpp)
target_link_libraries(FH_CG -lfreeGLUT -lglew32 -lopengl32 )

# Set the path to GLEW libraries
