# This file is part of the dune-pymor project:
#   https://github.com/pyMor/dune-pymor
# Copyright Holders: Felix Albrecht, Stephan Rave
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

add_library( dunepymor-example-lacontainer STATIC "container.cc" )

find_package(PythonLibs)
find_package(PythonInterp)
include(CheckPythonModule)
CheckPythonModule(pybindgen)

if( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND python_pybindgen_FOUND )
    message( STATUS "Python bindings will be created" )

    set(pythonTarget           lacontainerexample)
    set(pythonWrapperGenerator "${CMAKE_CURRENT_SOURCE_DIR}/container.py")
    set(pythonWrapperCpp       "${CMAKE_CURRENT_BINARY_DIR}/dunepymor_container_example.cc")
    set_source_files_properties( "${pythonWrapperCpp}" PROPERTIES GENERATED TRUE)

    add_custom_command(
        OUTPUT
            "${pythonWrapperCpp}"
        COMMAND
            "${PYTHON_EXECUTABLE}" "${pythonWrapperGenerator}"
            "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_SOURCE_DIR};${DUNE_MODULE_DIRS}"
        DEPENDS
            "${pythonWrapperGenerator}"
        WORKING_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT
            "  ${pythonTarget}: Generating bindings" VERBATIM
    )
    add_custom_target( "test-${pythonTarget}"
        COMMAND
            "${PYTHON_EXECUTABLE}" "${pythonWrapperTest}"
        DEPENDS
            ${pythonTarget}
        WORKING_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}"
    )

    include_directories( ${PYTHON_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}")
    add_library( ${pythonTarget} MODULE "${pythonWrapperCpp}" )
    set_target_properties( ${pythonTarget} PROPERTIES PREFIX "" )
    target_link_libraries( ${pythonTarget} dunepymor-example-lacontainer ${COMMON_LIBS} )

else( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND python_pybindgen_FOUND AND python_pygccxml_FOUND )
    message(STATUS "Python bindings will NOT be created")
    if( NOT PYTHONLIBS_FOUND )
        message( STATUS "\t because python libs were not found" )
    endif( NOT PYTHONLIBS_FOUND )
    if( NOT PYTHONINTERP_FOUND )
        message( STATUS "\t because python interpreter was not found" )
    endif( NOT PYTHONINTERP_FOUND )
    if( NOT python_pybindgen_FOUND )
        message( STATUS "\t because pybindgen was not found" )
    endif( NOT python_pybindgen_FOUND )
endif( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND python_pybindgen_FOUND )

