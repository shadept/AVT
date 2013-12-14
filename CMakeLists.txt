cmake_minimum_required (VERSION 2.8)
project(AVT)

find_package(OpenGL)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "Missing OpenGL")
endif()

find_package(GLUT)
if(NOT GLUT_FOUND)
	message(FATAL_ERROR "Missing GLUT")
endif()

find_package(GLEW)
if(NOT GLEW_FOUND)
	message(FATAL_ERROR "Missing GLEW")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -fmessage-length=0 -O0 -g3 -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -fmessage-length=0 -O3 -std=c++11")

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB AVT_SOURCES src/*.cpp)
file(GLOB AVT_HEADERS src/*.h)
file(COPY shaders/ DESTINATION shaders/)
file(COPY models/ DESTINATION models/)
file(COPY textures/ DESTINATION textures/)
file(COPY materials/ DESTINATION materials/)

add_executable(${PROJECT_NAME} ${AVT_SOURCES} ${AVT_HEADERS})
target_link_libraries(${PROJECT_NAME} -lstdc++ -lm -lglut -lGLU -lGL -lGLEW)