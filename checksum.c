//Lance Boza
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 256


char **readtxtfile (char *fn, size_t *idx);
char **realloc_char (char **p, size_t *n);
void prn_chararray (char **ca);
void free_chararray (char **ca);

unsigned char checksum (unsigned char *ptr, size_t sz) {
    unsigned char chk = 0;
    while (sz-- != 0)
        chk -= *ptr++;
    return chk;
}


char** readtxtfile (char *fn, size_t *idx)
{
    if (!fn) return NULL;           /* validate filename provided       */

    char *ln = NULL;                /* NULL forces getline to allocate  */
    size_t n = 0;                   /* max chars to read (0 - no limit) */
    ssize_t nchr = 0;               /* number of chars actually read    */
    size_t nmax = NMAX;             /* check for reallocation           */
    char **array = NULL;            /* array to hold lines read         */
    FILE *fp = NULL;                /* file pointer to open file fn     */

    /* open / validate file */
    if (!(fp = fopen (fn, "r"))) {
        fprintf (stderr, "%s() error: file open failed '%s'.", __func__, fn);
        return NULL;
    }

    /* allocate NMAX pointers to char* */
    if (!(array = calloc (NMAX, sizeof *array))) {
        fprintf (stderr, "%s() error: memory allocation failed.", __func__);
        return NULL;
    }

    /* read each line from fp - dynamicallly allocated   */
    while ((nchr = getline (&ln, &n, fp)) != -1)
    {
        /* strip newline or carriage rtn    */
        while (nchr > 0 && (ln[nchr-1] == '\n' || ln[nchr-1] == '\r'))
            ln[--nchr] = 0;

        array[*idx] = strdup (ln);  /* allocate/copy ln to array        */

        (*idx)++;                   /* increment value at index         */

        if (*idx == nmax)           /* if lines exceed nmax, reallocate */
            array = realloc_char (array, &nmax);
    }

    if (ln) free (ln);              /* free memory allocated by getline */
    if (fp) fclose (fp);            /* close open file descriptor       */

    return array;
}

/* print an array of character pointers. */
void prn_chararray (char **ca)
{
    register size_t n = 0;
    while (ca[n])
    {
        printf ("%s\n", ca[n]);
        n++;
    }
}

/* free array of char* */
void free_chararray (char **ca)
{
    if (!ca) return;
    register size_t n = 0;
    while (ca[n])
        free (ca[n++]);
    free (ca);
}

/*  realloc an array of pointers to strings setting memory to 0.
*  reallocate an array of character arrays setting
*  newly allocated memory to 0 to allow iteration
*/
char **realloc_char (char **p, size_t *n)
{
    char **tmp = realloc (p, 2 * *n * sizeof *p);
    if (!tmp) {
        fprintf (stderr, "%s() error: reallocation failure.\n", __func__);
        // return NULL;
        exit (EXIT_FAILURE);
    }
    p = tmp;
    memset (p + *n, 0, *n * sizeof *p); /* memset new ptrs 0 */
    *n *= 2;

    return p;
}


int main(int argc, char* argv[])
{
    size_t file1_size = 0;  /* placeholders to be filled by readtxtfile */

    /* read each file into an array of strings,
    number of lines read, returned in file_size */
    char **file1 = readtxtfile (argv[1], &file1_size);
    int checkSumSize = atoi(argv[2]);
    if(checkSumSize != 8 && checkSumSize != 16 && checkSumSize != 32){
      fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
    }

    int count = 0;
    int chars = 0;
    if (file1) prn_chararray(file1);
    char text[512];
    for(int i = 0; file1[i] != NULL; i++){
      if(file1[i]==NULL)
        break;
      count++;
    }
    for(int i = 0; i < count; i++){
      for(int a = 0; a < strlen(file1[i]); a++){
        chars++;
      }
    }
    chars++;
    strcpy(text,file1[0]);
    //printf("%s",text);

    //int checkSumSize = 8;
    long unsigned int checkSum;
    int characterCount = chars;
    unsigned char x[] = "";
    strcpy(x,text);
    unsigned char y = checksum (x, 5);
    checkSum = y;
    printf ("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checkSum, characterCount);
    return 0;
}
