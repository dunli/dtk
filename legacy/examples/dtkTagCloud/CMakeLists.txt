### CMakeLists.txt --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Fri Apr 10 09:15:55 2009 (+0200)
## Version: $Id$
## Last-Updated: Tue Apr  6 09:18:10 2010 (+0200)
##           By: Julien Wintz
##     Update #: 15
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

project(dtkTagCloud)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp)

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  dtkCore
  dtkGui)
