int ifAndOnlyIfFlag = 0;
int greaterImpliesFlag = 0;
int insertInLocalFlag = 0;
int insertInGlobalFlag = 0;
int findbug = 0;  // Should only be set when the identity is in the database.

void initialisePairsDatabase(Pairs_DB* pairs_db_ptr, int degree)
{
    /* size is the number of ordered pairs (a,b) with 0 <= a != b < d.
     If (a,b) is the i-th pair then *pairs_db_ptr[i] is an array of unsigned 32-bit ints
     that stores a bit-string of length size, and the j-th bit is set
     iff a > b -> c > d, where (c,d) is the j-th pair, and x > y means that x comes after y.
     length = 1 + (degree * (degree - 1) - 1) / 32 is the number of 32-bit unsigned ints needed to store size bits.
     The a > b -> a > b bit is set to 1, and all others are set to 0.
     */

    int size = degree * (degree - 1);
    int length = getDataBaseLength(degree);

    int i;
    int j;

    PairsSet* pairsset_ptr;

    if (!(*pairs_db_ptr = malloc(size * sizeof(pairsset_ptr))))
    {
        printf("Error in initialisePairsDatabase: malloc failed.");
        exit(0);
    }

    for (i = 0; i < size; i++)
    {
        (*pairs_db_ptr)[i] = malloc(length * sizeof(UNINT));

        if (!(*pairs_db_ptr)[i])
        {
            printf("Error in initialisePairsDatabase: malloc failed.");
            exit(0);
        }

        for (j = 0; j < length; j++)
        {
            (*pairs_db_ptr)[i][j] = 0;
        }

        (*pairs_db_ptr)[i][i/32] = ONE << (i%32);  // a > b -> a > b
    }
}

void initialisePairsSet(PairsSet* pairs_ptr, int degree)
{
    /* Length is the number of pairs (a,b) with 0 <= a != b < degree.
     Size is the number of 32 bit unsigned ints needed to hold length bits.
     If (a,b) is the i-th pair, the i-th bit will be set to 1 iff
     a > b is set in the data base, where a > b means that a comes after b.
     These bits are all set to 0.
     */

    int length = degree * (degree - 1);
    int size = 1 + (length - 1) / 32;
    int i;

    if (!(*pairs_ptr = malloc(size * sizeof(UNINT))))
    {
        printf("Error in initialisePairsSet: malloc failed.");
        exit(0);
    }

    for (i = 0; i < size; i++)
    {
        (*pairs_ptr)[i] = 0;
    }
}

void initialiseDataBase(DataBase* database_ptr, int degree)
{
    initialisePairsSet(&(database_ptr->Greater), degree);
    initialisePairsDatabase(&database_ptr->LocalGreater, degree);
}

int position(int i, int j, int degree) // returns k, where (i,j) is the k-th ordered pair.
{
    int k = i * (degree - 1) + j;

    if (j > i)
        return k - 1;

    return k;
}

int greaterImplies(int a, int b, int c, int d, DataBase* database_ptr, int degree)
// Has `a > b implies c > d' been recorded in the data base?
// Returns 1 if c > d is recorded, or if b > a is recorded.
// If neither of these has been recorded returns 0 if a > b has been recorded and the conclusions have been drawn,
// or d > c has been recorded.
// Note that greaterImplies(a,b,c,d) returns the same value as greaterImplies(d,c,b,a).

{
    int m;
    int n;
    int implies;

    if (greaterImpliesFlag) printf("Entering greaterImplies with (a,b,c,d) = (%d,%d,%d,%d)\n",a,b,c,d);

    if ((a==b) || (c == d) || (a < 0) || (a >= degree) || (b < 0) || (b >= degree)
         || (c < 0) || (c >= degree) || (d < 0) || (d >= degree))
    {
        printf("Bad parameters for greaterImplies %d, %d, %d, %d\n",a,b,c,d);
        exit(0);
    }

    if (greater(c,d,database_ptr,degree)) // if c > d then a > b -> c > d
    {
        if (greaterImpliesFlag)
            printf("Leaving greaterImplies(%d, %d, %d, %d) with 1 as %d > %d\n", a,b,c,d,c,d);
        return 1;
    }

    if (greater(b,a,database_ptr,degree)) // if b > a then a > b -> c > d
    {
        if (greaterImpliesFlag)
            printf("Leaving greaterImplies(%d, %d, %d, %d) with 1 as %d > %d\n", a,b,c,d,b,a);
        return 1;
    }

    m = position(a,b,degree);
    
    if (greater(d,c,database_ptr,degree))  // if d > c then a > b -> c > d is false unless b > a.
                                            // But the consequences of b > a may not have been drawn.
    {
        if (!database_ptr->LocalGreater[m]) // So the consequences of b > a will have been worked through.
        {
            if (greaterImpliesFlag) printf("Leaving greaterImplies(%d, %d, %d, %d) with 0 as %d > %d\n", a,b,c,d,d,c);
            return 0;
        }
    }
    
    if (greater(a,b,database_ptr,degree))
    {
        if (!database_ptr->LocalGreater[m]) // So the consequences of a>b have been worked through.  c>d is not set to true.
        {
            if (greaterImpliesFlag) printf("Leaving greaterImplies(%d, %d, %d, %d) with 0 as %d > %d\n", a,b,c,d,a,b);
            return 0;
        }
    }

    if (database_ptr->LocalGreater[m])
    {
        n = position(c,d,degree);
        implies = (database_ptr->LocalGreater[m][n / 32]) & ((ONE << (n % 32)));

        if (greaterImpliesFlag)
            printf("Leaving greaterImplies with (a, b, c, d), implies = (%d, %d, %d, %d), %d\n", a,b,c,d,implies);
        return implies;
    }

    printf("Error in greaterImplies. (a,b;c,d) = (%d,%d;%d,%d)\n", a, b, c, d);
    exit(0);
}

