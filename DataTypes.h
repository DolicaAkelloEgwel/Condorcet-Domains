//
//  DataTypes.h
//  
//
//  Created by C.R.Leedham-Green on 06/03/2017.
//
//

#ifndef DataTypes_h
#define DataTypes_h

typedef int triple[3];

typedef int pair[2];

typedef int *Permutation;

typedef uint32_t UNINT; // Defining the UNINT type

const UNINT ONE = 1; /* Used for operations with addPerm, allOnes, etc */

typedef UNINT* PairsSet;

typedef PairsSet* Pairs_DB;

/* Greater is a bit-string that encodes the set of pairs (a,b) for which a>b in the data base.
 LocalGreater is an array of bit-strings.  The array is of length d(d-1), where d is the degree.
 The entries in the array correspond to the pairs (a,b), and the bits set in that entry
 correspond to the pairs (c,d) for which a>b -> c>d is stored in the data base. */
typedef struct
{
    PairsSet Greater;
    Pairs_DB LocalGreater;
} DataBase;

/* Entries with the same hash address in a hash table are stored in a linked list. */
typedef struct node
{
    DataBase database;
    struct node* next;
} DBHashEntry;

typedef DBHashEntry* DBHashList;

/* size is the number of distinct hash values that are allowed, and is the length of
 the array table.  So hash values lie in the range 0 to size-1.
 count is the number of entries stored in the hash table.*/
typedef struct
{
    int size;
    DBHashList* table;
    int count;
} DBHashTable;

typedef struct
{
    int triple;
    DataBase db;
} DBInputData;


typedef struct link
{
    Permutation entry;
    struct link* next;
} SmallPermset;
    
#endif /* DataTypes_h */
