#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 50

// struct Table
// {
//     char *table_name;
// } Table_t;

void empty_buff(char buffer[BUFFER_SIZE])
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '-';
    }
    buffer[BUFFER_SIZE] = '\0';
}

int main()
{
    FILE *fptr;
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
    int is_table = 0;
    int is_column = 0;
    int is_param = 0;
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
            printf("%c", (char)current_char);
        }
        else if (in_word)
        {
            buffer_size = 0;
            in_word = 0;
            // Debug
            printf(" ----- init: %d, end: %d, line: %d\n", initial_position, i, line);
            // Debug
            printf("total: %d, buffer: %s\n", i - initial_position, buffer);
            int length = i - initial_position;
            char cmp[length];
            for (int i = 0; i < length; i++)
            {
                cmp[i] = buffer[i];
            }
            cmp[length] = '\0';
            if (strcmp("}", cmp) == 0)
            {
                is_column = 0;
            }
            if (is_param)
            {
                // debug: column param
                printf("column param: %s\n", cmp);
            }
            else if (is_column && strcmp("{", cmp) == 1)
            {
                // debug: column name
                printf("column name: %s\n", cmp);
                is_param = 1;
            }
            else if (is_table)
            {
                // debug: table name
                printf("table name: %s\n", cmp);
                is_table = 0;
                is_column = 1;
            }
            else if (strcmp("table", cmp) == 0 && !is_table)
            {
                is_table = 1;
            }
        }
        if ((char)current_char == '\n')
        {
            line++;
            is_param = 0;
        }
        i++;
    }
    fclose(fptr);
    return 0;
}