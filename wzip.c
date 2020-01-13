#include <stdio.h>
#include <stdlib.h>

int get_length (char* s) {
    int len = 0;
    while (s[len] != '\0' && s[len] != '\n') {
        len++;
    }
    return len;
}

int get_num_digits (int x) {
    int num_digits = 0;

    while (x) {
        num_digits++;
        x = x/10;
    }
    return num_digits;
}

void int_to_char (int x, char* word, int num_digits) {
    // malloc the space
    char* s = (char*) malloc ((num_digits+1)*sizeof(char));

    // get the digits
    int i = 0;

    while (x) {
        s[i] = '0' + x % 10;
        x = x/10;
        i++;
    }
    s[i] = '\0';

    // reverse the string
    for (i = num_digits-1; i >= 0; i--) {
        word[num_digits-1-i] = s[i];
    }
    word[num_digits] = '\0';

    free(s);
}

int is_space (char c) {
    if (c == ' ') {
        return 1;
    }
    return 0;
}

int main (int argc, char** argv) {

    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    int num_files = argc - 1;

    int cmprsd_s_idx = 0;
    char* compressed_s = (char*) malloc (10000*sizeof(char));
    compressed_s[cmprsd_s_idx] = '\n';
    int file_length = 0;

    for (int i = 0; i < num_files; i++) {

        char* filename_r = argv[i+1];

        FILE* rfptr = fopen(filename_r, "r");

        int size = 10000;
        char* s = (char*) malloc (size*sizeof(char));

        int num_line = 1;
        while (getline(&s, &size, rfptr) != -1) {

            int s_len = get_length(s);
            char* temp_compressed_s = (char*) malloc (sizeof(char)*(s_len+file_length));

            for (int m = 0; m < cmprsd_s_idx; m++) {
                temp_compressed_s[m] = compressed_s[m];
            }

            free(compressed_s);
            compressed_s = temp_compressed_s;

            file_length = file_length + s_len;

            // for each line
            int l = 0;
            while (s[l] != '\0' && s[l] != '\n')  {

                if (l >= get_length(s)) {
                    break;
                }

                // form words and handle compression wordwise
                char* word = (char*) malloc (100*sizeof(char));

                int j = 0;
                while (s[l] != '\0' && s[l] != '\n' && s[l] != ' ') {
                    word[j] = s[l];
                    l++;
                    j++;
                }
                word[j] = '\0';

                int word_len = get_length(word);
                char* compressed_word = (char*) malloc ((word_len+1)*sizeof(char));

                int cmprsd_idx = 0;

                for (int i = 0; i < word_len; i++) {

                    int count = 1;
                    char c = word[i];

                    // the last char which is also non-repeating
                    if (i == word_len-1) {
                        compressed_word[cmprsd_idx] = c;
                        cmprsd_idx++;
                        break;
                    }

                    // catch the repeating characters
                    while (word[i] == word[i+1]) {
                        count++;
                        i++;
                    }

                    // if repeating
                    if (count > 1) {

                        // count the no. of digits
                        int num_digits = get_num_digits(count);
                        char* num = (char*) malloc ((num_digits+1)*sizeof(char));

                        // change the int to char
                        int_to_char(count, num, num_digits);

                        int num_len = get_length(num);
                        for (int k = 0; k < num_len; k++) {
                            compressed_word[cmprsd_idx] = num[k];
                            cmprsd_idx++;
                        }
                        compressed_word[cmprsd_idx] = c;
                        cmprsd_idx++;
                        free(num);
                    }
                    else {
                        compressed_word[cmprsd_idx] = c;
                        cmprsd_idx++;
                    }
                }
                // the complete compressed word in a sentence
                compressed_word[cmprsd_idx] = '\0';

                cmprsd_idx = 0;
                while (compressed_word[cmprsd_idx] != '\0') {
                    compressed_s[cmprsd_s_idx] = compressed_word[cmprsd_idx];
                    cmprsd_s_idx++;
                    cmprsd_idx++;
                }

                // the whole complete complete sentence
                compressed_s[cmprsd_s_idx] = ' ';
                cmprsd_s_idx++;
                free(compressed_word);
                free(word);
                l++;
            }
            cmprsd_s_idx--;
            compressed_s[cmprsd_s_idx] = '\n';
            cmprsd_s_idx++;

            num_line++;
        }

        fclose(rfptr);
    }

    fwrite(compressed_s, cmprsd_s_idx, 1, stdout);

    free(compressed_s);
    return 0;
}
