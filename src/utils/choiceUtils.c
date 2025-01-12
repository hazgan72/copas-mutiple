#include <stdio.h>
#include <ctype.h>
#include "../headers/choiceUtils.h"

void choiceYesOrNo(char title[255], int *isBool)
{
    char choice[10];
    while (1)
    {
        printf("%s", title);
        scanf("%9s", choice);

        char charFirst = tolower(choice[0]);

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

void choiceNumber(char title[255], int *number)
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
        else
        {
            break;
        }
    }
}
