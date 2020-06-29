//
//
//  Created by C.R.Leedham-Green on 26/02/2018.
//
//

int isIn(int i, int n) // Is i-th triple (or pair) in the set of triples (or pairs) n?
{
    return (1<<i) & n;
}

int score(Permutation perm, int length) // Returns the total number of bits set.
{
    int i;
    int sum = 0;
    for(i = 0; i < length; i++) sum += card(perm[i]);
    return sum;
}

int sizeCD(int *Laws, int degree)
/*  Laws[t] is the set of laws satisfied by triple t.
 This set is replaced by its closure, and the size of the corresponding data base is returned.  NOT NOW USED.
  */
{
    DataBase DB;
    int NTrips = getNoTrips(degree);
    int NoLaws = 6; // Must never be changed.
    int t;
    int ls;
    int el;
    triple trip;
    pair law;
    int Size;
    
    initialiseDataBase(&DB, degree);
    for (t = 0; t < NTrips; t++)
    {
        ls = Laws[t];
        if (ls) // So some laws are imposed on this triple
        {
            intToTriple(t, trip);  // Turn this triple into three integers
            for (el = 0; el < NoLaws; el++)
            {
                if (isIn(el, ls))  // So law number el is imposed
                {
                    intToLaw(el, law);  // Turn this law into a pair of integers
                    insertCondorcet(trip[0], trip[1], trip[2], law[0], law[1], &DB, degree);
                }
            }
        }
    }
    // So now DB contains the CD defined by the input set of Laws.

    DBtoLaws(&DB, Laws, degree);  // So Laws is now closed.
    Size = DBsize(&DB, degree);
    freeDataBase(&DB, degree);
  
    return Size;
}

int card(int n)
{
    UNINT m;
    m = n;
    return popcount(m);
}

int purify1(Permutation Laws, int degree)
/*  Replaces Laws by its closure, and then, whenever more than one law
 is imposed on any triple, and some but not all of these laws are not implied
 by the laws imposed on other triples, then the laws not implied are
 removed.  If none of the laws imposed on this triple is implied, then an optimal
 choice of one of the imposed laws is chosen, and the rest are discarded, and the set
 of laws is closed.  This may reintroduce discarded laws.  For the definition of
 `optimal' see bestFill.
*/
{
    int flag = 0;
    int t;
    int c;
    int d;
    int NoTrips = getNoTrips(degree);
    int Size;
    Permutation Maws;
    int Score;
    
    if (flag) printf("purify1 called with laws(%d, %d, %d, %d)\n",Laws[0], Laws[1], Laws[2], Laws[3]);
    
    Size = E3Size(Laws, degree);  // Now Laws is closed.
    
    Score = score(Laws, NoTrips); // The total number of laws imposed.
    
    initialisePermutation(&Maws, NoTrips);
    copyPermutation(Laws, Maws, NoTrips); // Laws copied into Maws. Only the laws in Maws to be considered.
    
    for(t = 0; t < NoTrips; t++)
    {
        c = card(Laws[t]); // The total number of laws now imposed on triple t.
        if (c > 1)
        {
            if (flag) printf("purify triple %d ? c is %d\n", t, c);
            Laws[t] = 0;
            if (flag) printf("Calling E3size A\n");
            E3Size(Laws, degree);  // Form the closure of Laws.
            if (flag)
            {
                printf("Leaving E3size A, size is %d, Laws is ", Size);
                printPermutation(Laws, NoTrips);
            }
            d = card(Laws[t]);
            if (c == d) continue;  // All laws imposed on triple t are implied by the other laws.
            if (d > 0)  // So
            {
                if (flag) printf("success 1, d is %d\n", d);
                if (score(Laws, NoTrips) >= Score)
                {
                    printf("Error in purify\n");
                    printPermutation(Laws, NoTrips);
                    exit(0);
                }
                free(Maws);
                return purify1(Laws, degree);
            }
            //if (bestFill(Laws, Maws, MaxSize(degree), degree) > Size)
            bestFill(Laws, Maws, MaxSize(degree), degree);
                return purify1(Laws, degree);
        }
    }
    free(Maws);
    return Size; // Cannot purify.
}


void randomPerm(Permutation T, int length)
/* Sets T to a random permutation of length `length'.
 T has been initialised externally.  */
{
    int *used;
    int i;
    int n;
    
    initialisePermutation(&used, length);
    for (i = 0; i < length; i++) T[i] = used[i] = -1;
    for (i = 0; i < length; )
    {
        n = rand() % length;
        if (used[n] == -1)
        {
            T[i] = n;
            used[n] = i++;
        }
    }
    free(used);
}

