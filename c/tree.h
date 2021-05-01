#ifndef TREE_TREE_H
#define TREE_TREE_H

#define T int

typedef struct Tree_ Tree;

extern Tree *makeTree();

extern void insertElement(Tree *, T);
extern int removeElement(Tree *, T);

extern int contains(Tree *, T);
extern int getSize(Tree *);
extern int getDepth(Tree *);
extern int validate(Tree *);

extern void getElements(Tree *, T *);

extern void freeTree(Tree *);

#endif //TREE_TREE_H
