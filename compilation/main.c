#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include "merge.h"

static int array1[] = {2, 5, 6, 8};
static int array2[] = {0, 1, 3, 4, 7, 9, 10};

static size_t length1 = sizeof array1 / sizeof array1[0];
static size_t length2 = sizeof array2 / sizeof array2[0];

int main()
{
    int *merged = merge(array1, length1, array2, length2);
    for (int i = 0; i < length1 + length2; i++) {
        printf("%d ", merged[i]);
    }
    free(merged);
    return 0;
}

