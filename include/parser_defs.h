/*
 * Contains defines used in the lexer & parser
 */

#ifndef PARSER_DEFS_H_
#define PARSER_DEFS_H_

typedef struct file_info_s {
    char *filename;
    int lineno;
} file_info_t;

file_info_t file_info;

enum num_types {
    /* REALS */
    NUM_FLOAT=0,
    NUM_DOUBLE,
    NUM_LONGDOUBLE,
    /* INTEGERS */
    NUM_INT,
    NUM_UINT,
    NUM_LONG,
    NUM_ULONG,
    NUM_LONGLONG,
    NUM_ULONGLONG
};

/*
 * Struct contains elements regarding the number value and type
 */
typedef struct number_s {
    unsigned long long integer;
    long double real;
    int type;
} number_t;

typedef struct string_s {
    char *string_literal;
    int string_len;
} string_t;




#endif // !PARSER_DEFS_H_
