#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    FILE *fptr;
    int c;
    fptr = fopen("schema.prisma", "r");
    if (fptr == NULL)
    {
        printf("failed to open file");
        fclose(fptr);
        return 1;
    }
    int i = 0;
    int l = 0;
    int ip = 0;
    int in_word = 0;
    while ((c = getc(fptr)) != EOF)
    {
        if ((char)c == '\n')
        {
            l++;
        }
        if (!isspace((char)c))
        {
            if (!in_word)
            {
                ip = i;
            }
            in_word = 1;
            // Debug
            printf("%c", (char)c);
        }
        else if (in_word)
        {
            in_word = 0;
            // Debug
            printf(" ----- init: %d, end: %d, line: %d\n", ip, i, l);
        }
        i++;
    }

    fclose(fptr);
    return 0;
}