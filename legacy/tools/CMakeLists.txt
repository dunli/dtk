### CMakeLists.txt --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Mon Mar  9 21:06:43 2009 (+0100)
## Version: $Id$
## Last-Updated: mer. déc.  4 11:47:00 2013 (+0100)
##           By: Nicolas Niclausse
##     Update #: 32
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

add_subdirectory(dtkDeploy)
add_subdirectory(dtkGenerator)
add_subdirectory(dtkTestGenerator)
add_subdirectory(dtkPluginGenerator)
add_subdirectory(dtkHeadersBuilder)

if(DTK_BUILD_COMPOSER)
  add_subdirectory(dtkComposerEvaluator)
endif(DTK_BUILD_COMPOSER)

if(MPI_FOUND AND DTK_BUILD_DISTRIBUTED AND DTK_BUILD_COMPOSER)
  add_subdirectory(dtkComposerEvaluatorSlave)
endif(MPI_FOUND AND DTK_BUILD_DISTRIBUTED AND DTK_BUILD_COMPOSER)

if(DTK_BUILD_DISTRIBUTED AND DTK_BUILD_COMPOSER)
  add_subdirectory(dtkDistributedServer)
endif(DTK_BUILD_DISTRIBUTED AND DTK_BUILD_COMPOSER)

if(DTK_BUILD_COMPOSER AND DTK_BUILD_DISTRIBUTED)
  add_subdirectory(dtkCreator)
endif(DTK_BUILD_COMPOSER AND DTK_BUILD_DISTRIBUTED)
