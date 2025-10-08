#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE_TOKEN_ARR 20
#define SIZE_TOKEN_ARR_DATA 2

typedef struct KposItem
{
    char *column_name;
    char *column_type;
    int size;
} KposItem_t;
typedef struct Kpos
{
    int line;
    char *name;
    KposItem_t data[SIZE_TOKEN_ARR_DATA];
    int size;
} Kpos_t;

typedef struct Kparser
{
    Kpos_t arr[SIZE_TOKEN_ARR];
} Kparser_t;

void free_struct(Kparser_t *lex)
{
    for (size_t i = 0; i < SIZE_TOKEN_ARR; i++)
    {
        if (lex->arr[i].size != -1)
        {
            free(lex->arr[i].name);
            for (size_t j = 0; j < SIZE_TOKEN_ARR_DATA; j++)
            {
                if (lex->arr[i].data[j].size != 1)
                {
                    free(lex->arr[i].data[j].column_name);
                    free(lex->arr[i].data[j].column_type);
                }
            }
        }
    }
}

void fill_token_arr(Kparser_t *lex)
{
    for (int i = 0; i < SIZE_TOKEN_ARR; i++)
    {
        lex->arr[i].line = -1;
        lex->arr[i].name = NULL;
        lex->arr[i].size = -1;
        for (size_t j = 0; j < SIZE_TOKEN_ARR_DATA; j++)
        {
            lex->arr[i].data[j].column_name = NULL;
            lex->arr[i].data[j].column_type = NULL;
            lex->arr[i].data[j].size = -1;
        }
    }
}

void print_token_arr(Kparser_t lex)
{
    printf("[\n");
    for (int i = 0; i < SIZE_TOKEN_ARR; i++)
    {
        printf("{");
        printf("line: %d, name: %s, size: %d, data: [", lex.arr[i].line, lex.arr[i].name, lex.arr[i].size);
        for (int j = 0; j < SIZE_TOKEN_ARR_DATA; j++)
        {
            printf("{column_name: %s, column_type: %s, size: %d}", lex.arr[i].data[j].column_name, lex.arr[i].data[j].column_type, lex.arr[i].data[j].size);
            if (j == SIZE_TOKEN_ARR_DATA - 1)
                continue;
            printf(",");
        }
        printf("]");
        printf("}");
        if (i == SIZE_TOKEN_ARR - 1)
            continue;
        printf(",\n");
    }
    printf("\n]");
}

int find_index_table(char *buffer)
{
    char *search_str = "table";
    char *find = strstr(buffer, search_str);
    if (find == NULL)
    {
        return -1;
    }
    return find - buffer;
}

void handle_columns(char *buffer, Kparser_t *lex, int *line, int column_table)
{
    int i = 0;
    int c = 0;
    int init = 0;
    int in_word = 0;
    int qt_row = 0;

    // shit code
    while (buffer[i] != '\0' && buffer[i] != '}')
    {
        if (!isspace(buffer[i]))
        {
            if (!in_word)
            {
                init = i;
            }
            in_word = 1;
            c++;
        }
        else if (in_word)
        {
            // to debug
            printf("start %d, end: %d, line inside table: %d, total params: %d\n", init, i, column_table, qt_row);
            in_word = 0;
            init = 0;
            c = 0;
            qt_row++;
        }
        i++;
    }
}

int handle_table(char *buffer, Kparser_t *lex, int *line, int *column_table)
{
    char *table_name = NULL;
    lex->arr[*line].line = *line;
    while (*buffer != ' ')
    {
        buffer++;
    }
    // remove the space
    buffer++;
    int s = 0;
    for (size_t i = 0; i < 50; i++)
    {
        if (buffer[i] == ' ')
        {
            break;
        }
        s++;
    }
    table_name = malloc(sizeof(char) * s);
    if (table_name == NULL)
    {
        printf("(ERROR): Failed to allocate memory");
        return 1;
    }
    for (int i = 0; i < s; i++)
    {
        if (buffer[i] == ' ')
        {
            break;
        }
        table_name[i] = buffer[i];
    }
    table_name[s] = '\0';
    lex->arr[*line].size = s;
    lex->arr[*line].name = table_name;
    *line += 1;
    *column_table = 0;
}

int parse_buffer(char *buffer, Kparser_t *lex, int *line, int *column_table)
{
    int start = find_index_table(buffer);
    if (start == -1)
    {
        if (*column_table >= SIZE_TOKEN_ARR_DATA)
        {
            if (buffer[0] != '}')
            {
                printf("(ERROR): Max table columns reached");
                return 1;
            }
            *column_table = 0;
        }
        handle_columns(buffer, lex, line, *column_table);
        *column_table += 1;
        return 0;
    }
    if (handle_table(buffer, lex, line, column_table) == 1)
    {
        return 1;
    }
    return 0;
}

int main()
{
    FILE *fptr;
    Kparser_t lexical = {0};
    char buffer[100];
    fill_token_arr(&lexical);
    /*
        TODO: must change to get the current location when the program is called using the args
        and check the folder and file that is default i think.
    */
    fptr = fopen("schema.prisma", "r");
    if (fptr == NULL)
    {
        printf("failed to open file");
        fclose(fptr);
        return 1;
    }
    int i = 0;
    int j = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        if (i >= SIZE_TOKEN_ARR)
        {
            printf("max table size reached");
            return 1;
        }
        if (parse_buffer(buffer, &lexical, &i, &j) == 1)
        {
            free_struct(&lexical);
            fclose(fptr);
            return 1;
        }
    }
    fclose(fptr);
    // to debug only
    // print_token_arr(lexical);
    free_struct(&lexical);
    return 0;
}