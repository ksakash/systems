#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int main (int argc, char** argv) {

    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        FILE* fptr;

        fptr = fopen (filename, "r");
    
        if (fptr == NULL) {
            printf ("wcat: cannot open file\n");
            exit (1);
        }

        char s[MAX];
        while (fgets (s, MAX, fptr)){
            printf ("%s", s);
        }

        fclose (fptr);
    }
    return 0;
}
