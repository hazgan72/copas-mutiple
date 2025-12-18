#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#define MAX_FILES 255
#define MAX_FILENAME 256 // 255 + '\0'

void clearOutputDirectory(const char *outputDir, const char *title);
void createDirectoryRecursively(const char *path);
void copyFile(const char *source, const char *destination);

void copyAllFiles(
    char files[][MAX_FILENAME],
    int fileCount,
    const char *title,
    int sequenceEpisode,
    int isEpisode,
    int isAddEnding);

void getFiles(char files[MAX_FILES][MAX_FILENAME], int *fileCount);

#endif