int randomSpecial(int t_int, DataBase* database_ptr, int degree, int *Laws, Permutation T)
/* A law has been imposed or is implied for every triple T[i] for i < t_int.
 At most one law is satisfied on each triple.  Try to impose a law on triple
 T[t_in] in such a way as to preserve this condition.  Return 1 or 0 according as 
 we succeed or fail.  */
{
    int flag = 0;
    int lex; /* Loop counter: integer mapping of a law in range 0 to 5 */
    int i;
    triple t;
    DataBase database_copy; /* Copy of DataBase *database_ptr; */
    pair law;
    Permutation L;
    int done = 0;
    int satisfies;
    
    if (t_int == getNoTrips(degree))
    {
        DBtoLaws(database_ptr, Laws, degree);
        return 1;
    }
    
    // Check if the database already satisfies a law on this triple
    satisfies = DBsatisfiesLaw(database_ptr,T[t_int],degree);
    
    if (satisfies)
        // go to next triple
        {
            if (flag) printf("Law %d satisfied on triple %d\n", satisfies-1, T[t_int]);
            return randomSpecial(t_int + 1, database_ptr, degree, Laws, T);
        }
    
    else
    {
        intToTriple(T[t_int], t);
        initialisePermutation(&L,6);
        randomPerm(L,6);
        
        for (lex = 0; lex < 6; lex++)
        {
            copyDataBase(database_ptr,&database_copy,degree); // Initialises database_copy.
            
            intToLaw(L[lex], law);
            
            if (!insertCondorcet(t[0],t[1],t[2],law[0],law[1],&database_copy,degree))
            {
                printf("Error in insertCondorcet with triple %d-%d-%d and laws %d/%d.\n",t[0],t[1],t[2],law[0],law[1]);
                return 0;
            }
            
            if (oneLaw(&database_copy, degree))
            {
                done = randomSpecial(t_int + 1, &database_copy, degree, Laws, T);
                if (done)
                {
                    freeDataBase(&database_copy,degree);
                    free(L);
                    return 1;
                }
            }
            freeDataBase(&database_copy,degree);
        }
        free(L);
    }
    return 0;
}

int bestFill(Permutation Laws, Permutation Maws, int bound, int degree)
/* Returns the size of the largest UCD obtained by imposing a law from Maws on
 triples t for which Laws[t] == 0, and sets Laws to a corresponding set of laws;
 or accepts the first UCD it finds of size >= bound.
 Laws imposed by implication will satisfy the above Maws condition. */
{
    int flag = 0;
    int t;
    int lex;
    int NoTrips = getNoTrips(degree);
    int best;
    int score;
    int *Caws;
    int *Daws;
    int randlaws[6];
    int law;
    
    if(flag)
    {
        printf("Entering bestfill.\n");
        printf(" Laws is ");
        printPermutation(Laws, NoTrips);
        printf("\n");
    }
    
    for (t = 0; (t < NoTrips)  && Laws[t]; t++);  // Find first triple that needs filling.
    
    if (t == NoTrips)  // There are none.
    {
        score = E3Size(Laws, degree); // There were no places to fill.  This call replaces Laws by its closure.
        if (flag) printf("Leaving bestFill wth score %d, nothing to fill.\n", score);
        return score;
    }
    
    best = 0;
    initialisePermutation(&Caws, NoTrips); // For recursion
    initialisePermutation(&Daws, NoTrips); // For best completion
    randomPerm(randlaws, 6);
    for (law = 0; law < 6; law++)
        if (isIn(lex = randlaws[law], Maws[t])) // so lex is a possible choice of law.
        {
            copyPermutation(Laws, Caws, NoTrips); // Now Caws = Laws.
            Caws[t] = 1<<lex;  // Impose law lex
            E3Size(Caws, degree);  // Form the completion;
            score = bestFill(Caws, Maws, bound, degree);  // Fills any further gaps optimally.
            if (score > best)
            {
                copyPermutation(Caws, Daws, NoTrips);  // So Daws holds the best completion to date.
                best = score;
                if (best >= bound) break;
            }
        }
    copyPermutation(Daws, Laws, NoTrips);  // Now Laws is the best completion.
    if(!Laws[t])
    {
        printf("Error in bestFill, t is %d, Maws[t] is %d\n",t, Maws[t]);
        printf("Laws is ");
        printPermutation(Laws, NoTrips);
        exit(0);
    }
    free(Caws);
    free(Daws);
    if(flag)
    {
        printf("Leaving bestFill with score %d\n", best);
        printf("Laws is ");
        printPermutation(Laws, NoTrips);
        printf("\n");
    }
    return best;
}

