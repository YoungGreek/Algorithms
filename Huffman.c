#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define ALPHABET 256


typedef struct freq_table {
    int freq;
} freq_table;


typedef struct node {
    unsigned char data;
    int freq;
    struct node *left;
    struct node *right;
} node;


typedef struct heap {
    node **nodes;
    long long size;
} heap;


typedef struct bit_stream {
    FILE *file;
    unsigned char buff;
    int count;
} bit_stream;


freq_table *create_freq_table(FILE *f_in) {
    freq_table *table = (freq_table *) malloc(sizeof(freq_table) * ALPHABET);
    for (int i = 0; i < ALPHABET; i++) {
        table[i].freq = 0;
    }
    int tmp_symbol = getc(f_in);
    if (tmp_symbol == -1) {
        free(table);
        return 0;
    }
    table[tmp_symbol].freq += 1;
    while ((tmp_symbol = getc(f_in)) != EOF) {
        table[tmp_symbol].freq += 1;

    }
    return table;
}


node *create_node(unsigned char data, int freq) {
    node *n = malloc(sizeof(node));
    n->freq = freq;
    n->data = data;
    n->right = NULL;
    n->left = NULL;

    return n;
}


void swap(node **a, node **b) {
    node *tmp = *a;
    *a = *b;
    *b = tmp;
}


void sift_down(heap *h, long long index) {
    long long left_child = 2 * index + 1; // Индекс левого потомка
    long long right_child = 2 * index + 2; // Индекс правого потомка
    long long smallest = index; // Индекс текущего узла

    if (left_child < h->size && h->nodes[left_child]->freq < h->nodes[smallest]->freq) {
        smallest = left_child;
    }

    if (right_child < h->size && h->nodes[right_child]->freq < h->nodes[smallest]->freq) {
        smallest = right_child;
    }

    if (smallest != index) {
        swap(&h->nodes[index], &h->nodes[smallest]);
        sift_down(h, smallest);
    }
}


