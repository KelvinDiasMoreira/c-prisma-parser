#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    KEYWORD_TABLE,
    TABLE_NAME,
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
KTOKENS last_token_type(const TOKENS_T *tokens);
char *last_token(const TOKENS_T *tokens);
void init_tokens(TOKENS_T *tokens, size_t initial_capacity);
void add_token(TOKENS_T *tokens, TOKEN_T token);
void free_tokens(TOKENS_T *tokens);