int E3RecSize(Permutation Laws, Permutation NewLaws, Permutation perm, Permutation used, int i, int degree)
/*
 Laws is an array of length NTrips = d(d-1)(d-2)/6, d = degree in this comment.
 If t is the t-th triple then Laws[t] encodes a set of laws to be imposed on this triple.
 perm is an array of length d.  This is a partially defined permutation of [0..d-1].
 The first i slots have been filled in a way that is compatible with Laws.
 used is the inverse of perm, so if perm[n] = m for some n < i then used[m] = n.
 If m has not been used then used[m] = -1.
 The procedure calculates recursively the number of ways in which the partial permutation may
 be completed to a permutation that is compatible with Laws.
 NewLaws is only used if i = degree - 1.  It is initialised and set to have every entry set to 63
 in the external calling procedure. (All laws satisfied).  If i = d-1, and a permutation is
 successfully completed, the laws that this permutation does not satisfy are deleted from Laws.
 For the external call, used and perm are initialised with all entries set to -1, and i is set to 0.
*/
{
    int flag = 0; // print flag for debugging, flag can be set to 2.
    int sum; // to be returned
    int k;  // Attempt to put k into perm[i]
    int allowed;  // Can we allow perm[i] = k?
    Permutation usedNew;  // for recursive calls
    Permutation permNew;  // form recursive calls
    int a,b,c; // Check on triples (a,b,c) with one of these being k
    int t; // Integer encoding of (a,b,c)
    triple trip; // trip = (a,b,c)
    int lex; // a law in Laws[t]
    int j;
    int NoTrips = getNoTrips(degree);
    
    if (flag && (i == 0))
    {
        printf("Entering E3RecSize.\n");
        printf("Laws is ");
        printPermutation(Laws, NoTrips);
        printf("used is ");
        printPermutation(used, degree);
    }
    if (flag && (i>0))
    {
        printf("perm is ");
        printPermutation(perm, i);
    }
    
    sum = 0;
    
    initialisePermutation(&usedNew, degree);
    
    for (k = 0; k < degree; k++)
    {
        if (used[k] == -1)  // so k is avaialble
        {
            
            copyPermutation(used, usedNew, degree);
            usedNew[k] = i;
            allowed = 1;  // Can we allow perm[i] = k?  Now check this.
           
            if (i > 1)
                for(t = 0; (t < NoTrips) && allowed; t++)
                {
                    intToTriple(t, trip);
                    a = trip[0];
                    b = trip[1];
                    c = trip[2];
                    if ((usedNew[a] != -1) && (usedNew[b] != -1) && (usedNew[c] != -1) && ((a == k) || (b == k) || (c == k)) )
                    for (lex = 0; (lex < 6) && allowed ; lex++)
                        if (isIn(lex, Laws[t]))
                            {
                            if (flag > 1) printf("(%d,%d,%d), (%d,%d,%d),%d, %d\n",a,b,c,usedNew[a], usedNew[b], usedNew[c], lex,
                                    obeys(usedNew[a], usedNew[b], usedNew[c], lex));
                            if(!obeys(usedNew[a], usedNew[b], usedNew[c], lex))
                                allowed = 0;
                            }
                }
            
            if (allowed)
            {
                if (flag > 1) printf("k  = %d, i = %i was allowed.\n", k, i);
                if (i == degree - 1)
                {
                    if (flag)
                    {
                        printf("(");
                        for (j = 0; j < degree - 1; j++) printf("%d ", perm[j]);
                        printf("%d ", k);
                        printf(")\n");
                    }
                    killLaws(NewLaws, usedNew, degree);
                    return 1;
                }
                initialisePermutation(&permNew, degree);
                copyPermutation(perm, permNew, degree);
                permNew[i] = k;
                sum += E3RecSize(Laws, NewLaws, permNew, usedNew, i + 1, degree);
                free(permNew);
            }
        }
    }
    if (flag && (i == 0)) printf("size returning %d\n", sum);
    free(usedNew);
    return sum;
}

