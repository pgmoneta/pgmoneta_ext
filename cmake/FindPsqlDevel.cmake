#
# postgresql-server-devel Support
#

find_program(PG_CONFIG pg_config)

if (NOT PG_CONFIG)
    message(FATAL_ERROR "pg_config executable not found. Ensure PostgreSQL is installed and pg_config is in your PATH.")
endif()

# Use pg_config to get the include and library directories
execute_process(COMMAND ${PG_CONFIG} --includedir
                OUTPUT_VARIABLE PSQLDEVEL_INCLUDE_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${PG_CONFIG} --libdir
                OUTPUT_VARIABLE PSQLDEVEL_LIBRARY_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

find_path(PSQLDEVEL_INCLUDE_DIR
  NAMES libpq-fe.h
  PATHS ${PSQLDEVEL_INCLUDE_DIR}
)

find_library(PSQLDEVEL_LIBRARY
  NAMES pq
  PATHS ${PSQLDEVEL_LIBRARY_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PsqlDevel DEFAULT_MSG
                                  PSQLDEVEL_INCLUDE_DIR PSQLDEVEL_LIBRARY)

if (PSQLDEVEL_INCLUDE_DIR AND PSQLDEVEL_LIBRARY)
  set(PSQLDEVEL_FOUND TRUE)
else()
  set(PSQLDEVEL_FOUND FALSE)
endif()

if (PSQLDEVEL_FOUND)
  set(PSQLDEVEL_INCLUDE_DIRS ${PSQLDEVEL_INCLUDE_DIR})
  set(PSQLDEVEL_LIBRARIES ${PSQLDEVEL_LIBRARY})
endif()

mark_as_advanced(PSQLDEVEL_INCLUDE_DIR PSQLDEVEL_LIBRARY)
