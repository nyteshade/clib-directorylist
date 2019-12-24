#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

typedef struct dirent DirEntry;
typedef unsigned char DirEntryType;
typedef void (*DirEntryIterator)(DirEntry *entry);

typedef struct {
  DirEntry **entries;
  unsigned int count;
  unsigned int allocated;
} DirEntryList;

extern const DirEntryType DT_IGNORE;

extern const char *DT_BLK_STR;
extern const char *DT_CHR_STR;
extern const char *DT_DIR_STR;
extern const char *DT_FIFO_STR;
extern const char *DT_LNK_STR;
extern const char *DT_REG_STR;
extern const char *DT_SOCK_STR;
extern const char *DT_UNKNOWN_STR;

extern const char *DT_BLK_DESC;
extern const char *DT_CHR_DESC;
extern const char *DT_DIR_DESC;
extern const char *DT_FIFO_DESC;
extern const char *DT_LNK_DESC;
extern const char *DT_REG_DESC;
extern const char *DT_SOCK_DESC;
extern const char *DT_UNKNOWN_DESC;

DirEntryList *FilterDirectoryList(DirEntryList *list, DirEntryType type);
DirEntryList *ReadDirectoryList(char *directory);
DirEntryList *ReadDirectoryListForType(char *directory, DirEntryType type);
DirEntryList *NewDirectoryList(unsigned int entryCount);
DirEntryList *ResizeDirectoryList(DirEntryList *list, unsigned int newSize);
void FreeDirectoryList(DirEntryList *entryList);
void ForEachDirEntry(DirEntryList *list, DirEntryIterator iteratorFunction);
void PrintDirEntry(DirEntry *entry);
char *DirEntryAsString(DirEntry *entry);
const char *DirEntryTypeName(DirEntryType type);
const char *DirEntryTypeDesc(DirEntryType type);

#endif
