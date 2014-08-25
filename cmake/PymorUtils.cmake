include(CheckPythonModule)
find_package(PythonLibs 2.7)
find_package(PythonInterp 2.7)
CheckPythonModule(pybindgen)

MACRO(add_python_bindings target_name generator_filename header_filename libname)
    if( PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND python_pybindgen_FOUND )
        message( STATUS "Python bindings for ${target_name} will be created" )

        set(pythonTarget           ${target_name})
        set(pythonWrapperGenerator "${CMAKE_CURRENT_SOURCE_DIR}/${generator_filename}")
        set(pythonWrapperCpp       "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_bindings_generator.cc")
        set_source_files_properties("${pythonWrapperCpp}" PROPERTIES GENERATED TRUE)

        add_custom_command(
            OUTPUT
                "${pythonWrapperCpp}"
            COMMAND
                "${PYTHON_EXECUTABLE}"
                "${pythonWrapperGenerator}"
                "${target_name}"
                "${header_filename}"
                "${CMAKE_BINARY_DIR}/config.h"
                "${CMAKE_CURRENT_BINARY_DIR}"
            DEPENDS
                "${pythonWrapperGenerator}"
            WORKING_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT
                "${pythonTarget}: generating bindings" VERBATIM
        )

        include_directories( ${PYTHON_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}")
        add_library( ${pythonTarget} MODULE "${pythonWrapperCpp}" )
        set_target_properties( ${pythonTarget} PROPERTIES PREFIX "" )
        target_link_libraries( ${pythonTarget} ${libname} ${COMMON_LIBS} )

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
ENDMACRO(add_python_bindings)