void writeImplies(DataBase* database_ptr, int degree)
{
    int a;
    int b;
    int c;
    int d;

    for (a = 0; a < degree; a++)
        for (b = 0; b < degree; b++)
            if (a != b)
                for (c = 0; c < degree; c++)
                    for (d = 0; d < degree; d++)
                        if ((c != d) && (greaterImplies(a,b,c,d,database_ptr,degree)))
                        {
                            printf("%d > %d ---> %d > %d\n",a,b,c,d);
                        }
}

void fwriteImplies(FILE *f_ptr, DataBase* database_ptr, int degree)
{
    int a;
    int b;
    int c;
    int d;

    for (a = 0; a < degree; a++)
        for (b = 0; b < degree; b++)
            if (a != b)
                for (c = 0; c < degree; c++)
                    for (d = 0; d < degree; d++)
                        if ((c != d) && (greaterImplies(a,b,c,d,database_ptr,degree)))
                        {
                            fprintf(f_ptr, "%d > %d ---> %d > %d\n",a,b,c,d);
                        }
}

void writeInLocal(int a, int b, int c, int d, DataBase* database_ptr, int degree)
// Insert into the local data base the assertion: a > b -> c > d, assuming
// that neither a > b nor b > a has been recorded.
{
    int m,n;

    if ((a==b) || (c == d) || (a < 0) || (a >= degree) || (b < 0) || (b >= degree)
        || (c < 0) || (c >= degree) || (d < 0) || (d >= degree))
    {
        printf("Bad parameters for writeInLocal %d, %d, %d, %d\n",a,b,c,d);
        exit(0);
    }

    if (findbug && (a > b) && (c < d))
    {
        printf("Bad call to writeInLocal(a,b,c,d) = (%d, %d, %d, %d)\n", a,b,c,d);
        exit(0);
    }

    m = position(a,b,degree);

    if (database_ptr->LocalGreater[m])  // So neither a > b nor b > a is recorded
    {
        n = position(c,d,degree);
        database_ptr->LocalGreater[m][n / 32] = database_ptr->LocalGreater[m][n / 32] | (ONE << (n % 32));
    }

}

int greater(int a, int b, DataBase* database_ptr, int degree)
// Is `a > b' recorded in the data base?
{
    int m;
    m = position(a,b,degree);

    return (((database_ptr->Greater[m / 32]) & ((ONE << (m % 32))))!= 0);
}

void writeGreater(DataBase* database_ptr, int degree)
{
    int a;
    int b;

    for (a = 0; a < degree; a++)
        for (b = 0; b < degree; b++)
            if ((a != b) && (greater(a,b,database_ptr,degree) == 1))
            {
                printf("%d > %d\n",a,b);
            }
}

void fwriteGreater(FILE *f_ptr, DataBase* database_ptr, int degree)
{
    int a;
    int b;

    for (a = 0; a < degree; a++)
        for (b = 0; b < degree; b++)
            if ((a != b) && (greater(a,b,database_ptr,degree) == 1))
            {
                fprintf(f_ptr, "%d > %d\n",a,b);
            }
}


void writeInGlobal(int a, int b, DataBase* database_ptr, int degree)
// Insert into the global data base the assertion: a > b.
{
    int m;

    if ((a == b) || (a < 0) || (a >= degree) || (b < 0) || (b >= degree))
    {
        printf("Bad parameters for writeInGlobal %d, %d\n",a,b);
        exit(0);
    }

    if (findbug && (a < b))
    {
        printf("Bad call to writeInGlobal, (a, b) = (%d, %d)\n", a, b);
        exit(0);
    }

    m = position(a,b,degree);

    database_ptr->Greater[m / 32] |= (ONE << (m % 32));
}

