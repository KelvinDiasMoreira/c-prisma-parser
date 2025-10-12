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
    int c;
    int i = 0;
    int l = 0;
    int ip = 0;
    int in_word = 0;
    int bs = 0;
    int is_table = 0;
    int is_column = 0;
    int is_param = 0;
    while ((c = getc(fptr)) != EOF)
    {
        if (!isspace((char)c))
        {
            if (!in_word)
            {
                ip = i;
            }
            in_word = 1;
            if (bs >= BUFFER_SIZE)
            {
                printf("buffer exceed");
                return 1;
            }
            buffer[bs] = (char)c;
            bs++;
            // Debug
            printf("%c", (char)c);
        }
        else if (in_word)
        {
            bs = 0;
            in_word = 0;
            // Debug
            printf(" ----- init: %d, end: %d, line: %d\n", ip, i, l);
            // Debug
            printf("total: %d, buffer: %s\n", i - ip, buffer);
            int length = i - ip;
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
        if ((char)c == '\n')
        {
            l++;
            is_param = 0;
        }
        i++;
    }
    fclose(fptr);
    return 0;
}