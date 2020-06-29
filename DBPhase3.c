/***************************************************************************
 * FILENAME:                                                               *
 *   DBPhase3.c                                                            *
 *                                                                         *
 * PURPOSE:                                                                *
 *   Exports DBPhase3.c to DBMain.c                                        *
 *                                                                         *
 * GLOBALS USED:                                                           *
 *   SafeToPrint                                                           *
 *   NO_LAWS                                                               *
 *                                                                         *
 **************************************************************************/

int phase3flag = 1;

void phase3(int degree, int hashSize, int no_files, int n, int CD_size, unsigned int a)
{
    int i;
    char ch;
    int count = 0;
    DBHashTable *db_ht_ptr;
    FILE *f_ptr;
    FILE *fout_ptr;
    FILE *fs_ptr;
    int core_size;
    DataBase db;

    if (phase3flag) printf("Called phase 3, degree is %d, hashSize is %d, no_files is %d, name is %d, CD_size is %d\n", degree, hashSize, no_files, n, CD_size);

    db_ht_ptr = initialiseDBHashTable(hashSize);
    initialiseDataBase(&db,degree);

    if (phase3flag) printf("Initialisations complete\n");

    fout_ptr = P3openFile(n, CD_size, 1); /* Open a file FINALn_size for writing.  */

    for(i=0; i < no_files; i++)
    {
        if (!(f_ptr = openFile(i, 0, 0))) continue;  //  Open ou_i for reading.  Skip if file does not exist.
        if (phase3flag) printf("Opened file number %d\n", i);
        if (isEmpty(f_ptr))
        {
            fclose(f_ptr);
            continue;
        }
        if (phase3flag) printf("Not empty\n");
        while((!feof(f_ptr)) && (ch = fgetc(f_ptr)) != 'N')
        {
            freadDataBase(&db,degree,f_ptr);
            if (phase3flag)
            {
                // printf("DataBase is:\n");
                // printPermSet(ps, degree);
            }
            if (size(&db,degree) == CD_size)
            {
                if (phase3flag) printf("Calling isFirst\n");
                if (findFirst(&db, degree))
                {
                    db_ht_ptr = insertDBHashTable(db_ht_ptr, &db, degree);
                    core_size = coreSize(ps, degree);  // coreSize checks that a sensible value is returned.
                    count += CD_size/core_size;
                    fprintPermSet(ps, degree, fout_ptr);
                }
            }
        }

        fclose(f_ptr);
    }

    fclose(fout_ptr);
    fs_ptr = openStats(n);
    fprintf(fs_ptr, "The number of MUCD's of size %d and degree %d is %d\n", CD_size, degree, count);
    printf("The number of MUCD's of size %d and degree %d is %d\n", CD_size, degree, count);
    fclose(fs_ptr);
}

