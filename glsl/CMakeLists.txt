cmake_minimum_required(VERSION 3.5)

project(glsl)

find_package(OpenGL REQUIRED)

if(NOT OpenGL_FOUND)
        message(Error "OpenGL not found")
endif(NOT OpenGL_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

if(NOT GLEW_FOUND)
        message(Error "GLFW not found")
endif(NOT GLEW_FOUND)

find_package(glm REQUIRED)
include_directories(${glm_INCLUDE_DIRS})
include_directories(${glm_LIBRARIES})

if(NOT glm_FOUND)
        message(Error "GLM not found")
endif(NOT glm_FOUND)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_LIBRARIES})

add_library(gl SHARED gl_core_4_3.cpp gl_core_4_3.hpp)

add_executable(glsl main.cpp)

#add_library(gl_core STATIC gl_core_4_3.c)

add_library(engine STATIC engine.cpp)

target_compile_features(gl PUBLIC cxx_std_17)
target_link_libraries(glsl gl engine ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} ${glm_LIBRARIES})
