#include <malloc.h>

#include "tree.h"

#define maxOf(a, b) a > b ? a : b

typedef enum { BLACK, RED } Color;

struct Tree_ {
    struct Node *root;
    int size;
};

struct Node {
    T value;
    Color color;
    struct Node *parent;
    struct Node *right;
    struct Node *left;
};


static struct Node *makeNode(T);
static void freeNode(struct Node *);

static void printOrdered(struct Node *);
static int maxDepth(struct Node *);
static int validateNode(struct Node *);

static void rotateRight(Tree *tree, struct Node *);
static void rotateLeft(Tree *tree, struct Node *);

static struct Node *placeNode(struct Node *, T);
static void insertFixup(Tree *, struct Node *);

static void swapColors(struct Node *, struct Node *);
static void swapValues(struct Node *, struct Node *);

static struct Node *findReplacement(struct Node *);
static struct Node *findNode(struct Node *, T);
static struct Node *getSibling(struct Node *);

static int onLeft(struct Node *node);
static int onRight(struct Node *node);

static int deleteNode(Tree *, struct Node *);
static void removeFixup(Tree *, struct Node *);


Tree *makeTree()
{
    Tree *temp = (Tree *) malloc(sizeof (struct Tree_));
    temp->size = 0;
    return temp;
}

void insertElement(Tree *tree, T element)
{
    if (tree->root == NULL) {
        tree->root = makeNode(element);
        tree->root->color = BLACK;
        tree->size = 1;
        return;
    }
    struct Node *node = placeNode(tree->root, element);
    if (node == NULL) return;
    tree->size++;
    insertFixup(tree, node);
}

int removeElement(Tree *tree, T element)
{
    struct Node *deleted = findNode(tree->root, element);
    if (deleted == NULL) return 0;
    return deleteNode(tree, deleted);
}

int isPresent(Tree *tree, T element)
{
    if (tree->root == NULL) return 0;
    return findNode(tree->root, element) != NULL;
}

void printTreeSorted(Tree *tree)
{
    printOrdered(tree->root);
    printf("%c", '\n');
}

void freeTree(Tree *tree)
{
    freeNode(tree->root);
    free(tree);
}

int getSize(Tree *tree)
{
    return tree->size;
}

int getDepth(Tree *tree)
{
    return maxDepth(tree->root);
}

int validate(Tree *tree)
{
    return validateNode(tree->root) != -1;
}

static int validateNode(struct Node *node)
{
    if (node == NULL) return 0;
    if (node->color == RED) {
        if (node->left != NULL && node->left->color != BLACK || node->right != NULL && node->right->color != BLACK) {
            return -1;
        }
    }
    const int left = validateNode(node->left), right = validateNode(node->right);
    if (left == -1 || right == -1 || left != right) return -1;
    return left;
}

static int maxDepth(struct Node *node)
{
    if (node == NULL) return 0;
    return maxOf(maxDepth(node->left), maxDepth(node->right)) + 1;
}

static int deleteNode(Tree *tree, struct Node *deleted)
{
    struct Node *replacement = findReplacement(deleted);
    int bothBlack =
            (replacement == NULL || replacement->color == BLACK) && (deleted->color == BLACK);

    if (replacement == NULL) {
        if (deleted == tree->root) {
            tree->root = NULL;
        } else {
            if (bothBlack) {
                removeFixup(tree, deleted);
            } else {
                struct Node *sibling = getSibling(deleted);
                if (sibling != NULL) {
                    sibling->color = RED;
                }
            }
            if (onLeft(deleted)) {
                deleted->parent->left = NULL;
            } else {
                deleted->parent->right = NULL;
            }
        }
        tree->size--;
        free(deleted);
        return 1;
    }

    if (deleted->left == NULL || deleted->right == NULL) {
        if (deleted == tree->root) {
            tree->root = replacement;
            tree->size--;
            free(deleted);
            return 1;
        } else {
            if (onLeft(deleted)) {
                deleted->parent->left = replacement;
            } else {
                deleted->parent->right = replacement;
            }
            replacement->parent = deleted->parent;
            free(deleted);
            tree->size--;
            if (bothBlack) {
                removeFixup(tree, replacement);
            } else {
                replacement->color = BLACK;
            }
            return 1;
        }
    }

    swapValues(deleted, replacement);
    return deleteNode(tree, replacement);
}

static struct Node *findReplacement(struct Node *node)
{
    if (node->left != NULL && node->right != NULL) {
        struct Node *current = node->right;
        while (current->left) {
            current = current->left;
        }
        return current;
    }
    if (node->left == NULL && node->right == NULL) {
        return NULL;
    }
    if (node->left != NULL) {
        return node->left;
    } else {
        return node->right;
    }
}

static void swapColors(struct Node *n1, struct Node *n2)
{
    Color color = n1->color;
    n1->color = n2->color;
    n2->color = color;
}

static void swapValues(struct Node *n1, struct Node *n2)
{
    T value = n1->value;
    n1->value = n2->value;
    n2->value = value;
}

