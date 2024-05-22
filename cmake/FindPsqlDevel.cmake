#
# postgresql-server-devel Support
#

find_path(PSQLDEVEL_INCLUDE_DIR
  NAMES libpq-fe.h
  PATH_SUFFIXES postgresql
)

find_library(PSQLDEVEL_LIBRARY
  NAMES pq
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PsqlDevel DEFAULT_MSG
                                  PSQLDEVEL_INCLUDE_DIR PSQLDEVEL_LIBRARY)

if (PSQLDEVEL_FOUND)
  set(PSQLDEVEL_INCLUDE_DIRS ${PSQLDEVEL_INCLUDE_DIR})
  set(PSQLDEVEL_LIBRARIES ${PSQLDEVEL_LIBRARY})
endif()

mark_as_advanced(PSQLDEVEL_INCLUDE_DIR PSQLDEVEL_LIBRARY)
