int insertCondorcet(int a, int b, int c, int i, int j, DataBase* database_ptr, int degree)
{
   // printf("Entering insertCondorcet with triple (%d, %d, %d) and law %d N %d\n", a,b,c,i,j);

    if ((i == 1) && (j == 3))
    {
        return
        insertInLocal(b,c,b,a,database_ptr,degree) &&
        insertInLocal(c,b,c,a,database_ptr,degree);
    }
    if ((i == 2) && (j == 3))
    {
        return
        insertInLocal(a,c,a,b,database_ptr,degree) &&
        insertInLocal(c,a,c,b,database_ptr,degree);
    }
    if ((i == 1) && (j == 2))
    {
        return
        insertInLocal(a,b,a,c,database_ptr,degree)&&
        insertInLocal(b,a,c,a,database_ptr,degree)&&
        insertInLocal(a,c,a,b,database_ptr,degree)&&
        insertInLocal(c,a,b,a,database_ptr,degree);
    }
    if ((i == 3) && (j == 2))
    {
        return
        insertInLocal(b,c,a,c,database_ptr,degree)&&
        insertInLocal(c,b,c,a,database_ptr,degree)&&
        insertInLocal(a,c,b,c,database_ptr,degree)&&
        insertInLocal(c,a,c,b,database_ptr,degree);
    }
    if ((i == 2) && (j == 1))
    {
        return
        insertInLocal(a,c,b,c,database_ptr,degree)&&
        insertInLocal(c,a,b,a,database_ptr,degree);
    }
    if ((i == 3) && (j == 1))
    {
        return
        insertInLocal(a,b,c,b,database_ptr,degree) &&
        insertInLocal(b,a,c,a,database_ptr,degree);
    }
   // printf("Leaving insertCondorcet with (i,j) = (%d, %d)\n", i, j);
    exit(0);
}

int deduceCondorcet(int a, int b, int c, int i, int j, DataBase* database_ptr, int degree)
// Is the law iNj satisfied on the triple (a,b,c)?  1 <= i != j <= 3.
{
    if ((i == 1) && (j == 3))
    {
        return (greaterImplies(b,c,b,a,database_ptr,degree) && greaterImplies(c,b,c,a,database_ptr,degree));
    }
    if ((i == 2) && (j == 3))
    {
        return (greaterImplies(a,c,a,b,database_ptr,degree) && greaterImplies(c,a,c,b,database_ptr,degree));
    }
    if ((i == 1) && (j == 2))
    {
        return (greaterImplies(a,b,a,c,database_ptr,degree)
             && greaterImplies(b,a,c,a,database_ptr,degree)
             && greaterImplies(c,a,b,a,database_ptr,degree)
             && greaterImplies(a,c,a,b,database_ptr,degree));
    }
    if ((i == 3) && (j == 2))
    {
        return (greaterImplies(b,c,a,c,database_ptr,degree)
             && greaterImplies(c,b,c,a,database_ptr,degree)
             && greaterImplies(a,c,b,c,database_ptr,degree)
             && greaterImplies(c,a,c,b,database_ptr,degree));
    }
    if ((i == 2) && (j == 1))
    {
        return (greaterImplies(a,c,b,c,database_ptr,degree) && greaterImplies(c,a,b,a,database_ptr,degree));
    }
    if ((i == 3) && (j == 1))
    {
        return (greaterImplies(a,b,c,b,database_ptr,degree) && greaterImplies(b,a,c,a,database_ptr,degree));
    }
    
    printf("Error in deduceCondorcet: i = %d / j = %d\n",i,j);
    exit(0);

}

