/*
 * This command is used to check if a word is a C language keyword.
 * Supported C language standards include C89 C99 C11 C17.
 * 
 * If you want to compile this file, please make sure glib has been installed.
 * 
 * You can compile this file using the command: gcc -I/usr/include/glib-2.0 
 * -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Wall -Wextra -Werror ckw.c
 * -lglib-2.0 -o ckw
 * 
 */

#include <stdio.h>
#include <string.h>
#include <glib.h>

static const char *usage =
"usage: ckw [option] [input_word]\n\n"
"option:\n"
"    -std=<standard>    specify the C language standard\n\n"
"supported standard:\n"
"    c89 c99 c11 c17\n\n"
"example:\n"
"    ckw -std=c99 static\n\n"
"if standard is not specified, the default standard is c99\n";

enum c_std {
    STD_C89,
    STD_C99,
    STD_C11,
    STD_C17,
    NUM_C_STD
};

static const char *option = "-std=";
static const char *opt_values[NUM_C_STD] = {
    [STD_C89] = "c89",
    [STD_C99] = "c99",
    [STD_C11] = "c11",
    [STD_C17] = "c17"
};

static const char *all_keywords[] = {
    /* c89 */
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    /* c99 */
    "_Bool",
    "_Complex",
    "inline",
    "_Imaginary",
    "restrict",
    /* c11 */
    "_Alignas",
    "_Alignof",
    "_Atomic",
    "_Generic",
    "_Noreturn",
    "_Static_assert",
    "_Thread_local",
    /* c17 no additional keywords */
};

#define NUM_C89_KEYWORDS    (32)
#define NUM_C99_KEYWORDS    (NUM_C89_KEYWORDS + 5)
#define NUM_C11_KEYWORDS    (NUM_C99_KEYWORDS + 7)
#define NUM_C17_KEYWORDS    (NUM_C11_KEYWORDS + 0)

static enum c_std input_std;

static int parser_args(int argc, char *argv[])
{
    if (argc == 1) {
        goto fail;
    }

    if (argc == 2) {
        if (strncmp(option, argv[1], strlen(option)) == 0) {
            goto fail;
        }
        else {
            input_std = STD_C99;
            goto succ;
        }
    }
    
    if (argc == 3) {
        if (strncmp(option, argv[1], strlen(option)) != 0) {
            goto fail;
        }
        for (int i = 0; i < NUM_C_STD; i++) {
            if (strcmp(opt_values[i], argv[1] + strlen(option)) == 0) {
                input_std = (enum c_std)i;
                goto succ;
            }
        }
    }

    if (argc > 3) {
        goto fail;
    }

fail:
    return -1;

succ:
    return 0;
}

int main(int argc, char **argv)
{
    int num_keywords = 0;
    int i = 0;
    int kw_index = 0;

    if (parser_args(argc, argv) != 0) {
        goto wrong_arg;
    }

    if (argc == 2) {
        kw_index = 1;
    } else {
        kw_index = 2;
    }

    switch (input_std) {
    case STD_C89:
        num_keywords = NUM_C89_KEYWORDS;
        break;
    case STD_C99:
        num_keywords = NUM_C99_KEYWORDS;
        break;
    case STD_C11:
        num_keywords = NUM_C11_KEYWORDS;
        break;
    case STD_C17:
        num_keywords = NUM_C17_KEYWORDS;
        break;
    case NUM_C_STD:
        break;
    }

    for (i = 0; i < num_keywords; i++) {
        (void)g_quark_from_string(all_keywords[i]);
    }

    if (g_quark_try_string(argv[kw_index])) {
        printf("%s is %s standard keyword\n", argv[kw_index], opt_values[input_std]);
    }
    else {
        printf("%s is not %s standard keyword\n", argv[kw_index], opt_values[input_std]);
    }

    return 0;

wrong_arg:
    printf("%s", usage);
    return 0;
}