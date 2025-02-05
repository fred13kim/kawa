#include "tokens-manual.h"
#include "lexer.h"
#include <stdio.h>



/*
 * DESCRIPTION:
 * The get_punc() function returns the token string from the provided punctuator
 * enum
 */

char *get_punc(int punc)
{
    switch(punc) {
        case INDSEL:     return "INDSEL";
        case PLUSPLUS:   return "PLUSPLUS";
        case MINUSMINUS: return "MINUSMINUS";
        case SHL:        return "SHL";
        case SHR:        return "SHR";
        case LTEQ:       return "LTEQ";
        case GTEQ:       return "GTEQ";
        case EQEQ:       return "EQEQ";
        case NOTEQ:      return "NOTEQ";
        case LOGAND:     return "LOGAND";
        case LOGOR:      return "LOGOR";
        case ELLIPSIS:   return "ELLIPSIS";
        case TIMESEQ:    return "TIMESEQ";
        case DIVEQ:      return "DIVEQ";
        case MODEQ:      return "MODEQ";
        case PLUSEQ:     return "PLUSEQ";
        case MINUSEQ:    return "MINUSEQ";
        case SHLEQ:      return "SHLEQ";
        case SHREQ:      return "SHREQ";
        case ANDEQ:      return "ANDEQ";
        case OREQ:       return "OREQ";
        case XOREQ:      return "XOREQ";
        case HASHHASH:   return "HASHHASH";
    }
    return NULL;
}


/*
 * DESCRIPTION:
 * The get_kw() function returns the token string from the provided keyword enum
 */
char *get_kw(int kw)
{
    switch (kw) {
        case AUTO:       return "AUTO";
        case BREAK:      return "BREAK";
        case CASE:       return "CASE";
        case CHAR:       return "CHAR";
        case CONST:      return "CONST";
        case CONTINUE:   return "CONTINUE";
        case DEFAULT:    return "DEFAULT";
        case DO:         return "DO";
        case DOUBLE:     return "DOUBLE";
        case ELSE:       return "ELSE";
        case ENUM:       return "ENUM";
        case EXTERN:     return "EXTERN";
        case FLOAT:      return "FLOAT";
        case FOR:        return "FOR";
        case GOTO:       return "GOTO";
        case IF:         return "IF";
        case INLINE:     return "INLINE";
        case INT:        return "INT";
        case LONG:       return "LONG";
        case REGISTER:   return "REGISTER";
        case RESTRICT:   return "RESTRICT";
        case RETURN:     return "RETURN";
        case SHORT:      return "SHORT";
        case SIGNED:     return "SIGNED";
        case SIZEOF:     return "SIZEOF";
        case STATIC:     return "STATIC";
        case STRUCT:     return "STRUCT";
        case SWITCH:     return "SWITCH";
        case TYPEDEF:    return "TYPEDEF";
        case UNION:      return "UNION";
        case UNSIGNED:   return "UNSIGNED";
        case VOID:       return "VOID";
        case VOLATILE:   return "VOLATILE";
        case WHILE:      return "WHILE";
        case _BOOL:      return "_BOOL";
        case _COMPLEX:   return "_COMPLEX";
        case _IMAGINARY: return "_IMAGINARY";
    }
    return NULL;
}


/*
 * DESCRIPTION:
 * The print_char() function prints a characters and formats them depending on
 * whether they are escape or out of bounds characters. It is important to note
 * that c is of type unsigned char here. This is to ensure that chars are note
 * treated as negative numbers.
 */
void print_char(FILE *fp, unsigned char c) {
    switch (c) {
        case '\'': fprintf(fp,"\\'"); break;
        case '\"': fprintf(fp,"\\\""); break;
        // case '\?': fprintf(fp,"\\?"); break;
        case '\\': fprintf(fp,"\\\\"); break;
        case '\a': fprintf(fp,"\\a"); break;
        case '\b': fprintf(fp,"\\b"); break;
        case '\f': fprintf(fp,"\\f"); break;
        case '\n': fprintf(fp,"\\n"); break;
        case '\r': fprintf(fp,"\\r"); break;
        case '\t': fprintf(fp,"\\t"); break;
        case '\v': fprintf(fp,"\\v"); break;
        case '\0': fprintf(fp,"\\0"); break;
        default:
            if (c < 32 || c > 127) {
                fprintf(fp, "\\%03o", c);
            } else {
                fprintf(fp, "%c", c);
            }
    };
}

int main(void) {
    int t;
    while (t = yylex()) {
        if (file_info.filename) {
            fprintf(stdout,"%s\t", file_info.filename);
        } else {
            fprintf(stdout,"<STDIN>\t");
        }
        fprintf(stdout, "%d\t", file_info.lineno);

        if (t <= 255) {
            fprintf(stdout, "%c\t\n", t);
            continue;
        }

        switch(t)
        {
            case CHARLIT:
                fprintf(stdout,"CHARLIT\t");
                print_char(stdout,yylval.char_literal);
                fprintf(stdout,"\n");
                break;
            case STRING:
                fprintf(stdout,"STRING\t");
                for (int i = 0; i < yylval.str.string_len; i++) {
                    print_char(stdout,yylval.str.string_literal[i]);
                }
                fprintf(stdout, "\n");
                break;
            case NUMBER:
                fprintf(stdout, "NUMBER\t");
                if (yylval.num.type >= NUM_INT) {
                    fprintf(stdout, "INTEGER\t%lld\t", yylval.num.integer);
                } else {
                    fprintf(stdout, "REAL\t%Lg\t", yylval.num.real);
                }
                switch(yylval.num.type) {
                    case NUM_FLOAT: fprintf(stdout, "FLOAT\n"); break;
                    case NUM_DOUBLE: fprintf(stdout, "DOUBLE\n"); break;
                    case NUM_LONGDOUBLE: fprintf(stdout, "LONGDOUBLE\n"); break;
                    case NUM_INT: fprintf(stdout, "INT\n"); break;
                    case NUM_UINT: fprintf(stdout, "UNSIGNED,INT\n"); break;
                    case NUM_LONG: fprintf(stdout, "LONG\n"); break;
                    case NUM_ULONG: fprintf(stdout, "UNSIGNED,LONG\n"); break;
                    case NUM_LONGLONG: fprintf(stdout, "LONGLONG\n"); break;
                    case NUM_ULONGLONG: fprintf(stdout, "UNSIGNED,LONGLONG\n"); break;
                }
                break;
            case IDENT:
                fprintf(stdout, "IDENT\t%s\n", yylval.str.string_literal);
                break;
            default:
                if (t >= INDSEL && t <= HASHHASH)
                    fprintf(stdout, "%s\t\n", get_punc(t));
                else
                    fprintf(stdout, "%s\t\n", get_kw(t));

        }
    }
}
