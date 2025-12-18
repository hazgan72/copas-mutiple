#include <stdio.h>
#include <string.h>
#include "./src/headers/fileUtils.h"
#include "./src/headers/choiceUtils.h"

int main()
{
    char title[255], files[MAX_FILES][255];
    int fileCount = 0, sequenceEpisode = 1, isEpisode = 0, isAddEnding = 0;

    getFiles(files, &fileCount);

    printf("=== Rename All Files In This Folder and Sort by ASC ===\n");
    while (1)
    {
        printf("Input Title: ");
        if (fgets(title, sizeof(title), stdin))
        {
            title[strcspn(title, "\n")] = '\0';
            if (strlen(title) >= 255)
            {
                printf("Title is too long. Maximum 255 characters.\n");
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
    choiceYesOrNo("Flag Episode (if this videos and want adding name episode after title) | Enter 'y' for Yes or 'n' for No: ", &isEpisode);
    choiceYesOrNo("Do you want to put an (end) mark at the end | Enter 'y' for Yes or 'n' for No: ", &isAddEnding);

    clearOutputDirectory("output", title);
    copyAllFiles(files, fileCount, title, sequenceEpisode, isEpisode, isAddEnding);

    printf("Press Enter to exit...");
    getchar();
    return 0;
}
