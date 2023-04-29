cmake_minimum_required(VERSION 3.22)
project(FH_CG)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#add_compile_definitions(GLEW_STATIC)
#add_compile_definitions(FREEGLUT_STATIC)




add_executable(FH_CG simple.cpp simple.h Obj.h Obj.cpp)
target_link_libraries(FH_CG -lfreeGLUT -lopengl32 -lglew32)

# Set the path to GLEW libraries
