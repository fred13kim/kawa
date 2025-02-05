enum tokens {
	TOKEOF=0,
	IDENT=257,	/* This is where yacc will put it */
	CHARLIT,
	STRING,
	NUMBER,
	INDSEL,
	PLUSPLUS,
	MINUSMINUS,
	SHL,
	SHR,
	LTEQ,
	GTEQ,
	EQEQ,
	NOTEQ,
	LOGAND,
	LOGOR,
	ELLIPSIS,
	TIMESEQ,
	DIVEQ,
	MODEQ,
	PLUSEQ,
	MINUSEQ,
	SHLEQ,
	SHREQ,
	ANDEQ,
	OREQ,
	XOREQ,
    HASHHASH,
	AUTO,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	FLOAT,
	FOR,
	GOTO,
	IF,
	INLINE,
	INT,
	LONG,
	REGISTER,
	RESTRICT,
	RETURN,
	SHORT,
	SIGNED,
	SIZEOF,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,
	_BOOL,
	_COMPLEX,
	_IMAGINARY
};

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

typedef union {
    number_t num;
    string_t str;
    char char_literal;
} YYSTYPE;

YYSTYPE yylval;