static void insertFixup(Tree *tree, struct Node *node)
{
    struct Node *parent = NULL;
    struct Node *grand_parent = NULL;
    struct Node *uncle = NULL;
    while ((node != tree->root) && (node->color != BLACK) && (node->parent->color == RED)) {
        parent = node->parent;
        grand_parent = node->parent->parent;
        // Узел находится в левом поддереве дедушки.
        if (parent == grand_parent->left) {
            uncle = grand_parent->right;
            // Дядя узла тоже красный -> нужно только перекрасить
            if (uncle != NULL && uncle->color == RED) {
                grand_parent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grand_parent;
            } else {
                // Узел - правый ребенок родителя, нужен левый поворот
                if (node == parent->right) {
                    rotateLeft(tree, parent);
                    node = parent;
                    parent = node->parent;
                }
                // Узел - левый ребенок родителя, нужен правый поворот
                rotateRight(tree, grand_parent);
                swapColors(parent, grand_parent);
                node = parent;
            }
        } else { // Узел находится в правом поддереве дедушки
            uncle = grand_parent->left;
            // Дядя узла тоже красный -> нужно только перекрасить
            if ((uncle != NULL) && uncle->color == RED) {
                grand_parent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grand_parent;
            } else {
                // Узел - левый ребенок родителя, нужен правый поворот
                if (node == parent->left) {
                    rotateRight(tree, parent);
                    node = parent;
                    parent = node->parent;
                }
                // Узел - правый ребенок родителя, нужен левый поворот
                rotateLeft(tree, grand_parent);
                swapColors(parent, grand_parent);
                node = parent;
            }
        }
    }
    // Корень всегда черный
    tree->root->color = BLACK;
}

static void rotateLeft(Tree *tree, struct Node *node)
{
    struct Node* right = node->right;
    node->right = right->left;
    if (node->right) {
        node->right->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
}

static void rotateRight(Tree *tree, struct Node *node)
{
    struct Node* left = node->left;
    node->left = left->right;
    if (node->left) {
        node->left->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = left;
    } else if (node == node->parent->left) {
        node->parent->left = left;
    } else {
        node->parent->right = left;
    }
    left->right = node;
    node->parent = left;
}

static void printOrdered(struct Node *node)
{
    if (node == NULL) return;
    printOrdered(node->left);
    printf("%d ", node->value);
    printOrdered(node->right);
}

static struct Node *placeNode(struct Node *node, T element)
{
    if (element == node->value) return NULL;
    if (element > node->value) {
        if (node->right == NULL) {
            struct Node *new = makeNode(element);
            node->right = new;
            new->color = RED;
            new->parent = node;
            return new;
        }
        return placeNode(node->right, element);
    } else {
        if (node->left == NULL) {
            struct Node *new = makeNode(element);
            node->left = new;
            new->color = RED;
            new->parent = node;
            return new;
        }
        return placeNode(node->left, element);
    }
}

static void freeNode(struct Node *node)
{
    if (node->left) freeNode(node->left);
    if (node->right) freeNode(node->right);
    free(node);
}

static struct Node *makeNode(T value)
{
    struct Node *temp = (struct Node *) malloc(sizeof (struct Node));
    temp->value = value;
    temp->parent = NULL;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

static struct Node *findNode(struct Node *node, T element)
{
    if (element == node->value) return node;
    if (element > node->value) {
        return node->right == NULL ? NULL : findNode(node->right, element);
    } else {
        return node->left == NULL ? NULL : findNode(node->left, element);
    }
}

static struct Node *getSibling(struct Node *node)
{
    if (node->parent == NULL) {
        return NULL;
    }
    if (onRight(node)) {
        return node->parent->left;
    } else {
        return node->parent->right;
    }
}

static int onLeft(struct Node *node)
{
    return node->parent->left == node;
}

static int onRight(struct Node *node)
{
    return node->parent->right == node;
}

static void removeFixup(Tree *tree, struct Node *node)
{
    if (node == tree->root) {
        return;
    }

    struct Node *sibling = getSibling(node);
    struct Node *parent = node->parent;

    if (sibling == NULL) {
        removeFixup(tree, parent);
    } else {
        if (sibling->color == RED) {
            parent->color = RED;
            sibling->color = BLACK;
            if (onLeft(sibling)) {
                rotateRight(tree, parent);
            } else {
                rotateLeft(tree, parent);
            }
            removeFixup(tree, node);
        } else {
            if (sibling->left != NULL && sibling->left->color == RED) {
                if (onLeft(sibling)) {
                    sibling->left->color = sibling->color;
                    sibling->color = parent->color;
                    rotateRight(tree, parent);
                } else {
                    sibling->left->color = parent->color;
                    rotateRight(tree, sibling);
                    rotateLeft(tree, parent);
                }
                parent->color = BLACK;
            } else if (sibling->right != NULL && sibling->right->color == RED) {
                if (onRight(sibling)) {
                    sibling->right->color = sibling->color;
                    sibling->color = parent->color;
                    rotateLeft(tree, parent);
                } else {
                    sibling->right->color = parent->color;
                    rotateLeft(tree, sibling);
                    rotateRight(tree, parent);
                }
                parent->color = BLACK;
            } else {
                sibling->color = RED;
                if (parent->color == BLACK) {
                    removeFixup(tree, parent);
                } else {
                    parent->color = BLACK;
                }
            }
        }
    }
}