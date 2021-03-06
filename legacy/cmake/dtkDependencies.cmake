### dtkDependencies.cmake --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Fri Apr  2 09:11:53 2010 (+0200)
## Version: $Id$
## Last-Updated: Mon Dec 16 15:23:14 2013 (+0100)
##           By: Selim Kraria
##     Update #: 182
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

## #################################################################
## Qt
## #################################################################

set(QT_USE_QTOPENGL      TRUE)
set(QT_USE_QTXML         TRUE)
set(QT_USE_QTSQL         TRUE)
set(QT_USE_QTHELP        TRUE)
set(QT_USE_QTNETWORK     TRUE)
set(QT_USE_QTTEST        TRUE)
set(QT_USE_QTWEBKIT      TRUE)
set(QT_USE_GUI           TRUE)
set(QT_USE_QTDECLARATIVE TRUE)

if(WIN32)
  set(QT_USE_QTMAIN TRUE)
endif(WIN32)

find_package(Qt4 4.6.0 REQUIRED)

include(${QT_USE_FILE})

mark_as_advanced(QT_QMAKE_EXECUTABLE)
mark_as_advanced(QT_QTMOTIF_INCLUDE_DIR)
mark_as_advanced(QT_QTMOTIF_LIBRARY_DEBUG)
mark_as_advanced(QT_QTMOTIF_LIBRARY_RELEASE)

## #################################################################
## Wrapping
## #################################################################

if(DTK_BUILD_WRAPPERS)

## #################################################################
## Swig
## #################################################################

mark_as_advanced(SWIG_DIR)
mark_as_advanced(SWIG_EXECUTABLE)
mark_as_advanced(SWIG_VERSION)

find_package(SWIG REQUIRED)

if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  set(CMAKE_SWIG_FLAGS "")
  
  macro(dtk_wrap project target name language input deps)
    
    set(wrap_output ${project}_wrap_${language}.cpp)
    
    add_custom_command(
      OUTPUT ${wrap_output}
      COMMAND ${SWIG_EXECUTABLE}
      ARGS
      "-${language}"
      "-c++"
      "-module" ${name}
      "-I${PROJECT_SOURCE_DIR}/.."
      "-I${PROJECT_SOURCE_DIR}/"
      "-I${dtk_SOURCE_DIR}/"
      "-I${dtk_SOURCE_DIR}/src"
      "-I${dtk_SOURCE_DIR}/src/dtkCore"
      "-outdir" ${CMAKE_CURRENT_BINARY_DIR}
      "-o" ${wrap_output}
      ${input}
      MAIN_DEPENDENCY ${input}
      COMMENT "Wrapping ${input} to ${language}")
    
    set(${target} ${${target}} ${wrap_output})
    
  endmacro(dtk_wrap)

else(SWIG_FOUND)
  
  message("DTK_BUILD_WRAPPERS options requires SWIG.")

endif(SWIG_FOUND)

if(SWIG_FOUND)
  add_definitions(-DHAVE_SWIG)
endif(SWIG_FOUND)

## #################################################################
## Tcl
## #################################################################

find_package(TCL QUIET)

if(TCL_FOUND)
  include_directories(${TCL_INCLUDE_PATH})
endif(TCL_FOUND)

if(TCL_FOUND)
  add_definitions(-DHAVE_TCL) # Towards deprecation
  set(DTK_HAVE_TCL 1)
else(TCL_FOUND)
  set(DTK_HAVE_TCL 0)
endif(TCL_FOUND)

## #################################################################
## Python
## #################################################################

find_package(PythonLibs QUIET)

if(PYTHONLIBS_FOUND)
  include_directories(${PYTHON_INCLUDE_DIRS})
  get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)
  link_directories(${PYTHON_PATH})
  add_definitions(-DHAVE_PYTHON) # Towards deprecation
  set(DTK_HAVE_PYTHON 1)
else(PYTHONLIBS_FOUND)
  set(DTK_HAVE_PYTHON 0)
endif(PYTHONLIBS_FOUND)

## #################################################################
## Build wrappers (end)
## #################################################################

endif(DTK_BUILD_WRAPPERS)

## #################################################################
## Zlib
## #################################################################

find_package(ZLIB QUIET)

if(ZLIB_FOUND)
include_directories(${ZLIB_INCLUDE_DIRS})
endif(ZLIB_FOUND)

## #################################################################
## Ssl
## #################################################################

find_package(OpenSSL QUIET)

if(OPENSSL_FOUND)
include_directories(${OPENSSL_INCLUDE_DIR})
endif(OPENSSL_FOUND)

