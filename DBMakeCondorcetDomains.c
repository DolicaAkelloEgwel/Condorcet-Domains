/**************************************************************************
* FILENAME:                                                               *
*   DBMakeCondorcetDomains.c                                              *
*                                                                         *
* PURPOSE:                                                                *
*   Implements methods defined in DBMakeCondorcetDomains.h                *
*                                                                         *
* GLOBAL VARIABLES:                                                       *
*   makeclaws_trace - Flag for printing additional information during     *
*                     makeCondorcetLaws() execution; shows what           *
*                     law-triple and permutation is presently being       *
*                     examined                                            *
*   satisfied_trace - Flag for printing additional information during     *
*                     execution of makeCondorcetLaws(); displays result   *
*                     of isSatisfied() call along with law-triple         *
*                     and permutation that was tested                     *
*   file_count -      Counts files that have been written during          *
*                     makeCondorcetDomains() execution                    *
*                                                                         *
**************************************************************************/


int mCD_trace = 0;
int mCD_flag = 1;

/* Counts number of in_ files that have been written to during execution of
   makeCondorcetDomain */
int file_count = 0;

unsigned int leaf_count = 0;
unsigned int max_count = 0;

/*
* Name:
*   DBmakeCondorcetDomains(t_int,t_0,db,degree,dbht_ptr,k,phase,file_no, legal)
*
* Parameters:
*   t_int - An integer mapping of a triple
*   t_0 - In parallel mode, in phase 1, truncate at depth t_0 and send output to a file
*         in phase 2 set to total number of triples.
*   db - A data base
*   degree - The Permutation degree
*   dbht_ptr - A pointer to a HashTable.  If rehashing occurs the new pointer is returned; otherwise the old pointer
*   k is the cut off point.  Maximal Condorcet domains of size less than k are not constructed. k=-1 if no cut-off
*   phase is 1, 2, or 3.  Phase 1 works to depth t_0, and sends each leaf to an in_ file.
*   file_no - The number of the file from which input data is read in phase 2.
*   legal - a bit-string of length nine.  The n-th bit is set if the n-th law is allowed.
*   Description:
*   Recursively construct Condorcet Domains
*/
DBHashTable* DBmakeCondorcetDomains(int t_int, int t_0, DataBase* database_ptr, int degree, DBHashTable *dbht_ptr, int k, int phase, int file_no, unsigned int legal)
{
    if (mCD_trace)
    {
        printf("DBmakeCondorcetDomains called with phase = %d, legal = %d, file_no = %d, t_int = %d, t_0 = %d\n", phase, legal, file_no, t_int, t_0);
    }
    if (t_int>t_0) {
        if (phase != 1) {
            printf("mCD1 called with phase = %d", phase);
            exit(0);
        }
        DBmCD1(t_int, database_ptr, degree);

        if (mCD_trace)
        {
            printf("Leaving DBmCD1 with Database address: %p\n",database_ptr);
            if (mCD_trace > 1) writeDataBase(database_ptr, degree);
        }

        return dbht_ptr;
    }
    else if(t_int ==  getNoTrips(degree)) {
        if (phase != 2) {
            printf("mCD2 called with phase = %d", phase);
            exit(0);
        }

        if (mCD_trace > 1)
        {
            writeDataBase(database_ptr,degree);
        }
        
        return DBmCD2(database_ptr, degree, dbht_ptr, k);
    }

    if (mCD_trace)
    {
        printf("Making call to DBmCD3 with Database address: %p\n",database_ptr);

        if (mCD_trace > 1) writeDataBase(database_ptr, degree);
    }
    
    return DBmCD3(t_int, t_0, database_ptr, degree, dbht_ptr, k, phase, legal, file_no);
}

/* DBmCD1 is called when we have got to a node of depth t_int > t_0 in phase 1.  A new in_ file is opened,
 file_count is incremented, and the data base is written to the new file, which is then closed.  */
void DBmCD1(int t_int, DataBase* database_ptr, int degree)
{

    if (mCD_trace)
    {
        printf("DBmCD1 called with triple = %d\n", t_int);
    }
    
    if (mCD_flag)
    {
        file_count++;
        return;
    }
    
    FILE *f_ptr; /* Pointer to a file */

    // Open new in_ file for writing, and increase filecount

    f_ptr = openFile(file_count++,1,1);

    // Write triple and data base to file

    fprintf(f_ptr,"%d \n",t_int);
    
    fprintDataBase(database_ptr, degree, f_ptr);

    // Close file

    fclose(f_ptr);

    if (mCD_trace)
    {
        printf("Leaving DBmCD1.\n");
    }
}

