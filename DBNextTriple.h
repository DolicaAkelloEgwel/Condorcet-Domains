/**************************************************************************
* FILENAME:                                                               *
*   DBNextTriple.h                                                        *
*                                                                         *
* PURPOSE:                                                                *
*   Defines triple type                                                   *
*   Defines CondorcetLaws type - 2D array of PermSets                     *
*   Defines methods for interacting with CondorcetLaws and Triples        *
*                                                                         *
**************************************************************************/

/*  Changes to be made in allowing any set of laws.
 In PrepareCodorcetLaws we have 
 if (lex % 4 != 0)
 Here lex is in the range 0 to 8, but 0, 4, 8 correspond to invalid laws.
 We need a procedure isValid(lex, Varray), where Varray is an array of length
 9, set up at the beginning of the run in MakeCondorcet, and passed to PrepareCondorcetLaws as a parameter.
 
 We have isSatisfied and altIsSatisfied.  The former assumes only that we have the number
 of the permutation.  The latter assumes that this permutation and its inverse have already
 been constructed, as permutations, and takes these permutations as parameters. 
 Clearly the alt version is faster where applicable.  Is isSatisfied used?  Should it be?
 
 (Alt)isSatisfied should deal with all nine laws, just in case we need them.
 
 N_LAWS will be set up at the same time as Varray.
 */
// The following two procedures set up space for the Condorcet Laws tables, and insert the
// required values, respectively.  The number of laws allowed has been encoded in the global N_LAWS.
// The actual laws that are allowed are encoded in the parameter a to PrepareCondorcetLaws.

// Find the number of triples for a particular degree.
// Exported to MakeCondorcetDomains.c and to tests.c
int getNoTrips(int degree);

// Find the first triple
// Exported to MakeCondorcetDomains.c
void firstTriple(triple t);

// Find the next triple
// Exported to MakeCondorcetDomains.c
void nextTriple(triple t, int degree);

// Find next triple when degree is 8.
void nextTripleEight(triple t);

/* NextTripleNineB orders the 84 triples to go through seven disjoint Steiner systems in sequence, following
 Donald Quiring  A consruction of disjoint Steiner triple systems.
 Journal of Combinatorial Theory, Series A. Volume 27, Issue 3, November 1979, Pages 407-408
 This paper gives a partition of the set of triples of nine elements into
 seven disjoint Steiner triple systems.  We use this partition, starting with the system
 that contains (0,1,2) so that this remains the first triple. */

void sort(triple t);
/* For use in NextTripleNineB */

void nextTripleNineA(triple t);
/* Sets t to the next triple if t lies in the first Steiner triple system; else sets t[0] = - 1; */

void nextTripleNineB(triple t); /* Sets t to the next triple, or of t is the last triple sets t[0] = -1; */

// Find next triple when degree is 9.  Now obsolete.
void nextTripleNine(triple t);

// Find next triple when degree is not 8.  Sometimes called when degree is 9.
void nextTripleNot(triple t, int degree);

/*
int listLaws(int *A, DataBase* database_ptr, int degree);
 */

// Find the position in the Condorcet Laws array that corresponds with a certain law
//  Exported to MakeCondorcetDomains.c
int getLawPos(int lex, unsigned int a);

// Print a triple
// Exported to MakeCondorcetDomains.c
void printTriple(triple t);

// Print a law in the form of text
// Exported to MakeCondorcetDomains.c
void printLawText(int lex);

/* The total number of laws that can be applied to a triple */
// Initialised in main.c and in alt_main.c
int N_LAWS;

/* Sets (i,j) to (1,1) */
void firstPair(pair p);

void firstLegalPair(pair p);

int popcount(UNINT n);

/* Sets (i,j)) to the next pair; 1 <= i,j <= 3. */
void nextPair(pair p);

void nextLegalPair(pair p);
