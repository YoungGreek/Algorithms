#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
FILE *file;


typedef struct {
    unsigned short start;
    unsigned short end;
    int weight;
} edge;


int compare(const void *p1, const void *p2) {
    const edge *a = p1;
    const edge *b = p2;

    return a->weight - b->weight;
}


void swap(void *a, void *b, size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        char tmp = *((char *) b + i);
        *((char *) b + i) = *((char *) a + i);
        *((char *) a + i) = tmp;
    }
}


int find_set(unsigned short *parent, unsigned short curr_v) {
    if (curr_v == parent[curr_v]) {
        return curr_v;
    }
    return parent[curr_v] = find_set(parent, parent[curr_v]);
}


void update_unions(unsigned short *parent, unsigned short *unions, unsigned short v_1, unsigned short v_2) {
    v_1 = parent[v_1];
    v_2 = parent[v_2];

    if (v_1 == v_2) {
        return;
    }

    if (unions[v_1] < unions[v_2]) {
        swap(&v_1, &v_2, sizeof(unsigned short));
    }

    parent[v_2] = v_1;

    if (unions[v_1] == unions[v_2]) {
        unions[v_1] += 1;
    }

}


int ccp(short *N, int *M) {
    if (fscanf(file, "%hd", N) != 1 || fscanf(file, "%d", M) != 1) {
        return 0;
    }
    if (*N < 0 || *N > 5000) {
        printf("bad number of vertices");
        return 0;
    }
    if (*M < 0 || *M > *N * (*N + 1) / 2) {
        printf("bad number of edges");
        return 0;
    }
    return 1;
}


void free_memory(edge *edges, unsigned short *parents, unsigned short *unions, unsigned short *edges_2) {
    free(edges);
    free(parents);
    free(unions);
    free(edges_2);
}


int main(void) {
    file = fopen("in.txt", "r");
    short N;
    int M;
    if (ccp(&N, &M) == 0) {
        return 0;
    }

    if (N == 1) {
        return 0;
    }

    if (N == 0 || M == 0) {
        printf("no spanning tree");
        return 0;
    }

    edge *edges = (edge *) malloc(sizeof(edge) * M);
    unsigned short *parents = (unsigned short *) malloc(sizeof(unsigned short) * N);
    unsigned short *unions = (unsigned short *) malloc(sizeof(unsigned short) * N);
    unsigned short *edges_2 = (unsigned short *) malloc(sizeof(unsigned short) * M);

    assert(parents != NULL || unions != NULL || edges_2 != NULL || edges != NULL);

    for (int i = 0; i < M; i++) {
        if (fscanf(file, "%hu %hu %d", &edges[i].start, &edges[i].end, &edges[i].weight) != 3) {
            printf("bad number of lines");
            free_memory(edges, parents, unions, edges_2);
            return 0;
        }

        if (edges[i].start < 1 || edges[i].start > N || edges[i].end < 1 || edges[i].end > N) {
            printf("bad vertex");
            free_memory(edges, parents, unions, edges_2);
            return 0;
        }

        if (edges[i].weight < 0 || edges[i].weight > INT_MAX) {
            printf("bad length");
            free_memory(edges, parents, unions, edges_2);
            return 0;
        }

        edges[i].start--;
        edges[i].end--;
    }

    qsort(edges, M, sizeof(edge), compare);


    for (unsigned short i = 0; i < N; i++) {
        parents[i] = i;
        unions[i] = 0;
    }

    int counter = 0;
    for (int i = 0; i < M; i++) {
        unsigned short vertex_1 = find_set(&parents[0], edges[i].start);
        unsigned short vertex_2 = find_set(&parents[0], edges[i].end);

        if (vertex_1 != vertex_2) {
            update_unions(&parents[0], &unions[0], edges[i].start, edges[i].end);
            edges_2[counter] = i;
            counter += 1;
        }
    }

    unsigned short curr = parents[0];
    for (unsigned short i = 0; i < N; i++) {
        if (curr != find_set(&parents[0], i)) {
            printf("no spanning tree");
            free_memory(edges, parents, unions, edges_2);
            return 0;
        }
    }

    for (int i = 0; i < counter; i++) {
        printf("%d %d\n", edges[edges_2[i]].start + 1, edges[edges_2[i]].end + 1);
    }

    free_memory(edges, parents, unions, edges_2);
    return 0;
}
