#
# Generate manual
#
if(generation)
  # Files
  FILE(GLOB EN "manual/en/??-*.md")

  # Manuals definitions
  set(MANUAL_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")
  get_filename_component(PARENT_DIR ${CMAKE_BINARY_DIR} DIRECTORY)
  set(IMAGE_DIR "${PARENT_DIR}/doc/images")

  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.pdf
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.pdf --from markdown --template eisvogel --listings -N --toc ${EN}
    DEPENDS ${EN}
    COMMENT "Generating English PDF documentation"
  )

  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.html
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.html -s -f markdown-smart -N --toc -t html5 ${EN}
    DEPENDS ${EN}
    COMMENT "Generating English HTML documentation"
  )

  add_custom_target(
    all_docs ALL
    DEPENDS ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.pdf ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-en.html
  )
endif()
