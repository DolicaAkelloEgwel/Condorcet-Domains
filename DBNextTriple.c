/**************************************************************************
* FILENAME:                                                               *
*   CondorcetLaws.c                                                       *
*                                                                         *
* PURPOSE:                                                                *
*   Implements methods defined in CondorcetLaws.h                         *
*                                                                         *
* GLOBALS USED:                                                           *
*   N_LAWS - Number of Laws                                               *
*                                                                         *
**************************************************************************/
int isMaxFlag = 0;

/*
* Name:
*   getNoTrips(degree)
*
* Parameters:
*   degree - Permutation degree
*
* Return:
*   Number of triples for a given degree: degree-choose-3
*/
int getNoTrips(int degree)
{
	return degree * (degree -1) * (degree -2) / 6;
}

/*
* Name:
*   firstTriple(t)
*
* Parameters:
*   t - A triple
*
* Description:
*   Sets the triple to the first triple for all degrees: {0,1,2}
*/
void firstTriple(triple t)
{
	t[0] = 0;
	t[1] = 1;
	t[2] = 2;
}

void nextTriple(triple t, int degree)
{
    if (degree == 8) nextTripleEight(t);
    else if (degree == 9) nextTripleNineB(t);
    else nextTripleNot(t, degree);
}

/*
* Name:
*   nextTripleNot(t,degree)
*
* Parameters:
*   t - A triple
*   degree should not be 8.  It is sometimes called in nextTripleNine.
*
* Description:
*   Sets t to the next triple
*   Sets t[0] to -1 if t is the last triple.
*/
void nextTripleNot(triple t, int degree)
{
	if (t[0] == degree - 3)  // last triple
    {
        t[0] = -1;
        return;
    }

	if (t[0] < t[1] - 1)
	{
		t[0]++;
		return;
	}

	if (t[1] < t[2] - 1)
	{
		t[0] = 0;
		t[1]++;
		return;
	}

	if (t[2] < degree - 1)
	{
		t[0] = 0;
		t[1] = 1;
		t[2]++;
		return;
	}

    printf("Error in nextTriple\n");
    exit(0);
}

/*
int listLaws(int* arr, DataBase* database_ptr, int degree)

 Mallocs arr to an array of length the number of triples, and sets arr[t]
 to the first law satisfied by this triple and returns 1, except that if
 arr[t] is set to a value less than arr[0] then the process halts and 0 is returned.
 

{
    int n_trips = getNoTrips(degree);
    int i;
    triple t;
    arr = malloc(n_trips * sizeof(int));

    for (i = 0, firstTriple(t); i < n_trips; i++, nextTriple(t, degree))
    {
        arr[i] = lawSatisfied(t,database_ptr,degree);

        if (i && arr[i] < arr[0])
        {
            free(arr);
            return 0;
        }
    }

    return 1;
}
 
 */

/*
 * Name:
 *   nextTripleEight(t)
 *
 * Parameters:
 *   t - A triple

 *
 * Description:
 *   degree is eight
 *   Sets t to the next triple according to ordering provided by Klas Markstrom
 *   Error if t is the last triple (5,6,7).
 */

