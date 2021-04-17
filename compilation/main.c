#include "merge.h"

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

static void printArray(const int* array, const size_t length)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    const int array1[] = {2, 5, 6, 8};
    const int array2[] = {0, 1, 3, 4, 7, 9, 10};

    const size_t length1 = sizeof array1 / sizeof array1[0];
    const size_t length2 = sizeof array2 / sizeof array2[0];

    printArray(array1, length1);
    printArray(array2, length2);

    int *merged = merge(array1, length1, array2, length2);
    printArray(merged, length1 + length2);

    free(merged);
    return 0;
}