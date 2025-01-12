#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "./src/headers/fileUtils.h"
#include "./src/headers/choiceUtils.h"

#define MAX_FILES 255

void getFiles(char files[MAX_FILES][255], int *fileCount);

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
            title[strcspn(title, "\n")] = 0;

            if (strlen(title) == 0)
            {
                printf("Title cannot be empty. Please enter a valid title.\n");
            }
            else
            {
                break;
            }
        }
    }

    choiceNumber("What initial sequence of numbers do you want: ", &sequenceEpisode);
    choiceYesOrNo("Flag Episode (if this videos and want adding name episode after title) | Enter 'y' for Yes or 'n' for No: ", &isEpisode);
    choiceYesOrNo("Do you want to put an (end) mark at the end | Enter 'y' for Yes or 'n' for No: ", &isAddEnding);

    clearOutputDirectory("output", title);
    copyAllFiles(files, fileCount, title, sequenceEpisode, isEpisode, isAddEnding);

    printf("Press Enter to exit...");
    getchar();
    getchar();

    return 0;
}

void getFiles(char files[MAX_FILES][255], int *fileCount)
{
    DIR *dir;
    struct dirent *entry;
    char *dirPath = ".";

    if ((dir = opendir(dirPath)) == NULL)
    {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            if (strcmp(entry->d_name, "main.c") != 0 &&
                strcmp(entry->d_name, "Makefile") != 0 &&
                !strstr(entry->d_name, ".o") &&
                !strstr(entry->d_name, ".exe") &&
                !strstr(entry->d_name, ".c") &&
                !strstr(entry->d_name, ".h") &&
                !strstr(entry->d_name, ".md") &&
                entry->d_name[0] != '.')
            {
                strcpy(files[*fileCount], entry->d_name);
                (*fileCount)++;
            }
        }
    }
    closedir(dir);
}