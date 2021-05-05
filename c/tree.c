#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "tree.h"

#define maxOf(a, b) a > b ? a : b

#define MAX_PRINT_DEPTH 31
#define MAX_PRINT_WIDTH 255

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

static int maxDepth(struct Node *);
static int validateNode(struct Node *);
static void getElement(struct Node *);

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

static int drawLayer(struct Node *, int, int, int, char out[MAX_PRINT_DEPTH][MAX_PRINT_WIDTH]);

Tree *makeTree()
{
    Tree *temp = (Tree *) malloc(sizeof (struct Tree_));
    temp->size = 0;
    temp->root = NULL;
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

int contains(Tree *tree, T element)
{
    if (tree->root == NULL) return 0;
    return findNode(tree->root, element) != NULL;
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

void drawTree(Tree *tree)
{

    if (getDepth(tree) >= MAX_PRINT_DEPTH / 2) {
        printf("TREE IS TOO DEEP TO DRAW\n");
        return;
    }

    char out[MAX_PRINT_DEPTH][MAX_PRINT_WIDTH];
    for (int i = 0; i < MAX_PRINT_DEPTH; i++) {
        int j;
        for (j = 0; j < MAX_PRINT_WIDTH - 1; j++) {
            out[i][j] = ' ';
        }
        out[i][j] = '\0';
    }

    if (drawLayer(tree->root, 0, 0, 0, out) == -1) {
        printf("TREE IS TOO WIDE TO DRAW\n");
        return;
    }

    int depth = 0;
    for (int i = 0; i < MAX_PRINT_DEPTH; i++) {
        int last = 0;
        for (int j = 0; j < MAX_PRINT_WIDTH - 2; j++) {
            if (out[i][j] != ' ') {
                last = j;
            }
        }
        if (last == 0) {
            break;
        }
        out[i][last + 1] = '\0';
        depth++;
    }

    for (int i = 0; i < depth; i++) {
        printf("%s\n", out[i]);
    }
}

static T *pointer;
void getElements(Tree *tree, T *array)
{
    pointer = array;
    getElement(tree->root);
}

static void getElement(struct Node *node)
{
    if (node == NULL) return;
    getElement(node->left);
    *(pointer++) = node->value;
    getElement(node->right);
}

static int validateNode(struct Node *node)
{
    if (node == NULL) return 0;
    if (node->color == RED) {
        if ((node->left != NULL && node->left->color == RED) || (node->right != NULL && node->right->color == RED)) {
            return -1;
        }
    }
    const int left = validateNode(node->left);
    const int right = validateNode(node->right);
    if (left == -1 || right == -1 || left != right) return -1;
    const int add = node->color == BLACK ? 1 : 0;
    return left + add;
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
    // Удаляемый узел - лист
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
    // Есть 1 ребенок
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
    // Есть два ребенка - меняем значения с узлом (с самым маленьким значением в правом поддереве) и удаляем рекурсивно
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
    struct Node *grandParent = NULL;
    struct Node *uncle = NULL;
    while ((node != tree->root) && (node->color != BLACK) && (node->parent->color == RED)) {
        parent = node->parent;
        grandParent = node->parent->parent;
        // Узел находится в левом поддереве дедушки.
        if (parent == grandParent->left) {
            uncle = grandParent->right;
            // Дядя узла тоже красный -> нужно только перекрасить
            if (uncle != NULL && uncle->color == RED) {
                grandParent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandParent;
            } else {
                // Узел - правый ребенок родителя, нужен левый поворот
                if (node == parent->right) {
                    rotateLeft(tree, parent);
                    node = parent;
                    parent = node->parent;
                }
                // Узел - левый ребенок родителя, нужен правый поворот
                rotateRight(tree, grandParent);
                swapColors(parent, grandParent);
                node = parent;
            }
        } else { // Узел находится в правом поддереве дедушки
            uncle = grandParent->left;
            // Дядя узла тоже красный -> нужно только перекрасить
            if ((uncle != NULL) && uncle->color == RED) {
                grandParent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandParent;
            } else {
                // Узел - левый ребенок родителя, нужен правый поворот
                if (node == parent->left) {
                    rotateRight(tree, parent);
                    node = parent;
                    parent = node->parent;
                }
                // Узел - правый ребенок родителя, нужен левый поворот
                rotateLeft(tree, grandParent);
                swapColors(parent, grandParent);
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
            removeFixup(tree, node); // Делаем то же самое, но брат теперь черный
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
            } else { // Оба ребенка брата черные
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

static int drawLayer(struct Node *node, int onLeft, int offset, int depth, char out[MAX_PRINT_DEPTH][MAX_PRINT_WIDTH])
{
    if (node == NULL) return 0;
    if (offset >= MAX_PRINT_WIDTH) return -1;

    const int width = 7;
    char value[width + 1];
    sprintf(value, "<%04d%c>", node->value, node->color == BLACK ? 'B' : 'R');

    const int left = drawLayer(node->left,1, offset,depth + 1, out);
    const int right = drawLayer(node->right,0, offset + left + width, depth + 1, out);

    if(right == -1) return -1;

    for (int i = 0; i < width; i++) {
        out[2 * depth][offset + left + i] = value[i];
    }

    if (depth > 0) {
        if (onLeft) {
            for (int i = 0; i < width + right; i++)
                out[2 * depth - 1][offset + left + width/2 + i] = '-';
            out[2 * depth - 1][offset + left + width/2] = '+';
            out[2 * depth - 1][offset + left + width + right + width/2] = '+';
        } else {
            for (int i = 0; i < left + width; i++)
                out[2 * depth - 1][offset - width/2 + i] = '-';
            out[2 * depth - 1][offset + left + width/2] = '+';
            out[2 * depth - 1][offset - width/2 - 1] = '+';
        }
    }

    return left + width + right;
}