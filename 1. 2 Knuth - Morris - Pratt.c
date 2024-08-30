#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define length 18

FILE *file;
int sub_text_index = 0;


int check_correct_input(unsigned char *sub_text, unsigned char *text) {

    if ((file = fopen("in.txt", "r")) == NULL)
        return 1;

    if (fgets((char *) sub_text, length, file) == NULL)
        return 1;

    if (sub_text[0] == '\n') {
        return 1;
    }

    if (fread(text, 1, strlen((char *) sub_text) - 1, file) == 0) {
        printf("0");
        return 1;
    }
    return 0;
}


void prefix_function(unsigned char *sub_text, int *sub_hash) {
    int len_sub_text = (int) strlen((char *) sub_text);
    for (int i = 1; i < len_sub_text; i++) {
        int j = sub_hash[i - 1];
        while ((j > 0) && (sub_text[j] != sub_text[i])) {
            j = sub_hash[j - 1];
        }
        if (sub_text[j] == sub_text[i]) {
            j += 1;
        }
        sub_hash[i] = j;
    }
}

void continue_text(unsigned char *text, unsigned char *next_text, int shift, int sub_text_len) {
    int i = 0;

    for (; shift < sub_text_len; i++, shift++)
        next_text[i] = text[shift];

    for (; i < sub_text_len; i++)
        next_text[i] = fgetc(file);

    strcpy((char *) text, (char *) next_text);
}

void kmp_search(unsigned char *text, unsigned char *sub_text, const int *sub_prefix) {

    FILE *tmp_file = fopen("in.txt", "r");

    int sub_text_len = (int) strlen((char *) sub_text);

    fseek(tmp_file, 0, SEEK_END);
    long size = ftell(tmp_file) - sub_text_len - 1;
    fclose(tmp_file);

    int text_len = (int) strlen((char *) text);

    if (text_len < sub_text_len) {
        return;
    }
    unsigned char tmp_text[length];

    int text_index = 0;
    int flg = 0;
    int temp_text_index = 0;

    while (!feof(file)) {
        while ((sub_text_index > 0) && text[temp_text_index] != sub_text[sub_text_index]) {
            if (flg != 1 && (sub_text_len < size - text_index + sub_text_index)) {
                printf("%d %d ", text_index - sub_text_index + 1, sub_text_index);
//                    fprintf(stderr, "line = %d sub_text_index = %d\n", __LINE__, sub_text_index);
            }
            sub_text_index = sub_prefix[sub_text_index - 1];

        }

        if (text[temp_text_index] == sub_text[sub_text_index]) {
            sub_text_index += 1;
//                fprintf(stderr, "line = %d sub_text_index = %d\n", __LINE__, sub_text_index);
        }

        flg = 0;
        if ((sub_text_index >= sub_text_len)) {
            printf("%d %d ", text_index - sub_text_index + 2, sub_text_index);
            flg = 1;
//                fprintf(stderr, "line = %d sub_text_index = %d\n", __LINE__, sub_text_index);
        }

        temp_text_index += 1;
        text_index += 1;

        if (temp_text_index == sub_text_len) {
            continue_text(text, tmp_text, 1, sub_text_len);
            temp_text_index -= 1;
//                fprintf(stderr, "line = %d temp_text_index = %d size = %ld sub_text_index = %d\n", __LINE__, temp_text_index, size, sub_text_index);
        }
    }
}


int main() {
    unsigned char text[length];
    unsigned char sub_text[length];

    if (check_correct_input(sub_text, text) == 1) {
        return 0;
    }


    int sub_text_len = (int) strlen((char *) sub_text);
    sub_text[sub_text_len - 1] = '\0';


    int sub_hash[sub_text_len - 1];
    for (int i = 0; i < sub_text_len - 1; i++) {
        sub_hash[i] = 0;
    }

    prefix_function(sub_text, sub_hash);
    for (int i = 0; i < sub_text_len - 1; i++) {
        printf("%d ", sub_hash[i]);
    }
    printf("\n");

    kmp_search(text, sub_text, sub_hash);

    return 0;
}
