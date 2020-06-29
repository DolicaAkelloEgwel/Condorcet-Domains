#include "DBInputs.h"

void initialisePermutation(Permutation *perm, int degree);

void isAllowedTest()
{
    DataBase db;
    int n;
    int i;
    int degree = 6;

    Permutation perm;

    int local = 10;
    int global = 0;
    int seed = 56;

    initialiseDataBase(&db,degree);
    initialisePermutation(&perm, degree);
    randomDB(&db,local,global,seed,degree);

    for (i = 0; i < 4; i++)
        perm[i] = i;

    for (i = 0; i < 5; i++)
    {
        n = isAllowed(&db, perm, i, 4, degree);
        printf("%d %d\n", i, n);
    }

    writeDataBase(&db,degree);
}

void testExpand()
{
    SmallPermset *sps_ptr = NULL;
    Permutation perm;
    DataBase db;

    int d = 4;
    int degree = 6;
    int i;

    int local = 15;
    int global = 1;
    int seed = 32;

    initialisePermutation(&perm, d - 1);
    // initialisePermSet(&sps);

    for (i = 0; i < d - 1; i++)
    {
        perm[i] = i;
    }

    randomDB(&db, local, global, seed, degree);

    expand(&sps_ptr,perm,d,&db,degree);
    printf("Pointer: %p\n",sps_ptr);
    printSmallPermset(sps_ptr,d);
}

void testInitialisePermutation()
{
    Permutation perm;
    int degree = 6;
    int i;

    initialisePermutation(&perm, degree);

    printf("InitialisePermutation has run.\n");

    for (i = 0; i < degree; i++)
    {
        printf("%d\n",i);
        perm[i] = i;
    }

    printPermutation(perm, degree);
}

void altInitTest()
{
    Permutation perm;
    int degree = 6;
    int i;

    if (!(perm = malloc(degree * sizeof(int))))
    {
        printf("Error in intialisePerm: malloc failed.\n");
    }

    for (i = 0; i < degree; i++)
    {
        printf("%d\n",i);
        perm[i] = i;
    }

    printPermutation(perm, degree);
}

void initPerm(Permutation *perm_ptr, int degree)
{
    if (! (*perm_ptr = malloc(degree * sizeof(int))))
    {
        printf("Error in initialisePermutation: malloc failed.\n");
        exit(0);
    }
}

void newInitTest()
{
    Permutation perm;
    int degree = 6;
    int i;

    initPerm(&perm, degree);

    for (i = 0; i < degree; i++)
    {
        printf("%d\n",i);
        perm[i] = i;
    }

    printPermutation(perm, degree);
}

/*
void initPermSetTest(void)
{
    SmallPermset *sps_ptr;
    initialisePermSet(&sps_ptr);
    printSmallPermset(sps_ptr, 2);
}
 */

void dbToPermTest(void)

{
    DataBase db;
    int local = 115;
    int global = 7;
    int seed = 179;
    int degree = 12;
    SmallPermset *sps_ptr;

    randomDB(&db, local, global, seed, degree);
    // writeDataBase(&db, degree);
    sps_ptr = dbToPerm(&db, degree);
    printSmallPermset(sps_ptr, degree);
}

void shiftTest(void)
{
    uint64_t val;
    uint64_t sum = 0;
    uint64_t unus = 1;

    for (int i = 0; i < 64; i++)
    {
        val = unus << i;

        if (i % 5 == 0)
            sum += val;

        printf("unus << %d : %llu\n", i, val);
    }

    printf("Sum: %llu\n",sum);
    uint64_t newsum = sum | unus << 29;
    printf("Sum OR: %llu\nDifference: %llu\n", sum, newsum-sum);
}

int main(void)
{
    // isAllowedTest();
    // testExpand();
    // testInitialisePermutation();
    // altInitTest();
    // initPermSetTest();
    // dbToPermTest();
     shiftTest();
}
