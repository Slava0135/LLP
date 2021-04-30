#include <stdio.h>

#include "tree.h"

int main()
{
    const int limit = 1000;
    Tree *tree = makeTree();

    for (int i = 0; i < limit; i++) {
        insertElement(tree, i);
    }

    printTreeSorted(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    for (int i = 0; i < limit; i += 5) {
        removeElement(tree, i);
        if (isPresent(tree, i)) printf("ERROR!!!\n");
    }
    printTreeSorted(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    for (int i = 0; i < limit; i += 5) {
        insertElement(tree, i);
    }
    printTreeSorted(tree);
    printf("%s\n", validate(tree) ? "Valid!" : "Not Valid!");
    printf("Tree size: %d\n", getSize(tree));
    printf("Depth: %d\n", getDepth(tree));
    printf("\n");

    freeTree(tree);
    return 0;
}
