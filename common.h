//
// Created by inna- on 17.01.2022.
//

#ifndef EXAMPLE_COMMON_H
#define EXAMPLE_COMMON_H

#include <stdint-gcc.h>

int **alloc_2d_field(uint16_t rows, uint16_t columns);
void free_2d_field(int **ptr, int size);
#endif //EXAMPLE_COMMON_H
