cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(HW_OPENGL)

#########################################################
# FIND GLUT & GLEW
#########################################################
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# Include Files
#########################################################
add_executable(test 2014318086_Yuri_asn2.cpp cgmath.h support.h)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	${GLUT_LIBRARY}
	${GLEW_LIBRARY}
)
########################################################
# Linking & stuff
#########################################################

  # create the program "test"
  target_link_libraries(test ${ALL_LIBS}  )
  