void killLaws(Permutation Laws, Permutation used, int degree)
/* Delete from Laws[t] every law not obeyed by perm on this triple.*/
{
    int flag = 0;
    int t;
    triple trip;
    int NoTrips = getNoTrips(degree);
    int lex;
    
    for(t = 0; t < NoTrips; t++)
    {
        intToTriple(t, trip);
        for (lex = 0; lex < 6; lex++)
        {
            if (!obeys(used[trip[0]], used[trip[1]], used[trip[2]], lex))
            {
                Laws[t] &= ~(1<<lex);
                if (flag)
                {
                    printf("(%d,%d,%d) ,law %d, killed by\n", trip[0], trip[1], trip[1], lex);
                    printPermutation(used, degree);
                }
            }
    
        }
    }
}

int E3Size(Permutation Laws, int degree)
{
    int flag = 0;
    Permutation NewLaws;
    Permutation perm;
    Permutation used;
    int t;
    int NoTrips = getNoTrips(degree);
    int Size;
    
    initialisePermutation(&NewLaws, NoTrips);
    initialisePermutation(&perm, degree);
    initialisePermutation(&used, degree);
    for(t = 0; t < NoTrips; t++)
    {
        NewLaws[t] = 63;
    }
    
    for(t = 0; t < degree; t++)
    {
        perm[t] = used[t] = -1;
    }
    
    Size = E3RecSize(Laws, NewLaws, perm, used, 0, degree);
    
    if(flag)
    {
        printf("InE3Size with Laws and NewLaws thus:\n");
        printPermutation(Laws, degree);
        printf("\n");
        printPermutation(NewLaws, degree);
        printf("\n");

        for(t = 0; t < NoTrips; t++)
            if((Laws[t] & NewLaws[t]) != Laws[t])
            {
                printf("Error in E3Size\n");
                exit(0);
            }
    }
    for(t = 0; t < NoTrips; t++)
    {
        Laws[t] = NewLaws[t];
    }
    free(NewLaws);
    free(perm);
    free(used);
    return Size;
}

int purify2(Permutation Laws, Permutation Maws, int degree)
/* Laws is closed.  Set Laws[t] = 0 for all t for which more than
 one laws is imposed, and then fill these gaps optimally, but
 only imposing on the triple t laws in Maws[t].  These include
 all laws in Laws[t].  Return the size of the resulting MUCD
 */
 
{
    int t;
    int NoTrips = getNoTrips(degree);
    int Special = 1;  // Special means only one law is imposed on each triple.
    int size;
    
    for (t = 0; t < NoTrips; t++)
    {
        if (card(Laws[t]) > 1)
        {
            Laws[t] = 0;
            Special = 0;
        }
    }
    
    size = E3Size(Laws, degree);
    if (Special) return size;
    return bestFill(Laws, Maws, MaxSize(degree), degree);
}

int purify(Permutation Laws, int bound, int degree)
{
    int size;
    int newsize;
    Permutation Maws;
    int t;
    int NoTrips = getNoTrips(degree);
    
    size = E3Size(Laws, degree);
    if (size < bound)
    {
        printf("Too small to purify, size is %d\n", size);
        return size;
    }
    size = purify1(Laws, degree);
    
    initialisePermutation(&Maws, NoTrips);
    for (t = 0; t < NoTrips; t++) Maws[t] = Laws[t]; // Only the laws in Maws to be considered.
    
    newsize = purify2(Laws, Maws, degree);
    
    free(Maws);
    
    if (newsize > size) return purify(Laws, bound, degree);
    return size;
}

int recJump(Permutation Laws, Permutation Maws, int bound, int degree)
/* Called in Jump.  Laws is s set of laws that will be complete,
 and with Laws[t] != 0 for all t.  Maws is a copy of Laws, but
 with Maws[t] = 0 for some (perhaps empty) set of values of t.
 But bestFill sends Maws back to Laws.
 So set Maws[t] = 0 for another randomly chosen value of t.
 If bestFill now produces something new then Laws is set to this
 new set of laws, and the size of the resulting MUCD is returned.
 Otherwise, recurse.
 */
 
