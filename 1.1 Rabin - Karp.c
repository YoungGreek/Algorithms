#include <stdio.h>
#include <string.h>
#include <math.h>
#define length 18

FILE *file;

double exponentiation(double number, unsigned long long degree) {

    double result = 1.0;

    while (degree != 0) {
        if ((degree & 1) != 0) {
            result *= number;
        }
        number *= number;
        degree >>= 1;
    }

    return result;
}

int check_correct_input(unsigned char *sub_text, unsigned char *text) {

    if ((file = fopen("in.txt", "r")) == NULL)
        return 1;

    if (fgets((char*)sub_text, length, file) == NULL)
        return 1;


    if (fread(text, 1, strlen((char*)sub_text) - 1, file) == 0)
        return 0;


    return 0;
}



int calculate_hash(unsigned char *text){
    int len = (int)strlen((char*)text);
    int result = 0;
    for(int i = 0; i < len; i++) {
        result += (text[i] % 3) * pow(3, i);
    }
    return result;
}

unsigned char* continue_text(int length_pattern, unsigned char *text, unsigned char *next_text) { // берём следующий кусок текста

    for (int i = 0, j = 1; j < length_pattern; i++, j++)
        next_text[i] = text[j];

    next_text[length_pattern - 1] = getc(file);

    return next_text;
}

void fill_array_power(int sub_text_len, int *array_power, int number_to_exponentiation) {

    for (int i = 0; i < sub_text_len; i++)
        array_power[i] = exponentiation(number_to_exponentiation, i);

}


int update_hash(int hash_text, unsigned char first_symbol_text, int *array_exponentiation , int sub_text_len, unsigned char* text) {

    hash_text -= (first_symbol_text % 3) * array_exponentiation[0];
    hash_text /= 3;
    hash_text += (text[sub_text_len - 1] % 3)  * array_exponentiation[sub_text_len - 1];

    return hash_text;
}


void searcher(unsigned char *text, unsigned char *sub_text) { // ищем подстроку в строке с помощью робина бобина
    int sub_text_len = (int)strlen((char*)sub_text);

    int array_exponentiation[sub_text_len]; // массив в котором будут возведённые 3 в нужную степень
    fill_array_power(sub_text_len, array_exponentiation, 3);

    int text_hash = calculate_hash(text);
    int sub_text_hash = calculate_hash(sub_text);

    printf("%d ", sub_text_hash);

    int text_index = 1; // позиция в тексте
    int good_position = 1; // позиция, которую выводим

    unsigned char tmp_text[length] = {'0'};

    while(!feof(file)) {
        if(sub_text_hash == text_hash) {
            for(int j = 0; j < sub_text_len; j++) {
                if (sub_text[j] == text[j]) {
                    printf("%d ", good_position);
                    good_position += 1;
                } else{
                    printf("%d ", good_position);
                    good_position += 1;
                    break;
                }
            }
        }
        text_index += 1;
        good_position = text_index;

        unsigned char first_symbol_text = text[0]; // элемент, который выкинем из хэша

        text = continue_text(sub_text_len, text, tmp_text); // берём следующий кусок текста
        text_hash = update_hash(text_hash, first_symbol_text, array_exponentiation, sub_text_len, text); // вместо переписывания хэша, апдейтим его путём некоторых арифмитических вычислений
    }
}



int main(void) {
    unsigned char sub_text[length] = {'0'};
    unsigned char text[length] = {'0'};

    if((check_correct_input(sub_text, text)) == 1) {
        return 0;
    }
    int len = (int)strlen((char*)sub_text);
    sub_text[len - 1] = '\0';
    searcher(text, sub_text);
    return 0;
}
