#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "../headers/fileUtils.h"

#ifdef _WIN32
#include <direct.h>
#define DIRECTORY_SEPARATOR '\\'
#else
#include <unistd.h>
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
    if (snprintf(temp, sizeof(temp), "%s", path) >= sizeof(temp))
    {
        fprintf(stderr, "Path too long\n");
        return;
    }
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

    if (snprintf(folderPath, sizeof(folderPath), "%s%c%s",
                 outputDir, DIRECTORY_SEPARATOR, title) >= sizeof(folderPath))
    {
        fprintf(stderr, "Path too long\n");
        return;
    }

    DIR *dir = opendir(folderPath);
    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
                continue;

            char filePath[512];
            if (snprintf(filePath, sizeof(filePath), "%s%c%s",
                         folderPath, DIRECTORY_SEPARATOR, entry->d_name) >= sizeof(filePath))
            {
                continue;
            }

            struct stat st;
            if (stat(filePath, &st) == 0 && S_ISREG(st.st_mode))
            {
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

void getFiles(char files[MAX_FILES][MAX_FILENAME], int *fileCount)
{
    DIR *dir;
    struct dirent *entry;
    const char *dirPath = ".";

    if ((dir = opendir(dirPath)) == NULL)
    {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        /* skip . and .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        /* prevent overflow */
        if (*fileCount >= MAX_FILES)
            break;

        char filePath[512];
#ifdef _WIN32
        snprintf(filePath, sizeof(filePath), "%s\\%s", dirPath, entry->d_name);
#else
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
#endif

        struct stat st;
        if (stat(filePath, &st) != 0)
            continue;

        /* only regular files */
        if (!S_ISREG(st.st_mode))
            continue;

        /* filter unwanted files */
        if (strcmp(entry->d_name, "main.c") == 0 ||
            strcmp(entry->d_name, "Makefile") == 0 ||
            strstr(entry->d_name, ".o") ||
            strstr(entry->d_name, ".exe") ||
            strstr(entry->d_name, ".c") ||
            strstr(entry->d_name, ".h") ||
            strstr(entry->d_name, ".md") ||
            !strrchr(entry->d_name, '.') ||
            entry->d_name[0] == '.')
            continue;

        strncpy(
            files[*fileCount],
            entry->d_name,
            MAX_FILENAME - 1);
        files[*fileCount][MAX_FILENAME - 1] = '\0';

        (*fileCount)++;
    }

    closedir(dir);
}

void copyAllFiles(char files[][MAX_FILENAME],
                  int fileCount,
                  const char *title,
                  int sequenceEpisode,
                  int isEpisode,
                  int isAddEnding)
{
    char destination[512];
    for (int i = 0; i < fileCount; i++)
    {
        const char *extension = getFileExtension(files[i]);

        if (snprintf(destination, sizeof(destination),
                     "output%c%s%c%s%s %d%s%s",
                     DIRECTORY_SEPARATOR, title,
                     DIRECTORY_SEPARATOR, title,
                     isEpisode ? " Episode" : "",
                     sequenceEpisode + i,
                     (i == fileCount - 1 && isAddEnding) ? " End" : "",
                     extension) >= sizeof(destination))
        {
            fprintf(stderr, "Destination path too long\n");
            continue;
        }

        copyFile(files[i], destination);
        printf("Copied: %s to %s\n", files[i], destination);
    }
}