int oneLaw(DataBase* database_ptr, int degree)
/* Does the data base have a triple obeying more than one law?
 If so return FALSE; else TRUE.
 Any law other than 1N1, 2N2, 3N3 is considered.  */
{
    int a, b, c, i, j, count;
    //  (a,b,c) is a triple, a < b < c, and iNj a law; 1 <= i != j <=3.
    
    for (a = 0; a < degree - 2; a++)
    {
        for (b = a + 1; b < degree - 1; b++)
        {
            for (c = b + 1; c < degree; c++)
            {
                count = 0;
                
                for (i = 1; i < 4; i++)
                {
                    for (j = 1; j < 4; j++)
                    {
                        if ((i != j) && deduceCondorcet(a,b,c,i,j,database_ptr,degree))
                        {
                            count++;
                            
                            if (count == 2)
                                return 0;
                                
                        }
                    }
                }
            }
        }
    }

    return 1;
    
}

/* DBmCD2 is called when a leaf has been reached in phase 2.  If the data base has no triple obeying two laws it is inserted
 in the hash table if it is of size at least k.  leaf_count and max_count are updated  */
DBHashTable *DBmCD2(DataBase* database_ptr, int degree, DBHashTable *dbht_ptr, int k)
{
 
    if (mCD_trace)
    {
        printf("Entered DBmCD2\n");
    }

    leaf_count++;

    // Check if the data set satisfies at most one law on each triple, and is of size at least k
    // If so, add to HashTable and print HashTable count

    if ((k == -1) || (DBsize(database_ptr, degree) > k))
    {
        if (oneLaw(database_ptr, degree))
        {
            max_count++;
            dbht_ptr = insertDBHashTable(dbht_ptr, database_ptr, degree);
        }
        else if (mCD_trace) printf("oneLaw returned false\n");
    }
    
    if (mCD_trace)
    {
        printf("Leaving DBmCD2\n");
    }
    
    return dbht_ptr;
}

int DBsatisfiesLaw(DataBase* database_ptr, int t_int, int degree) // Does this triple satisfy some law?
{
    triple trip;
    pair law;
    int lex;
    
    intToTriple(t_int, trip);
    for (lex = 0; lex < 6; lex++)
    {
        intToLaw(lex, law);
        if (deduceCondorcet(trip[0],trip[1],trip[2],law[0],law[1],database_ptr,degree))
        {
            return lex + 1;  // lex + 1 as must not return 0 here
        }
    }
    return 0;
}

/*  DBmCD3 is called when we need to advance to the next triple t_int.
 If a law is already satisfied for t_int then call DBmakeCondorcetDomains with t_int incremented by 1.
 Otherwise, for each law, impose that law on the triple t_int, and call DBmakeCondorcetDomains
 with t_int incremented by 1 if the size of the new perm set is at least k, and no triple satisfies 2 laws.  */
DBHashTable *DBmCD3(int t_int, int t_0, DataBase* database_ptr, int degree, DBHashTable *dbht_ptr, int k, int phase, int legal, int file_no)
{
    int lex; /* Loop counter: integer mapping of a law in range 0 to N_LAWS */
    int i;
    triple t;
    DataBase database_copy; /* Copy of DataBase db */
    pair law;

    if (mCD_trace)
    {
        printf("DBmCD3 called with phase = %d, legal = %d, and triple = %d\n", phase, legal, t_int);
        if (mCD_trace > 1) writeDataBase(database_ptr, degree);
    }
    // Check if the database already satisfies a law on this triple

	if (DBsatisfiesLaw(database_ptr,t_int,degree))
	{
        // go to next triple
		dbht_ptr = DBmakeCondorcetDomains(t_int + 1,t_0,database_ptr,degree,dbht_ptr,k,phase,file_no,legal);
	}
	else
	{
	    // Iterate over all laws

		for (lex = 0; lex < N_LAWS; lex++)
		{
			copyDataBase(database_ptr,&database_copy,degree); // Initialises db_copy.
            
            firstTriple(t);
            firstLegalPair(law);
            
            for (i = 0; i < t_int; i++)
                nextTriple(t, degree);
                
            for (i = 0; i < lex; i++)
                nextLegalPair(law);

            if (mCD_trace)
            {
                printf("t[0] = %d / t[1] = %d / t[2] = %d\nlaw[0] = %d / law[1] = %d\n", t[0],t[1],t[2],law[0],law[1]);
                printf("insertCondorcet being called with database as follows.\n");
                // writeDataBase(db_copy, degree);
            }
            
            if (!insertCondorcet(t[0],t[1],t[2],law[0],law[1],&database_copy,degree))
            {
                printf("Error in insertCondorcet with triple %d-%d-%d and laws %d/%d.\n",t[0],t[1],t[2],law[0],law[1]);
                exit(0);
            }
            
            if (mCD_trace) printf("Call to insertCondorcet complete.\n");

            if ((k == -1) || (DBsize(database_ptr, degree) > k))
            {
                if (oneLaw(&database_copy, degree))
                {
                    max_count++;
                    dbht_ptr = DBmakeCondorcetDomains(t_int + 1,t_0,&database_copy,degree,dbht_ptr,k,phase,file_no,legal);
                    
                }
                else if (mCD_trace) printf("oneLaw returned False\n");
            }
           if (mCD_trace) printf("Freeing database with address: %p\n", &database_copy);
           freeDataBase(&database_copy,degree);
		}
	}

    if (mCD_trace)
    {
        printf("Leaving DBmCD3 with Database address: %p\n",database_ptr);
        if (mCD_trace > 1) writeDataBase(database_ptr, degree);
    }
    
	return dbht_ptr;
}


