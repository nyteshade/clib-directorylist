#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/****** dirlist/DirEntry ******
*
*  NAME
*    DirEntry
*
*  SYNOPSIS
*    typedef struct dirent DirEntry
*
*  FUNCTION
*    An easy to read representation of struct dirent.
*
******/
typedef struct dirent DirEntry;

/****** dirlist/DirEntryType ******
*
*  NAME
*    DirEntryType
*
*  SYNOPSIS
*    typedef of `unsigned char` used by struct dirent
*
*  FUNCTION
*    To more directly codify the use of dirent->d_type
*    within the dirlist library, the unsigned char type
*    has been given a typedef to make its use more clear
*
******/
typedef unsigned char DirEntryType;

/****** dirlist/DirEntryIterator ******
*
*  NAME
*    DirEntryIterator
*
*  SYNOPSIS
*    Function typedef for use with ForEachDirEntry()
*
*  FUNCTION
*    Function pointer definition for use with the for
*    each functionality provided by ForEachDirEntry
*
*  SEE ALSO
*    ForEachDirEntry()
*
*  EXAMPLE
*    void printIt(DirEntry *entry) {
*      PrintDirEntry(entry);
*    }
*
*    int main(void) {
*      DirEntryIterator iterator = printIt;
*      ForEachDirEntry(list, iterator);
*    }
*
******/
typedef void (*DirEntryIterator)(DirEntry *entry);

/****** direntry/DirEntryList ******
*
*  NAME
*    DirEntryList
*
*  SYNOPSIS
*    Structure used to contain information about a list of DirEntry structs
*
*  FUNCTION
*    Creating a list of DirEntry objects to pass around is fine, but the
*    need to know how many there are is useful for iterating over the elements
*    and freeing memory.
*
******/
typedef struct {
  DirEntry **entries;
  unsigned int count;
  unsigned int allocated;
} DirEntryList;

/****** direntry/DT_IGNORE ******
*
*  NAME
*    DT_IGNORE
*
*  SYNOPSIS
*    Additional constant created, to used as a DirEntryType value, that
*    represents that types are not important.
*
*  FUNCTION
*    The DT_IGNORE type is used for filtering lists of DirEntry structures
*    as well as for retrieving them on the initial read.
*
******/
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

/****** direntry/FilterDirectoryList ******
*
*  NAME
*    FilterDirectoryList
*
*  SYNOPSIS
*    Given a list of type DirEntryList, filter only the values that match
*    the supplied DirEntryType.
*
*  FUNCTION
*    Creates a newly allocated list of type DirEntryList, copied from the
*    supplied original, that contains only elements that match the supplied
*    DirEntryType
*
*   INPUTS
*     list        - an instance of DirEntryList containing the elements to
*                   filter down to smaller subset
*     type        - a DirEntryType, or unsigned char, that represents the
*                   type of entry to filter for.
*
*   RESULTS
*     a newly allocated and created DirEntryList containing only the elements
*     requested. DT_DIR for directories or DT_REG for regular files, etc...
*
*   EXAMPLE
*     DirEntryList *all = ReadDirectoryList("c:");
*     DirEntryList *files = FilterDirectoryList(all, DT_REG);
*     DirEntryList *dirs = FilterDirectoryList(all, DT_DIR);
*
*   NOTE
*     Memory must be freed when no longer in use. Call FreeDirectoryList()
*     on the results of this function when they are no longer needed.
*
*   SEE ALSO
*     FreeDirectoryList()
*
******/
DirEntryList *FilterDirectoryList(DirEntryList *list, DirEntryType type);

/****** direntry/ReadDirectoryList ******
*
*  NAME
*    ReadDirectoryList
*
*  SYNOPSIS
*    Read the directory specified and return a list of DirEntry structures
*    in the form of a DirEntryList structure
*
*  FUNCTION
*    Reads a directory specified and returns a DirEntryList structure filled
*    with DirEntry structures for each element found. Relies on <dirent.h>
*    and its underlying struct dirent structures.
*
*   INPUTS
*     directory   - a string denoting the directory to read
*
*   RESULTS
*     a newly allocated DirEntryList structure with all populated elements; be
*     sure to call FreeDirectoryList() on the results when done.
*
*   EXAMPLE
*     DirEntryList *all = ReadDirectoryList("c:");
*
*   NOTE
*     Memory must be freed when no longer in use. Call FreeDirectoryList()
*     on the results of this function when they are no longer needed.
*
*   SEE ALSO
*     FreeDirectoryList()
*
******/
DirEntryList *ReadDirectoryList(char *directory);

