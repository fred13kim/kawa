%option     outfile="lexer.c"
%option     noyywrap
%option     yylineno
%{
#include    <string.h>
#include    "tokens-manual.h"

#define MAX_STR_CONST   65535
%}

    /* RULES */
    /* Identifiers */
nondigit    [A-Za-z_]
digit       [0-9]
digits      {digit}+
identifier  {nondigit}({nondigit}|{digit})*


    /* Integer Constants */
decimal_constant    [1-9]{digit}*

octal_digit         [0-7]
octal_constant      0{octal_digit}*

hex_digit           [0-9a-fA-F]
hex_digits          {hex_digit}+
hex_constant        0[xX]{hex_digits}

dec_float_constant  ({digits}{exponent})|({dotted_digits}{exponent}?)
dotted_digits       ({digits}\.)|({digits}\.{digits})|(\.{digits})
exponent            [eE][+-]?{digits}

hex_float_constant  0[xX]({dotted_hex_digits}|{hex_digits}){binary_exponent}
dotted_hex_digits   ({hex_digits}\.)|({hex_digits}\.{hex_digits})|(\.{hex_digits})
binary_exponent     [pP][+-]?{digits}

    /* Character Constants */
hex_esc_s           \\x{hex_digits}
oct_esc_s           \\{octal_digit}{1,3}

    /* Start Conditions */
%x integer_suffix
%x floating_suffix
%x character_sc
%x string_sc


%%
            char buf[MAX_STR_CONST];
            char *buf_ptr;


            /* Let's get the file information */
            /* Preprocessor output format: */
            /* # linenum filename flags */
#.*\n       {
                file_info.lineno = atoi(strtok(yytext+1," "));
                char *s = strtok(NULL, " ");
                file_info.filename = strdup(s+1);
                file_info.filename[strlen(s)-3] = '\0';
            }

[ \t\r]+    /* ignore whitespaces */
            /* let's get line numbers though */
"\n"        { file_info.lineno++; }

        /* Time for Numerical Constants */
{decimal_constant}      {
                            yylval.num.integer = strtoull(yytext,NULL,10);
                            BEGIN(integer_suffix);
                        }
{octal_constant}        {
                            yylval.num.integer = strtoull(yytext,NULL,8);
                            BEGIN(integer_suffix);
                        }
{hex_constant}          {
                            yylval.num.integer = strtoull(yytext,NULL,16);
                            BEGIN(integer_suffix);
                        }
{dec_float_constant}    {
                            yylval.num.real = strtold(yytext,NULL);
                            BEGIN(floating_suffix);
                        }
{hex_float_constant}    {
                            yylval.num.real = strtold(yytext,NULL);
                            BEGIN(floating_suffix);
                        }


        /* Time for Character Constants */
L?\'                    { buf_ptr = buf; BEGIN(character_sc); }
L?\"                    { buf_ptr = buf; BEGIN(string_sc); }

<integer_suffix>{
    [lL]                        {
                                    yylval.num.type = NUM_LONG;
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
    [lL]{2}                     {
                                    yylval.num.type = NUM_LONGLONG; 
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
    [uU]                        {
                                    yylval.num.type = NUM_UINT;
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
    [lL][uU]|[uU][lL]           {
                                    yylval.num.type = NUM_ULONG;
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
    [lL]{2}[uU]|[uU][lL]{2}     {
                                    yylval.num.type = NUM_ULONGLONG;
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
    .|\n                        {
                                    /* Push back the trailing character */
                                    unput(yytext[0]);
                                    yylval.num.type = NUM_INT;
                                    BEGIN(INITIAL);
                                    return NUMBER;
                                }
}

<floating_suffix>{
    [fF]    {
                yylval.num.type = NUM_FLOAT;
                BEGIN(INITIAL);
                return NUMBER;
            }
    [lL]    {
                yylval.num.type = NUM_LONGDOUBLE;
                BEGIN(INITIAL);
                return NUMBER;
            }
    .|\n    {
                /* Push back the trailing character */
                unput(yytext[0]);
                yylval.num.type = NUM_DOUBLE;
                BEGIN(INITIAL);
                return NUMBER;
            }
}

<character_sc>{
    \'      {
                        /* truncate the char sequence if its longer than one
                         * char
                         */
                if ((buf_ptr - buf) > 1) {
                    fprintf(stderr,"%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n",file_info.filename,file_info.lineno);
                }
                yylval.char_literal = buf[0];
                BEGIN(INITIAL);
                return CHARLIT;
            }
    \"      {
                *buf_ptr++ = '\"';
            }
    \n      {
                fprintf(stderr, "%s:%d:Error: missing terminating \' character\n",file_info.filename,file_info.lineno);
                exit(-1);
            }
}
<string_sc>{
    \"      {
                        /* let's copy string from buf into yylval */
                *buf_ptr = '\0';
                yylval.str.string_len = buf_ptr - buf;
                yylval.str.string_literal = calloc((yylval.str.string_len+1), sizeof(char));
                memcpy(yylval.str.string_literal, buf, (yylval.str.string_len+1)*sizeof(char));
                BEGIN(INITIAL);
                return STRING;
            }
    \'      {
                *buf_ptr++ = '\'';
            }
    \n      {
                fprintf(stderr, "%s:%d:Error: missing terminating \" character\n",file_info.filename,file_info.lineno);
                exit(-1);
            }
}
<character_sc,string_sc>{
    {oct_esc_s}     {
                        int tmp;
                        (void) sscanf(yytext + 1, "%o", &tmp);
                        if (tmp > 0xff) {
                            fprintf(stderr, "%s:%d:Warning:Octal escape sequence %s out of range\n",file_info.filename,file_info.lineno,yytext);
                            *buf_ptr++ = 0xff; /* invalid escape character should just be 0xff */
                        } else {
                            *buf_ptr++ = tmp;
                        }
                    }
    {hex_esc_s}     {
                        int tmp;
                        (void) sscanf(yytext + 2, "%x", &tmp);
                        if (tmp > 0xff) {
                            fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n",file_info.filename,file_info.lineno,yytext);
                            *buf_ptr++ = 0xff; /* invalid escape character should just be 0xff */
                        } else {
                            *buf_ptr++ = tmp;
                        }
                    }

    \\'     { *buf_ptr++ = '\''; }
    \\\"    { *buf_ptr++ = '\"'; }
    \\\?    { *buf_ptr++ = '\?'; }
    \\\\    { *buf_ptr++ = '\\'; }
    \\a     { *buf_ptr++ = '\a'; }
    \\b     { *buf_ptr++ = '\b'; }
    \\f     { *buf_ptr++ = '\f'; }
    \\n     { *buf_ptr++ = '\n'; }
    \\r     { *buf_ptr++ = '\r'; }
    \\t     { *buf_ptr++ = '\t'; }
    \\v     { *buf_ptr++ = '\v'; }

                /* ignore the " ' and \n for any other character */
    [^\"\'\\\n]+    {
                        char *yptr = yytext;
                        while ( *yptr ) *buf_ptr++ = *yptr++;
                    }
}



