/**************************************************************************
* FILENAME:                                                               *
*   FileManagement.c ****                                                 *
*                                                                         *
* PURPOSE:                                                                *
*   Implement method for handling files define in FileManagement.h ****   *
*                                                                         *
**************************************************************************/

/*
* Name:
*   openFile(i, in, write)
*
* Parameters:
*   i - int suffix for name of file
*   in = 1 for in_ file, in = 0 for ou_ file
*   write - int = 1 for write, 0 for read
*
* Description:
*   Opens a file with filename in_i or out_i for reading or writing, 0 <= i <= 999 999.
*
* Return:
*   Pointer to the file, or 0 if write == 0 and the file could not be opened. i.e. does not exist.
*/
FILE* openFile(int i, int in, int write)
{
    char filename[10]; /* Filename string */
    FILE* fp_ptr; /* File pointer */

    if (i > 9999)
    {
        printf("Error in openFile: Called with %d\n",i);
        exit(0);
    }

    if (in) {
        filename[0] = 'i';
        filename[1] = 'n';
    }
    else {
        filename[0] = 'o';
        filename[1] = 'u';
    }
    
    filename[2] = '_';

    filename[8] = '0' + (i % 10);

    i = i / 10;

    filename[7] = '0' + (i % 10);

    i = i / 10;

    filename[6] = '0' + (i % 10);
    
    i = i / 10;
    
    filename[5] = '0' + (i % 10);
    
    i = i / 10;
    
    filename[4] = '0' + (i % 10);
    
    i = i / 10;
    
    filename[3] = '0' + i;

    filename[9] = 0;

    if (write)
    {
        if (!(fp_ptr = fopen(filename,"w")))
        {
            printf("Error in openFile: Unable to open file for writing '%s'\n",
                filename);
            exit(0);
        }
    }
    else
    {
        if (!(fp_ptr = fopen(filename,"r"))) return 0;
        /*
        {
            printf("Error in openFile: Unable to open file for reading '%s'\n",
                filename);
            exit(0);
        }
         */
    }

    return fp_ptr;
}

FILE* P3openFile(int n, int size, int write) /* Open a file FINALn_size for reading or writing in phase 3.  0 <= n < = 999 */
{
    char filename[13]; /* Filename string */
    FILE* fp_ptr; /* File pointer */
    if (n > 999)
    {
        printf("Error in P3openFile: Called with name %d\n",n);
        exit(0);
    }
    
    filename[0] = 'F';
    
    filename[1] = 'I';
    
    filename[2] = 'N';
    
    filename[3] = 'A';
    
    filename[4] = 'L';
    
    filename[5] = '0' + n/100;
    n = n%100;
    
    filename[6] = '0' + n/10;
    
    filename[7] = '0' + n%10;
    
    filename[8] = '_';
    
    filename[9] = '0' + size/100;
    size = size%100;
    
    filename[10] = '0' + size/10;
    
    filename[11] = '0' + size%10;
    
    filename[12] = 0;
    
    if (write)
    {
        if (!(fp_ptr = fopen(filename,"w")))
        {
            printf("Error in P3openFile: Unable to open file for writing '%s'\n",
                   filename);
            exit(0);
        }
    }
    else
    {
        if (!(fp_ptr = fopen(filename,"r")))
        {
            printf("Error in P3openFile: Unable to open file for reading '%s'\n",
                   filename);
            exit(0);
        }
    }
    
    return fp_ptr;
}

FILE* openStats(int n)
{
    char filename[8]; /* Filename string */
    FILE* fp_ptr; /* File pointer */
    
    if (n > 99)
    {
        printf("Error in openStats: Called with name %d\n",n);
        exit(0);
    }
    
    filename[0] = 'S';
    
    filename[1] = 'T';
    
    filename[2] = 'A';
    
    filename[3] = 'T';
    
    filename[4] = 'S';
    
    filename[5] = '0' + n/10;
    
    filename[6] = '0' + n%10;
    
    filename[7] = 0;
    
    if (!(fp_ptr = fopen(filename,"a")))
    {
        printf("Error in openStats: Unable to open file for reading and writing '%s'\n",
                   filename);
        exit(0);
    }
    
    return fp_ptr;
}

FILE* DopenStats(int n)
{
    char filename[9]; /* Filename string */
    FILE* fp_ptr; /* File pointer */
    
    if (n > 99)
    {
        printf("Error in DopenStats: Called with name %d\n",n);
        exit(0);
    }
    
    filename[0] = 'S';
    
    filename[1] = 'T';
    
    filename[2] = 'A';
    
    filename[3] = 'T';
    
    filename[4] = 'S';
    
    filename[5] = 'D';
    
    filename[6] = '0' + n/10;
    
    filename[7] = '0' + n%10;
    
    filename[8] = 0;
    
    if (!(fp_ptr = fopen(filename,"w")))
    {
        printf("Error in DopenStats: Unable to open file for writing '%s'\n",
               filename);
        exit(0);
    }
    
    return fp_ptr;
}

void checkfiles(int file_no)
{
    int i;
    FILE* fp;
    int success = 1;
    char c;
    
    for (i=0; i< file_no; i++) {
        fp = openFile(i, 0, 0);  //  Open ou_i for reading.
        do {
            c = getc(fp);
        }
        while((c != EOF) && (c != 'N'));
        if (c != 'N')  {
            success = 0;
            printf("File number %d failed\n", i);
        }
        fclose(fp);
    }
    if (!success) exit(0);
}
