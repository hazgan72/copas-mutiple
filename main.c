#include <stdio.h>
#include <string.h>
#include "./src/headers/fileUtils.h"
#include "./src/headers/choiceUtils.h"

int main()
{
    char title[MAX_FILENAME];
    char files[MAX_FILES][MAX_FILENAME];

    int fileCount = 0;
    int sequenceEpisode = 1;
    int isEpisode = 0;
    int isAddEnding = 0;

    getFiles(files, &fileCount);

    printf("=== Rename All Files In This Folder and Sort by ASC ===\n");

    while (1)
    {
        printf("Input Title: ");
        if (fgets(title, sizeof(title), stdin))
        {
            title[strcspn(title, "\n")] = '\0';

            if (strlen(title) >= MAX_FILENAME - 1)
            {
                printf("Title is too long. Maximum %d characters.\n", MAX_FILENAME - 1);
                continue;
            }
            if (strlen(title) == 0)
            {
                printf("Title cannot be empty. Please enter a valid title.\n");
                continue;
            }
            break;
        }
    }

    choiceNumber("What initial sequence of numbers do you want: ", &sequenceEpisode);
    choiceYesOrNo(
        "Flag Episode (if this videos and want adding name episode after title) | Enter 'y' for Yes or 'n' for No: ",
        &isEpisode);
    choiceYesOrNo(
        "Do you want to put an (end) mark at the end | Enter 'y' for Yes or 'n' for No: ",
        &isAddEnding);

    clearOutputDirectory("output", title);
    copyAllFiles(files, fileCount, title, sequenceEpisode, isEpisode, isAddEnding);

    printf("Press Enter to exit...");
    getchar();
    return 0;
}