<<EOF>>     { return TOKEOF; }

    /* PUNCTUATORS */
"["         { return '[';        }
"]"         { return ']';        }
"("         { return '(';        }
")"         { return ')';        }
"{"         { return '{';        }
"}"         { return '}';        }
"."         { return '.';        }
"->"        { return INDSEL;     }
"++"        { return PLUSPLUS;   }
"--"        { return MINUSMINUS; }
"&"         { return '&';        }
"*"         { return '*';        }
"+"         { return '+';        }
"-"         { return '-';        }
"~"         { return '~';        }
"!"         { return '!';        }
"/"         { return '/';        }
"%"         { return '%';        }
"<<"        { return SHL;        }
">>"        { return SHR;        }
"<"         { return '<';        }
">"         { return '>';        }
"<="        { return LTEQ;       }
">="        { return GTEQ;       }
"=="        { return EQEQ;       }
"!="        { return NOTEQ;      }
"^"         { return '^';        }
"|"         { return '|';        }
"&&"        { return LOGAND;     }
"||"        { return LOGOR;      }
"?"         { return '?';        }
":"         { return ':';        }
";"         { return ';';        }
"..."       { return ELLIPSIS;   }
"="         { return '=';        }
"*="        { return TIMESEQ;    }
"/="        { return DIVEQ;      }
"%="        { return MODEQ;      }
"+="        { return PLUSEQ;     }
"-="        { return MINUSEQ;    }
"<<="       { return SHLEQ;      }
">>="       { return SHREQ;      }
"&="        { return ANDEQ;      }
"^="        { return XOREQ;      }
"|="        { return OREQ;       }
","         { return ',';        }
"#"         { return '#';        }
"##"        { return HASHHASH;   }
"<:"        { return '[';        }
":>"        { return ']';        }
"<%"        { return '{';        }
"%>"        { return '}';        }
"%:"        { return '#';        }
"%:%"       { return HASHHASH;   }

        /* KEYWORDS */
auto        { return AUTO;       }
break       { return BREAK;      }
case        { return CASE;       }
char        { return CHAR;       }
const       { return CONST;      }
continue    { return CONTINUE;   }
default     { return DEFAULT;    }
do          { return DO;         }
double      { return DOUBLE;     }
else        { return ELSE;       }
enum        { return ENUM;       }
extern      { return EXTERN;     }
float       { return FLOAT;      }
for         { return FOR;        }
goto        { return GOTO;       }
if          { return IF;         }
inline      { return INLINE;     }
int         { return INT;        }
long        { return LONG;       }
register    { return REGISTER;   }
restrict    { return RESTRICT;   }
return      { return RETURN;     }
short       { return SHORT;      }
signed      { return SIGNED;     }
sizeof      { return SIZEOF;     }
static      { return STATIC;     }
struct      { return STRUCT;     }
switch      { return SWITCH;     }
typedef     { return TYPEDEF;    }
union       { return UNION;      }
unsigned    { return UNSIGNED;   }
void        { return VOID;       }
volatile    { return VOLATILE;   }
while       { return WHILE;      }
_Bool       { return _BOOL;      }
_Complex    { return _COMPLEX;   }
_Imaginary  { return _IMAGINARY; }

{identifier}    { yylval.str.string_literal = strdup(yytext); return IDENT; }

        /* Unrecognized character/ruleset */
.               {
                    fprintf(stderr,"%s:%d:Error: unrecognized character '%c'\n",file_info.filename,file_info.lineno,yytext[0]);
                    exit(-1);
                }
%%

