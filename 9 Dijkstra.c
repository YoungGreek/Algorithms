#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int Correct_Input(int *N, int *S, int *F, int *M) {
    if (scanf("%d", N) != 1) {
        return 0;
    }
    if (*N < 0 || *N > 5000) {
        printf("bad number of vertices");
        return 0;
    }
    if (scanf("%d %d", S, F) != 2) {
        return 0;
    }
    if (*S < 1 || *S > *N || *F < 1 || *F > *N) {
        printf("bad vertex");
        return 0;
    }
    if (scanf("%d", M) != 1) {
        return 0;
    }
    if (*M < 0 || *M > (*N * (*N + 1) / 2)) {
        printf("bad number of edges");
        return 0;
    }

    return 1;
}

int Min_Distance(const long long *best_way, const int *visited, int N) {
    long long min_way = __LONG_LONG_MAX__;
    int min_index = INT_MAX;

    for (int v = 0; v < N; v++) {
        if (visited[v] && best_way[v] <= min_way) {
            min_way = best_way[v];
            min_index = v;
        }
    }

    return min_index;
}

void destroy(int **AdjMatrix, int N) {
    for (int i = 0; i < N; i++) {
        free(AdjMatrix[i]);
    }
    free(AdjMatrix);
}

int main(void) {
    int N;
    int S;
    int F;
    int M;

    // ввод данных и проверка на корректность
    if (Correct_Input(&N, &S, &F, &M) == 0) {
        return 0;
    }

    // инициализация матрицы
    int **AdjMatrix = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        AdjMatrix[i] = malloc(N * sizeof(int));
    }

    // заполнение матрицы нулями
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            AdjMatrix[i][j] = 0;
        }
    }

    // ввод рёбер и проверка на корректность
    int edge_cnt = 0;
    for (int i = 0; i < M; i++) {
        int vertex_1;
        int vertex_2;
        long long length;
        if (scanf("%d %d %lld", &vertex_1, &vertex_2, &length) != 3) {
            destroy(AdjMatrix, N);
            return 0;
        }
        if (vertex_1 < 1 || vertex_1 > N || vertex_2 < 1 || vertex_2 > N) {
            printf("bad vertex");
            destroy(AdjMatrix, N);
            return 0;
        }
        if (length < 0 || length > INT_MAX) {
            printf("bad length");
            destroy(AdjMatrix, N);
            return 0;
        }
        edge_cnt += 1;
        AdjMatrix[vertex_1 - 1][vertex_2 - 1] = length;
        AdjMatrix[vertex_2 - 1][vertex_1 - 1] = length;
    }
    if (edge_cnt < M) {
        printf("bad number of lines");
        destroy(AdjMatrix, N);
        return 0;
    }

    // иницализация компонентов для алгоритма
    long long best_way_to[N];
    int visited[N];
    for (int i = 0; i < N; i++) {
        best_way_to[i] = __LONG_LONG_MAX__;
        visited[i] = 1;
    }
    best_way_to[S - 1] = 0;
    int path_to_F = 0;
    int huge_way_cnt = 0;

    // алгоритм дейкстры
    for (int i = 0; i < N - 1; i++) {
        int curr_v = Min_Distance(best_way_to, visited, N);
        visited[curr_v] = 0;

        for (int j = 0; j < N; j++) {
            if (AdjMatrix[curr_v][F - 1]) {
                path_to_F = 1;
            }
            if (visited[j] && AdjMatrix[curr_v][j] && best_way_to[curr_v] != __LONG_LONG_MAX__) {
                if (best_way_to[curr_v] + AdjMatrix[curr_v][j] < best_way_to[j])
                    best_way_to[j] = best_way_to[curr_v] + AdjMatrix[curr_v][j];
                if (best_way_to[curr_v] + AdjMatrix[curr_v][j] > INT_MAX)
                    huge_way_cnt += 1;

            }
        }
    }

    // вывод кратчайших путей до всех вершин
    for (int i = 0; i < N; i++) {
        if (best_way_to[i] == __LONG_LONG_MAX__) {
            printf("oo ");
        } else if (best_way_to[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%lld ", best_way_to[i]);
        }
    }

    if (best_way_to[F - 1] > INT_MAX && huge_way_cnt >= 2) {
        printf("\noverflow");
        destroy(AdjMatrix, N);
        return EXIT_SUCCESS;
    } else if (!path_to_F && (S != F)) {
        printf("\nno path");
        destroy(AdjMatrix, N);
        return EXIT_SUCCESS;
    }

    // инициализация компонентов для обратного пути
    int end = F - 1;
    int path[N];
    path[0] = end + 1;
    int path_index = 1;
    long long tmp_weight = best_way_to[end];

    // построение обратного пути
    while (end != S - 1) {
        for (int i = 0; i < N; i++) {
            if (AdjMatrix[i][end] != 0) {
                long long curr_weight = tmp_weight - AdjMatrix[i][end];
                if (best_way_to[i] == curr_weight) {
                    tmp_weight = curr_weight;
                    end = i;
                    path[path_index] = i + 1;
                    path_index++;
                }
            }
        }
    }

    // вывод обратного пути
    printf("\n");
    for (int i = 0; i < path_index; i++) {
        printf("%d ", path[i]);
    }
    destroy(AdjMatrix, N);
    return EXIT_SUCCESS;
}