int insertInGlobal(int a, int b, DataBase* database_ptr, int degree)
// Insert `a > b' into the database, if it is not there already, and draw consequences.
// Return 0 if this gives rise to a contradiction; otherwise return 1.
{
    int c, d;

    if (findbug && (a < b))
    {
        printf("Bad call to insertInGlobal, (a, b) = (%d, %d)\n", a, b);
        exit(0);
    }

    if (insertInGlobalFlag)
    {
        printf("Called insertInGlobal: a = %d / b = %d\n",a,b);
    }

    if (greater(a,b,database_ptr,degree))  // So already recorded.
    {
        if (insertInGlobalFlag) printf("Leaving insertInGlobal(%d,%d)\n",a,b);
        return 1;
    }


    if (greater(b,a,database_ptr,degree))  // So a contradiction
    {
        printf("In insertInGlobal, We have %d>%d and %d>%d\n", a, b, b, a);
        return 0;
    }

    writeInGlobal(a,b,database_ptr,degree);  // So recursion will terminate.

    for (c = 0; c < degree; c++)
    {
        for (d = 0; d < degree; d++)
        {
            if ((c != d) && ((c != a) || (d != b)) && greaterImplies(a,b,c,d,database_ptr,degree)) // If a>b -> c>d
            {
                if(!insertInGlobal(c,d,database_ptr,degree))  // then insert c>d
                    return 0;  // Hit a contradiction.
            }
        }
    }

    for (c = 0; c < degree; c++)
    {
        for (d = 0; d < degree; d++)
        {
            if ((c != d) && ((c != a) || (d != b)) )
            {
                if (!insertInLocal(c,d,a,b,database_ptr,degree)) return 0;  // if a>b then c>d -> a>b.
            // contrapositive b>a -> d>c is then called by insertInLocal.
            }
        }
    }

    for (c = 0; c < degree; c++)
    {
        if ((c != a) && (c != b) && greater(b,c,database_ptr,degree)) // If b>c
        {
            if (!insertInGlobal(a,c,database_ptr,degree)) return 0; // then a>c.
        }
    }


   for (c = 0; c < degree; c++)
    {
        if ((c != a) && (c != b) && greater(c,a,database_ptr,degree))  // If c>a
        {
            if (!insertInGlobal(c,b,database_ptr,degree)) return 0; // then c>b.
        }
    }

    for (c = 0; c < degree; c++) // a>b -> (c>a -> c>b)
        if ((c != a) && (c != b))
        {
            if (!insertInLocal(c,a,c,b, database_ptr, degree)) return 0;
            // contrapositive is then called by insertInLocal.
        }

    freeLocal(a,b,database_ptr,degree);
    freeLocal(b,a,database_ptr,degree);
    if (insertInGlobalFlag) printf("Leaving insertInGlobal(%d,%d)\n",a,b);
    return 1;
}

