cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build) 

project(project VERSION 1.0.0)

cmake_policy(SET CMP0072 NEW) #SET OPENGL TO CORE

set(CMAKE_CXX_STANDARD 14)

add_executable(project 
	src/main.cpp
	src/Shader.cpp
	src/Utils.cpp
	src/VertexLayout.cpp
	src/Mesh.cpp
	src/Texture.cpp
	src/TextureCube.cpp
	src/PBRMaterial.cpp
	)

find_package(GLEW REQUIRED)
target_link_libraries(project GLEW::GLEW)
find_package(OpenGL REQUIRED)
target_link_libraries(project glfw OpenGL::GL)
find_package(glm REQUIRED)
target_link_libraries(project glm::glm)
find_package(assimp REQUIRED)
target_link_libraries(project assimp::assimp)


FIND_PACKAGE( OpenCV REQUIRED )                              
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES (project ${OpenCV_LIBS})         