/****** direntry/ReadDirectoryListForType ******
*
*  NAME
*    ReadDirectoryListForType
*
*  SYNOPSIS
*    Read the directory specified and return a list of DirEntry structures
*    in the form of a DirEntryList structure only if they match the supplied
*    type
*
*  FUNCTION
*    Reads a directory specified and returns a DirEntryList structure filled
*    with DirEntry structures for each element found. Relies on <dirent.h>
*    and its underlying struct dirent structures. Only elements of the type
*    supplied will be returned
*
*   INPUTS
*     directory   - a string denoting the directory to read
*     type        - a constant from dirent.h or DT_IGNORE if all values are ok
*
*   RESULTS
*     a newly allocated DirEntryList structure with all populated elements; be
*     sure to call FreeDirectoryList() on the results when done.
*
*   EXAMPLE
*     DirEntryList *dirs = ReadDirectoryListForType("c:", DT_DIR);
*
*   NOTE
*     Memory must be freed when no longer in use. Call FreeDirectoryList()
*     on the results of this function when they are no longer needed.
*
*   SEE ALSO
*     FreeDirectoryList()
*
******/
DirEntryList *ReadDirectoryListForType(char *directory, DirEntryType type);

/****** direntry/NewDirectoryList ******
*
*  NAME
*    NewDirectoryList
*
*  SYNOPSIS
*    Creates a new DirEntryList structure with memory allocated for the
*    specified number of pointers to DirEntry structures.
*
*  FUNCTION
*    Should a blank DirEntryList be required, this function generate such
*    a list in a consistent fashion. The structures inside are all allocated
*    using calloc() and as such they will be zeroed out. The data here is
*    safe to be allocated or copied into using memcpy() or other similar
*    methods.
*
*   INPUTS
*     count       - number of DirEntry pointers to create space for
*
*   RESULTS
*     a newly allocated DirEntryList structure with zero populated elements,
*     but with pointers allocated for the specified number of entries
*
*   EXAMPLE
*     DirEntryList *fiveEmpty = NewDirectoryList(5);
*
*   NOTE
*     Memory must be freed when no longer in use. Call FreeDirectoryList()
*     on the results of this function when they are no longer needed.
*
*   SEE ALSO
*     FreeDirectoryList()
*
******/
DirEntryList *NewDirectoryList(unsigned int entryCount);

/****** direntry/ResizeDirectoryList ******
*
*  NAME
*    ResizeDirectoryList
*
*  SYNOPSIS
*    Given an existing DirEntryList, inreases or decreases the amount of
*    allocated space as specified.
*
*  FUNCTION
*    Increases, by allocating more memory, or decreases by safely freeing
*    up entries from the end of the list, using realloc(), the supplied
*    DirEntryList to the specified size.
*
*   INPUTS
*     list      - the DirEntryList to modify the size of
*     newSize   - the new size to resize the DirEntryList to
*
*   RESULTS
*     the supplied DirEntryList with a modified number of available entires
*
*   EXAMPLE
*     DirEntryList *all = ReadDirectoryList("c:");
*     ResizeDirectoryList(all, 5);
*     // Resizes the entries to only the first five elements
*
*   NOTE
*     Memory must be freed when no longer in use. Call FreeDirectoryList()
*     on the results of this function when they are no longer needed.
*
*   SEE ALSO
*     FreeDirectoryList()
*
******/
DirEntryList *ResizeDirectoryList(DirEntryList *list, unsigned int newSize);