int insertInLocal(int a, int b, int c, int d, DataBase* database_ptr, int degree)
// insert a > b -> c > d into database, and draw conclusions.
// Also insert the contrapositive, which, of course, has the same conclusions.
// Return 0 if this gives rise to a contradiction, otherwise return 1.
{
    int e, f;

    if (findbug && (a > b) && (d > c))
    {
        printf("Error in insertInLocal: %d > %d and %d < %d.\n",a,b,c,d);
        exit(0);
    }

    if (insertInLocalFlag) printf("/0 Entering insertInLocal with (a, b, c, d) = (%d, %d, %d, %d)\n", a, b, c, d);

    if (greaterImplies(a,b,c,d,database_ptr,degree))
    {
        if (insertInLocalFlag) printf("/1 Leaving insertInLocal(%d, %d, %d, %d).\n", a, b, c, d);
        return 1;
    }

    // So b>a is not at this point in the database.
    // Also greaterImplies(d,c,b,a,database, degree) will return 1.
    // So c>d is not in the database.

    if (greater(a,b,database_ptr,degree))  // if a>b then (a>b -> c>d) is equivalent to c>d.
        {
        if (insertInLocalFlag) printf("/2 Leaving insertInLocal(%d, %d, %d, %d)\n", a,b,c,d);

        if (findbug && (d > c))
        {
            printf("Error in insertInLocal(%d,%d,%d,%d) (1)as %d>%d.\n",a,b,c,d,a,b);
            exit(0);
        }
        return insertInGlobal(c,d,database_ptr,degree);
    }

    if (greater(d,c,database_ptr,degree))  // if d>c then (a>b -> c>d) is equivalent to b>a.  Contrapositive.
    {
        if (insertInLocalFlag) printf("/2 Leaving insertInLocal(%d, %d, %d, %d)\n", a,b,c,d);

        if (findbug && (a > b))
        {
            printf("Error in insertInLocal(%d,%d,%d,%d) (1) as %d>%d.\n",a,b,c,d,d,c);
            exit(0);
        }
        return insertInGlobal(b,a,database_ptr,degree);
    }

    // Now neither a>b nor d>c can be in the database.

    writeInLocal(a,b,c,d,database_ptr,degree);
    writeInLocal(d,c,b,a,database_ptr,degree); // contrapositive
    if (!greaterImplies(a,b,c,d,database_ptr, degree))
    {
        printf("Bug in insertInLocal\n");
        printf("(a,b,c,d) = (%d, %d, %d, %d)\n",a,b,c,d);
        writeDataBase(database_ptr, degree);
        exit(0);
    }

    // So recursion completes


    if ((a == d) && (b == c)) // a>b -> b>a.  This is equivalent to b>a.
    {
        if (findbug && (b < a))
        {
            printf("Error in insertInLocal (2): Cannot call insertInGlobal with %d < %d.\n",b,a);
            exit(0);
        }
        if (insertInLocalFlag) printf("/3 Leaving insertInLocal(%d, %d, %d, %d), so %d > %d\n", a, b, c, d, b, a);
        return insertInGlobal(b, a, database_ptr, degree);
    }

    //  If b>a -> c>d then a>b -> c>d is equivalent to c>d.
    if (greaterImplies(b,a,c,d, database_ptr, degree))
    {
        if (findbug && (c < d))
        {
            printf("Error in insertInLocal (3): Cannot call insertInGlobal with %d < %d.\na = %d / b = %d\n",c,d,a,b);
            exit(0);
        }
        return insertInGlobal(c,d,database_ptr,degree);  // So now greaterImplies(a,b,c,d)
    }

    // similarly the contrapositive, if a>b -> d>c then a>b -> c>d is equivalent to b>a.
    if (greaterImplies(a,b,d,c, database_ptr, degree))
    {
        if (findbug && (b < a))
        {
            printf("Error in insertInLocal (4): Cannot call insertInGlobal with %d < %d.\n c = %d / d = %d\n",b,a,c,d);
            exit(0);
        }
        return insertInGlobal(b,a,database_ptr,degree);  // So now greaterImplies(a,b,c,d)
    }


    for (e = 0; e < degree; e++)  // if a>b -> c>d and c>d -> e>f then a>b -> e>f.  Transitivity of implication.
    {
        for (f = 0; f < degree; f++)
        {
            if ((e != f) && ((c != e) || ( d!= f)) && greaterImplies(c,d,e,f,database_ptr,degree))
            {
                if (!insertInLocal(a,b,e,f,database_ptr,degree)) return 0;
            }
        }
    }

    for (e = 0; e < degree; e++)  // if a>b -> c>d and c>d -> e>f then a>b -> e>f.  Transitivity of implication.
    {
        for (f = 0; f < degree; f++)
        {
            if ((e != f) && ((c != e) || ( d!= f)) && greaterImplies(c,d,e,f,database_ptr,degree))
            {
                if (!insertInLocal(a,b,e,f,database_ptr,degree)) return 0;
            }
        }
    }

    for (e = 0; e < degree; e++)  // if a>b -> c>d and a>b -> d>e then a>b -> d>e.  Transitivity of >.
    {
        if ((d != e) && (c != e) && greaterImplies(a,b,d,e,database_ptr,degree))
        {
            if (!insertInLocal(a,b,c,e,database_ptr,degree)) return 0;
        }
    }

    for (e = 0; e < degree; e++)  // Contrapositive of previous clause.
    {
        if ((a != e) && (b != e) && greaterImplies(e,a,c,d,database_ptr,degree))
        {
            if (!insertInLocal(d,c,b,e,database_ptr,degree)) return 0;
        }
    }

    for (e = 0; e < degree; e++)  // if a>b -> c>d and a>b -> e>c then a>b -> e>d.  Transitivity of >.
    // Logically equivalent to the previous for clauses; but needed here.
    {
        if ((d != e) && (c != e) && greaterImplies(a,b,e,c,database_ptr,degree))
        {
            if (!insertInLocal(a,b,e,d,database_ptr,degree)) return 0;
        }
    }

    for (e = 0; e < degree; e++)  // if a>b -> c>d and b>e -> c>d then a>e -> c>d.  Contrapositive of previous result
    {
        if ((a != e) && (b != e) && greaterImplies(b,e,c,d,database_ptr,degree))
        {
            if (!insertInLocal(a,e,c,d,database_ptr,degree)) return 0;
        }
    }

    if ((b == c) && (a != d))
    {
        if (!insertInLocal(a,b,a,d, database_ptr, degree)) return 0; // if a>b -> b>d then a>b -> a>d.  Transitivity of >.
        if (!insertInLocal(d,b,d,a, database_ptr, degree)) return 0; // Contrapositive.  If a>b -> b>d then d>b -> d>a.
    }

    if ((d == a) && (c != b))
    {
        if (!insertInLocal(a,b,c,b, database_ptr, degree)) return 0; // if a>b -> c>a then a>b -> c>b.  Transitivity of >.
        if (!insertInLocal(a,c,b,c, database_ptr, degree)) return 0; // Contrapositive.  If a>b -> c>a then a>c -> b>c.
    }

    if (insertInLocalFlag) printf("/5 Leaving insertInLocal(%d, %d, %d, %d).\n", a, b, c, d);

    return 1;

}

/*
int equated(int a, int b, int c, int d, DataBase database, int degree)
{
    return database.LocalGreater[position(a,b,degree)] == database.LocalGreater[position(c,d,degree)];
}
*/

void freeLocal(int c, int d, DataBase* database_ptr, int degree)
{
    int p = position(c,d,degree);

    free(database_ptr->LocalGreater[p]);
    database_ptr->LocalGreater[p] = NULL;
}