{
    int flag = 0;
    Permutation Caws;
    Permutation Daws;
    int NoTrips = getNoTrips(degree);
    int t;
    int size;
    
    if (flag)
    {
        printf("Entering recJump with Laws and Maws as follows:\n");
        printPermutation(Laws, NoTrips);
        printf("\n");
        printPermutation(Maws, NoTrips);
        printf("\n");
    }
    
    initialisePermutation(&Caws, NoTrips);
    copyPermutation (Maws, Caws, NoTrips);  // Caws is a copy of Maws
    
    initialisePermutation(&Daws, NoTrips);
    for (t = 0; t < NoTrips; t++) Daws[t] = 63;  // For bestFill
    
    for(t = 0; (t < NoTrips) && (card(Laws[t]) != 1); t++);
    if (t == NoTrips)
    {
        printf(" Bad value of Laws in recJump\n");
        printPermutation(Laws, NoTrips);
        exit(0);
    }
    
    do
    {
        t = rand()%NoTrips;
    }
    while (card(Maws[t]) != t);  // Find a random value of t such that #Maws[t] = 1.
    
   // if (Maws[t] != 63) Daws[t] = 63 - Maws[t];  // Do not replace a value we have just deleted.
    Caws[t] = 0;  // Set this value to 0 in Caws.
    size = bestFill(Caws, Daws, bound, degree);  // Fill gaps and close with no further restraints.
    if (!equalPerm(Caws, Laws, degree))  // So we have got something new.
    {
        copyPermutation(Caws, Laws, NoTrips);  // Copy Caws into Laws
        free(Caws);
        free(Daws);
        return size;
    }
    Maws[t] = 0;  // We have failed to jump, so set Maws[t] to 0 and try again.
    free(Caws);
    free(Daws);
    return recJump(Laws, Maws, bound, degree);
}

int jump(Permutation Laws, int bound, int degree)
/* Modify Laws to produce a different set of laws that define
 an MUCD.  The change favours larger MUCD's.  */
{
    Permutation Maws;
    int NoTrips = getNoTrips(degree);
    int size;
    
    initialisePermutation(&Maws, NoTrips);
    copyPermutation(Laws, Maws, NoTrips);
    
    size = recJump(Laws, Maws, bound, degree);
    free(Maws);
    return size;
}

int equalPerm(Permutation P, Permutation Q, int size)
{
    int t;
    
    for (t = 0; t < size; t++)
    {
        if (P[t] != Q[t]) return 0;
    }
    return 1;
}

void Alternating(Permutation Laws, int degree)
{
    int t;
    
    int NoTrips = getNoTrips(degree);
    triple trip;
    
    for(t = 0; t < NoTrips; t++)
    {
        intToTriple(t, trip);
        if (trip[1] %2) Laws[t] = 8;
        else Laws[t] = 4;
    }
}

int MaxSize(int degree)
/* Returns the largest known size of an MUCD of the given degree.
 This is the size of the alternating scheme for degrees other than 8.  */
{
    int size;
    
    switch(degree)
    {
        case 3:
            size = 4;
            break;
        case 4:
            size = 9;
            break;
        case 5:
            size = 20;
            break;
        case 6:
            size = 45;
            break;
        case 7:
            size = 100;
            break;
        case 8:
            size = 224;
            break;
        case 9:
            size = 488;
            break;
        case 10:
            size = 1069;
            break;
        default:
            printf("Degree %d is too big for MaxSize\n", degree);
            exit(0);
    }
    return size;
}

int Choose(int n, int i)
{
    assert( n >= i);
    assert( i >= 0);
    if (i == 0) return 1;
    if (i == 1) return n;
    if (n == i) return 1;
    return (Choose(n,i-1) * (n-i+1))/i;
}

int setTripsToInt(Permutation P, int i, int length)
/* P is a an array of ints, of length length', and with exactly i terms set to 1.
 It represents a subset of [0,...,length-1] if size i.
 Return n where this is the n-th such i-tuple.  */
{
    Permutation NewP;
    int t;
    int n;
    
    assert(i <= length);
    
    if (length == 1) return 0;  // No choice
    
    if (i == length) return 0;  // No choice
    
    if (i == 0) return 0;  // No choice
    
    initialisePermutation(&NewP, length - 1);  // First entry deleted
    for(t = 0; t < length - 1; t++) NewP[t] = P[t+1];
    if (P[0] == 1)  // First entry was chosen.  These are placed first
    {
        n = setTripsToInt(NewP, i - 1, length - 1); // so i-1 to be chosen
        free (NewP);
        return n;
    }
    n = setTripsToInt(NewP, i, length - 1);   // First entry not chosen
    free (NewP);
    return n + Choose(length - 1, i - 1);  // There were Choose(length - 1, i - 1) earlier cases.
}

