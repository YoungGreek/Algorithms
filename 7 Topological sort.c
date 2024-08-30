#include <stdlib.h>
#include <stdio.h>


char *CreateAdjencyMatrix(int size) {
    int tmp;
    if (size % 8 != 0) {
        tmp = size * size / 8 + 1;
    } else {
        tmp = size * size;
    }
    char *AdjMatrix = (char *) malloc(sizeof(char) * tmp);
    for (int i = 0; i < tmp; i++) {
        AdjMatrix[i] = 0;
    }

    return AdjMatrix;
}

void Set_Edge(char *AdjMatrix, int row, int col, int N) {
    int bit_index = row * N + col;
    int symbol_index = bit_index / 8;
    unsigned char mask = 128 >> (bit_index % 8);
    AdjMatrix[symbol_index] |= mask;
}

int Has_Edge(const char *AdjMatrix, int row, int col, int N) {
    int bit_index = row * N + col;
    int symbol_index = bit_index / 8;
    unsigned char mask = 128 >> (bit_index % 8);
    return (AdjMatrix[symbol_index] & mask) != 0;
}

void Dfs(char *AdjMatrix, int vertex, char *visited, int *order, int N, int *counter) {
    if (visited[vertex] == 1) {
        printf("impossible to sort");
        exit(EXIT_SUCCESS);
    } else if (visited[vertex] == 0) {
        visited[vertex] = 1;
        for (int i = 0; i < N; i++) {
            int tmp = Has_Edge(AdjMatrix, vertex, i, N);
            if (visited[vertex] != 2 && tmp && vertex != i) {
                Dfs(AdjMatrix, i, visited, order, N, counter);
            }
        }
        order[*counter] = vertex;
        *counter += 1;
        visited[vertex] = 2;
    }
}


int main() {
    int N;
    int M;
    int count = 0;
    int vertex_1;
    int vertex_2;

    int counter = 0;

    if (scanf("%d", &N) != 1) {
        printf("bad number of lines");
        return 0;
    }

    if (N < 0 || N > 2000) {
        printf("bad number of vertices");
        return 0;
    }
    char *AdjMatrix = CreateAdjencyMatrix(N);

    char visited[N];
    int order[N];
    for (int i = 0; i < N; i++) {
        visited[i] = 0;
        order[i] = 0;
    }

    if (scanf("%d", &M) != 1) {
        printf("bad number of lines");
        free(AdjMatrix);
        return 0;
    }
    if (M < 0 || M > N * (N + 1) / 2) {
        printf("bad number of edges");
        free(AdjMatrix);
        return 0;
    }
    for (int i = 0; i < M; i++) {
        if (scanf("%d %d", &vertex_1, &vertex_2) != 2) {
            printf("bad number of lines");
            free(AdjMatrix);
            return 0;
        }
        if (vertex_1 < 1 || vertex_1 > N || vertex_2 < 1 || vertex_2 > N) {
            printf("bad vertex");
            free(AdjMatrix);
            return 0;
        }
        Set_Edge(AdjMatrix, vertex_1 - 1, vertex_2 - 1, N);
        count += 1;
    }
    if (count < M) {
        printf("bad number of lines");
        free(AdjMatrix);
        return 0;
    }
    for (int i = 0; i < N; i++) {
        if (visited[i] != 2) {
            Dfs(AdjMatrix, i, visited, order, N, &counter);
        }
    }
    for (int i = N - 1; i >= 0; i--) {
        printf("%d ", order[i] + 1);
    }
    free(AdjMatrix);
    return 0;
}
