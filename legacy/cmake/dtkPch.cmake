### dtkPch.cmake --- 
## 
## Author: Julien Wintz
## Copyright (C) 2008-2011 - Julien Wintz, Inria.
## Created: Thu Sep  1 17:18:46 2011 (+0200)
## Version: $Id$
## Last-Updated: Mon Apr 16 10:27:31 2012 (+0200)
##           By: Julien Wintz
##     Update #: 19
######################################################################
## 
### Commentary: 
##
## Adds precompiled header (pch) support to suitable files.  Currently
## only implemented for MSVC. For otherplatforms the macro does
## nothing.
##
## Usage : 
## add_precompiled_header(AddedPrecompiledSource PchIncludeFilename PchSourceFilename CandidateSourceFile [CandidateSourceFile1...])
##
## Arguments:
##
## Output :
##   AddedPrecompiledSource : variable name that will contain
##   source files that need to be added to your target.
##
## Input :
##   PchIncludeFilename : name of the header file
##   PchSourceFilename : name of the source file used to generate the pch
##   CandidateSourceFiles : source file names to which pch support may be
##   added.
##
## Note : Adding a PCH to a file has the considerable side effect of
##   implicity forcing #include <pchfile.h> at the start of the
##   file. This may have undesired consequences.
##
######################################################################
## 
### Change log:
## 
######################################################################

macro(add_msvc_precompiled_header AddedPrecompiledSource PrecompiledHeader PrecompiledSource CandidateSourceFiles)
  if(MSVC)
    get_filename_component(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    set(PrecompiledBinary "\$(IntDir)/${PrecompiledBasename}.pch")
    get_filename_component(PchSourceAbsolute ${PrecompiledSource} ABSOLUTE)
    set(_SourceFiles ${CandidateSourceFiles})
    list(APPEND _SourceFiles ${ARGN})
    set(PchSources)

    foreach(fileIt ${_SourceFiles})
        string(REGEX MATCH "(\\.cpp|\\.cxx|\\.c|\\.C|\\.CPP|\\.cc)\$" nameMatch "${fileIt}")
        string(LENGTH "${nameMatch}" nameMatchLength)
        get_filename_component(fileItAbsolute ${fileIt} ABSOLUTE)
        if( ${nameMatchLength} GREATER 0 AND ( NOT (${fileItAbsolute} STREQUAL ${PchSourceAbsolute}) ) )
            list(APPEND PchSources ${fileIt})
        endif( ${nameMatchLength} GREATER 0 AND ( NOT (${fileItAbsolute} STREQUAL ${PchSourceAbsolute}) ) )
    endforeach(fileIt)

    foreach(fileIt ${PchSources})
        get_property(fileIt_COMPILE_FLAGS SOURCE "${fileIt}" PROPERTY COMPILE_FLAGS)
        set_property(SOURCE "${fileIt}" PROPERTY
            COMPILE_FLAGS "${fileIt_COMPILE_FLAGS} /Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\" /MP")
        set_property(SOURCE "${fileIt}" APPEND PROPERTY
            OBJECT_DEPENDS "${PrecompiledBinary}")
    endforeach(fileIt)
    
    set_property(SOURCE "${PrecompiledSource}" APPEND PROPERTY
          COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\"")
    set_property(SOURCE "${PrecompiledSource}" PROPERTY OBJECT_OUTPUTS "${PrecompiledBinary}")

    list(APPEND ${AddedPrecompiledSource} ${PrecompiledSource})
endif(MSVC)

endmacro(add_msvc_precompiled_header)

macro(add_precompiled_header AddedPrecompiledSource PrecompiledHeader PrecompiledSource CandidateSourceFiles)

if(MSVC)
  add_msvc_precompiled_header(${AddedPrecompiledSource} ${PrecompiledHeader} ${PrecompiledSource} ${CandidateSourceFiles} ${ARGN})
endif(MSVC)

endmacro(add_precompiled_header)