void intToSetOfTrips(Permutation P, int n, int k, int i, int length)
/* P is an array of ints of length `length', the first k having been set to
 0 or 1.  i further entries are to be set to 1, and the remainder to 0.
 The n-th such set of choices is to be made. */
{
    int t;
    int c;
    
    if ( i == 0 )
    {
        if (n != 0)
        {
            printf("Error in intToSetOfTrips.\n");
            exit(0);
        }
        for (t = k; t < length; t++) P[t] = 0;
        return;
    }
        
    if (n < ( c = Choose(length - k - 1, i - 1)))
    {
        P[k] = 1;  // Those in which k is chosen come first
        intToSetOfTrips(P, n, k+1, i-1, length);
    }
    else
    {
        n = n - c;
        P[k] = 0;
        intToSetOfTrips(P, n, k+1, i, length);
    }
}

int countCands(Permutation P, int length)
/* P is of length `length'.  Count the number of entries that are powers of 2. */
{
    int sum = 0;
    int i;
    
    for(i = 0; i < length; i++)
        if (card(P[i]) == 1) sum++;
    return sum;
}

void getReps(Permutation P, Permutation Reps, int size, int length)
/* P is a permutation of length `length', with `size' entries
 being powers of 2.  Reps has been initialised as an array of length `size'.
 Set Reps[i] to be j, where P[j] is the i-th elelment of P that is a power of 2. */
{
    int flag = 0;
    int i = 0;
    int j;
    
    for(j = 0; i < size; j++)
        if (card(P[j]) == 1) Reps[i++] = j;
    
    if (flag)
    {
        printf("In getReps, P is ");
        printPermutation(P, length);
        printf("\n Reps is ");
        printPermutation(Reps, size);
    }
}

int testNull(Permutation Laws, Permutation Maws, Permutation Reps, Permutation Zeroes, int i, int n, int size, int degree)
/*  Maws is a copy of Laws.
 Reps and Zeroes are arrays of length size, with undefined entries.
 Take the n-th i-tuple (0 <= a_0 < a_1 < a_{i-1} < size), and
 for 0 <= j < i, set Maws[Reps[a_j]] = 0.
 Now form the closure of Maws.  If Maws is now equal to
 Laws return 0; else return 1.
 */
{
    int flag = 0;
    int j;
    int NoTrips = getNoTrips(degree);
    
    if (flag)
    {
        printf("Entering testNull with Laws: \n");
        printPermutation(Laws, NoTrips);
        printf("Maws is \n");
        printPermutation(Maws, NoTrips);
    }
    
    intToSetOfTrips(Zeroes, n, 0, i, size);
    
    for(j = 0; j < size; j++)
        if (Zeroes[j]) Maws[Reps[j]] = 0; // j is the next a_? as in above comment.
            
    E3Size(Maws, degree);
    if (flag)
    {
        printf("leaving testNull with Laws: \n");
        printPermutation(Laws, NoTrips);
        printf("Maws is \n");
        printPermutation(Maws, NoTrips);
    }
    return !equalPerm(Laws, Maws, NoTrips);
}

int Jump2(Permutation Laws, int bound, int degree)
{
    int NoTrips = getNoTrips(degree);
    int size = countCands(Laws, NoTrips);
    int n;
    int c;
    int i;
    Permutation Maws;
    Permutation Reps;
    Permutation Zeroes;
    Permutation Daws;
    Permutation T;
    
    initialisePermutation(&Maws, NoTrips);
    copyPermutation(Laws, Maws, NoTrips);
    initialisePermutation(&Reps, size);
    getReps(Laws, Reps, size, NoTrips);
    initialisePermutation(&Zeroes, size);
    initialisePermutation(&Daws, NoTrips);
    
    for (i = 0; i < NoTrips; i++) Daws[i] = 63;
    
    for (i = 0; i < size; i++)
    {
        c = Choose(size, i);
        initialisePermutation(&T, c);
        randomPerm(T, c);
        

        for(n = 0; n < c; n++)
            if (testNull(Laws, Maws, Reps, Zeroes, i, T[n], size, degree))
            {
                bestFill(Maws, Daws, bound, degree);
                copyPermutation(Maws, Laws, NoTrips);
                free(Maws);
                free(Reps);
                free(Zeroes);
                free(Daws);
                return 1;
            }
    }
    printf("Could not jump.\n");
    return 0;
}







