//
// Created by inna- on 14.01.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mpi/mpi.h"
#include "mpi_utils.h"
#include "sandPile.h"
#include "common.h"

int main(int argc, char **argv) {
    int rank, pids;
    int buffer;
    uint16_t size = 0;
    int **sandPile;
    FILE *fileStart, *fileResults;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &pids);//количество процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//номер конкретного процесса
    if (argc < 4) {
        log_from_0_pid("Not enough arguments\nUsage: <src_file_path> <cycle_count> <dest_dir_path>", rank, stderr);
        return 0;
    }

    uint32_t cycles = strtol(argv[2], NULL, 10);

    if ((fileStart = openFile(argv[1], "rt")) == NULL) return 0;
    if ((fileResults = openFile(argv[3], "wt")) == NULL) return 0;
    fclose(fileResults);
    while (!feof(fileStart)) {
        buffer = fgetc(fileStart);
        if (buffer == '\n') size++;
    }
    sandPileFields *sandPileField = read_field(fileStart, size);
    sandPileField->filepath = argv[1];

    uint16_t start_row_num_res = 0, row_size_res = 0;
    int res = get_field_part_coords(sandPileField, rank, pids, &start_row_num_res, &row_size_res);
    if (res == -1) {
        puts("Field is unsplittable");
        return EXIT_FAILURE;
    }

    int **part_field = alloc_2d_field(row_size_res, sandPileField->size);

    get_field_part(sandPileField, row_size_res, start_row_num_res, part_field);

    int **next_part = finalSandPile(part_field, start_row_num_res, size, row_size_res, rank);
//    printf("final proc%d", rank);

    char *w = findRank(rank);
    char *w1 = calloc(sizeof(argv[3]) + 1, sizeof(char));
    strncat(w1, argv[3], sizeof(argv[3]));
    strncat(w1, w, 1);

    writeResult(w1, next_part, row_size_res, size, rank, 1);
//    writeResult(argv[3], next_part, row_size_res, size, rank, 1, 0);
    finalSandPile1(sandPileField->data, size, size, argv[3], cycles);
    for (int i = 2; i <= cycles; ++i) {
        uint16_t rank_above = (rank + pids - 1) % pids;
        uint16_t rank_below = (rank + 1) % pids;
        send_part_async(next_part[0], sandPileField->size, rank_above, true);
        send_part_async(next_part[row_size_res - 1], sandPileField->size, rank_below, false);
        int row_above[sandPileField->size];
        int row_below[sandPileField->size];
        receive_part(row_below, sandPileField->size, rank_below, true);
        receive_part(row_above, sandPileField->size, rank_above, false);
        next_part = finalSandPile(part_field, start_row_num_res, size, row_size_res, rank);
        writeResult(w1, next_part, row_size_res, size, rank, i);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}