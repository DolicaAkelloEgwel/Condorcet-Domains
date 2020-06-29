#include "DBInputs.h"

void nreadLine(FILE* file_ptr);

void nreadLine(FILE* file_ptr)
{
    int a, b, c, i, j;

    fscanf(file_ptr," {%d,%d,%d} ",&a,&b,&c);
    printf("Reading {%d,%d,%d}\n",a,b,c);
}


void initialiseDatabaseTest()
{
    DataBase first_database;
    int degree = 5;
    int a;

    initialiseDataBase(&first_database,degree);
    a = insertInLocal(0,1,2,3,&first_database,degree);
    a = insertInGlobal(2,4,&first_database,degree);
    a = insertInGlobal(1,2,&first_database,degree);
    a = insertInLocal(2,3,0,1,&first_database,degree);
    writeGreater(&first_database,degree);
    writeImplies(&first_database,degree);

    DataBase second_database;
    copyDataBase(&first_database,&second_database,degree);
    writeDataBase(&second_database,degree);
    printf("Done.\n");
}

void iotest()
{
    DataBase first_database;
    DataBase second_database;
    int degree = 5;
    int a;
    int t;
    FILE *f_ptr;

    initialiseDataBase(&first_database,degree);
    initialiseDataBase(&second_database, degree);
    a = insertInLocal(0,1,2,3,&first_database,degree);
    a = insertInGlobal(4,2,&first_database,degree);
    a = insertInGlobal(2,1,&first_database,degree);
    a = insertInLocal(2,3,0,1,&first_database,degree);

    printf("Database constructed. \n");

    if (!(f_ptr = fopen("test", "w+")))
    {
        printf("Could not open file.\n");
        exit(0);
    }

    printf("File opened\n");

    fprintDataBase(&first_database, degree, f_ptr);

    printf("database written to file.\n");

    rewind(f_ptr);

    fscanf(f_ptr, " %d ", &t);

    printf("triple read.\n");

    freadDataBase(&second_database, degree, f_ptr);

    printf("database read from file into db.\n");

    printf("database:\n");

    writeDataBase(&first_database, degree);

    printf("db:\n");

    writeDataBase(&second_database, degree);

    if (equalDataBases(&first_database, &second_database, degree))
        printf("Test successful.\n");
    else printf("Test fails; databases not equal.\n");

    fclose(f_ptr);
}

void copytest(void)
{
    DataBase first_database;
    DataBase second_database;
    int degree = 5;
    int a;
    triple t;
    int i;
    FILE *f_ptr;
    int lex;
    pair law;
    DBHashTable *dbht_ptr;


    initialiseDataBase(&first_database,degree);
    initialiseDataBase(&second_database, degree);
    a = insertInLocal(0,1,2,3,&first_database,degree);
    a = insertInGlobal(4,2,&first_database,degree);
    a = insertInGlobal(2,1,&first_database,degree);
    a = insertInLocal(2,3,0,1,&first_database,degree);

    printf("Database constructed. \n");

    if (!(f_ptr = fopen("test", "w+")))
    {
        printf("Could not open file.\n");
        exit(0);
    }

    printf("File opened\n");

    dbht_ptr = initialiseDBHashTable(173);

    for (lex = 0; lex < 6; lex++)
        {
            copyDataBase(&first_database,&second_database,degree); // Initialises db.

            firstTriple(t);
            firstLegalPair(law);

            for (i = 0; i < 3; i++)
                nextTriple(t, degree);

            for (i = 0; i < lex; i++)
                nextLegalPair(law);

            if (!insertCondorcet(t[0],t[1],t[2],law[0],law[1],&second_database,degree))
            {
                printf("Error in insertCondorcet with triple %d-%d-%d and laws %d/%d.\n",t[0],t[1],t[2],law[0],law[1]);
                exit(0);
            }

            dbht_ptr = insertDBHashTable(dbht_ptr, &second_database, degree);
        }
    freeDataBase(&second_database,degree);
    printDBHashTable(*dbht_ptr, degree);
}

void randFullDB(DataBase* db_ptr, int degree)
{
    int a, b;

    initialiseDataBase(db_ptr, degree);

    for (a = 0; a < degree - 1; a++)
    {
        for (b = a + 1; b < degree; b++)
        {
            if (!greater(a, b, db_ptr, degree) && !greater(b, a, db_ptr, degree))
            {
                if (rand() % 2)
                    insertInGlobal(a, b, db_ptr, degree);
                else
                    insertInGlobal(b, a, db_ptr, degree);
            }
        }
    }
}

void testFirst(void)
{
    DataBase database;
    int degree = 5;
    int i;

    randFullDB(&database, degree);
    i = findFirst(&database, degree);
    printf("Find first was %d\n", i);

}

void testNext(void)
{
    int i, j;
    int degree = 5;
    DataBase database;

    randFullDB(&database, degree);

    i = findFirst(&database, degree);
    printf("%d ", i);

    for (j = 0; j < degree; j++)
    {
        // j = findNext(database_ptr, degree);
        printf("%d ", j);
        i = j;
    }
}

void testRandomDataBase(void)
{
    DataBase db;
    int degree = 5;
    int local = 7;
    int global = 3;
    int seed = 1756;

    randomDB(&db, local, global, seed, degree);

    fprintDataBase(&db,degree,stdout);

    writeDataBase(&db,degree);
}

/*void testCompletePerm(void)
{
    int degree = 5;
    Permutation p;
    Permutation used;
    Permutation rp;

    initialisePermutation(&p,degree);
    initialisePermutation(&used,degree);
    initialisePermutation(&rp,degree);

    for (int i = 0; i < degree; i++)
    {
        used[i] = 0;
    }

    rp = completePerm(p,used,0,degree);
    printPermutation(rp,degree);
}
 */

int main(int argc, char *argv[])
{
    /*

    FILE *f_ptr;
    int a,b,c;

    // int degree = 40;

    DataBase database;
    // initialiseDataBase(&database,degree);

    if (argc != 2)
    {
        printf("Input name of file.\n");
        exit(0);
    }

    if ( !(f_ptr = fopen(argv[1], "r")))
    {
        printf("Could not open file\n");
        exit(0);
    }
    nreadLine(f_ptr);
    // fscanf(f_ptr, " {%d,%d,%d} ", &a, &b, &c);
    // printf(" {a,b,c} is {%d, %d, %d}\n", a,b,c);
    fclose(f_ptr);
    */

    // initialiseDatabaseTest();
    //iotest();
    // copytest();

    // testNext();

    // testCompletePerm();
    
    testRandomDataBase();
}
