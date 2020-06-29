//
//  DBMain.c
//
//
//  Created by C.R.Leedham-Green on 10/12/2016.
//
//

#include "DBInputs.h"
extern int SafeToPrint; // Initialised in main.  Set to 0 in Phase 2 for parallel processing.

/* Main has seven arguments.
 1.  degree.  The degree of the Condorcet domains to be constructed.
 2.  t_0. In Phases 1 and 2, t_0 is the depth of the tree searched in Phase 1.
          In Phase 3 this is the number n, where Phase 1 has constructed files in_0 to in_n-1,
          and Phase 2 has constructed files ou_0 to ou_n-1.
 3.  hashSize.  The original size of the hash table.  Must be an odd integer.
 4.  k. In Phases 1 and 2 k is the cut-off.  Domains of size less than k are rejected.
     In Phase 3, process MUCD's of size k.
 5.  Phase is 1,2,or 3.
 6.  file_no  In Phase 1 this is irrelevant.  Phase 1 creates a number if in files,
     and file_no instructs an invocation for Phase 2 to start with that in file.
     In Phase 3 this is an integer for naming the FINAL files for this run.
 7.  a encodes the set of laws that we allow.
 See Read_Me.c for more detail.
 */

int main(int argc, char *argv[])
{
    int degree, t_0, hashSize, k, phase, file_no;
    unsigned int a;
    UNINT w;
    DataBase database;
    DBHashTable* dbht_ptr;
    extern int N_LAWS;
    extern int SafeToPrint;

    if (argc != 8)
    {
        printf("There should be 7 arguments.\n");
        exit(0);
    }
    else
    {
        degree = atoi(argv[1]);
        t_0 = atoi(argv[2]);
        hashSize = atoi(argv[3]);
        k = atoi(argv[4]);
        phase = atoi(argv[5]);
        file_no = atoi(argv[6]);
        a = atoi(argv[7]);
    }

    w = a;
    N_LAWS = popcount(w);
    printf("N_LAWS set to %d\n", N_LAWS);
    if ((N_LAWS > 6) || (a > 238))
    {
        printf("Bad value for last parameter a = %d", a);
        exit(0);
    }

    if ((phase < 3) && (t_0 > (degree * (degree-1) * (degree-2))/6 -1))
    {
        printf("t_0 = %d is too big.\n",t_0);
        exit(0);
    }

    if ((phase < 1) || (phase > 3))
    {
        printf("Bad value for phase = %d\n", phase);
        exit(0);
    }

    SafeToPrint = (phase == 2) ? 0 : 1;  // Safe if phase !=2

    if (phase == 3)  {
        // phase3(degree, hashSize, t_0, file_no, k, a);  // t_0 is no. of files, file_no is the user supplied name, k is the CD size.
        return 1;
    }

    initialiseDataBase(&database,degree);
    dbht_ptr = initialiseDBHashTable(hashSize);

    dbht_ptr = DBmakeCondorcet(&database, dbht_ptr, a, degree, t_0, k, phase, file_no);
    return 1;
}
