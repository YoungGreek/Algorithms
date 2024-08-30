#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 256
#define PATTERN 18

FILE *file;

int check_correct_input(unsigned char* pattern) {

    if ((file = fopen("in.txt", "r")) == NULL)
        return 1;

    if (fgets((char*)pattern, PATTERN, file) == NULL)
        return 1;

    return 0;
}

void create_table(unsigned char *sub_text, int *shifts) { // таблица со сдвигами для буковок
    int sub_text_len = (int)strlen((char*)sub_text);

    for(int i = 0; i < MAX_LENGTH; i++) {
        shifts[i] = -1;
    }

    for(int i = 0; i < sub_text_len - 1; i++) {
        shifts[(int)sub_text[i]] = i;
    }
}

void continue_text(unsigned char *text, unsigned char *tmp_text, int shift, int sub_text_len) { // следующий кусок текста

    int index = 0;

    for(int i = 0; shift < sub_text_len; i++, shift++) {
        tmp_text[i] = text[shift];
        index += 1;
    }

    for(; index < sub_text_len; index++) {
        tmp_text[index] = fgetc(file);

    }

    strcpy((char*)text, (char*)tmp_text);

}


void searcher(unsigned char *sub_text) {
    int sub_text_len = (int)strlen((char*)sub_text);

    unsigned char text[PATTERN] = {'0'};
    unsigned char tmp_text[PATTERN] = {"0"};

    if(fread((char*)text, 1, sub_text_len, file) == 0) {
        return;
    }

    strcpy((char*)tmp_text, (char*)text);

    int shifts[MAX_LENGTH];
    create_table(sub_text, shifts); // создаём таблицу со сдвигами для конкретных букв

    int index_last_sym_sub = sub_text_len - 1;
    int text_index = 0;

    while(!feof(file)) {
        int shift = 0;
        int tmp_text_index = index_last_sym_sub;

        printf("%d ", text_index + tmp_text_index + 1);

        while(tmp_text_index >= 0 && text[shift + tmp_text_index] == sub_text[tmp_text_index]) { // идём обратно, пока сравниваемые символы одинаковы
            tmp_text_index -= 1;

            if(tmp_text_index >= 0) { // чтобы не вылезало за рамки
                printf("%d ", text_index + tmp_text_index + 1);
            }
        }
        shift = index_last_sym_sub - shifts[abs((int)text[shift + index_last_sym_sub])]; // сдвиг если нашли такую-то букву, берём в abs() для русских буковок

        text_index += shift;

        continue_text(text, tmp_text, shift, sub_text_len);
    }

}



int main(void) {
    unsigned char sub_text[PATTERN] = {'0'};

    if (check_correct_input(sub_text) == 1) {
        return 0;
    }

    sub_text[strlen((char*)sub_text) - 1] = 0;

    searcher(sub_text);

    return 0;
}
