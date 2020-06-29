#include "DBInputs.h"

int test_tripleToInt()
{
    int i;
    triple t;

    for (i = 0; i < 20; i++)
    {
        intToTriple(i,t);
        printf("%d\n",tripleToInt(t));
    }
    return 0;
}

void test_randomPerm()
{
    int i;
    Permutation A;
    
    for (i = 6; i < 10; i++)
    {
        initialisePermutation(&A, i);
        randomPermutation(A, i);
        printPermutation(A, i);
        printf("\n");
    }
    for (i = 6; i < 10; i++)
    {
        initialisePermutation(&A, i);
        randomPermutation(A, i);
        printPermutation(A, i);
        printf("\n");
    }
}

int main()
{
    test_randomPerm();
    return 0;
}
