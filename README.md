# Small C Library for parsing directories

## Introduction
A library written for the Amiga to allow for easy reading of directories
and relative entries. It handles memory management and relies on
`dirent.h` to parse directories.

## Examples

```c
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include "dirent.h"

void printIt(DirEntry *entry) {
  PrintDirEntry(entry);
}

int main(int argc, char **argv) {
  DirEntryList *list = ReadDirectoryList(".");
  ForEachDirEntry(list, printIt);
  FreeDirectoryList(list);

  return 0;  
}
```

## New Types

`DirEntryList` is a structure with the following format:

```c
typedef struct {
  DirEntry **entries;
  unsigned int count;
  unsigned int allocated;
} DirEntryList;
```

`DirEntry` is a typedef for `struct dirent`

`DirEntryType` is a typedef for `unsigned char`

`DirEntryIterator` is a typedef for `void (*)(DirEntry *)`

`DirEntryType DT_IGNORE` is used for `FilterDirectoryList()` and `ReadDirectoryListForType()`

## API

```c
DirEntryList *FilterDirectoryList(DirEntryList *list, DirEntryType type);
  Parameters:
    list - a DirEntryList structure whose data should be filtered
    type - one of the dirent.h types to filter for
  Return:
    a newly allocated entry list with only the filtered items on it

DirEntryList *ReadDirectoryList(char *directory);
  Parameters:
    directory - a string denoting the path of the directory to read
  Return:
    a newly allocated entry list with all the entries in the directory shown

DirEntryList *ReadDirectoryListForType(char *directory, DirEntryType type);
  Parameters:
    directory - a string denoting the path of the directory to read
    type - one of the dirent.h types to filter for
  Return:
    a newly allocated entry list with only the filtered items on it

DirEntryList *NewDirectoryList(unsigned int entryCount);
  Parameters:
    entryCount - the number of allocated items that should be prepared
  Return:
    a newly allocated entry list with empty space allocated for each item

DirEntryList *ResizeDirectoryList(DirEntryList *list, unsigned int newSize);
  Parameters:
    list - a list of directory entries
    newSize - the number of entries to resize the list to
  Return:
    the same list but shrunken or enlarged as requested

void FreeDirectoryList(DirEntryList *entryList);
  Parameters:
    entryList - the list to free memory for

void ForEachDirEntry(DirEntryList *list, DirEntryIterator iteratorFunction);
  Parameters:
    list - the directory list to walk
    iteratorFunction - a function pointer that works on each entry

void PrintDirEntry(DirEntry *entry);
  Parameters:
    entry - the entry to read such that it can be printed

char *DirEntryAsString(DirEntry *entry);
  Parameters:
    entry - the entry to create a string describing
  Return:
    a string that must have free() called on it when done

const char *DirEntryTypeName(DirEntryType type);
  Parameters:
    type - the type as specified in dirent.h
  Return:
    a constant string denoting the type. DT_REG is "DT_REG" for example

const char *DirEntryTypeDesc(DirEntryType type);
  Parameters:
    type - the type as specified in dirent.h
  Return:
    a constant string describing the type. DT_REG is "Regular File" for example
```
