//
//  Optimisation.c
//  
//
//  Created by C.R.Leedham-Green on 14/02/2018.
//
//

void randomPermutation(int *A, int d)
// Enter the integers 0 to d-1 in a random order in the initialised
// array A of length d.
{
    int i;
    int j;
    int k;
    int place;
    
    //initialisePermutation(A,d);
    for(i = 0; i < d; i++) A[i] = -1;
    for(i = 0; i < d; i++)
    {
        j = rand()%(d-i); // put i in j-th empty slot, counting from 0;
        for(k = place = 0; k <= j;)
            if (A[place]==-1)
            {
                if (k == j)
                {
                    A[place] = i;
                }
                k++;
            }
    }
}
