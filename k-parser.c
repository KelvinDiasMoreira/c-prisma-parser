#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum
{
    KEYWORD_TABLE,
    IDENTIFIER,
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

#define BUFFER_SIZE 50

const char *token_to_string(KTOKENS token)
{
    switch (token)
    {
    case KEYWORD_TABLE:
        return "KEYWORD_TABLE";
    case IDENTIFIER:
        return "IDENTIFIER";
    case OPENING_BRACE:
        return "OPENING_BRACE";
    case CLOSING_BRACE:
        return "CLOSING_BRACE";
    default:
        "UNKNOWN";
    }
}

void empty_buff(char buffer[BUFFER_SIZE])
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '-';
    }
    buffer[BUFFER_SIZE] = '\0';
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

void tokenize_word(TOKENS_T *tokens, char *word, int initial_position, int line)
{
    // need free only when the token not have a value, like KEYWORDS,OPENING_BRACE etc..
    if (strcmp("table", word) == 0)
    {
        TOKEN_T token = {.line = line, .start = initial_position, .type = KEYWORD_TABLE};
        add_token(tokens, token);
        free(word);
    }
    else if (strcmp("{", word) == 0)
    {
        TOKEN_T token = {.line = line, .start = initial_position, .type = OPENING_BRACE};
        add_token(tokens, token);
        free(word);
    }
    else if (strcmp("}", word) == 0)
    {
        TOKEN_T token = {.line = line, .start = initial_position, .type = CLOSING_BRACE};
        add_token(tokens, token);
        free(word);
    }
    else
    {
        TOKEN_T token = {.line = line, .start = initial_position, .type = IDENTIFIER, .value = word};
        add_token(tokens, token);
    }
}

int main()
{
    FILE *fptr;
    TOKENS_T tokens;
    init_tokens(&tokens, 20);

    fptr = fopen("schema.prisma", "r");
    if (fptr == NULL)
    {
        printf("failed to open file");
        fclose(fptr);
        return 1;
    }
    char buffer[BUFFER_SIZE];
    empty_buff(buffer);
    int current_char;
    int i = 0;
    int line = 0;
    int initial_position = 0;
    int in_word = 0;
    int buffer_size = 0;
    while ((current_char = getc(fptr)) != EOF)
    {
        if (!isspace((char)current_char))
        {
            if (!in_word)
            {
                initial_position = i;
            }
            in_word = 1;
            if (buffer_size >= BUFFER_SIZE)
            {
                printf("\n[ERROR] Buffer exceed\n");
                return 1;
            }
            buffer[buffer_size] = (char)current_char;
            buffer_size++;
        }
        else if (in_word)
        {
            buffer_size = 0;
            in_word = 0;
            int total_length = i - initial_position;
            char *cmp_buff = malloc(sizeof(char) * total_length);
            if (cmp_buff == NULL)
            {
                printf("Failed to allocate memory\n");
                exit(1);
            }
            for (int i = 0; i < total_length; i++)
            {
                cmp_buff[i] = buffer[i];
            }
            cmp_buff[total_length] = '\0';
            tokenize_word(&tokens, cmp_buff, initial_position, line);
        }
        if ((char)current_char == '\n')
        {
            line++;
        }
        i++;
    }
    if (in_word)
    {
        buffer_size = 0;
        in_word = 0;
        int total_length = i - initial_position;
        char *cmp_buff = malloc(sizeof(char) * total_length);
        if (cmp_buff == NULL)
        {
            printf("Failed to allocate memory\n");
            exit(1);
        }
        for (int i = 0; i < total_length; i++)
        {
            cmp_buff[i] = buffer[i];
        }
        cmp_buff[total_length] = '\0';
        tokenize_word(&tokens, cmp_buff, initial_position, line);
    }
    fclose(fptr);
    // debug
    for (int i = 0; i < tokens.tokens_count; i++)
    {
        printf("value: %s, start: %d, line: %d, type: %s\n", tokens.tokens[i].value, tokens.tokens[i].start, tokens.tokens[i].line, token_to_string(tokens.tokens[i].type));
    }
    free_tokens(&tokens);
    return 0;
}