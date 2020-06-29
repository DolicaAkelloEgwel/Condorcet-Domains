/*
 A Condorcet domain of degree d is a set of orderings (or permutations) of
 {0,1,...,d-1} with the property that every triple obeys at least one Condorcet law.
 To find Condorcet domains of large size we construct a tree as follows.  Each
 node of the tree corresponds to a set of triples, and for each of these triples a law
 that is to be imposed on that triple.  So with each node we associate the set of
 orderings that obey these laws on these triples.  The leaves of the tree correspond to
 Condorcet domains.  Note that imposing Condorcet laws on a set of triples may impose
 Condorcet laws on tripls outside that set.  In order to reduce the size of the
 tree searched three conditions are (or may be) imosed, as follows:
 (a) Nodes that impose more than one law on any triple are rejected.
 (b) Nodes that corespond to a set of orderings of cardinal less than some cut-off point
 may be rejected.
 (c) When we choose a law that a triple should obey (as opposed to a law that has
 been enforced by laws on other triples) we may restrict the choice to some set of laws.
 
 An effect of (a) is that we may choose the triple to be processed at different
 nodes independently.
 
 makeCondorcet needs to construct Varray, and pass this as a parameter to PrepareCondorcetDomains,
 which is now in CondorcetLaws.c
 
 */

extern int mCD_trace;  /* From DBInputs.h */
extern int mCD_flag;  /* From DBInputs.h When this flag is set, in phase 1 we do not place the
                       DB in a hash table, but simply return when we hit depth t_0 */


// Generate Condorcet Domains of size at least k, and place them in a HashTable.
// Writes PermSet and Triple to a file  Now split into three functions, as below.
// Called by DBmakeCondorcet.
DBHashTable* DBmakeCondorcetDomains(int t_int, int t_0, DataBase* database_ptr, int degree, DBHashTable *dbht_ptr, int k, int phase, int file_no, unsigned int legal);

// Called by makeCondorcetDomains.  A leaf has been reached in Phase 1.
void DBmCD1(int t_int, DataBase* database_ptr, int degree);

int oneLaw(DataBase* db_ptr, int degree);

// A leaf has been reached in Phase 2.
DBHashTable *DBmCD2(DataBase* database_ptr, int degree, DBHashTable *dbht_ptr, int k);

// Is some law satisfied on this triple?  Returns 1 + first such law, or 0 if none is satisfied.
int DBsatisfiesLaw(DataBase* db_ptr, int t_int, int degree);

// Phase 1 or 2.  A leaf has not been reached.
DBHashTable *DBmCD3(int t_int, int t_0, DataBase* database_ptr, int degree,
                DBHashTable *dbht_ptr, int k, int phase, int legal, int file_no);

// Called by DBMain.  
DBHashTable* DBmakeCondorcet(DataBase* database_ptr, DBHashTable *dbht_ptr, unsigned int legal,
                   int degree, int t_0, int k, int phase, int file_no);

// Imported from DBHashTable
extern DBHashTable* insertDBHashTable(DBHashTable* dbht_ptr, DataBase* database_ptr, int degree);

// Imported from FileManagement
extern FILE* openFile(int i, int in, int write);

// Imported from DatabaseCondorcet.c
// Is the Law iNj, (a,b,c) satisfied by this data base? 1 <= i != j <= 3
int deduceCondorcet(int a, int b, int c, int i, int j, DataBase* database_ptr, int degree);

// Imports from DBNextTriple.c

// Sets law to (2,1)
extern void firstLegalPair(pair law);

//A pair (x,y) corresponds to a law xNy, 1 <= x,y <= 3.  Legal means `x != y'.
extern void nextLegalPair(pair p);

// Sets triple to (0,1,2);
extern void firstTriple(triple t);

// Sets triple to the next triple.  Sets t[0] = -1 if t is the last triple.
extern void nextTriple(triple t, int degree);

