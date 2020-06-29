//
//  Expt3tests.c
//  
//
//  Created by C.R.Leedham-Green on 26/02/2018.
//
//

#include "DBInputs.h"

extern int BugCount;

void testIsIn(void);
void testSizeDB(void);

void testIsIn(void)
{
    int i, j;
    for (i=0; i<64; i++)
    {
        printf("\n %d \n", i);
        for (j=0; j< 6; j++)
            if (isIn(j,i)) printf("%d,",j);
    }
}

void testSizeDB(void)
{
    int *Laws;
    int degree = 4;
    int NoTrips = getNoTrips(degree);
    int i;
    
    initialisePermutation(&Laws, NoTrips);
    for(i = 0; i < NoTrips; i++) Laws[i] = 3;
    
    printf("Size is %d\n", sizeCD(Laws, degree));
    printf("Degree is %d\n", degree);
    printf("Laws[i] set to %d for all i\n", Laws[0]);// May be wrong, Laws is updated.
}

void cardTest(void)
{
    int i;
    for (i=0; i<64; i++)
    {
        printf("%d  %d\n", i, card(i));
    }
}

void randomPermTest(void)
{
    int i;
    int size;
    int *T;
    
    for (size = 2; size < 7; size++)
    {
        initialisePermutation(&T, size);
        randomPerm(T, size);
        printf("(");
        for (i = 0; i < size; i++) printf(" %d ", (T[i]));
        printf(")\n");
    }
        
}

void randomSpecialTest(void)
{
    int degree = 8;
    Permutation Laws;
    DataBase DB;
    Permutation T;
    int NTrips = getNoTrips(degree);
    int i;
    int found;
    
    initialisePermutation(&T, NTrips);
    randomPermutation(T, NTrips);
    
    initialisePermutation(&Laws, NTrips);
    for (i=0; i < NTrips; i++) Laws[i] = 0;
    
    initialiseDataBase(&DB, degree);
    
    found = randomSpecial(0, &DB, degree, Laws, T);
    
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n");
}

void purify1Test(void)
{
    int degree = 6;
    Permutation Laws;
    DataBase DB;
    Permutation T;
    int NTrips = getNoTrips(degree);
    int i;
    int found;
    int Size;
    
    initialisePermutation(&T, NTrips);
    randomPermutation(T, NTrips);
    
    initialisePermutation(&Laws, NTrips);
    for (i=0; i < NTrips; i++) Laws[i] = 0;
    
    initialiseDataBase(&DB, degree);
    
    found = randomSpecial(0, &DB, degree, Laws, T);
    //Size = sizeCD(Laws, degree);
    
    printf("************\n");
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n");
    
    printf("Size is %d\n\n", E3Size(Laws, degree));
    printf("Now set Laws[0] = 2\n");
    
    Laws[0] = 2;
    printf("Size is %d\n", E3Size(Laws, degree));
    
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n\n");
   
    printf("Now purify.\n");
    
    purify1(Laws, degree);
    
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n");
    
     printf("Size is %d\n", E3Size(Laws, degree));
}

void ranSpec(Permutation* Laws_ptr, int degree)
{
    DataBase DB;
    Permutation T;
    int NTrips = getNoTrips(degree);
    int i;
    
    initialisePermutation(&T, NTrips);
    randomPermutation(T, NTrips);
    
    initialisePermutation(Laws_ptr, NTrips);
    
    for (i=0; i < NTrips; i++) (*Laws_ptr)[i] = 0;
    
    initialiseDataBase(&DB, degree);
    
    randomSpecial(0, &DB, degree, *Laws_ptr, T);
    
    E3Size(*Laws_ptr, degree);
    
    freeDataBase(&DB, degree);
    free(T);
}

