#
# Generate manual
#
if(generation)
  # Files
  FILE(GLOB PREDS "manual/0?-*.md")
  FILE(GLOB POSTDS "manual/9?-*.md")
  FILE(GLOB TDS "tutorial/??_*.md")
  FILE(GLOB UMDS "manual/user-??-*.md")
  FILE(GLOB DMDS "manual/dev-??-*.md")

  # Manuals definitions
  set(MANUAL_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")
  get_filename_component(PARENT_DIR ${CMAKE_BINARY_DIR} DIRECTORY)
  set(IMAGE_DIR "${PARENT_DIR}/doc/images")

  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.pdf
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.pdf --from markdown --template eisvogel --listings -N --toc ${PREDS} ${UMDS} ${TDS} ${POSTDS}
    DEPENDS ${UMDS}
    COMMENT "Generating User Guide PDF documentation"
  )
  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.pdf
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.pdf --from markdown --template eisvogel --listings -N --toc ${PREDS} ${DMDS} ${POSTDS}
    DEPENDS ${DMDS}
    COMMENT "Generating Developer Guide PDF documentation"
  )

  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.html
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.html -s -f markdown-smart -N --toc -t html5 ${PREDS} ${UMDS} ${TDS} ${POSTDS}
    DEPENDS ${UMDS}
    COMMENT "Generating User Guide HTML documentation"
  )
  add_custom_command(
    OUTPUT ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.html
    COMMAND ${PANDOC_EXECUTABLE} -o ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.html -s -f markdown-smart -N --toc -t html5 ${PREDS} ${DMDS} ${POSTDS}
    DEPENDS ${DMDS}
    COMMENT "Generating Developer Guide HTML documentation"
  )

  add_custom_target(
    all_docs ALL
    DEPENDS ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.pdf ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-user-guide.html ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.pdf ${MANUAL_OUTPUT_DIR}/pgmoneta_ext-dev-guide.html
  )
endif()