/*
void equatePointers(int a, int b, int c, int d, DataBase database, int degree)
// equate the (a,b) pointer with the (c,d) pointer, and the (b,a) pointer with the (d,c) pointer,
// after copying data across.  But this is illegitimate, as we loose the consequences.
// Called in ifAndOnlyIf, which is not now used.
{
    if (greater(a,b,database,degree) || greater(b,a,database,degree)
        || greater(c,d,database,degree) || greater(d,c,database,degree))
    {
        printf("bad call to equatePointers.\n");
        exit(0);
    }
    if (equated(a,b,c,d,database,degree)) return;

    int length = degree * (degree - 1);
    int size = 1 + (length - 1)/32;
    int posab = position(a,b,degree);
    int poscd = position(c,d,degree);
    int posba = position(b,a,degree);
    int posdc = position(d,c,degree);
    UNINT *ab_ptr;
    UNINT *ba_ptr;
    UNINT *cd_ptr;
    UNINT *dc_ptr;
    int e;
    int f;
    int posef;
    int i;

    if (!database.LocalGreater[posab])
    {
        printf("Bad call to equatePointers, (%d, %d) pointer is Null.\n",a, b);
        exit(0);
    }

    if (!database.LocalGreater[posba])
    {
        printf("Bad call to equatePointers, (%d, %d) pointer is Null.\n",b, a);
        exit(0);
    }

    if (!database.LocalGreater[poscd])
    {
        printf("Bad call to equatePointers, (%d, %d) pointer is Null.\n",c, d);
        exit(0);
    }

    if (!database.LocalGreater[posdc])
    {
        printf("Bad call to equatePointers, (%d, %d) pointer is Null.\n",d, c);
        exit(0);
    }

    ab_ptr = database.LocalGreater[posab];
    ba_ptr = database.LocalGreater[posba];
    cd_ptr = database.LocalGreater[poscd];
    dc_ptr = database.LocalGreater[posdc];

    for (i=0; i< size; i++)
        ab_ptr[i] |= cd_ptr[i];

    for (e = 0; e < degree; e++)
        for (f = 0; f < degree; f++)
            if (e != f)
            {
                posef = position(e,f,degree);
                if (database.LocalGreater[posef] == cd_ptr)
                {
                    freeLocal(e,f,database,degree);
                    database.LocalGreater[posef] = ab_ptr;
                }
            }

    for (i=0; i< size; i++)
        ba_ptr[i] |= dc_ptr[i];

    for (e = 0; e < degree; e++)
        for (f = 0; f < degree; f++)
            if (e != f)
            {
                posef = position(e,f,degree);
                if (database.LocalGreater[posef] == dc_ptr)
                {
                    freeLocal(e,f,database,degree);
                    database.LocalGreater[posef] = ba_ptr;
                }
            }
}
*/

/*void ifAndOnlyIf(int a, int b, int c, int d, DataBase database, int degree) //Not in use, and very unsafe.
{
    int e, f;

    if (ifAndOnlyIfFlag) printf("Entering afAndOnlyIf (a,b,c,d) = (%d, %d, %d, %d)\n", a, b, c, d);

    if (equated(a,b,c,d,database,degree))
    {
        return;
    }

    if (greater(a,b,database,degree))
    {
        insertInGlobal(c,d,database,degree); // Does nothing if c>d already recorded.
        equatePointers(a,b,c,d,database,degree);  // Does nothing if they are now equated. Else copies data and equates both ways.
        return;
    }

    if (greater(b,a,database,degree))
    {
        insertInGlobal(d,c,database,degree);  // Does nothing if d>c already recorded
        equatePointers(b,a,d,c,database,degree);  // Does nothing if they are now equated.
        return;
    }

    if (greater(c,d,database,degree))
    {
        insertInGlobal(a,b,database,degree);
        equatePointers(a,b,c,d,database,degree);
        return;
    }

    if (greater(d,c,database,degree))
    {
        insertInGlobal(b,a,database,degree);
        equatePointers(b,a,d,c,database,degree);
        return;
    }

    for (e = 0; e < degree; e++)
    {
        for (f = 0; f < degree; f++)
        {
            if ((e != f) && ((a != e) || (b != f)) && ((a != f) || (b != e))
                && ((a != c) || (b != d)) && ((a != d) || (b != c))
                && ((c != e) || (d != f)))
                // e != f and {c,d} != {a,b} != {e,f} !={c,d}
            {
                if (greaterImplies(c,d,e,f,database,degree)) // (c>d -> e>f) => (a>b -> e>f)
                {
                    insertInLocal(a,b,e,f,database,degree);
                }
                if (greaterImplies(d,c,e,f,database,degree)) // (d>c -> e>f) => (b>a -> e>f)
                {
                    insertInLocal(b,a,e,f,database,degree);
                }
                if (greaterImplies(e,f,a,b,database,degree)) // (e>f -> a>b) => (e>f -> c>d)
                {
                    insertInLocal(e,f,c,d,database,degree);
                }
                if (greaterImplies(e,f,b,a,database,degree)) // (e>f -> b>a) => (e>f -> d>c)
                {
                    insertInLocal(e,f,d,c,database,degree);
                }
            }
        }
    }

    equatePointers(a,b,c,d,database,degree);
}
 */

