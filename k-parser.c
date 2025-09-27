#include <stdio.h>
#include <string.h>

#define SIZE_TOKEN_ARR 20

typedef struct Kpos {
    int init;
    int line;
} Kpos_t;

typedef struct Kparser {
    Kpos_t arr[SIZE_TOKEN_ARR];
} Kparser_t;

void fill_token_arr(Kparser_t *lex){
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        lex->arr[i].init = -1;
        lex->arr[i].line = -1;
    }
}

void print_token_arr(Kparser_t lex){
    printf("[");
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        printf("{");
        printf("init: %d ", lex.arr[i].init);
        printf("line: %d", lex.arr[i].line);
        printf("}");
        if(i == SIZE_TOKEN_ARR - 1) continue;
        printf(", ");
    }
    printf("]");
}

void find_and_put(char *buffer, Kparser_t *lex, int *line){
    char table_name[50];
    char *search_str = "table";
    char *find = strstr(buffer, search_str);
    if(find == NULL){
        printf("(DEBUG): Not found table in buffer\n");
        return;
    }
    int start = find - buffer;
    lex->arr[*line].init = start;
    lex->arr[*line].line = *line;
    while(*buffer != ' '){
        buffer++;
    }
    int j = 0;
    for(int i = start + 1 ; i < sizeof(table_name); i++){
        if(buffer[i] == ' '){
            break;
        }
        if(buffer[i] != ' '){
            table_name[j] = buffer[i];
            j++;
        }
    }
    printf("%s\n", table_name);
    *line += 1;
}

int main()
{
    FILE *fptr;
    Kparser_t lexical;
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
              find_and_put(buffer, &lexical, &i);
    }
    // if(fgets(buffer, sizeof(buffer), fptr) != NULL){
    //     find_and_put(buffer, &lexical);
    // }
    fclose(fptr);
    // print_token_arr(lexical);
    return 0;
}