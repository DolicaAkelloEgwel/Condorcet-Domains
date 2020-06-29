void addEntry(Permutation *new_perm_ptr, Permutation old_perm, int i, int d)
/* old_perm is an array of length d - 1 whose entries give a permutation
 of {0,1, ... ,d-2}.  new_perm is malloced, and set to an array of length d, obtained from
 old_perm by inserting d-1 between entries i-1 and i, or first if i = 0, or last if i = d-1. */

{
    int j;

    if (( i < 0) || (i > d-1))
    {
        printf ("Bad call to addEntry, d = %d, i = %d\n", d, i);
        exit(0);
    }

    initialisePermutation(new_perm_ptr, d);

    for (j=0; j<i; j++)
        (*new_perm_ptr)[j] = old_perm[j];

    (*new_perm_ptr)[i] = d - 1;

    for (j = i; j < d - 1; j++)
        (*new_perm_ptr)[j + 1] = old_perm[j];
}

int isAllowed(DataBase* db_ptr, Permutation perm, int i, int d, int degree)
/* The permutation perm of degree d < degree obeys the conditions stored in *db_ptr.
 Will it continue to do so if we insert d between entries i-1 and i? (0 <= i <= d) */
{
    int a;
    int b;
    int j;

    if (( i < 0) || (i > d))
    {
        printf ("Bad call to isAllowed, d = %d, i = %d\n", d, i);
        exit(0);
    }

    for (j = i; j < d; j++)
    {
        if (greater(d, perm[j], db_ptr, degree))
        {
            //printf("First return: j = %d\n",j);
            return 0;
        }
    }

    for (j = 0; j < i; j++)
    {
        if (greater(perm[j], d, db_ptr, degree))
        {
            //printf("Second return: j = %d\n",j);
            return 0;
        }
    }

    for (j = 0; j < i; j++)
        for (a = 1; a < d; a++)
            for (b = 0; b < a; b++)
            {
                if (greaterImplies(d, perm[j], perm[b], perm[a], db_ptr, degree))
                {
                    //printf("Third return: j = %d / a = %d / b = %d\n",j,a,b);
                    return 0;
                }
            }

    for (j = i; j < d; j++)
        for (a = 1; a < d; a++)
            for (b = 0; b < a; b++)
            {
                if (greaterImplies(perm[j], d, perm[b], perm[a], db_ptr, degree))
                {
                    //printf("Fourth return: j = %d / a = %d / b = %d\n",j,a,b);
                    return 0;
                }
            }
    //printf("\n Is allowed: i is %d, d is %d\n", i, d);
    //printPermutation(perm, d);
    return 1;
}


void initialisePermSet(SmallPermset** sps_ptr_ptr, DataBase *db_ptr, int degree)
/* Create a small permutation set consisting of the permutations (0,1),
 and, if the database does not require 1 > 0, the permutation (1,0).
 It must not require 0 > 1.  See preamble to DBToPerm.h.
 */
{
    Permutation perm1;
    Permutation perm2;

    if (greater(0, 1, db_ptr, degree))
    {
        printf("Bad call to initialisePermSet.  *db_ptr requires 0 > 1.\n");
        printf("See preamble to DBToPerm.h\n");
        exit(0);
    }

    *sps_ptr_ptr = NULL;

    initialisePermutation(&perm1, 2);

    perm1[0] = 0;
    perm1[1] = 1;
    permInSmallPermset(sps_ptr_ptr, perm1, 2);

    if(!greater(1, 0, db_ptr, degree))
    {
        initialisePermutation(&perm2, 2);
        perm2[0] = 1;
        perm2[1] = 0;
        permInSmallPermset(sps_ptr_ptr, perm2, 2);
    }
}

void printPermutation(Permutation perm, int degree)
{
    int i;

    for (i = 0; i < degree; i++)
    {
        printf("%d ", perm[i]);
    }

    printf("\n");
}