/****** direntry/FreeDirectoryList ******
*
*  NAME
*    FreeDirectoryList
*
*  SYNOPSIS
*    Frees the memory allocated for the supplied DirEntryList, including all
*    allocated entries.
*
*  FUNCTION
*    Frees the memory allocated for the supplied DirEntryList, including all
*    allocated entries.
*
*   INPUTS
*     entryList - the DirEntryList to free
*
*   EXAMPLE
*     DirEntryList *all = ReadDirectoryList("c:");
*     FreeDirectoryList(all);
*
******/
void FreeDirectoryList(DirEntryList *entryList);

/****** direntry/ForEachDirEntry ******
*
*  NAME
*    ForEachDirEntry
*
*  SYNOPSIS
*    Iterates through each item in a DirEntryList and calls the supplied
*    DirEntryIterator function.
*
*  FUNCTION
*    Provides a more functional method for working on each element in a
*    given DirEntryList. For each element encountered, call the supplied
*    function pointer and pass in the entry in question.
*
*   INPUTS
*     list              - the DirEntryList to iterate over
*     iteratorFunction  - calls the DirEntryIterator function supplied
*
*   EXAMPLE
*     void printType(DirEntry *entry) {
*       printf("%s - %s", entry->d_name, DirEntryTypeDesc(entry->d_type));
*     }
*
*     DirEntryList *all = ReadDirectoryList("c:");
*     ForEachDirEntry(all, printType);
*
******/
void ForEachDirEntry(DirEntryList *list, DirEntryIterator iteratorFunction);

/****** direntry/PrintDirEntry ******
*
*  NAME
*    PrintDirEntry
*
*  SYNOPSIS
*    Calls DirEntryAsString() on the supplied entry and invokes printf()
*
*  FUNCTION
*    Using printf(), PrintDirEntry() will invoke the DirEntryAsString()
*    method on the supplied DirEntry structure and print out the name
*    followed by a string representation of the d_type and brief description
*    of that type.
*
*   INPUTS
*     entry     - the entry to be written to stdout.
*
*   EXAMPLE
*     void printEach(DirEntry *entry) {
*       PrintDirEntry(entry);
*     }
*
*     DirEntryList *all = ReadDirectoryList("c:");
*     ForEachDirEntry(all, printEach);
*
******/
void PrintDirEntry(DirEntry *entry);

/****** direntry/DirEntryAsString ******
*
*  NAME
*    DirEntryAsString
*
*  SYNOPSIS
*    Creates a string representation of a single DirEntry
*
*  FUNCTION
*    Creates an allocated string representation of a single DirEntry struct.
*    The string for a directory called "S" might look like
*
*    "S [DT_DIR/Directory]"
*
*   INPUTS
*     entry     - the entry to be stringified.
*
*   EXAMPLE
*     char *entry = DirEntryAsString(dirEntry);
*     printf("%s\n", entry);
*     free(entry);
*
*   NOTE
*     Memory is allocated using calloc and, as such, must be freed when
*     the string is no longer needed.
*
******/
char *DirEntryAsString(DirEntry *entry);

/****** direntry/DirEntryTypeName ******
*
*  NAME
*    DirEntryTypeName
*
*  SYNOPSIS
*    Converts the dirent.h constants to a string DT_DIR -> "DT_DIR"
*
*  FUNCTION
*    Returns the string constant matching the supplied dirent type.
*
*   INPUTS
*     entry     - the entry to be stringified.
*
*   EXAMPLE
*     printf("%s", DirEntryTypeName(DT_REG)); // "DT_REG"
*
*   NOTE
*     These are constants and as such should not be deallocated.
*
******/
const char *DirEntryTypeName(DirEntryType type);

/****** direntry/DirEntryTypeDesc ******
*
*  NAME
*    DirEntryTypeDesc
*
*  SYNOPSIS
*    Converts the dirent.h constants to a string description.
*
*  FUNCTION
*    There are several defined string constants for the types specified in
*    dirent.h. DT_DIR returns "Directory" and DT_REG returns "Regular File"
*
*   INPUTS
*     type        - the dirent.h constant for the type to be described
*
*   EXAMPLE
*     printf("%s", DirEntryTypeDesc(DT_REG)); // "Directory"
*
*   NOTE
*     These are constants and as such should not be deallocated.
*
******/
const char *DirEntryTypeDesc(DirEntryType type);

#endif
