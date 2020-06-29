/* A data base of degree d stores information of the form a>b and
 a>b -> u>v, where 0 <= a,b,u,v < d, and a != b and u!=v.
 When information of this type is added to the data base all
 inferences (that we can find) are added.  Procedures that add information
 return 1 if all goes well, and return 0 if a contradiction is obtained (so a>b and b>a
 are both deduced for some a and b).  Here and elsewhere a>b means `a always comes after b in some
 set of ordering of the integers 0, 1, ..., d-1.'  This set is the set of orderings that
 satisfy some set of Condorcet laws.
*/

extern void printPermutation(Permutation perm, int degree);

/* Initialises the field of a data base that stores implications of the form a>b -> c>d.
 Calls malloc, and sets a>b -> a>b for all a != b.  Other implications are set to 0. */
void initialisePairsDatabase(Pairs_DB* pairs_db_ptr, int degree);

/* Initilaises the field in a data base that stores the pairs (a,b) for which a>b
 is to be recorded.  These are all set to false (=0) at initialisation. */
void initialisePairsSet(PairsSet* pairs_ptr, int degree);

/* Calls the two previous procedures. */
void initialiseDataBase(DataBase* database_ptr, int degree);

/* Returns the integer k such that (i,j) is the k-th pair, where
 0 <= i != j < degree. */
int position(int i, int j, int degree);

/* returns 1 if the data base stores the information a>b -> c>d, otherwise 0.
 It allows for the possibility that the data base has stored the information a>b
 (or b>a) and has consequently set the bit strings for the consequences of a>b or of b>a
 to NULL.  In this case if a>b is stored a>b -> c>d is deemed to be stored iff c>d has been stored.
 Exported to DBtoPerm.c */
int greaterImplies(int a, int b, int c, int d, DataBase* database_ptr, int degree);

/* Writes in human readable form the implications a>b -> c>d that are stored. */
void writeImplies(DataBase* database_ptr, int degree);

/* Writes to a file in human readable form the implications a>b -> c>d that are stored. */
void fwriteImplies(FILE *f_ptr, DataBase* database_ptr, int degree);

void writeInLocal(int a, int b, int c, int d, DataBase* database_ptr, int degree);
/*Insert into the local data base the assertion: a > b -> c > d. */

int greater(int a, int b, DataBase* database_ptr, int degree);
/*Is `a > b' recorded in the data base?
 Exported to DBToPerm.c */

/* Writes in human readable form the implications a>b that are stored. */
void writeGreater(DataBase* database_ptr, int degree);

/* Writes to a file in human readable form the implications a>b that are stored. */
void fwriteGreater(FILE *f_ptr, DataBase* database_ptr, int degree);

/* Write into the global data base the assertion: a > b. */
void writeInGlobal(int a, int b, DataBase* database_ptr, int degree);

/* Insert `a > b' into the data base, if it is not there already, and draw consequences.
 Return 0 if this gives rise to a contradiction, otherwise return 1. */
int insertInGlobal(int a, int b, DataBase* database_ptr, int degree);

/* Insert `a > b -> c > d' into the data base, if it is not there already, and draw consequences.
 Return 0 if this gives rise to a contradiction, otherwise return 1. */
int insertInLocal(int a, int b, int c, int d, DataBase* database_ptr, int degree);

/* Are the bit strings for the consequences of a>b and of c>d the same bit string?
 We might have set them to be same if we have stored a>b -> c>d and c>d -> a>b; but this
 is not done at present.  */
int equated(int a, int b, int c, int d, DataBase* database_ptr, int degree);

/* Set the bit-string for the implications of c>d to NULL, because we have stored c>d or d>c */
void freeLocal(int c, int d, DataBase* database_ptr, int degree);

/* Not currently in use.  */
void equatePointers(int a, int b, int c, int d, DataBase* database_ptr, int degree);

/* Not currently in use. */
void ifAndOnlyIf(int a, int b, int c, int d, DataBase* database_ptr, int degree);

/* Returns the number of implications a>b -> c>d when (a,b) != (c,d). */
int noPairs(DataBase* database_ptr, int degree);

void copyDataBase(DataBase* old_database_ptr, DataBase* new_database_ptr, int degree);

void writeDataBase(DataBase* database_ptr, int degree);

int size(int* Laws, Permutation perm, Permutation used, int i, int degree);
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

int size2(int* Laws, Permutation perm, Permutation used, int i, int degree);
/*
 This has the same specifications as size; but should be faster.
*/

int isValid(int *Laws, Permutation perm, Permutation used, int i, int k, int degree);
/*
 For use in size2.  The parameters other than k have the same meaning.  Is it
 possible to set perm[i] = k without breaking a law or repeating a value in perm?
 */

int permit(int *Laws, Permutation used, triple trip);
/*
 For use in size2.  trip is a triple (a,b,c) with 0 <= a < b < c < degree.
 Laws tells us what laws are imposed on this triple, and used tells us where
 these entries arise in the permutation we are building.  See comment to size.
 */

int DBsize(DataBase *db_ptr, int degree);
/* Returns the size of the Condorcet Domain defined by the Condorcet laws implied by the data base.
 Warning.  Only the Condorcet laws are considered.  Local implications, for example, are NOT used
 for any purpose other than deciding if a given Condorcet Law is satisfied.  */

int obeys(int x, int y, int z, int lex);
/* If x,y,z occur in this order, is the law lex obeyed on this triple?
 For use in size. */

/* Return the next integer after i in DataBase, when this defines a unique permutation */
int findNext(DataBase* db_ptr, int i, int next, int degree);

/* Return the first integer in DataBase, when this defines a unique permutation */
int findFirst(DataBase* db_ptr, int degree);

/* Sets to NULL the bitstrings for the implications of a>b if a>b or b>a is stored.
 Not used, and not tested. */
/*void tidy(DataBase* database_ptr, int degree);*/

/* Returns the number of 64-bit words needed to hold degree times (degree-1) bits. */
int getDataBaseLength(int degree);

/* Do these two data bases contain the same information?
 Used in DBHashTable.c  */
int equalDataBases(DataBase* database_one_ptr, DataBase* database_two_ptr, int degree);

void fprintDataBase(DataBase* database_ptr, int degree, FILE* file_ptr);

void freadDataBase(DataBase* database_ptr, int degree, FILE* file_ptr);

int isEmpty(FILE *file_ptr);

void freeDataBase(DataBase* database_ptr, int degree);

void randomDB(DataBase* db_ptr, int local, int global, int seed, int degree);

/*
int barred(int i, int j);
 */
/* Returns i == j  Used in completePerm */

/*
Permutation completePerm(Permutation p, Permutation used, int i, int degree);
 */

/* Imports */

extern void copyPermutation(Permutation orig_perm, Permutation copy_perm, int degree);  // From DBToPerm.c

extern void initialisePermutation(Permutation *perm_ptr, int degree);  // From DBToPerm.c

extern void DBtoLaws(DataBase *DBptr, Permutation Laws, int degree);  // From DatabaseCondorcet.c

extern int getNoTrips(int degree);  // From LawsToPerms.c

extern void sort(triple t); // From DBNextTriple.c

extern int isIn(int lex, int laws);  // From LawsToPerms.c

extern int tripleToInt(triple t);  // From LawsToPerms.c