int noPairs(DataBase* database_ptr, int degree)
// Returns the number of implications a>b -> c>d when (a,b) != (c,d).
{
    int n = 0;
    int a,b,c,d;

    for (a = 0; a < degree; a++)
        for (b = 0; b < degree; b++)
            if (a != b)
                for (c = 0; c < degree; c++)
                    for (d = 0; d < degree; d++)
                        if ((c != d) && !(( a == c) && (b == d)))
                            if (greaterImplies(a,b,c,d,database_ptr,degree))
                                n++;
    return n;
}

void copyDataBase(DataBase* old_database_ptr, DataBase* new_database_ptr, int degree) // Initilaises new_database_ptr
{
    int i, j;
    int length = degree * (degree - 1); // Number of pairs (i,j) with 0 <=i != j <degree
    int size = 1 + (length - 1) / 32;  // Number of 32-bit unsigned ints needed to store this number of bits.

    initialiseDataBase(new_database_ptr,degree);

    for (i = 0; i < size; i++)
    {
        new_database_ptr->Greater[i] = old_database_ptr->Greater[i];
    }

    for (i = 0; i < length; i++)
    {
        if (old_database_ptr->LocalGreater[i] == NULL)
        {
            free(new_database_ptr->LocalGreater[i]);
            new_database_ptr->LocalGreater[i] = NULL;
            continue;
        }

        for (j = 0; j < size; j++)
        {
            new_database_ptr->LocalGreater[i][j] = old_database_ptr->LocalGreater[i][j];
        }
    }
}

void writeDataBase(DataBase* database_ptr, int degree)
{
    writeGreater(database_ptr,degree);
    writeImplies(database_ptr,degree);
}

int size(int* Laws, Permutation perm, Permutation used, int i, int degree)
/* 
 Laws is an array of length NTrips = d(d-1)(d-2)/6, d = degree in this comment.
 If t is the t-th triple then Laws[t] encodes a set of laws to be imposed on this triple.
 perm is an array of length d.  This is a partially defined permutation of [0..d-1].
 The first i slots have been filled in a way that is compatible with Laws.
 used is the inverse of perm, so if perm[n] = m for some n < i then used[m] = n.
 If m has not been used then used[m] = -1.
 The procedure calculates recursively the number of ways in which the partial permutation may
 be completed to a permutation that is compatible with Laws.
 For an external call used is initialised with all entries set to -1, and i = 0.
 perm may be initialised with any set of initial values.
 */
{
    int flag = 0; // print flag for debugging
    int sum; // to be returned
    int k;  // Attempt to put k into perm[i]
    int allowed;  // Can we allow perm[i] = k?
    Permutation usedNew;  // for recursive calls
    Permutation permNew;  // for recursive calls
    int a,b,c; // Check on triples (a,b,c) with one of these being k
    int t; // Integer encoding of (a,b,c)
    triple trip; // trip = (a,b,c)
    int lex; // a law in Laws[t]
    int j;
    
    if (flag && (i == 0)) printf("Entering Size.\n");
    if (flag && (i>0)) {
        printf("perm is ");
        printPermutation(perm, i);
    }
    sum = 0;
    
    for (k = 0; k < degree; k++)
    {
        if (used[k] == -1)  // so k is avaialble
        {
            initialisePermutation(&usedNew, degree);
            copyPermutation(used, usedNew, degree);
            usedNew[k] = i;
            allowed = 1;  // Can we allow perm[i] = k?  Now check this.
            for (a = 0; (a < degree - 2) && allowed; a++)
                if (usedNew[a] != -1)
                    for(b = a+1; (b < degree - 1) && allowed; b++)
                        if (usedNew[b] != -1)
                            for (c = b+1; (c < degree) && allowed; c++)
                                if ((usedNew[c] != -1) && ((a==k) || (b==k) || (c==k)))
                                {
                                    trip[0] = a;
                                    trip[1] = b;
                                    trip[2] = c;
                                    t = tripleToInt(trip);
                                    for (lex = 0; (lex < 6) && allowed; lex++)
                                        if (isIn(lex, Laws[t]) && !obeys(usedNew[a], usedNew[b], usedNew[c], lex))
                                        {
                                            allowed = 0;
                                            // if(i == degree - 1) printf(" %d was killed by law %d on triple %d\n", k, lex, t);
                                        }
                                    
                                }
            if (allowed)
            {
                //printf("k  = %d, i = %i was allowed.\n", k, i);
                if (i == degree - 1)
                {
                    if (flag)
                    {
                        printf("(");
                        for (j = 0; j < degree - 1; j++) printf("%d ", perm[j]);
                        printf("%d ", k);
                        printf(")\n");
                    }
                    return 1;
                }
                initialisePermutation(&permNew, degree);
                copyPermutation(perm, permNew, degree);
                permNew[i] = k;
                sum += size(Laws, permNew, usedNew, i + 1, degree);
                free(permNew);
            }
            free(usedNew);
        }
    }
    if (flag && (i == 0)) printf("size returning %d\n", sum);
    return sum;
}

