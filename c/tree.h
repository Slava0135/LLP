#ifndef TREE_TREE_H
#define TREE_TREE_H

#define T int

typedef struct Tree_ Tree;

extern Tree *makeTree();
extern void insertElement(Tree *, T);
extern int removeElement(Tree *, T);
extern int isPresent(Tree *, T);
extern void printTreeSorted(Tree *);
extern void freeTree(Tree *);
extern int getSize(Tree *);
extern int getDepth(Tree *);
extern int validate(Tree *);

#endif //TREE_TREE_H