void sift_up(heap *h, long long i) {
    while (i && h->nodes[i]->freq < h->nodes[(i - 1) / 2]->freq) {
        swap(&h->nodes[i], &h->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


node *extract_min(heap *h) {
    node *tmp = h->nodes[0];
    h->nodes[0] = h->nodes[h->size - 1];
    h->size--;

    sift_down(h, 0);

    return tmp;
}


void insert(heap *h, node *n) {
    h->size++;
    long long i = h->size - 1;
    h->nodes[i] = n;
    sift_up(h, i);
}


void add_to_heap(heap *h, int freq, unsigned char data) {
    node *n = create_node(data, freq);
    h->nodes[h->size] = n;
    h->size++;
}


void build_heap(heap *h) {
    for (long long i = h->size / 2; i >= 0; i--) {
        sift_down(h, i);
    }
}


heap *create_heap() {
    heap *h = (heap *) malloc(sizeof(heap));
    h->size = 0;
    h->nodes = (node **) malloc(sizeof(node) * ALPHABET);

    return h;
}


node *build_huff_tree(heap *h) {
    long long length = h->size;
    for (long long i = 0; i < length - 1; i++) {
        node *left = extract_min(h);
        node *right = extract_min(h);
        int sum = left->freq + right->freq;
        node *root = create_node('\0', sum);
        root->left = left;
        root->right = right;
        insert(h, root);
    }
    return extract_min(h);
}


int is_leaf(node *n) {
    return (n->left == NULL) && (n->right == NULL);
}


void write_bit(bit_stream *out, int bit) {
    out->buff |= (bit << out->count);
    out->count += 1;
    if (out->count == 8) {
        fwrite(&out->buff, sizeof(out->buff), 1, out->file);
        out->buff = 0;
        out->count = 0;
    }
}


void write_size(freq_table *table, FILE *out) {
    int size = 0;
    for (int i = 0; i < ALPHABET; i++) {
        size += table[i].freq;
    }
    fwrite(&size, sizeof(int), 1, out);
}


void write_tree(node *tree, bit_stream *for_tree) {
    if (is_leaf(tree)) {
        write_bit(for_tree, 0);
        for (int i = 0; i < 8; i++) {
            if ((tree->data & (1 << i)) != 0) {
                write_bit(for_tree, 1);
            } else {
                write_bit(for_tree, 0);
            }
        }
    } else {
        write_bit(for_tree, 1);
        write_tree(tree->left, for_tree);
        write_tree(tree->right, for_tree);
    }
}


void create_codes(unsigned char *code, unsigned char **codes_table, node *tree, int top) {
    if (tree->left != NULL) {
        code[top] = '0';
        code[top + 1] = '\0';
        create_codes(code, codes_table, tree->left, top + 1);
    }

    if (tree->right != NULL) {
        code[top] = '1';
        code[top + 1] = '\0';
        create_codes(code, codes_table, tree->right, top + 1);
    }

    if (is_leaf(tree)) {
        unsigned char *tmp = (unsigned char *) malloc(sizeof(unsigned char) * (top + 1));
        memcpy(tmp, code, sizeof(unsigned char) * top);
        tmp[top] = '\0';
        memcpy(codes_table[tree->data], tmp, sizeof(unsigned char) * top);
        free(tmp);
    }
}


void write_data(unsigned char **codes_table, FILE *f_in, bit_stream *for_data) {
    fseek(f_in, 0, SEEK_SET);
    int tmp;
    while ((tmp = getc(f_in)) != EOF) {
        int j = 0;
        while (codes_table[tmp][j] && (codes_table[tmp][j] == '1' || codes_table[tmp][j] == '0')) {
            int sym = codes_table[tmp][j] & 1;
            write_bit(for_data, sym);
            j += 1;
        }
    }
}


void free_heap(heap *h) {
    for (int i = 0; i < h->size; i++) {
        free(h->nodes[i]);
    }
    free(h->nodes);
    free(h);
}


void free_huffman_tree(node *root) {
    if (root == NULL) {
        return;
    }

    free_huffman_tree(root->left);
    free_huffman_tree(root->right);

    free(root);
}


void free_freq_table(freq_table *table) {
    free(table);
}


void free_bit_stream(bit_stream *stream) {
    free(stream);
}


void free_encode_resources(freq_table *table, heap *h, unsigned char **codes_table, unsigned char *code, node *huff_tree, bit_stream *out) {
    free(out);
    free_huffman_tree(huff_tree);
    for (int i = 0; i < ALPHABET; i++) {
        if (codes_table[i] != NULL) {
            free(codes_table[i]);
        }
    }
    free(codes_table);
    free(code);
    free_heap(h);
    free_freq_table(table);
}


void encode(FILE *f_in, FILE *f_out) {
    freq_table *table = create_freq_table(f_in);
    if (table == 0) {
        free_freq_table(table);
        exit(0);
    }
    heap *h = create_heap();
    unsigned char **codes_table = (unsigned char **) malloc(sizeof(unsigned char *) * ALPHABET);
    for (int i = 0; i < ALPHABET; i++) {
        codes_table[i] = (unsigned char *) malloc(sizeof(unsigned char) * (ALPHABET - 1));
    }
    unsigned char *code = (unsigned char *) malloc(sizeof(unsigned char) * (ALPHABET - 1));
    int top = 0;
    for (int i = 0; i < ALPHABET; i++) {
        if (table[i].freq != 0) {
            add_to_heap(h, table[i].freq, i);
        }
    }
    build_heap(h);
    node *huff_tree = build_huff_tree(h);
    bit_stream *out = malloc(sizeof(bit_stream));
    memset(out, 0, sizeof(bit_stream));
    out->file = f_out;

    write_size(table, f_out);
    write_tree(huff_tree, out);

    create_codes(code, codes_table, huff_tree, top);

    write_data(codes_table, f_in, out);
    if (out->count > 0) {
        fwrite(&out->buff, sizeof(out->buff), 1, f_out);
    }

    free_encode_resources(table, h, codes_table, code, huff_tree, out);
}


bool read_bit(bit_stream *in) {
    if (in->count == 8) {
        if (fread(&in->buff, sizeof(in->buff), 1, in->file) == 0) {
            printf("Read error");
            exit(0);
        }
        in->count = 0;
    }
    bool bit = (in->buff) & (1 << in->count);
    in->count++;
    return bit;
}


int get_size(FILE *f_in) {
    int size = 0;
    if (fread(&size, sizeof(int), 1, f_in) == 0) {
        exit(0);
    }
    return size;
}

node *create_head(node *left, node *right) {
    node *head = (node *) malloc(sizeof(node));
    memset(head, 0, sizeof(node));
    head->data = '0';
    head->left = left;
    head->right = right;

    return head;
}

node *create_leaf(unsigned char data) {
    node *leaf = (node *) malloc(sizeof(node));
    memset(leaf, 0, sizeof(node));
    leaf->data = data;

    return leaf;
}

node *get_tree(bit_stream *in) {
    bool bit = read_bit(in);
    if (bit) {
        node *left = get_tree(in);
        node *right = get_tree(in);
        return create_head(left, right);
    } else {
        unsigned char data = 0;
        for (int i = 0; i < 8; i++) {
            int tmp_bit = read_bit(in);
            data += tmp_bit * pow(2, i);
        }
        return create_leaf(data);
    }
}


unsigned char read_data(bit_stream *in, node *tree) {
    node *cur = tree;
    while (cur->left != NULL && cur->right != NULL) {
        int bit = read_bit(in);
        if (bit == true) {
            cur = cur->right;
        } else {
            cur = cur->left;
        }
    }
    return cur->data;
}


void decode(FILE *f_in, FILE *f_out) {
    int size = get_size(f_in);
    bit_stream *in = (bit_stream *) malloc(sizeof(bit_stream));
    memset(in, 0, sizeof(bit_stream));
    in->file = f_in;
    in->buff = getc(f_in);
    in->count = 0;


    node *tree = get_tree(in);

    int counter = 0;
    while (counter != size) {
        unsigned char curr_symbol = read_data(in, tree);
        fputc(curr_symbol, f_out);
        counter += 1;
    }

    free_bit_stream(in);
    free_huffman_tree(tree);

}


void what_do(char to_do, FILE *f_in, FILE *f_out) {
    if (to_do == 'c') {
        encode(f_in, f_out);
    } else if (to_do == 'd') {
        decode(f_in, f_out);
    } else {
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 4){
        printf("Недостаточно введённых аргументов");
        return 0;
    }

    char to_do = argv[1][0];
    char *name_in = argv[2];
    char *name_out = argv[3];
    FILE *f_in = fopen(name_in, "rb");
    FILE *f_out = fopen(name_out, "wb");
    what_do(to_do, f_in, f_out);

    fclose(f_in);
    fclose(f_out);
    return 0;
}
