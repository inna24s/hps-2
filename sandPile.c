//
// Created by inna- on 14.01.2022.
//
#include <stdlib.h>
#include "sandPile.h"
#include "common.h"
#include <math.h>
#include <malloc.h>
#include <string.h>

int **finalSandPile(int **part, int rows, int size, int rows_res, int rank) {
    int top = 0, down = 0, left = 0, right = 0;
    for (int i = 0; i < rows_res; i++) {
        for (int j = 0; j <= size; j++) {
            if (part[i][j] >= 4) {
                if (i - 1 >= 0) {
                    top = 1;
                    part[i - 1][j] += 1;
                }
                if (i + 1 < (size + 1)) {
                    down = 1;
                    part[i + 1][j] += 1;
                }
                if (j - 1 >= 0) {
                    left = 1;
                    part[i][j - 1] += 1;
                }
                if (j + 1 < (size + 1)) {
                    right = 1;
                    part[i][j + 1] += 1;
                }
                part[i][j] -= (top + down + left + right);
//                printf("proc%d [%d][%d] = %d\n", rank, i, j, part[i][j]);
            }
        }
//        printf("end %d\n", i);
    }
//    printf("END\n");
    return part;
}
void finalSandPile1(int **part, int rows, int size, char* fileName, int cycles){
    int processAgain = 1, top, down, left, right;
    int cycle = 1;
    FILE *fileResults = openFile(fileName, "w");
    while(processAgain == 1 && cycle< cycles+1){
    fprintf(fileResults, "cycle %d\n", cycle);
        processAgain = 0;
        top = 0;
        down = 0;
        left = 0;
        right = 0;

        for(int i=0;i<=size;i++){
            for(int j=0;j<=rows;j++){
                if(part[i][j]>=4){
                    if(i-1>=0){
                        top = 1;
                        part[i-1][j]+=1;
                        if(part[i-1][j]>=4)
                            processAgain = 1;
                    }
                    if(i+1<(size+1)){
                        down = 1;
                        part[i+1][j]+=1;
                        if(part[i+1][j]>=4)
                            processAgain = 1;
                    }
                    if(j-1>=0){
                        left = 1;
                        part[i][j-1]+=1;
                        if(part[i][j-1]>=4)
                            processAgain = 1;
                    }
                    if(j+1<(size+1)){
                        right = 1;
                        part[i][j+1]+=1;
                        if(part[i][j+1]>=4)
                            processAgain = 1;
                    }
                    part[i][j] -= (top + down + left + right);
                    if(part[i][j]>=4)
                        processAgain = 1;
                }
            }
        }
        cycle++;
        for (int i = 0; i < size+1; i++) {
            for (int j = 0; j < size + 1; j++)
                fprintf(fileResults, "%d ", part[i][j]);
            fprintf(fileResults, "\n");
        }
    }
    fclose(fileResults);
//    for(int i=0; i<= size; i++)
//        printf("%d", part[i][0]);
//    printf("\n");
}

FILE *openFile(char *fileName, char *mode) {
    FILE *file;
    if ((file = fopen(fileName, mode)) == NULL) {
        printf("Errror: Ошибка при открытии файла %s для чтения\nPress any key", fileName);
        return NULL; // выход из функции main
    } else return file;
}

sandPileFields *read_field(FILE *fileStart, int size) {
    int **sandPile = alloc_2d_field(size, size);
    int k = 0;
    rewind(fileStart);
    for (int i = 0; i <= size; i++)
        for (int j = 0; j <= size; j++)
            sandPile[i][j] = 0;

    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= size; j++) {
            fscanf(fileStart, "%d", &sandPile[i][j]);
//            printf("%d ", sandPile[i][j]);
        }
        fgetc(fileStart);
//        printf("\n");
    }
    fclose(fileStart);

    sandPileFields *sandPileFields1 = calloc(1, sizeof(sandPileFields));
    sandPileFields1->data = sandPile;
    sandPileFields1->size = size;
    sandPileFields1->cycle = 0;
    return sandPileFields1;
}

uint16_t get_field_part_coords(sandPileFields *field, int part_num, int total_parts, uint16_t *start_row_num,
                               uint16_t *row_size) {
    if (part_num > total_parts) {
        return -1;
    }
    uint16_t sizes[total_parts];
    if (total_parts > field->size) {
        for (int i = 0; i < field->size; ++i) {
            sizes[i] = 1; //по размеру массива
        }
    } else {
        divide_by_parts(sizes, total_parts, field->size);
    }
    for (int i = 0; i < part_num; ++i) {
        *start_row_num = *start_row_num + sizes[i];
    }
    *row_size = sizes[part_num];
    return 0;
}

void divide_by_parts(uint16_t *arr, int size, uint32_t dividend) {
    uint32_t total_rows = dividend + 1;
//    printf("total rows %d\n", total_rows);
    uint16_t mean_part_size = floor(((double) dividend + 1) / ((double) size));
    for (int i = 0; i < size; ++i) {
        arr[i] = mean_part_size;
        total_rows -= mean_part_size;
    }
    for (int i = 0; i < size; ++i) {
        if (total_rows > 0) {
            arr[i] += 1;
            total_rows--;
        }
    }
}

void get_field_part(sandPileFields *field, const uint16_t row_size_res, const uint16_t start_row_num_res, int **arr) {
    for (int i = 0; i < row_size_res; ++i) {
        memcpy(arr[i], field->data[start_row_num_res + i], sizeof(int) * field->size);
    }
}

void writeResult(char *fileName, int **next_part, int row_size_res, int size, int rank, int cycle) {
    FILE *fileResults;
    fileResults = openFile(fileName, "a");
    fprintf(fileResults, "Cycle %d process%d\n", cycle,rank);
    for (int i = 0; i < row_size_res; i++) {
        for (int j = 0; j < size + 1; j++)
            fprintf(fileResults, "%d ", next_part[i][j]);
        fprintf(fileResults, "\n");
    }
    fclose(fileResults);
}

char* findRank(int rank) {
    char* w = calloc(1, sizeof(char));
    switch (rank) {
        case 0:
            w[0] = '0';
            break;
        case 1:
            w[0] = '1';
            break;
        case 2:
            w[0] = '2';
            break;
        case 3:
            w[0] = '3';
            break;
        case 4:
            w[0] = '4';
            break;
        case 5:
            w[0] = '5';
            break;
        case 6:
            w[0] = '6';
            break;
        case 7:
            w[0] = '7';
            break;
        case 8:
            w[0] = '8';
            break;
        default:
            break;
    }

    return w;
}