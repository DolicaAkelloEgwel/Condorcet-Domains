#include "DBInputs.h"

int testDeduce(void)
{
    int degree = 6;
    
    int t;
    int law;
    triple trip;
    pair lex;
    int NoTrips = getNoTrips(degree);
    DataBase DB;
    int NoEntries = 1000;
    int i;
    
    initialiseDataBase(&DB, degree);
    
    for(i = 0; i < NoEntries; i++)
    {
        t = rand()%NoTrips;
        law = rand()%6;
        intToTriple(t, trip);
        intToLaw(law, lex);
        insertCondorcet(trip[0], trip[1], trip[2], lex[0], lex[1], &DB, degree);
        if(!deduceCondorcet(trip[0], trip[1], trip[2], lex[0], lex[1], &DB, degree))
        {
            printf ("FAIL\n");
            return 0;
        }
    }
    printf("Success\n");
    return 1;
}

int testDeduce2(void)
{
    int degree = 4;
    DataBase DB;
    int t;
    int law;
    triple trip;
    pair lex;
    int NoTrips = getNoTrips(degree);
    int lt;
    
    initialiseDataBase(&DB, degree);
    insertCondorcet(0,1,2,1,3, &DB, degree);
    insertCondorcet(0,1,3,1,3, &DB, degree);
    insertCondorcet(0,1,3,3,2, &DB, degree);
    insertCondorcet(0,2,3,2,3, &DB, degree);
    insertCondorcet(1,2,3,2,1, &DB, degree);
    
    for(t = 0; t < NoTrips; t++)
    {
        lt = 0;
        intToTriple(t, trip);
        for(law = 0; law < 6; law++)
        {
            intToLaw(law, lex);
            printf("t = %d, law = %d, %d\n", t, law, deduceCondorcet(trip[0], trip[1], trip[2], lex[0], lex[1], &DB, degree));
            if(deduceCondorcet(trip[0], trip[1], trip[2], lex[0], lex[1], &DB, degree)) lt |= 1 <<law;
            printf("lt is %d\n", lt);
        }
    }
    return 1;
}

int main(void)
{
    return testDeduce2();
}
    
    
    
    
    


/*int main(int argc, char *argv[])
{
    FILE* inputfile_ptr;
    FILE* outputfile_ptr;
    int degree;
    DataBase database;
    int n;
    clock_t t1;
    clock_t t2;
    
    if (argc != 4)
    {
        printf("Input three parameters, degree, input file, output file.\n");
        exit(0);
    }
    
    degree = atoi(argv[1]);
    
    writeAlternating(argv[2], degree);
    
    initialiseDataBase(&database,degree);
    
    if (!(inputfile_ptr = fopen(argv[2],"r")))
    {
        printf("Could not open input file.\n");
        exit(0);
    }
    
    if (!(outputfile_ptr = fopen(argv[3],"w")))
    {
        printf("Could not open output file.\n");
        exit(0);
    }
    
    readCondorcet(inputfile_ptr,&database,degree);
    writeCondorcet(outputfile_ptr, &database, degree);
    fwriteGreater(outputfile_ptr, &database, degree);
    fwriteImplies(outputfile_ptr, &database, degree);
    fclose(inputfile_ptr);
    fclose(outputfile_ptr);
    n = noPairs(&database, degree);
    
    t1 = clock();
    printf("Size: %d\n",size(&database,degree));
    t2 = clock();
    
    printf("No. pairs is %d.\n", n);
    printf("Time is %lu seconds.\n", (t2-t1)/CLOCKS_PER_SEC);
    return 1;
}
*/
