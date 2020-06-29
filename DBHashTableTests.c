//
//  DBHashTableTests.c
//  
//
//  Created by C.R.Leedham-Green on 25/05/2017.
//
//

#include "DBInputs.h"

void DBHashIOTest(int degree);
void randomDB(DataBase* db_ptr, int degree);

void randomDB(DataBase* db_ptr, int degree)
{
    initialiseDataBase(db_ptr, degree);

    int a;
    int b;
    int c;
    int d;

    int i;

    srand(7);

    for (i = 0; i < 6; i++)
    {
        a = rand() % degree;
        
        do
        {
            b = rand() % degree;
        }
        while (a == b);

        c = rand() % degree;
        
        do
        {
            d = rand() % degree;
        }
        while (c == d);

        printf("a = %d, b = %d, c = %d, d = %d\n",a,b,c,d);
        writeInLocal(a,b,c,d,*db_ptr,degree);
    }

    printf("a = %d / b = %d / c = %d / d = %d\n"a,b,c,d);

    if (! greaterImplies(a,b,c,d,db_ptr,degree))
        writeInGlobal(a,b,*db_ptr,degree);
        
    
}

void DBHashIOTest(int degree)
{
    FILE *fp;
    int i;
    //PermSet ps;
    DataBase db;
    DBHashTable* ht_ptr;
    DBHashTable* kt_ptr;

    initialiseDataBase(&db,degree);
    ht_ptr = initialiseDBHashTable(5);
    kt_ptr = initialiseDBHashTable(5);

    for (i = 0; i < 15; i++)
    {
        randomDB(&db, degree);
        // printf("\nINSERT\n");
        ht_ptr = insertDBHashTable(ht_ptr, db, degree);
    }
    
    fp = fopen("TestIO", "w");
    fprintDBHashTable(ht_ptr,degree,fp);
    fclose(fp);
    
    printf("\n Wrote Hash table");
    
    fp = fopen("TestIO", "r");
    kt_ptr = freadDBHashTable(kt_ptr, degree, fp);
    fclose(fp);
    
    printf("\n Read Hash table");
    
    printDBHashTable(*kt_ptr, degree);

    printf("Test complete.\n");
}

void randomTest()
{
    int degree = 6;
    DataBase db;

    randomDB(&db, degree);

    writeDataBase(db,degree);
}

int main()
{
    DataBase db;
    int degree = 6;

    // randomDB(&db,degree);
    // DBHashIOTest(degree);
     randomTest();
}