void DBtoLaws(DataBase *DBptr, Permutation Laws, int degree)
/* Laws has been initialised to an array of integers of length d(d-1(d-2)/6; d = degree.
 The procedure sets Laws[t] to an encoding of the set of laws satisfied in *DBptr on the triple t.
 The procedure is called in DBsize and elsewhere.
 */
{
    int t;
    int lex;
    int NoTrips = getNoTrips(degree);
    triple trip;
    pair law;
    
    for(t = 0; t < NoTrips; t++) Laws[t] = 0;
    
    for(t = 0; t < NoTrips; t++)
    {
        intToTriple(t, trip);
        for(lex = 0; lex < 6; lex++)
        {
            intToLaw(lex, law);
            if (deduceCondorcet(trip[0], trip[1], trip[2], law[0], law[1], DBptr, degree))
                Laws[t] |= 1 << lex;
        }
    }
}

int lawSatisfied(triple t, DataBase* database_ptr, int degree)
{
    int law = 0;
    int i;
    int j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i != j)
            {
                if (deduceCondorcet(t[0],t[1],t[2],i,j,database_ptr,degree))
                {
                    return law;
                }
                else
                {
                    law++;
                }
            }
        }
    }

    printf("Error in lawSatisfied: No law satisfied.\n");
    exit(0);
}

void readLine(FILE* file_ptr, DataBase* database_ptr, int degree)
{
    int a, b, c, i, j;
    
    fscanf(file_ptr," {%d,%d,%d} ",&a,&b,&c);
    printf("Reading {%d,%d,%d}\n",a,b,c);
    
    if (!((0 <= a) && (a<b) && (b<c) && (c<degree)))
    {
        printf("Bad input triple {%d, %d, %d}\n", a, b, c);
        exit(0);
    }
    
    while (fscanf(file_ptr, " [%dN%d] ", &i, &j) == 2)
    {
        if ((i < 1) || (j < 1) || (i>3) || (j>3))
        {
            printf("Bad input law [%dN%d]\n", i, j);
            exit(0);
        }
        
        insertCondorcet(a,b,c,i,j,database_ptr,degree);
    }
}

void readCondorcet(FILE* file_ptr, DataBase* database_ptr, int degree)
{
    while (!feof(file_ptr))
        readLine(file_ptr,database_ptr,degree);
}

void writeCondorcet(FILE* file_ptr, DataBase* database_ptr, int degree)
{
    int a,b,c,i,j;
    
    for (a = 0; a < degree - 2; a++)
        for (b = a + 1; b < degree - 1; b++)
            for (c = b + 1; c < degree; c++)
            {
                fprintf(file_ptr, "{%d,%d,%d} ",a,b,c);
                
                for (i = 1; i < 4; i++)
                    for (j = 1; j < 4; j++)
                        if ((i != j) && deduceCondorcet(a,b,c,i,j,database_ptr,degree))
                            fprintf(file_ptr, "[%dN%d] ",i,j);
                
                fprintf(file_ptr,"\n");
            }
}

void writeAlternating(char* filename, int degree)
{
    int a,b,c;
    FILE* file_ptr;
    
    if (!(file_ptr = fopen(filename,"w")))
    {
        printf("Could not open file for writeAlternating.\n");
        exit(0);
    }
    
    for (a = 0; a < degree; a++)
        for (b = a + 1; b < degree - 1; b++)
            for (c = b + 1; c < degree; c++)
            {
                fprintf(file_ptr, "{%d,%d,%d} ",a,b,c);
                fprintf(file_ptr, "[%dN%d]\n", 2,(b%2)? 1:3);
            }
    fclose(file_ptr);
}

void writeLaws(DataBase* database_ptr, int degree)
{
    int a,b,c,i,j;
    
    for (a = 0; a < degree - 2; a++)
        for (b = a + 1; b < degree - 1; b++)
            for (c = b + 1; c < degree; c++)
                for (i = 1; i < 4; i++)
                    for (j = 1; j < 4; j++)
                        if ((i != j) && deduceCondorcet(a,b,c,i,j,database_ptr,degree))
                            printf("(%d, %d, %d) %dN%d ",a, b, c, i, j);
                
    printf("\n");
}
