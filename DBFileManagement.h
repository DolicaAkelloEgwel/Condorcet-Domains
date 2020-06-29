/**************************************************************************
* FILENAME:                                                               *
*   FileManagement.h                                                      *
*                                                                         *
* PURPOSE:                                                                *
*   Define methods for handling files                                     *
*                                                                         *
**************************************************************************/

// Opens a file for Phase 2.
// Exported to makeDomains.c and to tests.c.
FILE* openFile(int i, int in, int write);

// Opens another file for Phase 3.
// Exported to tests.c
FILE* P3openFile(int n, int size, int write);

// Opens another file read and write, for Phase 3.
// Exported to tests.c
FILE* openStats(int n);

// Sees that all ou_n files contain `Normal Exit'.
// Exported to tests.c  May be commented out as it runs slowly on very big files.
void checkfiles(int file_no);

// Open another file for better stats.
FILE* DopenStats(int n);