int size2(int* Laws, Permutation perm, Permutation used, int i, int degree)
/*
 This has the same specifications as Size; but should be faster.
 */
{
    int sum = 0;  // returned
    int k;        // to be inserted in perm[i]
    if (i == degree) return 1;  // permutation is complete
    for (k = 0; k < degree; k++)
        if (isValid(Laws, perm, used, i, k, degree)) { // So we can put k in the i-th slot
            perm[i] = k;
            used[k] = i;
            sum += size2(Laws, perm, used, i + 1, degree);
            perm[i] = used[k] = -1; // So size2 does not change perm or used.
        }
    return sum;
}

int isValid(int *Laws, Permutation perm, Permutation used, int i, int k, int degree)
/*
 For use in size2.  The parameters other than k have the same meaning.  Is it
 possible to set perm[i] = k without breaking a law or repeating a value?
 */
{
    int x, y;
    triple t;
    
    if (used[k] != -1) return 0;  // So now k has not been used earlier in perm
    if (i < 2) return 1;  // No laws to check
    used[k] = i;
    for(x = 0; x < i-1; x++)
        for(y = x+1; y < i; y++) {
            t[0] = perm[x];
            t[1] = perm[y];
            t[2] = k;
            sort(t);
            if (!permit(Laws, used, t)){
                used[k] = -1;  // Must not change used
                return 0;  // Broke a law
            }
        }
    used[k] = -1;  // Must not change used
    return 1;
}

int permit(int *Laws, Permutation used, triple trip)
/*
 For use in size2.  trip is a triple (a,b,c) with 0 <= a < b < c < degree.
 Laws tells us what laws are imposed on this triple, and used tells us where
 these entries arise in the permutation we are building.  See comment to size.
 */
{
    int t;
    int lex;
    t = tripleToInt(trip);
    for(lex = 0; lex < 6; lex++)
        if (isIn(lex, Laws[t]) && !obeys(used[trip[0]], used[trip[1]], used[trip[2]], lex))
            return 0;
    return 1;
}


int DBsize(DataBase *db_ptr, int degree)
/* Returns the size of the Condorcet Domain defined by the Condorcet laws implied by the data base.
 Warning.  Only the Condorcet laws are considered.  Local implications, for example, are NOT used 
 for any purpose other than deciding if a given Condorcet Law is satisfied.  */
{
    Permutation Laws;
    Permutation perm;
    Permutation used;
    int NoTrips = getNoTrips(degree);
    int t;
    int i;
    int Size;
    
    initialisePermutation(&Laws, NoTrips);
    for(t = 0; t < NoTrips; t++) Laws[t] = 0;
    
    initialisePermutation(&perm, degree);
    initialisePermutation(&used, degree);
    for(i = 0; i < degree; i++) perm[i] = used[i] = -1;
    
    DBtoLaws(db_ptr, Laws, degree);  // Now Laws[t] encodes the set of laws obeyed on this triple.
    
    Size = size2(Laws, perm, used, 0, degree);
    free(Laws);
    free(perm);
    free(used);
    return Size;
}


int obeys(int x, int y, int z, int lex)
/* If x,y,z occur in this order, is the law lex obeyed on this triple? */
{
    switch(lex)
    {
        case 0:  // 1N2
            return ( (x < y) && (x < z) ) || ( (x > y) && (x > z) ); // x is first or last
        case 1:  // 1N3
            return (x < y) || (x < z); // x less than y or z
        case 2:  // 2N1
            return (x < y) || (z < y); // y not least
        case 3:  // 2N3
            return (y < x) || (y < z) ; // y not greatest
        case 4:  // 3N1
            return (x < z) || (y < z); // z not least
        case 5:  // 3N2
            return ( (z < x) && (z < y) ) || ( (z > x) && (z > y) ); // z is greatest or least
        default:
            printf("Error in obeys(%d, %d, %d), %d\n", x,y,z,lex);
            exit(0);
    }
}


int findNext(DataBase* database_ptr, int i, int next, int degree)  // Not used
/* Return the next integer after i in the DataBase, which defines a unique permutation */
{
    int j, k;
    j = 0;

    while ((i != j) && (j < degree) && greater(i, j, database_ptr, degree))
        j++;

    if (j == degree)
        return -1;

    for (k = 0; k < degree; k++)
    {
        if ((j != k) && greater(k, i, database_ptr, degree) && greater(i, k, database_ptr, degree))
        {
            j = k;
        }
    }

    return j;
}

int findFirst(DataBase* database_ptr, int degree) // Only used in test
{
    int j, k;
    j = 0;

    for (k = 0; k < degree; k++)
    {
        if ((j != k) && greater(j, k, database_ptr, degree))
        {
            j = k;
        }
    }

    return j;
}

/*void tidy(DataBase database, int degree)
{

    int a, b, i;

    for (a = 0; a < degree - 1; a++)
    {
        for (b = a + 1; b < degree; b++)
        {
            if (greater(a,b,database,degree) || greater(b,a,database,degree))
            {
                i = position(a,b,degree);

                if (database.LocalGreater[i])
                {
                    freeLocal(a,b,database,degree);
                    database.LocalGreater[i] = 0;
                }

                i = position(b,a,degree);

                if (database.LocalGreater[i])
                {
                    freeLocal(b,a,database,degree);
                    database.LocalGreater[i] = 0;
                }
            }
        }
    }
}
*/

