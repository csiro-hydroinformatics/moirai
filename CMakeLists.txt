cmake_minimum_required(VERSION 3.10)

# NOTE: the libpng set of symlinks does not appear to be the prevalent standard, so sticking to libmoirai.so -> libmoirai.so.1 -> libmoirai.so.1.0.3 rather than adopt the libpng scheme
# SET LIBNAME
# set(TARGET_LIB_NAME moirai${TARGET_MAJOR}${TARGET_MINOR}) libpng scheme, too peculiar
set(TARGET_LIB_NAME moirai)
SET (this_target ${TARGET_LIB_NAME})

set(TARGET_MAJOR 1)
set(TARGET_MINOR 0)
set(TARGET_RELEASE 1)
# set(TARGET_NAME libmoirai${TARGET_MAJOR}${TARGET_MINOR})
set(TARGET_VERSION ${TARGET_MAJOR}.${TARGET_MINOR}.${TARGET_RELEASE})
SET (VERSION ${TARGET_VERSION})

PROJECT(${this_target} VERSION ${TARGET_VERSION} DESCRIPTION "Manage C++ Objects's lifetime when exposed through a C API")

include(GNUInstallDirs)

## section: include directory
INCLUDE_DIRECTORIES(
	./include
)

## section: library directories
# LINK_DIRECTORIES(
# )

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
set_target_properties(${this_target} PROPERTIES
  OUTPUT_NAME ${TARGET_LIB_NAME}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${TARGET_MAJOR}
  # CLEAN_DIRECT_OUTPUT 1
)
# if(MSVC)
#   # msvc does not append 'lib' - do it here to have consistent name
#   set_target_properties(${this_target} PROPERTIES PREFIX "lib")
#   set_target_properties(${this_target} PROPERTIES IMPORT_PREFIX "lib")
# endif()
# target_link_libraries(${this_target} ${ZLIB_LIBRARY} ${M_LIBRARY})

# if(UNIX AND AWK)
#   if(HAVE_LD_VERSION_SCRIPT)
#     set_target_properties(${this_target} PROPERTIES LINK_FLAGS
#       "-Wl,--version-script='${CMAKE_CURRENT_BINARY_DIR}/lib${this_target}.vers'")
#   elseif(HAVE_SOLARIS_LD_VERSION_SCRIPT)
#     set_target_properties(${this_target} PROPERTIES LINK_FLAGS
#       "-Wl,-M -Wl,'${CMAKE_CURRENT_BINARY_DIR}/lib${this_target}.vers'")
#   endif()
# endif()

configure_file(${TARGET_LIB_NAME}.pc.in ${TARGET_LIB_NAME}.pc @ONLY)

## Additional material used for unit tests:

list(APPEND EXE_NAMES moirai_test moirai_test_api)
list(APPEND GCOV_EXCLUDES "tests/moirai_test_api/*" "tests/moirai_test/*")
list(APPEND LIB_NAMES moirai_test_lib moirai_test_lib_a moirai_test_lib_b)

# for cmake 3.17+ it would be (much) better to use ZIP_LISTS
# foreach(TARGET_NAME EXCL IN ZIP_LISTS EXE_NAMES GCOV_EXCLUDES)
# meanwhile many thanks to https://stackoverflow.com/a/7933072/2752565 for the working example:
list(LENGTH EXE_NAMES len1)
math(EXPR len2 "${len1} - 1")
foreach(val RANGE ${len2})
  list(GET EXE_NAMES ${val} TARGET_NAME)
  list(GET GCOV_EXCLUDES ${val} EXCL)
  ADD_executable(${TARGET_NAME} ${${TARGET_NAME}_SOURCE_FILES} )
  IF (ENABLE_CODECOVERAGE)
    setup_target_for_coverage_gcovr_html(
      NAME ${TARGET_NAME}_coverage
      EXECUTABLE ${TARGET_NAME}
      BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
      EXCLUDE ${EXCL}
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

# Files to be installed
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
    # EXPORT libmoirai # apparently not in favor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # LIBRARY DESTINATION lib
    # ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  # create_symlink(libmoirai${CMAKE_SHARED_LIBRARY_SUFFIX} TARGET moirai)
  # install(FILES $<TARGET_LINKER_FILE_DIR:moirai>/libmoirai${CMAKE_SHARED_LIBRARY_SUFFIX}
  #         DESTINATION ${CMAKE_INSTALL_LIBDIR})

  install(FILES ${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME}.pc
    # DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig) from examplar, but this appears not prevalent, most I see are under /usr/lib/x86_64-linux-gnu/pkgconfig, so:
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    # The above also prevents a lintian error
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