/*
* Name:
*   DBmakeCondorcet(database_ptr,dbht_ptr,legal,degree,t_0,k,phase,file_no)
*
* Parameters:
*   database_ptr - Pointer to a DataBase that is initialised in main
*   dbht_ptr - Poiunt to a DBHashTable that is initialised in main
*   legal - Encodes the set of laws in use
*   t_0 - in Phase 1 stop at triple t_0
*   k cut off - only construct MUCD's of size at least k; k = -1 if no bound is imposed
*   phase 1, 2 or 3.  
*   Phase 1 searches the tree to depth t_0, and places the results in the in_ files.
*   Phase 2 takes input from an in_ file, and places the resulting MUCD's in an ou_ file.
*   Phase 3 combines the results of the ou_ files, and places one representative of each
*   equivalence class in a FINAL file, the MUCD's of a given size going into the corresponding
*   FINAL file.  The code for Phase 3 is in file Phase3.c.
* Description:
*   Called by CDmain.
*   The procedure is recursive, and places intermediate results in the Hash table.
*   The address of the hash table may be changed if it has to be re-sized.
*
*/
DBHashTable* DBmakeCondorcet(DataBase* database_ptr, DBHashTable *dbht_ptr, unsigned int legal,
                   int degree, int t_0, int k, int phase, int file_no)
{
	int t_int = 0; /* Integer mapping of first triple */
	file_count = 0;  /* Number of in_ files created.  Note that file_count is a global variable, defined in this file */
	DBInputData db_id; /* Variable to which file contents are written */
	FILE *f_ptr; /* Pointer to a file */
    
    if (mCD_trace)
    {
        printf("DBmakeCondorcet called with phase = %d, file_no = %d, and legal = %d\n", phase, file_no, legal);
    }
    
    if (phase == 1) {

        /* Perform makeCondorcetDomains algorithm; begin with first triple and
         full PermSet */

        dbht_ptr = DBmakeCondorcetDomains(t_int,t_0,database_ptr,degree,dbht_ptr,k,phase,file_no,legal);
        printf("\n The number of files created is %d\n", file_count);
    }
    
    else if (phase == 2)  {
	    f_ptr = openFile(file_no, 1, 0);  // Read from file in_file_no
	    initialiseDataBase(&(db_id.db),degree);

        fscanf(f_ptr,"%d \n",&(db_id.triple));

        freadDataBase(&(db_id.db),degree,f_ptr);
        
        fclose(f_ptr);
        
        // printf("Phase 2 called with the following database:\n");
        // writeDataBase(db_id.db,degree);

	    dbht_ptr = DBmakeCondorcetDomains(db_id.triple, getNoTrips(degree), &(db_id.db),
                                   degree, dbht_ptr, k, phase, file_no, legal);
        
        f_ptr = openFile(file_no, 0, 1);  //Write to file ou_file_no
        fprintDBHashTable(dbht_ptr, degree, f_ptr);
        fprintf(f_ptr, "\n Normal Exit");
        fclose(f_ptr);
    }

    else {
        printf("makeCondorcetDomains called with phase = %d", phase);
        exit(0);
    }
    return dbht_ptr;
}
