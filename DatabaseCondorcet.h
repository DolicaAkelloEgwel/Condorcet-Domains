/* Code for procesing a set of Laws that are to be read from a file.
 
 All triples (a,b,c) are expected to satisfy 0 <= a < b < c < degree.  */

/* Insert into the data base the Law iNj, (a,b,c). Return 1 if this proves consistent. */
int insertCondorcet(int a, int b, int c, int i, int j, DataBase* database_ptr, int degree);

/* Is the Law iNj, (a,b,c) satisfied by this data base? 1 <= i != j <=3 */
int deduceCondorcet(int a, int b, int c, int i, int j, DataBase* database_ptr, int degree);

void DBtoLaws(DataBase *DBptr, Permutation Laws, int degree);
/* Laws has been initialised to an array of integers of length d(d-1(d-2)/6; d = degree;
 with the entries set to 0
 The procedure sets Laws[t] to an encoding of the set of laws satisfied in *DBptr on the triple t.
 The procedure is called in DBsize.
 */

/*
 The implications stored in DataBase imply that some law is satified on the triple t.
 The first such law is returned.
 */
int lawSatisfied(triple t, DataBase* database_ptr, int degree);

/* Read a Law from the file and insert it in the data base. 
 The consequences of inserting this law will then be entered. */
void readLine(FILE* file_ptr, DataBase* database_ptr, int degree);

/* Read a set of Laws from the file, and insert them in the data base, 
 the consequences being deduced and added.  */
void readCondorcet(FILE* file_ptr, DataBase* database_ptr, int degree);

/* For every triple (a,b,c) write to file the set of laws iNj that are obeyed
 by the data base. */
void writeCondorcet(FILE* file_ptr, DataBase* database_ptr, int degree);

/* Write the alternating scheme to a file. */
void writeAlternating(char* filename, int degree);

/* Write the Condorcet laws satisfied by this database. */
void writeLaws(DataBase* database_ptr, int degree);

/* Imports */
void intToLaw(int i, pair law);  // From ConvertTriple.c

void intToTriple(int n, triple t);  // From ConvertTriple.c