void nextTripleEight(triple t)
{
    if ((t[0] == 4) && (t[1] == 5) && (t[2] == 6)) // last triple
    {
        t[0] = -1;
        return;
    }

    switch(t[0]) {
        case 0:
            switch(t[1]) {
                case 1:
                    switch(t[2]) {
                        case 2: t[0] = 3; t[1] = 4; t[2] = 5; return;
                        case 3: t[0] = 2; t[1] = 4; t[2] = 5; return;
                        case 4: t[0] = 3; t[1] = 5; t[2] = 6; return;
                        case 5: t[0] = 4; t[1] = 6; t[2] = 7; return;
                        case 6: t[0] = 2; t[1] = 3; t[2] = 7; return;
                        case 7: t[0] = 4; t[1] = 5; t[2] = 6; return;
                    }
                case 2:
                    switch(t[2]) {
                        case 3: t[0] = 1; t[1] = 4; t[2] = 7; return;
                        case 4: t[0] = 5; t[1] = 6; t[2] = 7; return;
                        case 5: t[0] = 3; t[1] = 4; t[2] = 7; return;
                        case 6: t[0] = 1; t[1] = 4; t[2] = 5; return;
                        case 7: t[0] = 1; t[1] = 5; t[2] = 6; return;
                    }
                case 3:
                    switch(t[2]) {
                        case 4: t[0] = 1; t[1] = 5; t[2] = 7; return;
                        case 5: t[0] = 1; t[1] = 2; t[2] = 7; return;
                        case 6: t[0] = 4; t[1] = 5; t[2] = 7; return;
                        case 7: t[0] = 1; t[1] = 2; t[2] = 5; return;
                    }
                case 4:
                    switch(t[2]) {
                        case 5: t[0] = 1; t[1] = 3; t[2] = 7; return;
                        case 6: t[0] = 2; t[1] = 5; t[2] = 7; return;
                        case 7: t[0] = 1; t[1] = 3; t[2] = 5; return;
                    }
                case 5:
                    switch(t[2]) {
                        case 6: t[0] = 2; t[1] = 4; t[2] = 7; return;
                        case 7: t[0] = 2; t[1] = 4; t[2] = 6; return;
                    }
                case 6: t[0] = 1; t[1] = 2; t[2] = 3; return;
            }
        case 1:
            switch(t[1]) {
                case 2:
                    switch(t[2]) {
                        case 3: t[0] = 0; t[1] = 4; t[2] = 7; return;
                        case 4: t[0] = 0; t[1] = 6; t[2] = 7; return;
                        case 5: t[0] = 3; t[1] = 6; t[2] = 7; return;
                        case 6: t[0] = 0; t[1] = 3; t[2] = 4; return;
                        case 7: t[0] = 0; t[1] = 4; t[2] = 6; return;
                    }
                case 3:
                    switch(t[2]) {
                        case 4: t[0] = 2; t[1] = 5; t[2] = 6; return;
                        case 5: t[0] = 0; t[1] = 2; t[2] = 7; return;
                        case 6: t[0] = 0; t[1] = 4; t[2] = 5; return;
                        case 7: t[0] = 0; t[1] = 2; t[2] = 6; return;
                    }
                case 4:
                    switch(t[2]) {
                        case 5: t[0] = 2; t[1] = 3; t[2] = 6; return;
                        case 6: t[0] = 3; t[1] = 5; t[2] = 7; return;
                        case 7: t[0] = 2; t[1] = 3; t[2] = 5; return;
                    }
                case 5:
                    switch(t[2]) {
                        case 6: t[0] = 2; t[1] = 3; t[2] = 4; return;
                        case 7: t[0] = 3; t[1] = 4; t[2] = 6; return;
                    }
                case 6: t[0] = 0; t[1] = 2; t[2] = 5; return;
            }
        case 2:
            switch(t[1]) {
                case 3:
                    switch(t[2]) {
                        case 4: t[0] = 1; t[1] = 6; t[2] = 7; return;
                        case 5: t[0] = 1; t[1] = 4; t[2] = 6; return;
                        case 6: t[0] = 0; t[1] = 1; t[2] = 7; return;
                        case 7: t[0] = 0; t[1] = 5; t[2] = 6; return;
                    }
                case 4:
                    switch(t[2]) {
                        case 5: t[0] = 0; t[1] = 3; t[2] = 7; return;
                        case 6: t[0] = 0; t[1] = 3; t[2] = 5; return;
                        case 7: t[0] = 0; t[1] = 1; t[2] = 5; return;
                    }
                case 5:
                    switch(t[2]) {
                        case 6: t[0] = 0; t[1] = 1; t[2] = 4; return;
                        case 7: t[0] = 0; t[1] = 3; t[2] = 6; return;
                    }
                case 6: t[0] = 1; t[1] = 3; t[2] = 4; return;
            }
        case 3:
            switch(t[1]) {
                case 4:
                    switch(t[2]) {
                        case 5: t[0] = 2; t[1] = 6; t[2] = 7; return;
                        case 6: t[0] = 0; t[1] = 5; t[2] = 7; return;
                        case 7: t[0] = 0; t[1] = 1; t[2] = 6; return;
                    }
                case 5:
                    switch(t[2]) {
                        case 6: t[0] = 1; t[1] = 2; t[2] = 4; return;
                        case 7: t[0] = 1; t[1] = 2; t[2] = 6; return;
                    }
                case 6: t[0] = 0; t[1] = 2; t[2] = 4; return;
            }
        case 4:
            switch(t[1]) {
                case 5:
                    switch(t[2]) {
                        case 6: printf("nextTripleEight called on last triple\n"); exit(0);
                        case 7: t[0] = 1; t[1] = 3; t[2] = 6; return;
                    }
                case 6: t[0] = 0; t[1] = 1; t[2] = 3; return;
            }
        case 5:
            t[0] = 0; t[1] = 2; t[2] = 3; return;
    }
    printf("Error in nextTripleEight\n");
    exit(0);
}


