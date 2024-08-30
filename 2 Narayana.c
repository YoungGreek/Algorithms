#include <stdio.h>
#include <string.h>
#define N 200

// проверка на повторяющиеся символы
int repeated_symbols(char *array){
    int len = (int)strlen(array);
    for(int i = 0; i < len; i++){
        int count = 0;
        char elem = array[i];
        for(int j = i + 1; j < len; j ++){
            char elem_tmp = array[j];
            if (elem == elem_tmp){
                count += 1;
                if(count > 0){
                    return -1;
                }
            }
        }
    }
    return 0;
}


// проверка на посторонние символы
int other_symbols(char *array){
    char alphabet[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    int alphabet_len = (int)strlen(alphabet);
    int len = (int)strlen(array);
    for(int i = 0; i < len; i++){
        int count = 0;
        for(int j = 0; j < alphabet_len; j ++){
            if(array[i] == alphabet[j]){
                count += 1;
            }
        }
        if(count == 0){
            return -1;
        }
    }
    return 0;
}

// стандартный своп
void swap(char *array, int index_1, int index_2){
    char tmp = array[index_1];
    array[index_1] = array[index_2];
    array[index_2] = tmp;
}


// ищем максимальный индекс такого элемента, чтобы A(i) было меньше A(i + 1)
int find_max_index(char *array){
    int len = (int)strlen(array);
    for(int i = len - 2; i > -1; i--){
        if(array[i] < array[i + 1]){
            return i;
        }

    }
    return -1;
}


// ищем максимальный индекс такого элемента, чтобы A(j) было больше A(i)
int find_index_of_biggest_elem(char *array, char elem){
    int len = (int)strlen(array);
    for(int i = len - 1; i > -1; i--){
        if(array[i] - '0' > elem - '0'){
            return i;
        }

    }
    return -1;
}


// переворачиваем строку с какого-то индекса (в алгоритме Нарайаны это i + 1)
void reverse(char *array, int index)
{
    int l = (int)strlen(array);
    int start = index + 1;
    int end = l - 1;
    while (start < end)
    {
        char tmp = array[start];
        array[start] = array[end];
        array[end] = tmp;
        start++;
        end--;
    }
}


int main(){

    char input[N];
    if (fgets(input, N, stdin) == 0){
        return 0;
    }
    int len = (int)strlen(input);
    input[len - 1] = '\0';
    len = (int)strlen(input);
    int count;
    if (scanf("%d", &count) != 1){
        return 0;
    }
    // Проверяем, чтобы символы не повторялись
    if(repeated_symbols(input) == -1){
        printf("bad input");
        fprintf(stderr, "bad input");
        return 0;
    }
    // Проверяем, чтобы не было посторонних символов
    if(other_symbols(input) == -1){
        printf("bad input");
        fprintf(stderr, "bad input");
        return 0;
    }
    // Проверяем, чтобы строка не была из одного элемента
    if(len == 1){
        return 0;
    }
    // В цикле реализуем алгоритм Нарайаны
    int index = find_max_index(input);
    int k = 0;
    while((index != -1) && (k != count)){
        k = k + 1;
        char elem = input[index];
        int swap_index = find_index_of_biggest_elem(input, elem);
        swap(input, index, swap_index);
        reverse(input, index);
        printf("%s\n", input);
        fprintf(stderr, "%s\n", input);
        index = find_max_index(input);
    }
    return 0;
}
