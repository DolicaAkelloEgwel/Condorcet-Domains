//
//  Expt3.h
//  
//
//  Created by C.R.Leedham-Green on 26/02/2018.
//
//

#ifndef Expt3_h
#define Expt3_h

int score(Permutation perm, int length); // For debugging purposes.

int isIn(int i, int n); // Is i-th triple (or pair) in the set of triples (or pairs) n?

int sizeCD(int *laws, int degree);
/*  Not now used.
 laws[t] is the set of laws satisfied by triple t.
 This set is replaced by its closure, and the size of the corresponding data base is returned.
 Here the size is the number of implications a > b -> c > d.
 To get the size of the Condorcet domain replace noPairs by size in the last line of the procedure. */

int card(int n);  /* popcount for ints */

int purify1(int *Laws, int degree);
/* Remove superfluous laws.  Return the resulting size. */

int purify2(Permutation Laws, Permutation Maws, int degree);
/* Laws is closed.  Set Laws[t] = 0 for all t for which more than
 one laws is imposed, and then fill these gaps optimally, but
 only imposing on the triple t laws in Maws[t].  These include
 all laws in Laws[t].  Return the size of
 */

int purify(Permutation Laws, int bound, int degree);

void killLaws(Permutation Laws, Permutation used, int degree);

int E3Size(Permutation Laws, int degree);

int E3RecSize(Permutation Laws, Permutation NewLaws, Permutation perm, Permutation used, int i, int degree);

void randomPerm(Permutation T, int length);
/* Sets T to a random permutation of length `length'.
 T has been initialised externally.  */


int randomSpecial(int t_int, DataBase* database_ptr, int degree, int *Laws, Permutation T);

int bestFill( int *Laws, int *Maws, int bound, int degree);
/* Returns the size of the largest UCD obtained by imposing a law from Maws on
 triples t for which Laws[t] == 0, and sets Laws to the corresponding set of laws.
 Laws imosed by implication will satisfy the above condition. 
 But gives up on optimality if it can get size >= bound.
 */

int recJump(Permutation Laws, Permutation Maws, int bound, int degree);
/* Called in Jump.  Laws is s set of laws that will be complete,
 and with Laws[t] != 0 for all t.  Maws is a copy of Laws, but
 with Maws[t] = 0 for some (perhaps empty) set of values of t.
 But bestFill sends Maws back to Laws.
 So set Maws[t] = 0 for another randomly chosen value of t.
 If bestFill now produces something new then Laws is set to this
 new set of laws, and the size of the resulting MUCD is returned.
 Otherwise, recurse.
 */

int jump(Permutation Laws, int bound, int degree);
/* Modify Laws to produce a different set of laws that define 
 an MUCD.  The change favours larger MUCD's.  */

int equalPerm(Permutation P, Permutation Q, int size);

void Alternating(Permutation Laws, int degree);
/* Produces the alternating scheme.*/

int MaxSize(int degree);
/* Returns the largest known size of an MUCD of the given degree.
 Requires degree <= 10.
 This is the size of the alternating scheme for degrees other than 8.  */

int Choose(int n, int i);

int setTripsToInt(Permutation P, int i, int length);
/* P is a an array of ints, of length length', and with exactly i terms set to 1.
 It represents a subset of [0,...,length-1] if size i.
 Return n where this is the n-th such i-tuple.  */


void intToSetOfTrips(Permutation P, int n, int k, int i, int length);
/* P is an array of ints of length `length', the first k having been set to
 0 or 1.  i further entries are to be set to 1, and the remainder to 0.
 The n-th such set of choices is to be made. */

int countCands(Permutation P, int length);
/* P is of length `length'.  Count the number of entries that are powers of 2. */


int testNull(Permutation Laws, Permutation Maws, Permutation Reps, Permutation Zeroes, int i, int n, int size, int degree);
/*  Maws is a copy of Laws.
 Reps and Zeroes are arrays of length size, with undefined entries.
 Take the n-th i-tuple (0 <= a_0 < a_1 < a_{i-1} < size), and
 for 0 <= j < i, set Maws[Reps[a_j]] = 0.
 Now form the closure of Maws.  If Maws is now equal to
 Laws return 0; else return 1.
 */

void getReps(Permutation P, Permutation Reps, int size, int length);
/* P is a permutation of length `length', with `size' entries
 being powers of 2.  Reps has been initialised as an array of length `size'.
 Set Reps[i] to be j, where P[j] is the i-th elelment of P that is a power of 2. */

int Jump2(Permutation Laws, int bound, int degree);



/*  IMPORTS  */
int popcount(UNINT n);  // From DBNextTriple.c
void initialisePermutation(Permutation* perm, int length); // From DBToPerm.c
void copyPemutation(Permutation* perm, int length);  // From DBToPerm.c
int getNoTrips(int degree);  // From DBNextTriple.c
void intToTriple(int t, triple trip);  // From ConvertTriple.c
int obeys(int a, int b, int c, int lex);  // From Database.c
void printPermutation(Permutation perm, int length);  // From DBToPerm.c
// Imports for randomSpecial have been omitted.


#endif /* Expt3_h */