void sort(triple t)
{
    int s;
    if (t[0] > t[1])
    {
        s = t[0];
        t[0] = t[1];
        t[1] = s;
    }
    if (t[1] > t[2])
    {
        s = t[1];
        t[1] = t[2];
        t[2] = s;
    }
    if (t[0] > t[1])
    {
        s = t[0];
        t[0] = t[1];
        t[1] = s;
    }
}

void nextTripleNineA(triple t)
{
    switch(t[0])
    {
        case 0:
            if ((t[1] == 1) && (t[2] == 2))
            {
                t[0] = 3;
                t[1] = 4;
                t[2] = 7;
                return;
            }
            if ((t[1] == 4) && (t[2] == 6))
            {
                t[0] = 2;
                t[1] = 6;
                t[2] = 7;
                return;
            }
            if ((t[1] == 5) && (t[2] == 7))
            {
                t[1] = 3;
                t[2] = 8;
                return;
            }
            if ((t[1] == 3) && (t[2] == 8))
            {
                t[0] = 1;
                t[1] = 4;
                t[2] = 5;
                return;
            }
            if ((t[1] == 1) && (t[2] == 2))
            {
                t[0] = 3;
                t[1] = 4;
                t[2] = 7;
                return;
            }
            break;
        case 1:
            if ((t[1] == 3) && (t[2] == 6))
            {
                t[0] = 0;
                t[1] = 5;
                t[2] = 7;
                return;
            }
            if ((t[1] == 4) && (t[2] == 5))
            {
                t[0] = 5;
                t[1] = 6;
                t[2] = 8;
                return;
            }
            if ((t[1] == 7) && (t[2] == 8)) // last of first twelve
            {
                t[0] = 1;
                t[1] = 2;
                t[2] = 3;
                return;  // first of second twelve
            }
            break;
        case 2:
            if ((t[1] == 3) && (t[2] == 5))
            {
                t[0] = 1;
                t[1] = 7;
                t[2] = 8;
                return;
            }
            if ((t[1] == 6) && (t[2] == 7))
            {
                t[1] = 4;
                t[2] = 8;
                return;
            }
            if ((t[1] == 4) && (t[2] == 8))
            {
                t[0] = 2;
                t[1] = 3;
                t[2] = 5;
                return;
            }
            break;
        case 3:
            if ((t[1] == 4) && (t[2] == 7))
            {
                t[0] = 1;
                t[1] = 3;
                t[2] = 6;
                return;
            }
            break;
        case 5:
            if ((t[1] == 6) && (t[2] == 8))
            {
                t[0] = 0;
                t[1] = 4;
                t[2] = 6;
                return;
            }
            break;
    }
    t[0] = -1;  // Triple not in the first twelve
}

void nextTripleNineB(triple t)
// Find the next triple; or set t[0] = -1 if t is the last
{
    int i;
    int j;
    int t0 = t[0];

    if ((t[0] == 0) && (t[1] == 7) && (t[2] == 8)) // last triple
    {
        t[0] = -1;
        return;
    }

    for(i=0; i<7; i++)
    {
        nextTripleNineA(t);

        if (t[0] != -1)
        {
            if (i)
            {
                for (j=0; j<3; j++)
                {
                    if (t[j] < 7)
                    {
                        t[j] = (t[j] + i) % 7; // entries equal to 7 and 8 are fixed
                    }
                }
                sort(t);
            }
            return;
        }

        t[0] = t0;  // restore to original value
        for (j=0; j<3; j++)
        {
            if (t[j] < 7)
            {
                t[j] = (t[j] + 6) % 7; // entries equal to 7 and 8 are fixed
            }
        }
        sort(t);
        t0 = t[0];
    }
    printf("Error in NexTripleNineB (%d, %d, %d)\n", t[0], t[1], t[2]);
    exit(0);
}

