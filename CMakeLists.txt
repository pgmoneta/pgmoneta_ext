cmake_minimum_required(VERSION 3.14)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

#
# Avoid source tree pollution
#
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

If(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

project(pgmoneta_ext VERSION ${VERSION_STRING} LANGUAGES  C)

include(CTest)
enable_testing()

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES
  "/build/;/.git/;/.github/;/*.patch;/.bundle/;/_site/;/vendor/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "pgmoneta_ext ${VERSION_STRING}")

include(CheckCCompilerFlag)
include(CheckCSourceCompiles)
include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif ()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

set(SUPPORTED_COMPILERS "GNU" "Clang" "AppleClang")

# Check for a supported compiler
if (NOT CMAKE_C_COMPILER_ID IN_LIST SUPPORTED_COMPILERS)
   message(FATAL_ERROR "Unsupported compiler ${CMAKE_C_COMPILER_ID}. Supported compilers are: ${SUPPORTED_COMPILERS}")
endif ()

CHECK_C_COMPILER_FLAG("-std=c17" COMPILER_SUPPORTS_C17)
if(NOT COMPILER_SUPPORTS_C17)
  message(FATAL_ERROR "The compiler ${CMAKE_C_COMPILER} has no C17 support. Please use a different C compiler.")
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/")

set(generation TRUE)
set(check TRUE)
set(container FALSE)

find_package(Check)
if (CHECK_FOUND)
  message(STATUS "check found")
  add_library(Check::check SHARED IMPORTED)
  set_target_properties(Check::check PROPERTIES
    IMPORTED_LOCATION ${CHECK_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${CHECK_INCLUDE_DIR})
else ()
  set(check FALSE)
  message(STATUS "check needed. The test suite process will be skipped.")
endif()

find_package(Docker)
find_package(Podman)
if (DOCKER_FOUND OR PODMAN_FOUND)
  set(container TRUE)
  message(STATUS "Docker or podman found")
else ()
  message(STATUS "Docker or podman needed. The test suite will be skipped.")
endif()

find_package(Pandoc)
if (PANDOC_FOUND)
  message(STATUS "pandoc found")
else ()
  set(generation FALSE)
  message(STATUS "pandoc needed. The generation process will be skipped.")
endif()

find_package(Pdflatex)
if (PDFLATEX_FOUND)
  message(STATUS "pdflatex found")
else ()
  set(generation FALSE)
  message(STATUS "pdflatex needed. The generation process will be skipped.")
endif()

find_package(PsqlDevel)
if (PSQLDEVEL_FOUND)
  message(STATUS "Found PostgreSQL development package")
else()
  message(FATAL_ERROR "PostgreSQL development package not found")
endif()

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(sql)
add_subdirectory(test)