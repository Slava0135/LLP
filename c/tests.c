#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "tree.h"

#define LIMIT 1000

static int array[LIMIT];

int main()
{
    Tree *tree = makeTree();

    for (int i = 0; i < LIMIT; i++) {
        insertElement(tree, i);
    }

    if (getSize(tree) < LIMIT) {
        printf("NOT ALL ELEMENTS WERE ADDED");
        return -1;
    }
    if (getDepth(tree) < log2(LIMIT) || getDepth(tree) > 2 * log2(LIMIT)) {
        printf("FAILED DEPTH TEST");
        return -1;
    }
    if (!validate(tree)) {
        printf("TREE IS NOT VALID");
        return -1;
    }

    for (int i = 0; i < LIMIT / 2; i++) {
        int elem = rand() % LIMIT;
        removeElement(tree, elem);
        if (contains(tree, elem)) {
            printf("ELEMENT WAS NOT DELETED");
            return -1;
        }
        if (!validate(tree)) {
            printf("TREE IS NOT VALID");
            return -1;
        }
    }

    for (int i = 0; i < LIMIT; i++) {
        insertElement(tree, i);
    }
    if (getSize(tree) > LIMIT) {
        printf("DUPLICATE NODES");
        return -1;
    }
    if (getSize(tree) < LIMIT) {
        printf("NOT ALL ELEMENTS WERE ADDED");
        return -1;
    }
    if (getDepth(tree) < log2(LIMIT) || getDepth(tree) > 2 * log2(LIMIT)) {
        printf("FAILED DEPTH TEST");
        return -1;
    }

    getElements(tree, array);
    for (int i = 0; i < LIMIT; i++) {
        if (array[i] != i) {
            printf("ELEMENTS DISORDER");
            return -1;
        }
    }

    freeTree(tree);
    printf("TESTS PASSED");
    return 0;
}