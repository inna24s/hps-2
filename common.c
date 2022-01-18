//
// Created by inna- on 17.01.2022.
//

#include "common.h"
#include <malloc.h>
#include <stdint-gcc.h>

int **alloc_2d_field(uint16_t rows, uint16_t columns) {
    int **result = calloc(rows+1, sizeof(int *));
    for (int i = 0; i < rows+1; ++i) {
        result[i] = calloc(columns+1, sizeof(int));
    }
    return result;
}

void free_2d_field(int **ptr, int size) {
    for (int i = 0; i < size; ++i) {
        free(ptr[i]);
    }
    free(ptr);
}