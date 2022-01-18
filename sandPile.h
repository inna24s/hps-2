//
// Created by inna- on 14.01.2022.
//

#ifndef EXAMPLE_SANDPILE_H
#define EXAMPLE_SANDPILE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint-gcc.h>

typedef struct {
    char *filepath;
    uint32_t cycle;
    uint32_t size;
    int **data;
} sandPileFields;
FILE* openFile(char* fileName, char *mode);
sandPileFields* read_field(FILE *fileStart, int size);

int **finalSandPile(int **part, int rows, int size, int rows_res, int pid);
void finalSandPile1(int **part, int rows, int size, char* fileName, int cycles);
uint16_t get_field_part_coords(sandPileFields *field, int part_num, int total_parts, uint16_t *start_row_num,
                          uint16_t *row_size);
void divide_by_parts(uint16_t *arr, int size, uint32_t dividend);
void get_field_part(sandPileFields *field, uint16_t row_size_res, uint16_t start_row_num_res, int **arr);

void writeResult(char *fileName, int** next_part, int row_size_res, int size, int rank, int i);
char* findRank(int rank);
#endif //EXAMPLE_SANDPILE_H
