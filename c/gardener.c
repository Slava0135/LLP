#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"

#define forever for(;;)

static void work();
static void removeSuffix(char *);
static void printTree(Tree *tree);

int main(int argc, char *argv[])
{
    switch (argc) {
        case 1: {
            work();
            return 0;
        }
        case 2: {
            if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
                printf(""
                       "Usage: gardener\n"
                       "Creates a tree and reads following commands from stdin:\n"
                       "    insert [INT]...     insert an element to the tree\n"
                       "    remove [INT]...     remove an element from the tree\n"
                       "    contains [INT]      checks if an element in the tree\n"
                       "    elements            writes a list of elements\n"
                       "    size                writes a size of the tree\n"
                       "    depth               writes a depth of the tree\n"
                       "    valid               checks if the tree is a valid Red-Black tree\n"
                       "Output is printed to stdout.\n"
                       "Enter $ to finish the program.\n"
                );
                return 0;
            } else {
                printf("INVALID ARGUMENTS");
                return -1;
            }
        }
        default: {
            printf("TOO MANY ARGUMENTS");
            return -1;
        }
    }
}

static void work()
{
    Tree *tree = makeTree();

    char *line = NULL;
    size_t size;
    ssize_t length;
    forever {
        length = getline(&line, &size, stdin);
        if (length > 0) {
            removeSuffix(line);
            if (strcmp(line, "$") == 0) break;
            char *command = strsep(&line, " ");
            char *token;
            if (strcmp(command, "insert") == 0) {
                while ((token = strsep(&line, " "))) {
                    int arg = (int) strtol(token, (char **) NULL, 10);
                    insertElement(tree, arg);
                }
            } else if (strcmp(command, "remove") == 0) {
                while ((token = strsep(&line, " "))) {
                    int arg = (int) strtol(token, (char **) NULL, 10);
                    removeElement(tree, arg);
                }
            } else if (strcmp(command, "contains") == 0) {
                token = strsep(&line, " ");
                int arg = (int) strtol(token, (char **) NULL, 10);
                printf("%s %d\n", contains(tree, arg) ? "Tree does contain" : "Tree doesn't contain", arg);
            } else if (strcmp(command, "elements") == 0) {
                printTree(tree);
            } else if (strcmp(command, "size") == 0) {
                printf("Tree has %d elements\n", getSize(tree));
            } else if (strcmp(command, "depth") == 0) {
                printf("Tree is %d node deep\n", getDepth(tree));
            } else if (strcmp(command, "valid") == 0) {
                printf("%s\n", validate(tree) ? "Tree is valid" : "Tree is not valid");
            } else {
                printf("INVALID COMMAND\n");
            }
        }
    }
    freeTree(tree);
}

static void removeSuffix(char *line) {
    strtok(line, "\n");
}

static void printTree(Tree *tree) {
    if (getSize(tree) == 0) printf("Tree is empty");
    int *array = (int *) malloc(getSize(tree) * sizeof (int));
    getElements(tree, array);
    for (int i = 0; i < getSize(tree); i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    free(array);
}