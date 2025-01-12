#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "../headers/fileUtils.h"

const char *getFileExtension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return (dot && dot != filename) ? dot : "";
}

void clearOutputDirectory(const char *outputDir, const char *title)
{
    char folderPath[512];
    snprintf(folderPath, sizeof(folderPath), "%s/%s", outputDir, title);

    DIR *dir = opendir(folderPath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                char filePath[512];
                snprintf(filePath, sizeof(filePath), "%s/%s/%s", outputDir, title, entry->d_name);
                remove(filePath);
            }
        }
        closedir(dir);
    }
    else
    {
#ifdef _WIN32
        _mkdir(folderPath);
#else
        mkdir(folderPath, 0777);
#endif
    }
}

void copyFile(const char *source, const char *destination)
{
    char buffer[1024];
    FILE *src = fopen(source, "rb");
    FILE *dest = fopen(destination, "wb");

    if (src == NULL || dest == NULL)
    {
        perror("Error opening file");
        return;
    }

    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, n, dest);
    }

    fclose(src);
    fclose(dest);
}

void copyAllFiles(char files[][255], int fileCount, const char *title, int sequenceEpisode, int isEpisode, int isAddEnding)
{

    char destination[512];
    for (int i = 0; i < fileCount; i++)
    {
        char num[12];

        const char *extension = getFileExtension(files[i]);

        snprintf(destination, sizeof(destination), "output/%s/%s ", title, title);
        sprintf(num, "%d", sequenceEpisode + i);

        if (isEpisode)
            strcat(destination, "Episode ");

        strcat(destination, num);

        if (i == fileCount - 1 && isAddEnding)
            strcat(destination, " End");

        strcat(destination, extension);

        copyFile(files[i], destination);
        printf("Copied: %s to %s\n", files[i], destination);
    }
}