void purifyTest(void)
{
    int degree = 7;
    int bound = 40;
    Permutation Laws;
    int NTrips = getNoTrips(degree);
    int i;
    int Size;
    
    ranSpec(&Laws, degree);
    
    printf("************\n");
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n");
    
    printf("Size is %d\n\n", E3Size(Laws, degree));
    printf("Now set Laws[0] = 2\n");
    
    Laws[0] = 2;
    printf("Size is %d\n", E3Size(Laws, degree));
    
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n\n");
    
    printf("Now purify.\n");
    
    Size = purify(Laws, bound, degree);
    
    printf("(");
    for (i = 0; i < NTrips; i++) printf("%d ", Laws[i]);
    printf(")\n");
    
    printf("Size is %d\n", Size);
}

void AlternatingTest(void)
{
    int degree;
    Permutation Laws;
    int size;
    int NoTrips;
    
    for (degree = 4; degree < 11; degree ++)
    {
        NoTrips = getNoTrips(degree);
        initialisePermutation(&Laws, NoTrips);
        Alternating(Laws, degree);
        size = E3Size(Laws, degree);
        printf("degree = %d, Size is %d\n", degree, size);
        Laws[0] = 2;
        size = purify(Laws, 1, degree);
        printf("Now size is %d\n", size);
    }
}

void JumpTest(void)
{
    int degree = 6;
    Permutation Laws;
    int size;
    int NoTrips;
    int bound = 40;
    int i;
    
    NoTrips = getNoTrips(degree);
    initialisePermutation(&Laws, NoTrips);
    Alternating(Laws, degree);
    
    size = E3Size(Laws, degree);
    printf("degree = %d, Size is %d\n", degree, size);

    size = jump(Laws, bound, degree);
    printf("size is now %d\n", size);
    printPermutation(Laws, NoTrips);
    
    for(i = 0; i < 10; i++)
    {
        bound = size;
        size = jump(Laws, size, degree);
        printf("size is now %d\n", size);
        printPermutation(Laws, NoTrips);
    }
}

void JumpTest2(void)
{
    int degree = 6;
    int bound = 1;
    Permutation Laws;
    int NoTrips = getNoTrips(degree);
    int i;
    int size;
    
    printf("****************\n");
    
    ranSpec(&Laws, degree);
    
    size = purify(Laws, bound, degree);
    
    printPermutation(Laws, NoTrips);
    printf("Size is %d\n", size);
    
    /* All this is to get a sensible starting point.  */

    
    bound = size - 1;
    Laws[0] = 1;
    Laws[5] = 32;
    Laws[1] = 16;
    Laws[2] = 1;

    if (Jump2(Laws, bound, degree))
    {
        size = purify(Laws, 1, degree);
        printf("size is now %d\n", size);
        printPermutation(Laws, NoTrips);
    }
    else printf("jump failed\n ");
        
    if (size == MaxSize(degree)) return;


}

void Dtest(void)
{
    int i;
    
    for (i = 0; i < 63; i++)
        printf("%d: %d\n", i, (~i) & 63); // returns 63 - i
}

void chooseTest(void)
{
    int n,i;
    
    for (n = 0; n < 7; n++)
        for (i = 0; i <=n; i++)
            printf("%d, %d, %d\n", n, i, Choose(n,i));
}

void intToTripsTest(void)
{
    int n;
    Permutation P;
    int length = 7;
    int i;
    
    initialisePermutation(&P, length);
    
    for (i = 1; i < length; i++)
    {
        printf("i is %d\n", i);
        for (n = 0; n < Choose(length,i); n++)
        {
            intToSetOfTrips(P, n, 0, i, length);
            printf("%d ", setTripsToInt(P, i, length));
        }
        printf("\n");
    }
}

int main()
{
    //intToTripsTest();
    //chooseTest();
    // Dtest();
    int i;
    for(i = 0; i < 10; i++)
    JumpTest2();
    //AlternatingTest();
  /*  int i;
    for(i=0; i<10; i++)
    purifyTest();*/
    //randomSpecialTest();
    //randomPermTest();
    //cardTest();
    //testSizeDB();
    return 1;
}