int getDataBaseLength(int degree)  // Used for computing hash values
{
    return 1 + (degree * (degree - 1) - 1) / 32;
}

int equalDataBases(DataBase* database_one_ptr, DataBase* database_two_ptr, int degree)
// Returns 1 if the two databases are identical; else returns 0.
// Warning.  It is just possible that two non-identical databases define the same sets of permutations.
{
    int i;
    int j;
    int length = getDataBaseLength(degree);
    int size = degree * (degree - 1);

    // printf("Entering equalDataBases, with addresses %p and %p.\n", &database_one, &database_two);

    for (i = 0; i < length; i++)
    {
        if (database_one_ptr->Greater[i] != database_two_ptr->Greater[i])
            return 0;

    }

    for (i = 0; i < size; i++)
    {
        if (database_one_ptr->LocalGreater[i] == NULL && database_two_ptr->LocalGreater[i] != NULL)
            return 0;

        if (database_two_ptr->LocalGreater[i] == NULL && database_one_ptr->LocalGreater[i] != NULL)
            return 0;

        if (database_one_ptr->LocalGreater[i] != NULL)
        for (j = 0; j < length; j++)
        {
            if (database_one_ptr->LocalGreater[i][j] != database_two_ptr->LocalGreater[i][j])
                return 0;
        }
    }

    return 1;
}

void fprintDataBase(DataBase* database_ptr, int degree, FILE* file_ptr)
// prints '***' and then the data base.  The triple is written by a different call.
{
    int i;
    int j;
    int length = getDataBaseLength(degree);
    int size = degree * (degree - 1);

    fprintf(file_ptr, "***");

    for (i = 0; i < length; i++)
        fprintf(file_ptr, "\n%u", database_ptr->Greater[i]);

    for (i = 0; i < size; i++)
    {
        if (database_ptr->LocalGreater[i] == NULL)
        {
            fprintf(file_ptr, "\nNULL");
        }
        else
        {
            for (j = 0; j < length; j++)
                fprintf(file_ptr, "\n%u", database_ptr->LocalGreater[i][j]);
        }
    }
}

void freadDataBase(DataBase* database_ptr, int degree, FILE* file_ptr)
// reads `***' and then the data base.  The triple is read by a separate call.
{
    int i;
    int j;
    int size = degree * (degree - 1);
    int length = getDataBaseLength(degree);
    char ch;

    fscanf(file_ptr, " *** ");

    for (i = 0; i < length; i++)
        fscanf(file_ptr, " %u ", &database_ptr->Greater[i]);

    for (i = 0; i < size; i++)
    {
        if ((ch = fgetc(file_ptr)) == 'N')
        {
           // printf("LocalGreater[%d] is NULL\n", i);
            free(database_ptr->LocalGreater[i]);
            database_ptr->LocalGreater[i] = NULL;
            fscanf(file_ptr,"NULL\n");
        }
        else
        {
            ungetc(ch, file_ptr);
            for (j = 0; j < length; j++)
            {
                fscanf(file_ptr, " %u ", &database_ptr->LocalGreater[i][j]);
               // printf("LocalGreater[%d][%d] is %u\n", i, j, database.LocalGreater[i][j]);
            }
        }
    }
}

int isEmpty(FILE *file_ptr)
{
    // char ch;
    int empty;

    empty = (fgetc(file_ptr) != '*');
    rewind(file_ptr);
    return empty;
}

void freeDataBase(DataBase* database_ptr, int degree)
{
    int i;
    int size = degree * (degree - 1);

    free(database_ptr->Greater);

    for (i = 0; i < size; i++)
    {
        if (database_ptr->LocalGreater[i] != NULL)
            free(database_ptr->LocalGreater[i]);
    }

    free(database_ptr->LocalGreater);
}

void randomDB(DataBase* db_ptr, int local, int global, int seed, int degree)
/* Constructs a data base that contains the identity permutation. */
{
    initialiseDataBase(db_ptr, degree);

    int a;
    int b;
    int c;
    int d;

    int i;

    srand(seed);

    for (i = 0; i < local; i++)
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

        // printf("a = %d / b = %d / c = %d / d = %d\n",a,b,c,d);

        if ((b > a) || (c > d))
            insertInLocal(a,b,c,d,db_ptr,degree);
    }

    // printf("a = %d / b = %d / c = %d / d = %d\n", a,b,c,d);

    for (i = 0; i < global; i++)
    {
        do
        {
            a = rand() % degree;
            b = rand() % degree;
        }
        while (b >= a);

        insertInGlobal(a,b,db_ptr,degree);
    }
}

/*
 int barred(int i, int j)
{
    return i == j;
}
 */

/*
 Permutation completePerm(Permutation p, Permutation used, int i, int degree)  // Should this return p or void? Is it ever used?
{
    int j;

    if (i == degree)
        return p;

    for (j = 0; j < degree; j++)
    {
        if ((!used[j]) && (!barred(i,j)))
        {
            p[i] = j;
            used[j] = 1;
            printPermutation(used,degree);
            completePerm(p, used, i+1, degree);
        }
    }
    return p;
}
*/
