#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "dirlist.h"

void printIt(DirEntry *entry) {
  PrintDirEntry(entry);
}

int main(int argc, char **argv) {
  DirEntryList *list = ReadDirectoryList(argv[1]);
  DirEntryList *dirs = FilterDirectoryList(list, DT_DIR);
  DirEntryList *files = FilterDirectoryList(list, DT_REG);
  DirEntryList *all = ReadDirectoryListForType(argv[1], DT_DIR);
  int i;

  printf("\x1b[1mDirectories (%d)\x1b[0m\n", dirs->count);
  ForEachDirEntry(dirs, printIt);

  printf("\n\x1b[1mFiles (%d)\x1b[0m\n", files->count);
  ForEachDirEntry(files, printIt);

  printf("\n\x1b[1mFiltered (%d)\x1b[0m\n", all->count);
  ForEachDirEntry(all, printIt);

  FreeDirectoryList(list);
  FreeDirectoryList(dirs);
  FreeDirectoryList(files);
  FreeDirectoryList(all);

  return(0);
}
