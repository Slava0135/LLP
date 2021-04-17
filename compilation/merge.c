#include <malloc.h>
#include <stddef.h>

int* merge(const int array1[], const size_t length1, const int array2[], const size_t length2)
{
    const size_t length = length1 + length2;

    int* result = (int *) malloc(sizeof array1[0] * length);

    int index1 = 0, index2 = 0;
    for (int index = 0; index < length; index++) {
        if (index2 >= length2 || (index1 < length1 && array1[index1] < array2[index2])) {
            result[index] = array1[index1];
            index1++;
        } else {
            result[index] = array2[index2];
            index2++;
        }
    }

    return result;
}
