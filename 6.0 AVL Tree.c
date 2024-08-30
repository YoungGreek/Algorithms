#include <stdlib.h>
#include <stdio.h>


typedef struct Node {
    char height;
    int value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Node* NodePtr;

typedef struct Memory{
    int count;
    int index;
    void* Buffer;
}Memory;


Memory Memory_Create(int count, int size) {
    Memory Memory = { count, 0, NULL };

    Memory.Buffer = malloc(count * size);
    if (Memory.Buffer == NULL)
    {
        exit(EXIT_FAILURE);
    }

    return Memory;
}

NodePtr Memory_Locate(Memory* Memory) {
    if (Memory->count == Memory->index)
    {
        return NULL;
    }

    NodePtr node = (NodePtr)((char*)Memory->Buffer + Memory->index * sizeof(Node));
    Memory->index++;
    return node;
}


int max(int left_height, int right_height) {
    if (left_height > right_height) {
        return left_height + 1;
    } else {
        return right_height + 1;
    }
}

int get_height(Node *node) {
    if (node == NULL) {
        return -1;
    } else {
        return node->height;
    }
}

Node *update_height(Node *node) {
    node->height = max(get_height(node->left), get_height(node->right));

    return node;
}

int get_balance(Node *node) {
    if (node == NULL) {
        return 0;
    } else {
        return get_height(node->right) - get_height(node->left);
    }
}


Node *right_rotate(Node *node) {
    int node_value = node -> value;
    node -> value = node -> left -> value;
    node -> left -> value = node_value;

    Node *buff = node->right;
    node->right = node->left;
    node->left = node->right->left;
    node->right->left = node->right->right;
    node->right->right = buff;

    node -> right = update_height(node->right);
    node = update_height(node);

    return node;
}

Node *left_rotate(Node *node) {
    int node_value = node -> value;
    node -> value = node -> right -> value;
    node -> right -> value = node_value;

    Node *buff = node->left;
    node->left = node->right;
    node->right = node->left->right;
    node->left->right = node->left->left;
    node->left->left = buff;

    node -> left = update_height(node->left);
    node = update_height(node);

    return node;
}

Node *balance(Node *node) {
    int balance = get_balance(node);
    if (balance == -2) {
        if (get_balance(node->left) == 1)
            node -> left = left_rotate(node->left);
        node = right_rotate(node);
    }else if (balance == 2) {
        if (get_balance(node->right) == -1)
            node -> right = right_rotate(node->right);
        node = left_rotate(node);
    }

    return node;
}

NodePtr new_node(int value, Memory *memory) {
    NodePtr new = Memory_Locate(memory);

    new->height = 1;
    new->value = value;
    new->left = NULL;
    new->right = NULL;

    return new;
}

void add(int value, NodePtr *node, Memory *memory){
    if(*node == NULL){
        *node = new_node(value, memory);
    }
    else if(value < (*node)->value){
        add(value, &(*node)->left, memory);
    }
    else{
        add(value, &(*node)->right, memory);
    }
    update_height(*node);
    balance(*node);

}

void destroy_memory(Memory *memory){
    free(memory -> Buffer);
}

int main(void) {

    int N;
    if(scanf("%d", &N) != 1){
        return 0;
    }

    if(N == 0){
        printf("0");
        return 0;
    }
    Memory memory = Memory_Create(N, sizeof(Node));

    NodePtr node;
    node = NULL;

    int tmp;
    for (int i = 0; i < N; i++) {
        if(scanf("%d ", &tmp) != 1){
            break;
        }
        add(tmp, &node, &memory);
    }
    printf("%d ", node -> height + 1);
    destroy_memory(&memory);

    return EXIT_SUCCESS;
}
