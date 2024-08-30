#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    char word;
    int length;
    char *key_string;
    struct node *next_node;
    struct node *child_node;
} node;


void cci(int *n) {
    if (scanf("%d", n) != 1) {
        exit(0);
    }
    if (*n == 0) {
        printf("None\n");
        printf("0\n");
        exit(0);
    }
}


node *create_node(char *str, int len, char word) {
    node *n = malloc(sizeof(node));

    n->word = word;
    n->length = len;
    n->next_node = NULL;
    n->child_node = NULL;

    n->key_string = malloc((len + 1) * sizeof(char));
    strcpy(n->key_string, str);
    n->key_string[len] = '\0';
    return n;
}


void split_node(node *root, int index) {
    node *new_node = create_node(root->key_string + index,
                                 root->length - index, 0);

    new_node->child_node = root->child_node;
    root->child_node = new_node;

    char *buff_fp = (char *) malloc((index + 1) * sizeof(char));
    strncpy(buff_fp, root->key_string, index);
    buff_fp[index] = '\0';

    free(root->key_string);
    root->key_string = buff_fp;
    root->length = index;
}


node *insert(node *root, char *str, int i) {
    if (i == 0) {
        i = strlen(str) + 1;
    }

    if (root == NULL) {
        return create_node(str, i, 1);
    }

    int pref_counter = 0;
    while ((pref_counter < i) && (pref_counter < root->length) &&
           (str[pref_counter] == root->key_string[pref_counter])) {
        pref_counter++;
    }

    if (pref_counter == 0) {
        root->next_node = insert(root->next_node, str, i);

    }
    else if (pref_counter <= i) {
        if (pref_counter < root->length) {
            split_node(root, pref_counter);
            if (root->word == 1) {
                root->child_node->word = 1;
            }
            root->word = 0;
        }

        if (strcmp(str, root->key_string) != 0) {
            root->child_node = insert(root->child_node, str + pref_counter, i - pref_counter);
        }
        else {
            root->word = 1;
            if (root->child_node != NULL) {
                root->child_node->word = 1;
            }
        }
    }
    return root;
}


void print_keys(node *root, char *acc_word, char *flg) {
    if (root == NULL) {
        return;
    }

    if (root->word) {
        printf("%s%s ", acc_word, root->key_string);
    }

    if ((root->next_node == NULL) && (root->child_node == NULL)) {
        return;
    }

    if (root->next_node != NULL) {
        *flg = 1;
        print_keys(root->next_node, acc_word, flg);
    }

    if (root->child_node != NULL) {
        int len = strlen(acc_word);
        acc_word = strcat(acc_word, root->key_string);
        print_keys(root->child_node, acc_word, flg);
        acc_word[len] = '\0';
    }
}


void prefix_search(node *root, char *pref, int pref_len, char *form, char *keys) {
    if (pref_len == 0) {
        pref_len = strlen(pref) + 1;
    }

    if (root == NULL) {
        return;
    }

    int pref_cur = 0;
    while ((pref_cur < root->length && (pref_cur < pref_len))
           && (pref[pref_cur] == root->key_string[pref_cur])) {
        pref_cur++;
    }

    if (pref_cur == 0) {
        if (root->next_node == NULL) {
            return;
        }
        prefix_search(root->next_node, pref, pref_len, form, keys);

    } else if ((root->child_node == NULL) && (pref_len - 1 > root->length)) {
        return;

    } else if (pref_cur == (int) strlen(pref)) {
        strcat(form, root->key_string + pref_cur);
        if (root->word) {
            *keys = 1;
            printf("%s ", form);
        }
        print_keys(root->child_node, form, keys);
        return;

    } else if (pref_cur == root->length) {
        prefix_search(root->child_node, pref + pref_cur, pref_len - pref_cur, form, keys);
    }
}


int tree_length(node *root) {
    if (root == NULL) {
        return 0;
    }
    int count = 1;

    if (root->child_node != NULL) {
        count += tree_length(root->child_node);
    }

    if (root->next_node != NULL) {
        count += tree_length(root->next_node);
    }

    return count;
}


int check_next_node(node *root) {
    if (root == NULL) {
        return 0;
    }

    return (root->next_node != NULL);
}


int count_nodes(node *root) {
    return tree_length(root) + check_next_node(root);
}


void result_free(char *keys, char *word) {
    free(keys);
    free(word);
}


void result(node *root, char *pref) {
    if (root == NULL) {
        printf("None\n");
        exit(0);
    }

    char *word = malloc((10000 + 1) * sizeof(char));
    char *keys = malloc(sizeof(char));

    strcpy(word, pref);
    *keys = 0;
    prefix_search(root, pref, 0, word, keys);
    if (!*keys) {
        printf("None");
    }

    printf("\n");
    printf("%d\n", count_nodes(root));
    result_free(keys, word);
    exit(0);
}


void destroy(node *root) {
    if (root == NULL) {
        return;
    }

    if (root->child_node != NULL) {
        destroy(root->child_node);
    }

    if (root->next_node != NULL) {
        destroy(root->next_node);
    }

    free(root->key_string);
    free(root);
}


int main(void) {
    int n;
    cci(&n);

    node *root = NULL;
    char *buff = malloc((10000 + 1) * sizeof(char));

    for (int i = 0; i < n; i++) {
        if (scanf("%10000s", buff) != 1) {
            return 0;
        }
        root = insert(root, buff, strlen(buff));
    }

    if (scanf("%10000s", buff) != 1) {
        return 0;
    }

    result(root, buff);

    destroy(root);
    free(buff);
    return 0;
}
