#include <stdio.h>
#include <stdlib.h>

#define FAM_SIZE 29

typedef struct TNode {
    int value;
    char color;
    struct TNode *left;
    struct TNode *right;
} TNode;

//проверка на корректность ввода
int Correct_Input(int *value) {
    if (scanf("%d", value) != 1) {
        return 0;
    }
    if (*value == 0) {
        printf("0");
        return 0;
    }
    return 1;
}

TNode *TLeft_Rotate(TNode *x, TNode *parent) {
    TNode *y = x->right;
    x->right = y->left;
    y->left = x;

    // Обновление родителя
    if (parent != NULL) {
        if (parent->right == x) {
            parent->right = y;
        } else {
            parent->left = y;
        }
    }

    return y;
}


TNode *TRight_Rotate(TNode *x, TNode *parent) {
    TNode *y = x->left;
    x->left = y->right;
    y->right = x;

    // Обновление родителя
    if (parent != NULL) {
        if (parent->left == x) {
            parent->left = y;
        } else {
            parent->right = y;
        }
    }

    return y;
}

// балансировка дерева
TNode *TBalance(TNode *new_node, TNode **fam_arr, int fam_arr_index) {
    while (fam_arr_index > 0 && fam_arr[fam_arr_index - 1]->color == 1) {
        if (fam_arr_index > 1 && fam_arr[fam_arr_index - 1] == fam_arr[fam_arr_index - 2]->left) {
            if (fam_arr[fam_arr_index - 2]->right && fam_arr[fam_arr_index - 2]->right->color == 1) {
                fam_arr[fam_arr_index - 1]->color = 0;
                fam_arr[fam_arr_index - 2]->right->color = 0;
                fam_arr[fam_arr_index - 2]->color = 1;

                new_node = fam_arr[fam_arr_index - 2];
                fam_arr_index -= 2;
            } else {
                if (fam_arr[fam_arr_index - 1]->right == new_node) {

                    new_node = fam_arr[fam_arr_index - 1];
                    fam_arr_index -= 1;

                    fam_arr[fam_arr_index - 1]->left = TLeft_Rotate(new_node, fam_arr[fam_arr_index - 1]);

                    TNode *tmp = fam_arr[fam_arr_index + 1];
                    fam_arr[fam_arr_index + 1] = fam_arr[fam_arr_index];
                    fam_arr[fam_arr_index] = tmp;

                    fam_arr_index++;
                }

                fam_arr[fam_arr_index - 1]->color = 0;

                if (fam_arr_index > 1) {
                    fam_arr[fam_arr_index - 2]->color = 1;
                    if (fam_arr_index - 3 >= 0) {
                        TRight_Rotate(fam_arr[fam_arr_index - 2], fam_arr[fam_arr_index - 3]);
                    } else {
                        TRight_Rotate(fam_arr[fam_arr_index - 2], NULL);
                    }

                    TNode *tmp = fam_arr[fam_arr_index - 1];
                    fam_arr[fam_arr_index - 1] = fam_arr[fam_arr_index - 2];
                    fam_arr[fam_arr_index - 2] = tmp;

                } else {
                    fam_arr[fam_arr_index - 1]->right = new_node;
                }
                new_node = fam_arr[fam_arr_index - 1];
                fam_arr_index -= 2;
            }
        } else {
            if (fam_arr_index > 1 && fam_arr[fam_arr_index - 2]->left && fam_arr[fam_arr_index - 2]->left->color == 1) {
                fam_arr[fam_arr_index - 1]->color = 0;
                fam_arr[fam_arr_index - 2]->left->color = 0;
                fam_arr[fam_arr_index - 2]->color = 1;

                new_node = fam_arr[fam_arr_index - 2];
                fam_arr_index -= 2;
            } else {
                if (fam_arr[fam_arr_index - 1]->left == new_node) {
                    new_node = fam_arr[fam_arr_index - 1];
                    fam_arr_index -= 1;

                    fam_arr[fam_arr_index - 1]->right = TRight_Rotate(new_node, fam_arr[fam_arr_index - 1]);

                    TNode *tmp = fam_arr[fam_arr_index + 1];
                    fam_arr[fam_arr_index + 1] = fam_arr[fam_arr_index];
                    fam_arr[fam_arr_index] = tmp;
                    fam_arr_index++;
                }
                fam_arr[fam_arr_index - 1]->color = 0;

                if (fam_arr_index > 1) {
                    fam_arr[fam_arr_index - 2]->color = 1;

                    if (fam_arr_index - 3 >= 0) {
                        TLeft_Rotate(fam_arr[fam_arr_index - 2], fam_arr[fam_arr_index - 3]);
                    } else {
                        TLeft_Rotate(fam_arr[fam_arr_index - 2], NULL);
                    }
                    TNode *tmp = fam_arr[fam_arr_index - 1];
                    fam_arr[fam_arr_index - 1] = fam_arr[fam_arr_index - 2];
                    fam_arr[fam_arr_index - 2] = tmp;
                } else {
                    fam_arr[fam_arr_index - 1]->right = new_node;
                }

                new_node = fam_arr[fam_arr_index - 1];
                fam_arr_index -= 2;
            }
        }
    }
    return fam_arr[0];
}

// для добавления новой вершины
TNode *TNew_Node(TNode *new_node, int value){
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->value = value;
    new_node->color = 1;

    return new_node;
}

// функция добавления вершинки
TNode *TAdd(TNode *root, TNode *new_node, int value) {
    new_node = TNew_Node(new_node, value);

    if (root == NULL) {
        return new_node;
    }

    TNode *cur_root = root;
    TNode *parent = NULL;
    //для хранения всех тех, кто выше, папа, дядя, дед, прадед
    TNode **fam_arr = malloc(FAM_SIZE * sizeof(TNode));
    int fam_arr_index = 0;

    while (cur_root != NULL) {

        fam_arr[fam_arr_index] = cur_root;
        fam_arr_index++;

        parent = cur_root;
        if (cur_root->value < new_node->value) {
            cur_root = cur_root->right;
        } else {
            cur_root = cur_root->left;
        }
    }

    if (parent->value < new_node->value) {
        parent->right = new_node;
    } else {
        parent->left = new_node;
    }

    fam_arr[fam_arr_index] = new_node;

    if (parent->color == 1) {
        new_node = TBalance(new_node, fam_arr, fam_arr_index);
        free(fam_arr);
        return new_node;
    }
    free(fam_arr);
    return root;

}

int Black_Height(TNode *root) {
    int h = 0;
    while (root != NULL) {
        if (root->color == 0) {
            h += 1;
        }
        root = root->right;
    }

    return h;
}


int main(void) {
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    int N;
    if (Correct_Input(&N) == 0) {
        return 0;
    }

    TNode *rb_tree = NULL;
    TNode *mem_arr = malloc(sizeof(TNode) * N);

    for (int i = 0; i < N; i++) {
        TNode *new_node;
        int tmp_value;
        if (scanf("%d", &tmp_value) != 1) {
            printf("0");
            return 0;
        }
        new_node = &mem_arr[i];
        rb_tree = TAdd(rb_tree, new_node, tmp_value);
        rb_tree->color = 0;
    }
    printf("%d", Black_Height(rb_tree) + 1);
    free(mem_arr);
    return EXIT_SUCCESS;
}
