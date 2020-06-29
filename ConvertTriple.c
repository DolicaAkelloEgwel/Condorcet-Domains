int tripleToInt(triple t)
{
    int a = t[0];
    int b = t[1];
    int c = t[2];

    return a + (b*(b-1))/2 + (c*(c-1)*(c-2))/6;
}

void intToTriple(int n, triple t)
{
    int a,b,c,sum;

    for (c = 2, sum = 0; sum < n; c++, sum+=((c*(c-1))/2));
    n -= (c*(c-1)*(c-2))/6;
    for (b = 1, sum = 0; sum < n; b++, sum += b);
    n -= (b*(b-1))/2;
    a = n ;
    
    t[0] = a;
    t[1] = b;
    t[2] = c;
}

void intToLaw(int i, pair law)
{
    if (i == 0)
    {
        law[0] = 1;
        law[1] = 2;
        return;
    }
    if (i == 1)
    {
        law[0] = 1;
        law[1] = 3;
        return;
    }
    if (i == 2)
    {
        law[0] = 2;
        law[1] = 1;
        return;
    }
    if (i == 3)
    {
        law[0] = 2;
        law[1] = 3;
        return;
    }
    if (i == 4)
    {
        law[0] = 3;
        law[1] = 1;
        return;
    }
    if (i == 5)
    {
        law[0] = 3;
        law[1] = 2;
        return;
    }
}

void randomPermutation(Permutation A, int d)
// Enter the integers 0 to d-1 in a random order in the initialised
// array A of length d. Used in randomDBmCD3.
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
        for(k = place = 0; k <= j; place++)
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

/*  randomDBmCD3 is called when we need to advance to the next triple t_int.
 If a law is already satisfied for t_int then call DBmakeCondorcetDomains with t_int incremented by 1.
 Otherwise, for each law, impose that law on the triple t_int, and call randomDBmCD#
 with t_int incremented by 1 if the size of the new perm set is at least k, and no triple satisfies 2 laws.  The point here is that the laws are chosen in a random order, independently,
 for each node in the tree. The value `work' that is returned measures the total number
 of nodes constructed.  */
int randomDBmCD3(int t_int, DataBase* database_ptr, int degree, int work, Permutation T)
{
    int lex; /* Loop counter: integer mapping of a law in range 0 to N_LAWS */
    int i;
    triple t;
    DataBase database_copy; /* Copy of DataBase db */
    pair law;
    Permutation L;
    
    if (t_int == getNoTrips(degree)) return work;
    
    // Check if the database already satisfies a law on this triple
    if (DBsatisfiesLaw(database_ptr,T[t_int],degree))
    {
        // go to next triple
        work = randomDBmCD3(t_int + 1,database_ptr,degree,work, T);
    }
    else
    {
        // Iterate over all laws
        initialisePermutation(&L,6);
        randomPermutation(L,6);
        
        for (lex = 0; lex < 6; lex++)
        {
            copyDataBase(database_ptr,&database_copy,degree); // Initialises db_copy.
            
            intToTriple(T[t_int], t);
            
            intToLaw(L[lex], law);
            
            if (!insertCondorcet(t[0],t[1],t[2],law[0],law[1],&database_copy,degree))
            {
                printf("Error in insertCondorcet with triple %d-%d-%d and laws %d/%d.\n",t[0],t[1],t[2],law[0],law[1]);
                return 0;
            }
            
            if (oneLaw(&database_copy, degree)) // No triple satisfies more than one law.
            {
                work = randomDBmCD3(t_int + 1,&database_copy,degree, work+1, T);
            }
                else if (mCD_trace) printf("oneLaw returned False\n");
            freeDataBase(&database_copy,degree);
        }
    }
    
    return work;
}
