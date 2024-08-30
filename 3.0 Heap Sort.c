#include <stdio.h>
#include <stdlib.h>

void pyramid(int *array, int root, int size){ // построение пирамиды
    int index_max_child; // индекс максимального потомка
    int flg = 0; // флаг указывающий, построена пирамида или нет

    while((root * 2 <= size) && (flg != 1)){ // нумерация начинается с 1, поэтому root * 2, а не root * 2 + 1
        if(root * 2 == size){
            index_max_child = root * 2;
        }
        else if (array[root * 2] > array[root * 2 + 1]){
            index_max_child = root * 2;
        }
        else{
            index_max_child = root * 2 + 1;
        }
        if (array[root] < array[index_max_child]){ // swap корня с потомком большим корня
            int tmp = array[root];
            array[root] = array[index_max_child];
            array[index_max_child] = tmp;
            root = index_max_child;
        }
        else{
            flg = 1; // пирамида построена
        }
    }
}

void heap_sort(int *array, int size){ // сортировка на построенной пирамиде
    for(int i = (size / 2); i >= 0; i--){ // строим пирамиду, продвигаем максимальный элемент на array[0]
        pyramid(array, i, size - 1);
    }

    for (int i = size - 1; i >= 1; i--){
        // меняем местами первый(максимальный) и последний элементы пирамиды,
        // тем самым окончательно находим максимальный элемент, далее пересобираем
        // пирамиду и повторяем действия
        int tmp = array[0];
        array[0] = array[i];
        array[i] = tmp;
        pyramid(array, 0, i - 1);
    }
}

int main()
{
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
    heap_sort(array, N);
    for (int i = 0; i < N; i++)
        printf("%d ", array[i]);
    return EXIT_SUCCESS;
}

