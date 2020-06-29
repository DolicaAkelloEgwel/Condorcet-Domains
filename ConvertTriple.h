int tripleToInt(triple t); // A triple (a,b,c) with 0 <= a < b < c is mapped to
// a + (b choose 2) + (c choose 3), so triples occur in the order
// (0,1,2), (0,1,3), (0,2,3), (0,1,4), (0,2,4), (1,2,4), ...

void intToTriple(int n, triple t);  //Inverse of tripleToInt

void intToLaw(int i, pair law);  // There are six laws xNy, 1 <= x <> y <= 3

void randomPermutation(Permutation A, int d);
// Enter the integers 0 to d-1 in a random order in the initialised
// array A of length d.  Used in randomDBmCD3.  In wrong file.

int randomDBmCD3(int t_int, DataBase* database_ptr, int degree, int work, Permutation T);
// In wrong file.
/*  randomDBmCD3 is called when we need to advance to the next triple t_int.
 If a law is already satisfied for t_int then call DBmakeCondorcetDomains with t_int incremented by 1.
 Otherwise, for each law, impose that law on the triple t_int, and call randomDBmCD#
 with t_int incremented by 1 if the size of the new perm set is at least k, and no triple satisfies 2 laws.  The point here is that the laws are chosen in a random order, independently,
 for each node in the tree. The value `work' that is returned counts the total number
 of nodes constructed.  */
