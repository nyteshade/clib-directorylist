#include "dirlist.h"
#include <stdio.h>

const DirEntryType DT_IGNORE = '?';

const char *DT_BLK_STR = "DT_BLK";
const char *DT_CHR_STR = "DT_CHR";
const char *DT_DIR_STR = "DT_DIR";
const char *DT_FIFO_STR = "DT_FIFO";
const char *DT_LNK_STR = "DT_LNK";
const char *DT_REG_STR = "DT_REG";
const char *DT_SOCK_STR = "DT_SOCK";
const char *DT_UNKNOWN_STR = "DT_UNKNOWN";

const char *DT_BLK_DESC = "Block";
const char *DT_CHR_DESC = "Character Device";
const char *DT_DIR_DESC = "Directory";
const char *DT_FIFO_DESC = "Named Pipe";
const char *DT_LNK_DESC = "Symbolic Link";
const char *DT_REG_DESC = "Regular File";
const char *DT_SOCK_DESC = "UNIX Domain Socket";
const char *DT_UNKNOWN_DESC = "Unknown File Type";

DirEntryList *ReadDirectoryList(char *directory) {
  return ReadDirectoryListForType(directory, DT_IGNORE);
}

DirEntryList *ReadDirectoryListForType(char *directory, DirEntryType type) {
  DirEntryList *list = 0L;
  DirEntry **entries = 0L;
  DirEntry *dir = 0L;
  DIR *d;
  int blockCount = 10;
  int allocCount = 10;
  int count = 0;

  list = calloc(1, sizeof(DirEntryList));
  if (!list) {
    return NULL;
  }

  entries = (DirEntry **)calloc(blockCount, allocCount * sizeof(DirEntry *));
  if (entries) {
    d = opendir(directory);

    if (d) {
      while ((dir = readdir(d)) != NULL) {
        if (type != DT_IGNORE) {
          if (type != dir->d_type) {
            continue;
          }
        }

        if (strcmp(".", dir->d_name) == 0 || strcmp("..", dir->d_name) == 0) {
          continue;
        }

        if (count == allocCount - 1) {
          entries = (DirEntry **)realloc(
            entries,
            (allocCount + blockCount) * sizeof(DirEntry *)
          );
          allocCount += blockCount;
        }

        entries[count] = calloc(1, sizeof(DirEntry));
        memcpy(entries[count], dir, sizeof(DirEntry));

        count++;
      }

      entries = (DirEntry **)realloc(entries, count * sizeof(DirEntry *));
    }

    list->entries = entries;
    list->count = count;
    list->allocated = count;

    closedir(d);
  }
  else {
    return NULL;
  }

  return list;
}

DirEntryList *FilterDirectoryList(DirEntryList *list, DirEntryType type) {
  DirEntryList *newList = (DirEntryList *)calloc(1, sizeof(DirEntryList));
  unsigned int newCount = 0;
  int i;

  newList->entries = calloc(list->count, sizeof(DirEntry *));
  newList->count = list->count;

  for (i = 0; i < list->count; i++) {
    if (list->entries[i]->d_type != type) {
      continue;
    }

    newList->entries[newCount] = calloc(1, sizeof(DirEntry));
    memcpy(newList->entries[newCount], list->entries[i], sizeof(DirEntry));
    newCount++;
  }

  newList->count = newCount;
  newList->allocated = newCount;
  newList->entries = realloc(newList->entries, newCount * sizeof(DirEntry *));

  return newList;
}

void ForEachDirEntry(DirEntryList *list, DirEntryIterator iteratorFunction) {
  int i;

  for (i = 0; i < list->count; i++) {
    iteratorFunction(list->entries[i]);
  }
}

DirEntryList *NewDirectoryList(unsigned int entryCount) {
  DirEntryList *list = calloc(1, sizeof(DirEntryList));

  list->count = 0;
  list->allocated = entryCount;
  list->entries = calloc(entryCount, sizeof(DirEntry *));

  return list;
}

DirEntryList *ResizeDirectoryList(DirEntryList *list, unsigned int newSize) {
  int i;

  if (newSize < list->allocated) {
    for (i = list->allocated - 1; i >= (list->allocated - newSize); i++) {
      if (list->entries[i]) {
        printf("Removing %s\n", list->entries[i]->d_name);

        free(list->entries[i]);
      }
    }
  }

  list = realloc(list, newSize * sizeof(DirEntry *));
  list->allocated = newSize;
  list->count = list->count < newSize ? newSize : list->count;

  return list;
}

void PrintDirEntry(DirEntry *entry) {
  char *string = DirEntryAsString(entry);

  printf("%s\n", string);
  free(string);
}

char *DirEntryAsString(DirEntry *entry) {
  int length = 5 + strlen(entry->d_name);
  char *string = 0L;

  length += strlen(DirEntryTypeName(entry->d_type));
  length += strlen(DirEntryTypeDesc(entry->d_type));

  string = malloc(length);
  sprintf(
    string,
    "%s [%s/%s]",
    entry->d_name,
    DirEntryTypeName(entry->d_type),
    DirEntryTypeDesc(entry->d_type)
  );

  return string;
}

void FreeDirectoryList(DirEntryList *list) {
  int i;

  if (!list) {
    return;
  }

  for (i = 1; i < list->count; i++) {
    free(list->entries[i]);
  }

  free(list->entries);
}

const char *DirEntryTypeName(DirEntryType type) {
  switch(type) {
    case DT_BLK:
      return DT_BLK_STR;
    case DT_CHR:
      return DT_CHR_STR;
    case DT_DIR:
      return DT_DIR_STR;
    case DT_FIFO:
      return DT_FIFO_STR;
    case DT_LNK:
      return DT_LNK_STR;
    case DT_REG:
      return DT_REG_STR;
    case DT_SOCK:
      return DT_SOCK_STR;
    case DT_UNKNOWN:
    default:
      return DT_UNKNOWN_STR;
  }
}

const char *DirEntryTypeDesc(DirEntryType type) {
  switch(type) {
    case DT_BLK:
      return DT_BLK_DESC;
    case DT_CHR:
      return DT_CHR_DESC;
    case DT_DIR:
      return DT_DIR_DESC;
    case DT_FIFO:
      return DT_FIFO_DESC;
    case DT_LNK:
      return DT_LNK_DESC;
    case DT_REG:
      return DT_REG_DESC;
    case DT_SOCK:
      return DT_SOCK_DESC;
    case DT_UNKNOWN:
    default:
      return DT_UNKNOWN_DESC;
  }
}
