#include "token.h"

const char *token_to_string(KTOKENS token)
{
    switch (token)
    {
    case KEYWORD_TABLE:
        return "KEYWORD_TABLE";
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

char *last_token(const TOKENS_T *ptr_token)
{
    if(ptr_token->tokens_count > 1) {
        return ptr_token->tokens[ptr_token->tokens_count - 1].value;
    }
    return NULL;
}

void init_tokens(TOKENS_T *ptr_token, size_t initial_capacity)
{
    ptr_token->tokens = (TOKEN_T *)malloc(sizeof(TOKEN_T) * initial_capacity);
    if (ptr_token->tokens == NULL)
    {
        printf("Failed to allocate memory");
        exit(1);
    }
    ptr_token->tokens_count = 0;
    ptr_token->capacity = initial_capacity;
}

void add_token(TOKENS_T *ptr_token, TOKEN_T token)
{
    if (ptr_token->capacity == ptr_token->tokens_count)
    {
        // TODO: realloc memory
        printf("\ncannot added token, limit reached\n");
        exit(1);
    }
    else
    {
        ptr_token->tokens[ptr_token->tokens_count] = token;
        ptr_token->tokens_count++;
    }
}

void free_tokens(TOKENS_T *ptr_token)
{
    free(ptr_token->tokens);
}