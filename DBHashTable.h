/* The parameter is the number of cells in the hash table.  It should be odd. */
DBHashTable* initialiseDBHashTable(int size);

/* Returns a hash value in the range 0 to size - 1. */
int dbHashValue(DataBase* database_ptr, int size, int degree);

DBHashEntry setDBHashEntry(DataBase* database_ptr, int degree);

/* Insert a data base into the Hash table. */
/* If the hash table is full its contents will be rehashed, and the new adress is returned. */
DBHashTable* insertDBHashTable(DBHashTable* dbht_ptr, DataBase* database_ptr, int degree);

void printDBHashTable(DBHashTable dbht, int degree);

void fprintDBHashTable(DBHashTable *htp, int degree, FILE *fp);

void fprintDBHashEntry(DBHashEntry dbhe, int degree, FILE *file_ptr);

/* The file contains a hash table of data bases.  These data bases are copied into 
 the hash table dbht_ptr.  */
DBHashTable* freadDBHashTable(DBHashTable* dbht_ptr, int degree, FILE* file_ptr);

/* Is a data base with the same data as database contained in the hash table? */
int inDBHashTable(DBHashTable db_hashtable, DataBase* database_ptr, int degree);

/* For use in freeDBHashTable.  */
void freeDBLinkedList(DBHashEntry* db_he_ptr, int degree);

void freeDBHashTable(DBHashTable* db_ht_ptr, int degree);

/* New hash table has twice the size of the old hash table + 1.
 Checks to see if this will exceed maximum supposed size for a hash table.  */
DBHashTable* dbReHash(DBHashTable *old_dbht_ptr, int degree);

int SafeToPrint = 1;
