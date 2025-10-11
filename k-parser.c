#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 50

void empty_buff(char buffer[BUFFER_SIZE]){
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
            printf("buffer: %s\n", buffer);
        }
        if ((char)c == '\n')
        {
            l++;
        }
        i++;
    }
    fclose(fptr);
    return 0;
}