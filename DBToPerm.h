/* The purpose of this file is to construct the procedure dbToPerm, which
 takes as inut a pointer to a database db, and the degree, and returns the set
 of permutations that satisfy the conditions contained in db as a linked list.
 It is assumed that db does NOT contain the condition 0 > 1, as it is presumed to
 contain the identity permutation.  If db does contain the condition 0 > 1 a warning is
 issued, and the program halts.  This restriction can easily be removed, as we now
 explain.  dbToPerm starts with a set of one or two permutations of degree 2, being (of course)
 (0,1) and (1,0) if 1 > 0 is NOT contained in db, but omitting (1,0) if 1 > 0 IS contained in db.
 If the condition that db must allow the identity permutation is to be removed then this
 initial set of permutations is to be subset of {(0,1), (1,0)} that obey any of the laws 1 > 0 or 0 > 1
 that are contained in db.  If both of these laws are contained then dbToPerm should return the empty set.
 dbToPerm, having constructed this initial small permutation set constructs recursively permutations
 of degrees 3 to degree by inserting the elements 2 to degree - 1 in turn wherever they will go
 without violating the laws in db. */

void addEntry(Permutation *new_perm_ptr, Permutation old_perm, int i, int d);
/* addEntry takes as input a permutation of degree d-1, and creates a new
 permutation *new_perm_ptr by inserting d-1 between entries i-1 and i. */

int isAllowed(DataBase* db_ptr, Permutation perm, int i, int d, int degree);
/* The permutation perm of degree d < degree obeys the conditions stored in *db_ptr.
 Will it continue to do so if we insert d between entries i-1 and i? (0 <= i <= d).
 Note a discrepancy of 1 between the meaning of d here and in addEntry.
 */

void initialisePermSet(SmallPermset** sps_ptr_ptr, DataBase *db_ptr, int degree);
/* Create a small permutation set consisting of the permutations (0,1),
 and, if the database does not require 1 > 0, the permutation (1,0).
 It must not require 0 > 1.  See preamble.
 */

void printPermutation(Permutation perm, int degree);

void printSmallPermset(SmallPermset* sps_ptr, int degree);

void permInSmallPermset(SmallPermset** sps_ptr_ptr, Permutation perm, int degree);
/* Inserts perm at the end of ** sps_ptr_ptr. */

void expand(SmallPermset** new_sps_ptr_ptr, Permutation perm, int d, DataBase* db_ptr, int degree);
/* perm is a permutation of degree d-1, 3 <= d <= degree.
 Insert d-1 in all slots allowed by the data base *db_ptr, and add the resulting permutations of degree d
 to the small permutation set **new_sps_ptr_ptr.  */

void initialisePermutation(Permutation *perm_ptr, int degree);
/* Uses malloc to create storage.  Does not assign any entries. */

void copyPermutation(Permutation orig_perm, Permutation copy_perm, int degree);

SmallPermset smallPermsetEntry(Permutation perm, int degree);
/* Creates a cell with entry the permutation for use in a linked list. */

void freeSPS(SmallPermset *sps_ptr);

SmallPermset* dbToPerm(DataBase *db_ptr, int degree);
/* The principal procedure for this file.  See preamble.*/

/* Imports from Database.c. */

int greater(int a, int b, DataBase* database_ptr, int degree);
/*Is `a > b' recorded in the data base? */

/* returns 1 if the data base stores the information a>b -> c>d, otherwise 0.
 It allows for the possibility that the data base has stored the information a>b
 (or b>a) and has consequently set the bit strings for the consequences of a>b or of b>a
 to NULL.  In this case if a>b is stored a>b -> c>d is demed to be stored iff c>d has ben stored. */
int greaterImplies(int a, int b, int c, int d, DataBase* database_ptr, int degree);


