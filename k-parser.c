#include <string.h>
#include <ctype.h>
#include "token.h"

#define BUFFER_SIZE 50

void empty_buff(char buffer[BUFFER_SIZE])
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '-';
    }
    buffer[BUFFER_SIZE] = '\0';
}

void tokenize_word(TOKENS_T *tokens, char *word, int initial_position, int line)
{
    // need free only when the token not have a value, like KEYWORDS,OPENING_BRACE etc..
    TOKEN_T token = {.line = line, .start = initial_position};
    if (strcmp("table", word) == 0)
    {
        token.type = KEYWORD_TABLE;
        add_token(tokens, token);
        free(word);
    }
    else if (strcmp("{", word) == 0)
    {
        token.type = OPENING_BRACE;
        add_token(tokens, token);
        free(word);
    }
    else if (strcmp("}", word) == 0)
    {
        token.type = CLOSING_BRACE;
        add_token(tokens, token);
        free(word);
    }
    else if (word[0] == '@')
    {
        token.type = IDENTIFIER_ARG;
        token.value = word;
        add_token(tokens, token);
    }
    else
    {
        token.type = IDENTIFIER;
        token.value = word;
        printf("last token: %s\n", last_token(tokens));
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
    // char last_buffer[BUFFER_SIZE];
    empty_buff(buffer);
    // empty_buff(last_buffer);
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
    /* Debug */
    for (int i = 0; i < tokens.tokens_count; i++)
    {
        printf("value: %s, start: %d, line: %d, type: %s\n",
               tokens.tokens[i].value,
               tokens.tokens[i].start,
               tokens.tokens[i].line,
               token_to_string(tokens.tokens[i].type));
    }
    free_tokens(&tokens);
    return 0;
}