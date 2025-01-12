#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "../headers/fileUtils.h"

#ifdef _WIN32
#include <direct.h>
#define DIRECTORY_SEPARATOR '\\'
#else
#include <errno.h>
#define DIRECTORY_SEPARATOR '/'
#endif

const char *getFileExtension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return (dot && dot != filename) ? dot : "";
}

void createDirectoryRecursively(const char *path)
{
    char temp[512];
    snprintf(temp, sizeof(temp), "%s", path);
    for (char *p = temp + 1; *p; p++)
    {
        if (*p == '/' || *p == '\\')
        {
            *p = '\0';
#ifdef _WIN32
            _mkdir(temp);
#else
            mkdir(temp, 0777);
#endif
            *p = '/';
        }
    }
#ifdef _WIN32
    _mkdir(temp);
#else
    mkdir(temp, 0777);
#endif
}

void clearOutputDirectory(const char *outputDir, const char *title)
{
    char folderPath[512];
    snprintf(folderPath, sizeof(folderPath), "%s%c%s", outputDir, DIRECTORY_SEPARATOR, title);

    DIR *dir = opendir(folderPath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                char filePath[512];
                snprintf(filePath, sizeof(filePath), "%s%c%s", folderPath, DIRECTORY_SEPARATOR, entry->d_name);
                remove(filePath);
            }
        }
        closedir(dir);
    }
    else
    {
        createDirectoryRecursively(folderPath);
    }
}

void copyFile(const char *source, const char *destination)
{
    char buffer[1024];
    FILE *src = fopen(source, "rb");
    FILE *dest = fopen(destination, "wb");

    if (src == NULL)
    {
        fprintf(stderr, "Error opening source file: %s\n", source);
        return;
    }
    if (dest == NULL)
    {
        fprintf(stderr, "Error creating destination file: %s\n", destination);
        fclose(src);
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
        const char *extension = getFileExtension(files[i]);
        snprintf(destination, sizeof(destination), "output%c%s%c%s Episode %d%s%s",
                 DIRECTORY_SEPARATOR, title, DIRECTORY_SEPARATOR, title,
                 sequenceEpisode + i,
                 (i == fileCount - 1 && isAddEnding) ? " End" : "",
                 extension);

        copyFile(files[i], destination);
        printf("Copied: %s to %s\n", files[i], destination);
    }
}
