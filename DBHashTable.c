DBHashTable* initialiseDBHashTable(int size) // Create a hash table of dtata bases with with size slots.
{
	DBHashTable *dbht_ptr; /* New DBHashTable */
	int i; /* Loop counter */

	// Malloc the DBHashTable

 	if(!(dbht_ptr = malloc(sizeof(DBHashTable))))
	{
		printf("Error in initialiseDBHashTable: malloc failed.");
		exit(0);
	}

	// Set the size and count

	dbht_ptr->size = size;
	dbht_ptr->count = 0;

	// Malloc the table field of the DBHashTable

	if(!(dbht_ptr->table = malloc(size * sizeof(DBHashEntry))))
	{
		printf("Error in initialiseDBHashTable: malloc failed.");
		exit(0);
	}

	// Set every element in the table array to null

	for (i = 0; i < size; i++)
	{
		dbht_ptr->table[i] = NULL;
	}

	return dbht_ptr;
}

int dbHashValue(DataBase* database_ptr, int size, int degree) // size < 2^{30}
{
    int i; /* Loop counter */
    int j; /* Loop counter */
    int length = getDataBaseLength(degree);
    UNINT dbhashvalue = 0; /* Hash value for the database in range 0 to size - 1 */

    // Compute the hash value

    for (i = 0; i < degree * (degree - 1); i++)
    {
        if (database_ptr->LocalGreater[i])
        {
            for (j = 0; j < length; j++)
            {
                dbhashvalue += database_ptr->LocalGreater[i][j] % size; // So now dbhashvalue < 2 ^ 31
                // dbhashvalue += (database_ptr->LocalGreater[i][j] >> 32) % size; // Now dbhashvlalue < 2 ^ 32.
                dbhashvalue = dbhashvalue % size; // Now dbhashvalue < size.
            }
        }
    }

    return (int) dbhashvalue;
}

DBHashEntry setDBHashEntry(DataBase* database_ptr, int degree)
// mallocs a dbhashentry, enters the database, and sets the next entry pointer to NULL.
{
    DBHashEntry dbhashentry;

    copyDataBase(database_ptr, &(dbhashentry.database), degree); // initialises dbhashentry.database

    dbhashentry.next = NULL;
    return dbhashentry;
}

DBHashTable* insertDBHashTable(DBHashTable* dbht_ptr, DataBase* database_ptr, int degree)
{
	DBHashEntry *dbhe_one_ptr; /* A DBHashEntry to store database */

    /* A DBHashEntry to traverse the linkedlist and check if the database is already
       in the HashTable */
    DBHashEntry *dbhe_two_ptr;

	int hashvalue; /* The database's hash value */
	int in_hash_table; /* Set to 1 if the database is in the HashTable */

    // printf("Entering DBHashTable.\n");
	// Find the database's hash value

	hashvalue = dbHashValue(database_ptr,dbht_ptr->size, degree);

	// Is this slot occupied?

	if (dbht_ptr->table[hashvalue] == NULL)
	{
        // printf("Hash table place was not occupied.\n");
		if (!(dbhe_one_ptr = malloc(sizeof(DBHashList))))  // typedef DBHashEntry* DBHashList
		{
			printf("Error in insertDBHashTable: malloc failed.");
			exit(0);
		}
		*dbhe_one_ptr = setDBHashEntry(database_ptr, degree);
        dbht_ptr->table[hashvalue] = dbhe_one_ptr;
        dbht_ptr->count++;
	}
	else
	{
        // printf("Hash table place was occupied.\n");
		dbhe_two_ptr = dbht_ptr->table[hashvalue];

		while ((!(in_hash_table = equalDataBases(database_ptr,&(dbhe_two_ptr->database),degree)))&&
         (dbhe_two_ptr->next != NULL))
		{
            // printf("in_hash_table = %d\n", in_hash_table);
			dbhe_two_ptr = dbhe_two_ptr->next;
        }

        // printf("Got to head of queue.\n");

		if(!in_hash_table)
		{
            // printf("Data base not in hash table.\n");
			if (!(dbhe_one_ptr = malloc(sizeof(DBHashList))))
			{
				printf("Error in insertDBHashTable: malloc failed.");
				exit(0);
			}

			*dbhe_one_ptr = setDBHashEntry(database_ptr, degree);
			dbhe_two_ptr->next = dbhe_one_ptr;
			dbht_ptr->count++;
            // printf("Data base added to hash table.\n");
		}
        else
        {
            printf("in_hash_table returning true.  Error\n");
            printf("database:\n");
            writeLaws(database_ptr, degree);
            printf("dbhe_two_ptr->database:\n");
            writeLaws(&(dbhe_two_ptr->database), degree);
            exit(0);
        }
	}


	// Check if the HashTable is 3/4 full
	// ReHash if necessary and possible

	if (dbht_ptr->count > 3 * (dbht_ptr->size/4))
	{
        printf("Rehashing.\n");

	    dbht_ptr = dbReHash(dbht_ptr,degree);
    }

    // printf("Leaving insertDBHashTable.\n");

    return dbht_ptr;
}

