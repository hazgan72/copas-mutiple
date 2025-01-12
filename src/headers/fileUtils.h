#ifndef FILE_UTILS_H
#define FILE_UTILS_H

void clearOutputDirectory(const char *outputDir, const char *title);
void createDirectoryRecursively(const char *path);
void copyFile(const char *source, const char *destination);
void copyAllFiles(char files[][255], int fileCount, const char *title, int sequenceEpisode, int isEpisode, int isAddEnding);

#endif