## #################################################################
## Mpi
## #################################################################

if(DTK_BUILD_MPI)
  mark_as_advanced(MPI_EXTRA_LIBRARY)
  mark_as_advanced(MPI_LIBRARY)

  find_package(MPI QUIET)

  if(MPI_FOUND)
    include_directories(${MPI_INCLUDE_PATH})
    set(COMPILE_FLAGS ${COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(DTK_HAVE_MPI "YES")
  endif(MPI_FOUND)

endif(DTK_BUILD_MPI)


## #################################################################
## Video / FFMPEG
## #################################################################

if(DTK_BUILD_VIDEO)
  find_package(FFmpeg QUIET)
  if(FFMPEG_FOUND)
    include_directories(${FFMPEG_INCLUDE_DIRS})
    set(DTK_HAVE_FFMPEG "YES")
  endif(FFMPEG_FOUND)
endif(DTK_BUILD_VIDEO)

## #################################################################
## Vrpn
## #################################################################

find_path(QUAT_INCLUDES           quat.h /usr/include /usr/local/include)
find_path(VRPN_INCLUDES vrpn_Configure.h /usr/include /usr/local/include)

if(QUAT_INCLUDES AND VRPN_INCLUDES)
include_directories(${QUAT_INCLUDES})
include_directories(${VRPN_INCLUDES})
endif(QUAT_INCLUDES AND VRPN_INCLUDES)

mark_as_advanced(VRPN_INCLUDES)
mark_as_advanced(QUAT_INCLUDES)

find_library(QUAT_LIBRARY NAMES quat PATHS /usr/lib /usr/local/lib)
find_library(VRPN_LIBRARY NAMES vrpn PATHS /usr/lib /usr/local/lib)

if(QUAT_LIBRARY AND VRPN_LIBRARY)
  add_definitions(-DHAVE_VRPN)
  set(DTK_HAVE_VRPN "YES")
endif(QUAT_LIBRARY AND VRPN_LIBRARY)

mark_as_advanced(QUAT_LIBRARY)
mark_as_advanced(VRPN_LIBRARY)

## #################################################################
## 
## #################################################################

if(DTK_BUILD_MPI AND MPI_FOUND AND QUAT_LIBRARY AND VRPN_LIBRARY)
  add_definitions(-DDTK_WRAP_VRPN)
endif(DTK_BUILD_MPI AND MPI_FOUND AND QUAT_LIBRARY AND VRPN_LIBRARY)

## #################################################################
## Qwt
## #################################################################

if(DTK_BUILD_PLOT)
find_package(Qwt 6 REQUIRED)
if(QWT_FOUND)
  message(STATUS "Qwt Found")
  add_definitions(-DHAVE_PLOT)
  set(DTK_HAVE_PLOT "YES")
  include_directories(${QWT_INCLUDE_DIR})
else(QWT_FOUND)
  message(SEND_ERROR "dtkPlot needs Qwt. Please install Qwt or deactivate DTK_BUILD_PLOT. If Qwt has been locally installed, set the environment variable QWT_DIR")
endif(QWT_FOUND)

endif(DTK_BUILD_PLOT)

## #################################################################
## OpenNI / Nite
## #################################################################

find_path(OPENNI_INCLUDES XnOpenNI.h /usr/include/ni)
find_path(  NITE_INCLUDES XnVNite.h  /usr/include/nite)

if(OPENNI_INCLUDES AND NITE_INCLUDES)
include_directories(/usr/include/ni)
include_directories(/usr/include/nite)
endif(OPENNI_INCLUDES AND NITE_INCLUDES)

find_library(OPENNI_LIBRARY NAMES OpenNI  PATHS /usr/lib)
find_library(  NITE_LIBRARY NAMES XnVNite PATHS /usr/lib)

if(OPENNI_LIBRARY AND NITE_LIBRARY)
link_directories(/usr/lib)
endif(OPENNI_LIBRARY AND NITE_LIBRARY)

if(OPENNI_INCLUDES AND NITE_INCLUDES AND NOT DEFINED DTK_HAVE_NITE)
set(DTK_HAVE_NITE "YES")
endif(OPENNI_INCLUDES AND NITE_INCLUDES AND NOT DEFINED DTK_HAVE_NITE)

mark_as_advanced(OPENNI_INCLUDES)
mark_as_advanced(NITE_INCLUDES)

mark_as_advanced(OPENNI_LIBRARY)
mark_as_advanced(NITE_LIBRARY)

## #################################################################
## 
## #################################################################

mark_as_advanced(file_cmd)