void printDBHashTable(DBHashTable dbht, int degree)
{
	int i; /* Loop counter */
	int size = dbht.size; /* Number of slots in DBHashTable */
	DBHashEntry *dbhe_ptr;

    for (i = 0; i < size; i++)
    {
		dbhe_ptr = dbht.table[i];

		while (dbhe_ptr != NULL)
		{
            writeDataBase(&(dbhe_ptr->database),degree);
            dbhe_ptr = dbhe_ptr->next;
		}
	}

	printf("Hashtable count: %d\n",dbht.count);
}

void fprintDBHashTable(DBHashTable *htp, int degree, FILE *fp)
{
    int i;
    int size = htp -> size;
    DBHashEntry *hep;

    for (i=0;  i<size;  i++) {
        hep = htp -> table[i];
        while (hep != NULL)  {
            fprintDBHashEntry(*hep, degree, fp);
            hep = hep -> next;
        }
    }
}

void fprintDBHashEntry(DBHashEntry dbhe, int degree, FILE *file_ptr)
{
    fprintDataBase(&(dbhe.database), degree, file_ptr);
}

DBHashTable* freadDBHashTable(DBHashTable* dbht_ptr, int degree, FILE* file_ptr)
// Read data bases from the file into the hash table.
{
    DataBase database;
    char ch;

    if (isEmpty(file_ptr))
        return dbht_ptr;

    initialiseDataBase(&database,degree);

    while ((!feof(file_ptr)) && (ch = fgetc(file_ptr)) != 'N')
    {
        freadDataBase(&database, degree, file_ptr);
        dbht_ptr = insertDBHashTable(dbht_ptr, &database, degree);
    }

    return dbht_ptr;
}


int inDBHashTable(DBHashTable db_hashtable, DataBase* database_ptr, int degree)
// Is this database in the hash table?
{
    DBHashEntry* db_he_ptr = db_hashtable.table[dbHashValue(database_ptr, db_hashtable.size, degree)];

    while (db_he_ptr != NULL)
    {
        if (equalDataBases(database_ptr,&(db_he_ptr->database),degree))
            return 1;

        db_he_ptr = db_he_ptr->next;
    }

    return 0;
}

void freeDBLinkedList(DBHashEntry* db_he_ptr, int degree)
{
    DBHashEntry* db_he_temp = db_he_ptr;

    while (db_he_temp != NULL)
    {
        db_he_ptr = db_he_ptr->next;
        freeDataBase(&(db_he_temp->database), degree);
        free(db_he_temp);
        db_he_temp = db_he_ptr;
    }
}

void freeDBHashTable(DBHashTable* db_ht_ptr, int degree)
{
    int i; /* Loop counter */
    int size = db_ht_ptr->size; /* Number of slots in the data base */
    // printf("Entering freeDBHashTable\n");
    // Call freeDBLinkedList() for every entry in DBHashEntry array

    for (i = 0; i < size; i++)
        if (db_ht_ptr->table[i] != NULL)
            freeDBLinkedList(db_ht_ptr->table[i],degree);
    // printf("Leaving freeDBHashTable.\n");
}

DBHashTable* dbReHash(DBHashTable *old_dbht_ptr, int degree)
{
    int i; /* Loop counter */
    DBHashTable *new_dbht_ptr; /* A new DBHashTable */
    DBHashEntry *db_he_ptr; /* DBHashEntry */

    // Initialise new DBHashTable

    if (old_dbht_ptr -> size >= 1 << 30)
    {
        printf("WARNING DBHashTable too full and too big to resize.\n");
        return old_dbht_ptr;
    }

    new_dbht_ptr = initialiseDBHashTable((old_dbht_ptr -> size * 2) + 1);
    new_dbht_ptr -> count = 0;

    // Copy all DataBases to new DBHashTable

    for (i = 0; i < old_dbht_ptr->size; i++)
	{
	 	db_he_ptr = old_dbht_ptr->table[i];

		while (db_he_ptr != NULL)
		{
            new_dbht_ptr = insertDBHashTable(new_dbht_ptr,&(db_he_ptr->database),degree);
            db_he_ptr = db_he_ptr -> next;
        }
    }

    // Free the old DBHashTable

    freeDBHashTable(old_dbht_ptr,degree);
    // if (SafeToPrint) printf("\nMade a new hashtable of size %d.\n",newht_ptr->size);

    return new_dbht_ptr;
}
