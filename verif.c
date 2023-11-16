#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* children[4];
};

struct Node* createNode(int value) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->value = value;
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insertByLine(struct Node* root, int* arr, int size) {
    if (root == NULL || arr == NULL || size == 0) {
        return;
    }
    int i = 1;
    int level = 0;
    struct Node* current = root;
    while (i < size) {
        if (current->children[level] == NULL) {
            struct Node* new_node = createNode(arr[i]);
            current->children[level] = new_node;
            i++;
            level = (level + 1) % 4;
            current = root;
        }
        else {
            current = current->children[level];
            level = (level + 1) % 4;
        }
    }
}


void printTree(struct Node* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->value);
    for (int i = 0; i < 4; i++) {
        printTree(root->children[i]);
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(arr) / sizeof(arr[0]);
    struct Node* root = createNode(arr[0]);
    insertByLine(root, arr, size);
    printf("%p", root->children[3]->children[0]);
    //printTree(root);
    return 0;
}