void printSmallPermset(SmallPermset* sps_ptr, int degree)
{
    SmallPermset* next_ptr;
    next_ptr = sps_ptr;

    //printf("Entering printSmallPermSet.\n");

    while (next_ptr != NULL)
    {
        // printf("%p\n",next_ptr->entry);
        printPermutation(next_ptr->entry,degree);
        next_ptr = next_ptr->next;
    }
}

void permInSmallPermset(SmallPermset** sps_ptr_ptr, Permutation perm, int degree)
/* Inserts perm at the end of ** sps_ptr_ptr. */
{
    SmallPermset *pointer;
    SmallPermset *sps_p;

    //printf("Entering permInSmallPermset.\n");

    if (! (sps_p = malloc(sizeof(SmallPermset))))
    {
        printf("Malloc failed in permInSmallPermset\n");
        exit(0);
    }

    *sps_p = smallPermsetEntry(perm, degree);

    if (*sps_ptr_ptr == NULL)
    {
        *sps_ptr_ptr = sps_p;
        return;
    }

    pointer = *sps_ptr_ptr;

    while (pointer -> next != NULL) pointer = pointer -> next;

    pointer -> next = sps_p;
}

void expand(SmallPermset** new_sps_ptr_ptr, Permutation perm, int d, DataBase* db_ptr, int degree)
/* perm is a permutation of degree d-1, 3 <= d <= degree.
 Insert d-1 in all slots allowed by the data base *db_ptr, and add the resulting permutations of degree d
 to the small permutation set **new_sps_ptr_ptr.  */
{
    int i;
    Permutation new_perm;

    if ((d < 3) || (d > degree))
    {
        printf("Bad call to extend.  d = %d, degree = %d.\n", d, degree);
        exit(0);
    }
    for (i = 0; i < d; i++)
    {
        if (isAllowed(db_ptr, perm, i, d-1, degree))
        {
            addEntry(&new_perm,perm,i,d);  // Adds d-1 between entries i-1 and i.
            permInSmallPermset(new_sps_ptr_ptr,new_perm,d);
        }
    }
}

void initialisePermutation(Permutation *perm_ptr, int degree)
{
    int i;

    if (! (*perm_ptr = malloc(degree * sizeof(int))))
    {
        printf("Error in initialisePermutation: malloc failed.\n");
        exit(0);
    }

    for (i = 0; i < degree; i++)
    {
        (*perm_ptr)[i] = 0;
    }

}

void copyPermutation(Permutation orig_perm, Permutation copy_perm, int degree)
{
    int i;

    for (i = 0; i < degree; i++)
    {
        copy_perm[i] = orig_perm[i];
    }
}

SmallPermset smallPermsetEntry(Permutation perm, int degree)
/* Essentially the same as setHashEntry in HasTable.c in Code in con_parallel. */

{
    SmallPermset sps;

    initialisePermutation(&(sps.entry), degree);
    copyPermutation(perm, sps.entry, degree);

    sps.next = NULL;

    return sps;
}

void freeSPS(SmallPermset *sps_ptr)
{
    SmallPermset *sps_p;
    while (sps_ptr)
    {
        sps_p = sps_ptr;
        sps_ptr = sps_ptr -> next;
        free(sps_p -> entry);
        free(sps_p);
    }
}

SmallPermset* dbToPerm(DataBase *db_ptr, int degree)
/* The principal procedure for this file.  See DBToPerm.c.  */
{
    SmallPermset *sps1_ptr, *sps2_ptr;
    int d;

    initialisePermSet(&sps1_ptr, db_ptr, degree);

    sps2_ptr = NULL;

    for(d=3; d<=degree; d++)
    {
        while(sps1_ptr != NULL)
        {
            expand(&sps2_ptr, sps1_ptr -> entry, d, db_ptr, degree);
            sps1_ptr = sps1_ptr -> next;
        }
        free (sps1_ptr);
        sps1_ptr = sps2_ptr;
        sps2_ptr = NULL;
    }
    return sps1_ptr;
}