/*
 * Name:
 *   nextTripleNine(t)  Now obsolete.
 *
 * Parameters:
 *   t - A triple

 *
 * Description:
 *   degree is nine
 *   Sets t to the next triple according to ordering provided by Klas Markstrom
 *   Error if the last triple (6,7,8) has been reached
 */

void nextTripleNine(triple t)
{
    if ((t[0] == 5) && (t[1] == 7) && (t[2] == 8))  // last triple
    {
        t[0] = -1;
        return;
    }
    /* This is because, under the default mechanism, (5,7,8) -> (6,7,8).
     But (6,7,8) lies in the initial Steiner triple system.  */

    if ((t[0] == 0) && (t[1] == 1) && (t[2] == 2))
    {
        t[0] = 3;
        t[1] = 4;
        t[2] = 5;
        return;
    }
    if ((t[0] == 3) && (t[1] == 4) && (t[2] == 5))
    {
        t[0] = 6;
        t[1] = 7;
        t[2] = 8;
        return;
    }
    if ((t[0] == 6) && (t[1] == 7) && (t[2] == 8))
    {
        t[0] = 0;
        t[1] = 3;
        t[2] = 6;
        return;
    }
    if ((t[0] == 0) && (t[1] == 3) && (t[2] == 6))
    {
        t[0] = 1;
        t[1] = 4;
        t[2] = 7;
        return;
    }
    if ((t[0] == 1) && (t[1] == 4) && (t[2] == 7))
    {
        t[0] = 2;
        t[1] = 5;
        t[2] = 8;
        return;
    }
    if ((t[0] == 2) && (t[1] == 5) && (t[2] == 8))
    {
        t[0] = 0;
        t[1] = 4;
        t[2] = 8;
        return;
    }
    if ((t[0] == 0) && (t[1] == 4) && (t[2] == 8))
    {
        t[0] = 1;
        t[1] = 5;
        t[2] = 6;
        return;
    }
    if ((t[0] == 1) && (t[1] == 5) && (t[2] == 6))
    {
        t[0] = 2;
        t[1] = 3;
        t[2] = 7;
        return;
    }
    if ((t[0] == 2) && (t[1] == 3) && (t[2] == 7))
    {
        t[0] = 0;
        t[1] = 5;
        t[2] = 7;
        return;
    }
    if ((t[0] == 0) && (t[1] == 5) && (t[2] == 7))
    {
        t[0] = 1;
        t[1] = 3;
        t[2] = 8;
        return;
    }
    if ((t[0] == 1) && (t[1] == 3) && (t[2] == 8))
    {
        t[0] = 2;
        t[1] = 4;
        t[2] = 6;
        return;
    }
    if ((t[0] == 2) && (t[1] == 4) && (t[2] == 6))
    {
        t[0] = 0;
        t[1] = 1;
        t[2] = 3;
        return;
    }

    /* These are the initial triples; Klas' Steiner triple system.  The last of these, namely (2,4,6),
    being mapped to the first default triple, namely (0,1,3).
    Now we make the triples that would map to these in our generic ordering skip over these triples. */

    if ((t[0] == 2) && (t[1] == 4) && (t[2] == 5))
    {
        t[0] = 0;
        t[1] = 1;
        t[2] = 6;
        return;
    }
    if ((t[0] == 1) && (t[1] == 2) && (t[2] == 6))
    {
        t[1] = 3;
        return;
    }
    if ((t[0] == 0) && (t[1] == 4) && (t[2] == 7))
    {
        t[0] = 2;
        return;
    }
    if ((t[0] == 1) && (t[1] == 5) && (t[2] == 8))
    {
        t[0] = 3;
        return;
    }
    if ((t[0] == 2) && (t[1] == 3) && (t[2] == 8))
    {
        t[0] = 1;
        t[1] = 4;
        return;
    }
    if ((t[0] == 0) && (t[1] == 5) && (t[2] == 6))
    {
        t[0] = 2;
        return;
    }
    if ((t[0] == 1) && (t[1] == 3) && (t[2] == 7))
    {
        t[0] = 0;
        t[1] = 4;
        return;
    }
    if ((t[0] == 3) && (t[1] == 4) && (t[2] == 7))
    {
        t[0] = 1;
        t[1] = 5;
        return;
    }
    if ((t[0] == 0) && (t[1] == 3) && (t[2] == 8))
    {
        t[0] = 2;
        return;
    }
    if ((t[0] == 1) && (t[1] == 4) && (t[2] == 6))
    {
        t[0] = 3;
        return;
    }
    /* The remaining cases are dealt with by the default mechanism.
     Note that (5,7,8) is now the last triple.  */

    nextTripleNot(t, 9);  // nextTripleNot should detect bad input.
}



