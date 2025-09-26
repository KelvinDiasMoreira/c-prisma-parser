#include <stdio.h>
#include <string.h>

#define SIZE_TOKEN_ARR 20

typedef struct Kparser
{
    int token_arr[SIZE_TOKEN_ARR];
} Kparser_t;

void fill_token_arr(Kparser_t *lex){
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        lex->token_arr[i] = -1;
    }
}

void print_token_arr(Kparser_t lex){
    for(int i = 0; i < SIZE_TOKEN_ARR; i++){
        printf("pos: %d\n", i);
        printf("value: %d\n", lex.token_arr[i]);
    }
}

int main()
{
    FILE *fptr;
    Kparser_t lexical;
    char buffer[100];
    fill_token_arr(&lexical);
    print_token_arr(lexical);
    // fptr = fopen("schema.prisma", "r");
    // if(fptr == NULL){
    //     printf("failed to open file");
    // }
    // if(fgets(buffer, sizeof(buffer), fptr) != NULL){
    //     printf("%s", buffer);

    // }
    // fclose(fptr);
    return 0;
}