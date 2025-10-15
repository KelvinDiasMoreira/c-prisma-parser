#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum
{
    KEYWORD_TABLE,
    OPENING_BRACE,
    CLOSING_BRACE,
} KTOKENS;

typedef struct TOKEN
{
    int start;
    int line;
    KTOKENS type;
} TOKEN_T;

typedef struct TOKENS
{
    TOKEN_T *tokens;
    int tokens_count;
} TOKENS_T;

#define BUFFER_SIZE 50

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
    ptr_token->tokens_count = 0;
}

void free_tokens(TOKENS_T *ptr_token)
{
    free(ptr_token->tokens);
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
            // Debug
            // printf("%c", (char)current_char);
        }
        else if (in_word)
        {
            buffer_size = 0;
            in_word = 0;
            int total_length = i - initial_position;
            for (int i = 0; i < total_length; i++)
            {
                printf("%c", buffer[i]);
            }
            printf("\n");
        }
        if ((char)current_char == '\n')
        {
            line++;
        }
        i++;
    }
    fclose(fptr);
    free_tokens(&tokens);
    return 0;
}