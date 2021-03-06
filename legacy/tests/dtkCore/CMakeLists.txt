### CMakeLists.txt --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008-2011 - Julien Wintz, Inria.
## Created: Tue Jun 19 18:22:57 2012 (+0200)
## Version: $Id$
## Last-Updated: mer. janv. 29 09:32:38 2014 (+0100)
##           By: Thibaud Kloczko
##     Update #: 39
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

project(dtkCoreTest)

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS_MOC
  dtkArrayTest.h
  dtkProcessControllerTest.h
  dtkSingletonTest.h
  dtkSmartPointerTest.h
  dtkSparseMatrixTest.h)

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS_MOC})

set(${PROJECT_NAME}_SOURCES
  dtkArrayTest.cpp
  dtkProcessControllerTest.cpp
  dtkSingletonTest.cpp
  dtkSmartPointerTest.cpp
  dtkSparseMatrixTest.cpp)

## #################################################################
## Build rules
## #################################################################

qt4_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

set(${PROJECT_NAME}_MAIN_SOURCES
  dtkArrayTest.cpp
  dtkProcessControllerTest.cpp
  dtkSingletonTest.cpp
  dtkSmartPointerTest.cpp
  dtkSparseMatrixTest.cpp)

create_test_sourcelist(${PROJECT_NAME}_SOURCES_TEST ${PROJECT_NAME}.cxx
  ${${PROJECT_NAME}_MAIN_SOURCES})

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES_TEST}
  ${${PROJECT_NAME}_SOURCES_MOC} 
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME}
  ${QT_QTTEST_LIBRARY}
  ${QT_LIBRARIES}
  dtkLog
  dtkCore)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")

foreach (test ${${PROJECT_NAME}_MAIN_SOURCES})
  get_filename_component(TName ${test} NAME_WE)
  add_test(NAME ${TName} COMMAND $<TARGET_FILE:${PROJECT_NAME}> ${TName})
endforeach()

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)
SOURCE_GROUP("Generated Files" FILES ${${PROJECT_NAME}_SOURCES_MOC})
