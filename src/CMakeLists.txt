find_package(BISON)
find_package(FLEX)

flex_target(
    lexer
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h
)

bison_target(
    parser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.c
    VERBOSE
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

add_flex_bison_dependency(lexer parser)

add_executable(
    parsertester
    parsertester.c
    ast.c
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)


target_include_directories(
    parsertester
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)


