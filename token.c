#include "token.h"

const char *token_to_string(KTOKENS token)
{
    switch (token)
    {
    case KEYWORD_TABLE:
        return "KEYWORD_TABLE";
    case TABLE_NAME:
        return "TABLE_NAME";
    case IDENTIFIER:
        return "IDENTIFIER";
    case IDENTIFIER_ARG:
        return "IDENTIFIER_ARG";
    case OPENING_BRACE:
        return "OPENING_BRACE";
    case CLOSING_BRACE:
        return "CLOSING_BRACE";
    default:
        "UNKNOWN";
    }
}

KTOKENS last_token_type(const TOKENS_T *tokens)
{
    if (tokens->tokens_count > 1)
    {
        return tokens->tokens[tokens->tokens_count - 1].type;
    }
    return KEYWORD_TABLE;
}

char *last_token(const TOKENS_T *tokens)
{
    if (tokens->tokens_count > 1)
    {
        return tokens->tokens[tokens->tokens_count - 1].value;
    }
    return NULL;
}

void init_tokens(TOKENS_T *tokens, size_t initial_capacity)
{
    tokens->tokens = (TOKEN_T *)malloc(sizeof(TOKEN_T) * initial_capacity);
    if (tokens->tokens == NULL)
    {
        printf("Failed to allocate memory");
        exit(1);
    }
    tokens->tokens_count = 0;
    tokens->capacity = initial_capacity;
}

void add_token(TOKENS_T *tokens, TOKEN_T token)
{
    if (tokens->capacity == tokens->tokens_count)
    {
        // TODO: realloc memory
        printf("\ncannot added token, limit reached\n");
        free_tokens(tokens);
        exit(1);
    }
    else
    {
        tokens->tokens[tokens->tokens_count] = token;
        tokens->tokens_count++;
    }
}

void free_tokens(TOKENS_T *tokens)
{
    free(tokens->tokens);
}