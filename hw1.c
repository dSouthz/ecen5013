#include <stdio.h>
#include "hw1.h"

char reverse_string(char *str, int length)
{
    char *end;
    char tmp;
    int i;

    if (str == NULL)
    {
        return 1;
    }

    if (length < 0)
    {
        return 2;
    }

    end = str + length - 1;

    for (i=0; i<length/2; i++)
    {
        tmp = *str;
        *str = *end;
        *end = tmp;

        str++;
        end--;
    }

    return 0;
}


int main()
{
    char test1[] = "This is a string.";
    char test2[] = "some NUMbers12345";
    char test3[] = "Does it reverse \n\0\t correctly?";

    reverse_string(test1, 17);
    reverse_string(test2, 17);
    reverse_string(test3, 29);

    printf("%s\n%s\n%s\n", test1, test2, test3);

    return 0;
}


