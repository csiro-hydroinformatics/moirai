cmake_minimum_required(VERSION 3.0)

## start setting
SET (this_target moirai)
PROJECT(${this_target})

SET (VERSION 1.0)

# Find catch.hpp
find_path (catch_INCLUDE_DIRS NAMES catch/catch.hpp PATHS )
# find_path (catch_INCLUDE_DIRS NAMES catch/catch.hpp PATHS ./include)

## section: include directory
INCLUDE_DIRECTORIES(
	./include
   ${catch_INCLUDE_DIRS}
)

## section: library directories
LINK_DIRECTORIES(
)


## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
		src/reference_handle.cpp
    src/reference_handle_map_export.cpp
)

# moirai_test
SET (moirai_test_SOURCE_FILES
  ./tests/moirai_test/main.cpp
)
# moirai_test_api
SET (moirai_test_api_SOURCE_FILES
  ./tests/moirai_test_api/main.cpp
)
# moirai_test_lib
SET (moirai_test_lib_SOURCE_FILES
  ./tests/moirai_test_lib/c_interop_api.cpp
  ./tests/moirai_test_lib/species.cpp
)
# moirai_test_lib_a
SET (moirai_test_lib_a_SOURCE_FILES
  ./tests/moirai_test_lib_a/c_interop_api.cpp
  ./tests/moirai_test_lib_a/domain_a.cpp
)
# moirai_test_lib_b
SET (moirai_test_lib_b_SOURCE_FILES
  ./tests/moirai_test_lib_b/c_interop_api.cpp
  ./tests/moirai_test_lib_b/domain_b.cpp
)

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!


SET(${this_target}_HEADER_FILES
  ./include/moirai/error_reporting.h
  ./include/moirai/extern_c_api_as_opaque.h
  ./include/moirai/extern_c_api_as_transparent.h
  ./include/moirai/opaque_pointers.hpp
  ./include/moirai/reference_handle.h
  ./include/moirai/reference_handle.hpp
  ./include/moirai/reference_handle_map_export.h
  ./include/moirai/reference_handle_test_helper.hpp
  ./include/moirai/reference_type_converters.hpp
  ./include/moirai/setup_modifiers.h
)

## section: precompiled header
#ADD_MSVC_PRECOMPILED_HEADER("precompiled.h" "precompiled.cpp" MySources)
#ADD_LIBRARY(MyLibrary ${MySources})

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

## section: add definitions
#   add prefix -D. example> -DSHP
#  - DO NOT add  the following definitions(already defined in ${OSP_DEFINITIONS}:
#   -DSHP, -DWIN32, -D_WINDOWS, -D_DEBUG, -D_USRDLL, -D_CRT_SECURE_NO_DEPRECATE
ADD_DEFINITIONS(
    # -DCMAKE_CXX_FLAGS:STRING=' -std=c++11 '    
    )

# JM needed to use this as of 2016-07, after an upgrade to latest Debian setup. The Add_definitions macro was not working, somehow.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


IF (ENABLE_CODECOVERAGE)
  INCLUDE(CodeCoverage)
  append_coverage_compiler_flags()
ENDIF()
  
## section: add target
ADD_LIBRARY(${this_target} SHARED ${${this_target}_SOURCE_FILES} )

list(APPEND EXE_NAMES moirai_test moirai_test_api)
list(APPEND LIB_NAMES moirai_test_lib moirai_test_lib_a moirai_test_lib_b)

foreach(TARGET_NAME IN LISTS EXE_NAMES)
  # set(src_files "${TARGET_NAME}_SOURCE_FILES")
  ADD_executable(${TARGET_NAME} ${${TARGET_NAME}_SOURCE_FILES} )
  IF (ENABLE_CODECOVERAGE)
    setup_target_for_coverage_gcovr_html(
      NAME ${TARGET_NAME}_coverage
      EXECUTABLE ${TARGET_NAME}
    )
  endif()
endforeach()

foreach(TARGET_NAME IN LISTS LIB_NAMES)
  # set(src_files "${TARGET_NAME}_SOURCE_FILES")
  ADD_LIBRARY(${TARGET_NAME} SHARED ${${TARGET_NAME}_SOURCE_FILES} )
endforeach()

## section: set link libraries
TARGET_LINK_LIBRARIES( ${this_target})
TARGET_LINK_LIBRARIES( moirai_test_lib ${this_target})
TARGET_LINK_LIBRARIES( moirai_test moirai_test_lib)
TARGET_LINK_LIBRARIES( moirai_test_lib_b ${this_target})
TARGET_LINK_LIBRARIES( moirai_test_lib_a moirai_test_lib_b)
TARGET_LINK_LIBRARIES( moirai_test_api moirai_test_lib_a moirai_test_lib)

# Files to be installed - used by packaging stuff below
INSTALL(FILES ${${this_target}_HEADER_FILES} DESTINATION include/moirai)

########### Add uninstall target ###############
# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
########### End uninstall target ###############



if(WIN32)
  INSTALL(TARGETS ${this_target}
    RUNTIME DESTINATION bin
  )
else()
  INSTALL(TARGETS ${this_target}
    LIBRARY DESTINATION lib
  )
endif()

# Packages - currently just an RPM
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "RPM;TGZ")
set(CPACK_PACKAGE_NAME "moirai")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
set(CPACK_PACKAGE_CONTACT "Jean-Michel Perraud")
set(CPACK_PACKAGE_VENDOR "CSIRO")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
