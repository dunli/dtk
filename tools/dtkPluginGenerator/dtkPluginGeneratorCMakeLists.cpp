/* dtkPluginGeneratorCMakeLists.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Mar 10 10:18:39 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Tue Aug  4 20:37:11 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 21
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkPluginGenerator.h"

bool dtkPluginGenerator::generateCMakeLists(void)
{
    QFile cmakeFile(d->target.absoluteFilePath("CMakeLists.txt"));

    if(!cmakeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
	qWarning() << "dtkPluginGenerator: unable to open CMakeLists.txt for writing";
	return false;
    }
        
    QTextStream stream(&cmakeFile);

    stream << QString(
        "## #################################################################\n"
        "## Generated by dtkPluginGenerator\n"
        "## #################################################################\n"
        "\n"
        "cmake_minimum_required(VERSION 2.6)\n"
        "\n"
        "project(%1Plugin)\n"
        "\n"
        "## #################################################################\n"
        "## Setup version numbering\n"
        "## #################################################################\n"
        "\n"
        "set(${PROJECT_NAME}_VERSION_MAJOR 0 CACHE STRING \"${PROJECT_NAME} major version number.\")\n"
        "set(${PROJECT_NAME}_VERSION_MINOR 1 CACHE STRING \"${PROJECT_NAME} minor version number.\")\n"
        "set(${PROJECT_NAME}_VERSION_BUILD 0 CACHE STRING \"${PROJECT_NAME} build version number.\")\n"
        "set(${PROJECT_NAME}_VERSION\n"
        "  \"${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD}\")\n"
        "\n"
        "mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)\n"
        "mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)\n"
        "mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)\n"
        "\n"
        "set(CMAKE_COLOR_MAKEFILE ON)\n"
        "set(CMAKE_VERBOSE_MAKEFILE OFF)\n"
        "set(CMAKE_INCLUDE_CURRENT_DIR TRUE)\n"
        "\n"
        "## #################################################################\n"
        "## Setup output paths\n"
        "## #################################################################\n"
        "\n"
        "if(WIN32)\n"
        "  set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)\n"
        "  set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)\n"
        "  set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY bin)\n"
        "else(WIN32)\n"
        "  set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)\n"
        "  set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)\n"
        "  set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY lib)\n"
        "endif(WIN32)\n"
        "\n"
        "set(CMAKE_DEBUG_POSTFIX \"d\")\n"
        "\n"
        "set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})\n"
        "set(ARCHIVE_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY})\n"
        "set(RUNTIME_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})\n"
        "set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})\n"
        "\n"
        "set(${PROJECT_NAME}_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/src)\n"
        "\n"
        "set(${PROJECT_NAME}_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH})\n"
        "set(${PROJECT_NAME}_RUNTIME_DIRS ${RUNTIME_OUTPUT_PATH})\n"
        "\n"
        "## #################################################################\n"
        "## Resolve dependencies\n"
        "## #################################################################\n"
        "\n"
        "mark_as_advanced(CMAKE_BACKWARDS_COMPATIBILITY)\n"
        "mark_as_advanced(CMAKE_BUILD_TYPE)\n"
        "mark_as_advanced(CMAKE_INSTALL_PREFIX)\n"
        "if(APPLE)\n"
        "mark_as_advanced(CMAKE_OSX_ARCHITECTURES)\n"
        "mark_as_advanced(CMAKE_OSX_SYSROOT)\n"
        "endif(APPLE)\n"
        "\n"
        "set(QT_USE_QTOPENGL  TRUE)\n"
        "set(QT_USE_QTSQL     TRUE)\n"
        "set(QT_USE_QTHELP    TRUE)\n"
        "set(QT_USE_QTNETWORK TRUE)\n"
        "set(QT_USE_QTWEBKIT  TRUE)\n"
        "\n"
        "find_package(Qt4 REQUIRED)\n"
        "include(${QT_USE_FILE})\n"
        "\n"
        "mark_as_advanced(QT_QMAKE_EXECUTABLE)\n"
        "\n"
        "find_package(dtk REQUIRED)\n"
        "include(${dtk_USE_FILE})\n"
        "\n"
        "## #################################################################\n"
        "## Input\n"
        "## #################################################################\n"
        "\n"
        "set(${PROJECT_NAME}_HEADERS_MOC\n"
        "  %1Plugin.h\n"
        "  %1.h\n"
        ")\n"
        "\n"
        "set(${PROJECT_NAME}_SOURCES\n"
        "  %1Plugin.cpp\n"
        "  %1.cpp\n"
        ")\n"
        "\n"
        "## #################################################################\n"
        "## Build rules\n"
        "## #################################################################\n"
        "\n"
        "add_definitions(${QT_DEFINITIONS})\n"
        "add_definitions(-DQT_PLUGIN)\n"
        "add_definitions(-DQT_SHARED)\n"
        "add_definitions(-DQT_NO_DEBUG)\n"
        "\n"
        "qt4_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})\n"
        "\n"
        "add_library(${PROJECT_NAME} SHARED\n"
        "  ${${PROJECT_NAME}_SOURCES_MOC} \n"
        "  ${${PROJECT_NAME}_SOURCES}\n"
        ")\n"
        "\n"
        "target_link_libraries(${PROJECT_NAME}\n"
        "  ${QT_LIBRARIES}\n"
        "  dtkCore\n"
        ")\n"
        "\n"
        "if(APPLE)\n"
        "  set(CMAKE_SHARED_LINKER_FLAGS \"-undefined dynamic_lookup\")\n"
        "elseif(UNIX)\n"
        "  string(REPLACE \"-Wl,--no-undefined\" \"\" CMAKE_SHARED_LINKER_FLAGS \"${CMAKE_SHARED_LINKER_FLAGS}\")\n"
        "endif()\n"
	)
	.arg(QString(d->plugin));

    cmakeFile.close();

    return true;
}
