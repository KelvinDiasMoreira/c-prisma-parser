#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_TOKEN_ARR 20

typedef struct Kpos {
    int line;
    char *name;
    int size;
} Kpos_t;

typedef struct Kparser {
    Kpos_t arr[SIZE_TOKEN_ARR];
} Kparser_t;

void free_struct(Kparser_t *lex){
    for(size_t i = 0; i < SIZE_TOKEN_ARR; i++){
        if(lex->arr[i].size != -1){
            free(lex->arr[i].name);
        }
    }
}

void fill_token_arr(Kparser_t *lex){
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        lex->arr[i].line = -1;
        lex->arr[i].name = NULL;
        lex->arr[i].size = -1;
    }
}

void print_token_arr(Kparser_t lex){
    printf("[");
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        printf("{");
        printf("line: %d, name: %s, size: %d", lex.arr[i].line, lex.arr[i].name, lex.arr[i].size);
        printf("}");
        if(i == SIZE_TOKEN_ARR - 1) continue;
        printf(", ");
    }
    printf("]");
}

int find_index_table(char *buffer){
    char *search_str = "table";
    char *find = strstr(buffer, search_str);
    if(find == NULL){
        return -1;
    }
    return find - buffer;
}

int find_and_put(char *buffer, Kparser_t *lex, int *line){
    int start = find_index_table(buffer);
    if(start == -1){
        int i = 0;
        while(buffer[i] != '\0'){
            printf("%c\n", buffer[i]);
            // if(buffer[i] == ' ') 
            i++;
        }
        return 0;
    }
    char *table_name = NULL;
    lex->arr[*line].line = *line;
    while(*buffer != ' '){
        buffer++;
    }
    // remove the space
    buffer++;
    int s = 0;
    for (size_t i = 0; i < 50; i++){
        if(buffer[i] == ' '){
            break;
        }
        s++;
    }
    table_name = malloc(sizeof(char) * s);
    if(table_name == NULL){
        printf("(ERROR): Failed to allocate memory");
        return 1;
    }
    for (int i = 0; i < s; i++){
        if(buffer[i] == ' '){
            break;
        }
        table_name[i] = buffer[i];
    }
    table_name[s] = '\0';
    lex->arr[*line].size = s;
    lex->arr[*line].name = table_name;
    *line += 1;
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
    if(fptr == NULL){
        printf("failed to open file");
        fclose(fptr);
        return 1;
    }
    int i = 0;
    while(fgets(buffer, sizeof(buffer), fptr) != NULL){
        if(find_and_put(buffer, &lexical, &i) == 1){
            free_struct(&lexical);
            fclose(fptr);
            return 1;
        }
    }
    fclose(fptr);
    print_token_arr(lexical);
    free_struct(&lexical);
    return 0;
}