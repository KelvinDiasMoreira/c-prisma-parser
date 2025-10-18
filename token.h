#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    KEYWORD_TABLE,
    IDENTIFIER,
    IDENTIFIER_ARG,
    OPENING_BRACE,
    CLOSING_BRACE,
} KTOKENS;

typedef struct TOKEN
{
    char *value;
    int start;
    int line;
    KTOKENS type;
} TOKEN_T;

typedef struct TOKENS
{
    TOKEN_T *tokens;
    size_t tokens_count;
    size_t capacity;
} TOKENS_T;

const char *token_to_string(KTOKENS token);
char *last_token(const TOKENS_T *ptr_token);
void init_tokens(TOKENS_T *ptr_token, size_t initial_capacity);
void add_token(TOKENS_T *ptr_token, TOKEN_T token);
void free_tokens(TOKENS_T *ptr_token);