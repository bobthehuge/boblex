#include <string.h>

#include "../utils/bth_log.h"
#define BTH_LEX_ERRX(c, f, ...) ERRX((c), fmt, __VA_ARGS__)
#include "../utils/bth_lex.h"
#define BTH_IO_IMPLEMENTATION
#include "../utils/bth_io.h"

#define ERR_UNEXP_KIND_MSG "Unexpected token of kind %zu (%s)"
#define ERR_UNEXP_KIND(t) ERRX(1, ERR_UNEXP_KIND_MSG, t->kind,\
                            t->kind < BTH_LEX_KIND_COUNT ?\
                                bth_lex_kind2str(t->kind) : NULL);

#define ERR_UNEXP_IDX_MSG "Unexpected token of idx %zu (%s)"
#define ERR_UNEXP_IDX(t) ERRX(1, ERR_UNEXP_IDX_MSG, t->idx,\
                            bth_lex_kind2str(t->kind));

enum CTYPES
{
    VOID_T,
    CHAR_T,
    SHORT_T,
    INT_T,
    LONG_T,
    CTYPES_COUNT,
};

enum CKEYWORDS
{
    RETURN_T = CTYPES_COUNT,
    LET_T,
    CKEYWORD_COUNT,
};

enum COPERATORS
{
    ADD_ASSIGN_T = CKEYWORD_COUNT,
    SUB_ASSIGN_T,
    MUL_ASSIGN_T,
    DIV_ASSIGN_T,
    MOD_ASSIGN_T,
    AND_ASSIGN_T,
    OR_ASSIGN_T,
    XOR_ASSIGN_T,
    PLUS_T,
    DASH_T,
    STAR_T,
    SLASH_T,
    PERCENT_T,
    UPPERSAND_T,
    PIPE_T,
    CARET_T,
    EXCLAM_T,
    EQUAL_T,
    COLON_T,
    ANGLE_O_T,
    ANGLE_C_T,
    DOT_T,
    QMARK_T,
    TILDE_T,
    COPERATORS_COUNT,
};

enum EKIND
{
    EK_NONE,
    EK_EXPR,
    EK_BLOCK,
    EK_UNOP,
    EK_BINOP,
    EK_DECL,
    EK_IFCOND,
};

struct op_node
{
};

struct expr_node {
    enum EKIND kind;
    union {
        struct expr_node *expr;
        struct op_node *unop;
        struct op_node *binop;
        struct decl_node *decl;
        struct cond_node *cond;
    };
};

void parse(struct bth_lex_token *t)
{
    switch (t->kind)
    {
    case LK_IDENT:
        break;
    case LK_SYMBOL:
        if (t->idx >= CTYPES_COUNT)
            ERR_UNEXP_IDX(t);
        break;
    case LK_DELIMITED:
        break;
    default:
        ERR_UNEXP_KIND(t);
        break;
    }
}

int main(void)
{
    char *sample1;
    size_t sample1_len = readfn(&sample1, 0, "sample1.c");

    // const char symbols[][2]
    const char *symbols[] = {
        // types
        "UNIT_T"       , "()",
        "U8_T"         , "u8",
        "U16_T"        , "u16",
        "U32_T"        , "u32",
        "U64_T"        , "u64",

        // keywords
        "RETURN"       , "return",
        "LET"          , "let",

        // operators
        "ADD_ASSIGN"   , "+=",
        "SUB_ASSIGN"   , "-=",
        "MUL_ASSIGN"   , "*=",
        "DIV_ASSIGN"   , "/=",
        "MOD_ASSIGN"   , "%=",
        "AND_ASSIGN"   , "&=",
        "OR_ASSIGN"    , "|=",
        "XOR_ASSIGN"   , "^=",
        "PLUS"         , "+",
        "DASH"         , "-",
        "STAR"         , "*",
        "SLASH"        , "/",
        "PERCENT"      , "%",
        "UPPERSAND"    , "&",
        "PIPE"         , "|",
        "CARET"        , "^",
        "EXCLAM"       , "!",
        "EQUAL"        , "=",
        "COLON"        , ":",
        "ANGLE_O"      , "<",
        "ANGLE_C"      , ">",
        "DOT"          , ".",
        "QMARK"        , "?",
        "TILDE"        , "~",
        
        // punctuation
        "PAREN_O"      , "(",
        "PAREN_C"      , ")",
        "BRACE_O"      , "{",
        "BRACE_C"      , "}",
        "BRACKET_O"    , "[",
        "BRACKET_O"    , "]",
        "BACKSLASH"    , "\\",
        "STAB"         , "    ",
        "SPACE"        , " ",
        "NEWLINE"      , "\n",
        "TAB"          , "\t",
        "SEMICOLON"    , ";",
        "COMMA"        , ",",
        "UNDERSCORE"   , "_",
    };

    // const char delims[][3]
    const char *delims[] = {
    //   <name>         <begin>      <end>
        "ML_COMMENT"   , "/*"       , "*/",
        "SL_COMMENT"   , "//"       , "\n",
        "STRING"       , "\""       , "\"",
        "CHAR"         , "\'"       , "\'",
    };

    struct bth_lexer lex = {
        .buffer = sample1,
        .size = sample1_len,
        .filename = "",

        .cur = 0,
        .col = 0,
        .row = 0,

        .symbols = symbols,
        .symbols_count = sizeof(symbols) / (2 * sizeof(char *)),

        .delims = delims,
        .delims_count = sizeof(delims) / (3 * sizeof(char *)),
    };

    // int loop = 1;
    struct bth_lex_token t;

    while (1)
    {
        t = bth_lex_get_token(&lex);

        if (t.kind == LK_END)
            break;

        parse(&t);
    }

    free(sample1);

    return 0;
}
