#include <stdio.h>

#include "tree.h"

#define LIMIT 1000

static int array[LIMIT];

static void printTree(Tree *tree) {
    getElements(tree, array);
    for (int i = 0; i < getSize(tree); i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    Tree *tree = makeTree();

    for (int i = 0; i < LIMIT; i++) {
        insertElement(tree, i);
    }

    printTree(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    for (int i = 0; i < LIMIT; i += 5) {
        removeElement(tree, i);
        if (contains(tree, i)) printf("ERROR!!!\n");
    }
    printTree(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    for (int i = 0; i < LIMIT; i += 5) {
        insertElement(tree, i);
    }
    printTree(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    freeTree(tree);
    return 0;
}