#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int get_length (char* str) {
    int len = 0;
    while (str[len] != '\n' && str[len] != '\0') {
        len++;
    }
    return len;
}

int main (int argc, char** argv) {

    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    else if (argc == 2) {
        char* word = argv[1];
        int len_word = get_length(word);

        char* line;
        int size = 10;

        line = (char*)malloc(size);

        while (getline(&line, &size, stdin) != -1) {
            int len = get_length(line);
            int i = 0;
            while (line[i] != '\0' && line[i] != '\n') {
                if (line[i] == word[0]) {
                    int found = 1;
                    for (int j = 0; j < len_word; j++) {
                        if (i+j >= len) {
                            found = 0;
                            break;
                        }
                        if (word[j] != line[i+j]) {
                            found = 0;
                            break;
                        }
                    }
                    if (found) {
                        printf("%s", line);
                    }
                }
                i++;
            }
        }
    }
    else if (argc > 2) {
        int file_count = argc - 2;

        char* word = argv[1];
        int len_word = get_length(word);

        for (int k = 0; k < file_count; k++) {
            char* filename = argv[k+2];

            FILE* fptr = fopen(filename, "r");

            if (fptr == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            char* line;
            int size = 10000;

            line = (char*)malloc(size);
            int while_count = 0;

            while (getline(&line, &size, fptr) != -1) {
                int len = get_length(line);
                int i = 0;
                while (line[i] != '\0' && line[i] != '\n') {
                    if (line[i] == word[0]) {
                        int found = 1;
                        for (int j = 0; j < len_word; j++) {
                            if (i+j >= len) {
                                found = 0;
                                break;
                            }
                            if (word[j] != line[i+j]) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            printf("%s", line);
                            break;
                        }
                    }
                    i++;
                }
                while_count++;
            }
        }
    }
    return 0;
}