/*
 * Name:
*   getLawPos(lex, a)
*
* Parameters:
*   lex - An integer representation of a law in the range 0 to 8
*   They come in the following order, with the returned value
*   when a is 2 + 2^2 + 2^3 + 2^5 + 2^6 + 2^7, so all UCD laws, below
* 0   1   2   3   4   5   6   7   8
* 0N0 0N1 0N2 1N0 1N1 1N2 2N0 2N1 2N2
* x   0   1   2   x   3   4   5   x
* a encodes the allowed laws, as above.
*/
int getLawPos(int lex, unsigned int a)
{
    int i;
    int law = 0;

    if (!(a & (1 << lex)))
    {
        printf("getLawPos called with illegal lex = %d\n", lex);
        exit(0);
    }
    for (i = 0; i < lex; i++)
    {
        if (a & (1 << i)) law++;
    }
    return law;
}

/*
 * Name:
 *   printTriple(t)
 *
 * Parameters:
 *   t - A triple
 *
 * Description:
 *   Prints the triple t
 */
void printTriple(triple t)
{
	int i; /* Loop counter */

	for (i = 0; i < 3; i++)
	{
		printf("%d ",t[i]);
	}

	printf("\n");
}

/*
* Name:
*   printLawText(lex)
*
* Parameters:
*   lex - A law-integer
*
* Description:
*   Prints the law in text form, law in range 0 to 8
*   Used for testing of MakeCondorcetLaws()
*/
void printLawText(int lex)
{
    switch(lex)
	{
		case 1: // xN1 (xIn0 OR xIn2)
		{
			printf("xN1 ");
			return;
		}
		case 2: // xN2 (xIn0 OR xIn1)
		{
			printf("xN2 ");
			return;
		}
		case 3: // yN0 (yIn1 OR yIn2)
		{
			printf("yN0 ");
			return;
		}
		case 5: // yN2 (yIn0 or yIn1)
		{
			printf("yN2 ");
			return;
		}
		case 6: // zN0 (zIn1 OR zIn2)
		{
			printf("zN0 ");
			return;
		}
		case 7: // zN1 (zIn0 or zIn2)
		{
		    printf("zN1 ");
		    return;
		}
		default:
		{
			printf("Error in printLawText: %d",lex);
			exit(0);
		}
	}
}

void firstPair(pair p)
{
    p[0] = 1;
    p[1] = 1;
}

void firstLegalPair(pair p)
{
    p[0] = 2;
    p[1] = 1;
}

void nextPair(pair p)
{
    if (p[0] == -1)
    {
        // printf("Error in nextPair: Called on last pair.\n");
        return;
    }

    if ((p[0] == 3) && (p[1] == 3))
    {
        p[0] = -1;
        return;
    }

	if (p[0] == 3)
    {
        p[1]++;
        p[0] = 1;
        return;
    }

    p[0]++;
}

void nextLegalPair(pair p)
{
    nextPair(p);

    if (p[0] == p[1])
        nextPair(p);
}

int popcount(UNINT n)
{
	n = (((n>>1)  &0x5555555555555555) + (n &0x5555555555555555));
	n = (((n>>2)  &0x3333333333333333) + (n &0x3333333333333333));
	n = (((n>>4)  &0x0F0F0F0F0F0F0F0F) + (n &0x0F0F0F0F0F0F0F0F));
	n = (((n>>8)  &0x00FF00FF00FF00FF) + (n &0x00FF00FF00FF00FF));
	n = (((n>>16) &0x0000FFFF0000FFFF) + (n &0x0000FFFF0000FFFF));
    // n = (((n>>32) &0x00000000FFFFFFFF) + (n &0x00000000FFFFFFFF));
	return (int) n;
}

