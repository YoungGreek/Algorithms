#include <stdlib.h>
#include <stdio.h>


void quick_sort(int *array, int left, int right) { // алгоритм быстрой сортировки
    if(left < right){ // указатели на элементы массива
        int left_border = left;
        int right_border = right;
        int pivot = array[(left + right) / 2]; // первый элемент, по которому будем разбивать массив на два
        while(left_border <= right_border){ // текущие указатели двигаются справа - налево и наоборот
            while(array[left_border] < pivot){ // двигаем левую границу, пока число под номером левой границы меньше нашего пивота
                left_border++;
            }
            while(array[right_border] > pivot){ // двигаем правую границу, пока число под номером правой границы больше нашего пивота
                right_border--;
            }
            if(left_border <= right_border){ // меняем местами число которое больше пивота и находится слева, с числом которое меньше пивота и находится справа
                int temp_number = array[left_border];
                array[left_border] = array[right_border];
                array[right_border] = temp_number;
                left_border++;
                right_border--;
            }
        }
        quick_sort(array, left, right_border); // рекурсивно сортируем массивы, на которые распался наш исходный массив
        quick_sort(array, left_border, right);
    }
}

int main(void) {
    int N;
    if(scanf("%d", &N) > 1){
        return 0;
    }
    int array[N];
    for (int i = 0; i < N; i++){
        if(scanf("%d", &array[i]) < 1){
            return 0;
        }
    }
    quick_sort(array, 0, N - 1);
    for (int i = 0; i < N; i++)
        printf("%d ", array[i]);
    return EXIT_SUCCESS;
}
