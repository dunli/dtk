### FindQwt.cmake --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008-2011 - Julien Wintz, Inria.
## Created: Mon Jun  6 16:20:06 2011 (+0200)
## Version: $Id$
## Last-Updated: Tue Jun  7 15:13:19 2011 (+0200)
##           By: Julien Wintz
##     Update #: 7
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

# QWT_FOUND - system has Qwt
# QWT_INCLUDE_DIR - where to find qwt.h
# QWT_LIBRARIES - the libraries to link against to use Qwt
# QWT_LIBRARY - where to find the Qwt library (not for general use)

set(QWT_FOUND "NO")

if(QT4_FOUND)

  find_path(QWT_INCLUDE_DIR qwt.h
    /usr/local/qwt/include
    /usr/local/include
    /usr/include/qwt
    /usr/include)

  set(QWT_NAMES ${QWT_NAMES} qwt libqwt qwt-qt4)

  find_library(QWT_LIBRARY
    NAMES ${QWT_NAMES}
    PATHS /usr/local/qwt/lib /usr/local/lib /usr/lib)
  
  if(QWT_LIBRARY)
    set(QWT_LIBRARIES ${QWT_LIBRARY})
    set(QWT_FOUND "YES")
    
    if (CYGWIN)
      if(NOT BUILD_SHARED_LIBS)
        set(QWT_DEFINITIONS -DQWT_STATIC)
      endif(NOT BUILD_SHARED_LIBS)
    endif(CYGWIN)
    
  endif(QWT_LIBRARY)
endif(QT4_FOUND)

mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY)
