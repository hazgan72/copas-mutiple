#include <stdio.h>
#include <ctype.h>
#include "../headers/choiceUtils.h"

void choiceYesOrNo(const char *title, int *isBool)
{
    char choice[10];

    while (1)
    {
        printf("%s", title);
        scanf("%9s", choice);

        while (getchar() != '\n')
            ;

        char charFirst = tolower((unsigned char)choice[0]);

        if (charFirst == 'y')
        {
            *isBool = 1;
            break;
        }
        else if (charFirst == 'n')
        {
            *isBool = 0;
            break;
        }
        else
        {
            printf("Invalid input. Please enter 'y' for Yes or 'n' for No.\n");
        }
    }
}

void choiceNumber(const char *title, int *number)
{
    while (1)
    {
        printf("%s", title);

        if (scanf("%d", number) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");

            while (getchar() != '\n')
                ;
        }
        else if (*number >= 0)
        {
            break;
        }
    }
}
