### CMakeLists.txt --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Mon Jul 20 19:22:37 2009 (+0200)
## Version: $Id$
## Last-Updated: mar. oct. 15 09:17:52 2013 (+0200)
##           By: Nicolas Niclausse
##     Update #: 62
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

if(DTK_BUILD_ZIP)
  add_subdirectory(dtkZip)
endif(DTK_BUILD_ZIP)

add_subdirectory(dtkContainer)
add_subdirectory(dtkLog)
add_subdirectory(dtkMath)
add_subdirectory(dtkCore)
add_subdirectory(dtkGui)
add_subdirectory(dtkCrypto)
add_subdirectory(dtkJson)
add_subdirectory(dtkNotification)

if(DTK_BUILD_VR)
  add_subdirectory(dtkVr)
endif(DTK_BUILD_VR)

if(DTK_BUILD_SCRIPT)
  add_subdirectory(dtkScript)
endif(DTK_BUILD_SCRIPT)

if(DTK_BUILD_COMPOSER)
  add_subdirectory(dtkComposer)
endif(DTK_BUILD_COMPOSER)

if(DTK_BUILD_DISTRIBUTED)
  add_subdirectory(dtkDistributed)
endif(DTK_BUILD_DISTRIBUTED)

if(DTK_BUILD_PLOT)
  add_subdirectory(dtkPlot)
endif(DTK_BUILD_PLOT)

if(DTK_BUILD_VIDEO)
  add_subdirectory(dtkVideo)
endif(DTK_BUILD_VIDEO)
