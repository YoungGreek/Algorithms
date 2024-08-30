#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    unsigned short size;
    unsigned short capacity;
    unsigned short *arr;
} heap;


typedef struct {
    unsigned short curr_id;
    unsigned short capacity;
    unsigned short *arr;
} stack;

// Создать кучу
heap *create_heap(unsigned short N) {
    heap *lot = (heap *) malloc(sizeof(heap));
    lot->size = N;
    lot->capacity = N;
    lot->arr = (unsigned short *) malloc(sizeof(unsigned short) * N);
    for (int i = 0; i < N; i++) {
        lot->arr[i] = i;
    }

    return lot;
}

// Поменять местами два элемента в куче
void swap(unsigned short *a, unsigned short *b) {
    unsigned short tmp = *a;
    *a = *b;
    *b = tmp;
}

// Просеять кучу
void sift_down(heap *lot, const unsigned long *keys, unsigned short i) {
    while (2 * i + 1 < lot->size) {
        unsigned short curr_left = 2 * i + 1;
        unsigned short curr_right = 2 * i + 2;
        unsigned short tmp = curr_left;
        if (curr_right < lot->size && keys[lot->arr[curr_right]] < keys[lot->arr[curr_left]]) {
            tmp = curr_right;
        }
        if (keys[lot->arr[i]] <= keys[lot->arr[tmp]]) {
            break;
        }
        swap(&lot->arr[i], &lot->arr[tmp]);
        i = tmp;
    }
}

// Отсортировать кучу
void heap_sort(unsigned long *keys, heap *lot) {
    for (int i = lot->capacity / 2; i >= 0; i--) {
        sift_down(lot, keys, i);
    }
}

// Создать стек
stack *create_stack(unsigned short N) {
    stack *st = (stack *) malloc(sizeof(stack));
    st->curr_id = 0;
    st->capacity = N;
    st->arr = (unsigned short *) malloc(sizeof(unsigned short) * N);
    return st;
}

// Удалить вершину из стека
unsigned short pop_from_stack(stack *st) {
    st->curr_id--;
    return st->arr[st->curr_id];
}

// Добавить вершину в стек
void push_to_stack(stack *st, unsigned short value) {
    st->arr[st->curr_id] = value;
    st->curr_id++;
}

// Переместить минимальную вершину в начало
int min_distance(heap *lot) {
    unsigned short min = lot->arr[0];
    lot->arr[0] = lot->arr[lot->size - 1];
    lot->size -= 1;
    return min;
}

// Очищение памяти
void destroy_memory(unsigned int **AdjMatrix, stack *st, heap *lot, unsigned short N) {
    free(st->arr);
    free(st);
    free(lot->arr);
    free(lot);
    for (int i = 0; i < N; i++) {
        free(AdjMatrix[i]);
    }
    free(AdjMatrix);
}

// Корректность ввода
int CCP(short *N, int *M) {
    if (scanf("%hd", N) != 1 || scanf("%d", M) != 1) {
        return 0;
    }
    if (*N < 0 || *N > 5000) {
        printf("bad number of vertices\n");
        return 0;
    }

    if (*M < 0 || *M > (*N + 1) * (*N) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    if (*N == 0 && *M == 0) {
        printf("no spanning tree\n");
        return 0;
    }
    return 1;
}

// Создание матрицы
unsigned int **create_matrix(short N) {
    unsigned int **AdjMatrix = (unsigned int **) malloc(N * sizeof(unsigned int *));
    for (int i = 0; i < N; i++) {
        AdjMatrix[i] = (unsigned int *) calloc(N, sizeof(unsigned int));
    }
    return AdjMatrix;
}


int main() {
    short N;
    int M;
    // Проверка N и M на корректность
    if (CCP(&N, &M) == 0) {
        return 0;
    }
    // Инициализация матрицы и заполнение нулями
    unsigned int **AdjMatrix = create_matrix(N);
    // Инициализация массива рёбер (keys) и массива предыдущих вершин (parents)
    unsigned long keys[N];
    short parents[N];
    for (int i = 0; i < N; i++) {
        keys[i] = ULONG_MAX;
        parents[i] = -1;
    }
    // Инициализация кучи для хранения доступных вершин и стека для хранения пар вида вершина_1 - вершина_2
    heap *lot = create_heap(N);
    stack *st = create_stack(2 * N);
    // Ввод и проверка рёбер на корректность
    unsigned short vertex_1;
    unsigned short vertex_2;
    int length;
    for (int i = 0; i < M; i++) {
        if (scanf("%hu %hu %d", &vertex_1, &vertex_2, &length) != 3) {
            printf("bad number of lines\n");
            destroy_memory(AdjMatrix, st, lot, N);
            return 0;
        }

        if (vertex_1 < 1 || vertex_1 > N || vertex_2 < 1 || vertex_2 > N) {
            printf("bad vertex\n");
            destroy_memory(AdjMatrix, st, lot, N);
            return 0;
        }

        if (length < 0 || length > __INT_MAX__) {
            printf("bad length\n");
            destroy_memory(AdjMatrix, st, lot, N);
            return 0;
        }
        AdjMatrix[vertex_1 - 1][vertex_2 - 1] = length;
        AdjMatrix[vertex_2 - 1][vertex_1 - 1] = length;
    }
    // Реализация алгоритма Эль Прима (тот самый из brawl stars)
    bool have_spanning_tree = true;
    unsigned short start = 0;
    keys[start] = 0;
    parents[start] = 0;
    while (lot->size != 0) {
        unsigned short curr_v = min_distance(lot);
        for (int i = 0; i < N; i++) {
            if (AdjMatrix[curr_v][i] && AdjMatrix[curr_v][i] < keys[i]) {
                parents[i] = curr_v;
                keys[i] = AdjMatrix[curr_v][i];
            }
        }
        heap_sort(keys, lot);
        if (parents[curr_v] == -1) {
            have_spanning_tree = false;
        }
        if (curr_v != start && parents[curr_v] != -1) {
            push_to_stack(st, curr_v + 1);
            push_to_stack(st, parents[curr_v] + 1);
        }
    }
    if (have_spanning_tree == false) {
        printf("no spanning tree\n");
    } else {
        while (st->curr_id != 0) {
            printf("%hu ", pop_from_stack(st));
            printf("%hu\n", pop_from_stack(st));
        }
    }

    destroy_memory(AdjMatrix, st, lot, N);
    return 0;
}
