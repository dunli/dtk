### CMakeLists.txt --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Fri Apr 16 09:26:50 2010 (+0200)
## Version: $Id$
## Last-Updated: Wed Apr  4 08:57:18 2012 (+0200)
##           By: tkloczko
##     Update #: 16
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

project(dtkReferenceCounting)

## #################################################################
## Build rules
## #################################################################

set(${PROJECT_NAME}_SOURCE_MOC main.moc)

qt4_generate_moc(main.cpp ${CMAKE_CURRENT_BINARY_DIR}/main.moc)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp
  ${${PROJECT_NAME}_SOURCE_MOC})

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  dtkCore
  dtkGui
  dtkLog)
