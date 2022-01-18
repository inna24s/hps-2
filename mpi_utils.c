//
// reated by inna- on 04.01.2022.
//

#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "mpi_utils.h"
#include "sandPile.h"

#define PART_ABOVE_TAG 13
#define PART_BELOW_TAG 14
#define RESULT_TAG 15


void log_from_0_pid(char *msg, int rank, FILE *dst) {
    if (rank == 0) {
        fputs(msg, dst);
    }
}

void send_part_async(int *row, uint16_t columns, int dest_rank, bool is_sender_above) {
    MPI_Request request;
    int tag = is_sender_above ? PART_ABOVE_TAG : PART_BELOW_TAG;
    MPI_Isend(row, columns, MPI_C_BOOL, dest_rank, tag, MPI_COMM_WORLD, &request);
}

void receive_part(int *buf, uint16_t columns, int src_rank, bool is_sender_above) {
    MPI_Status status;
    int tag = is_sender_above ? PART_ABOVE_TAG : PART_BELOW_TAG;
    MPI_Recv(buf, columns, MPI_C_BOOL, src_rank, tag, MPI_COMM_WORLD, &status);
}

void send_result(int **result, uint16_t rows, uint16_t columns) {
    bool buf[rows * columns];
    MPI_Request request;
    for (int i = 0; i < rows; ++i) {
        memcpy(&buf[i * columns], result[i], columns * sizeof(bool));
    }
    MPI_Isend(buf, rows * columns, MPI_C_BOOL, 0, RESULT_TAG, MPI_COMM_WORLD, &request);
}

void receive_result(int rank, uint16_t rows, sandPileFields *field, uint16_t start_row) {
    bool buf[rows * field->size];
    MPI_Status status;
    MPI_Recv(buf, rows * field->size, MPI_C_BOOL, rank, RESULT_TAG, MPI_COMM_WORLD, &status);
    for (int i = 0; i < rows; ++i) {
        memcpy(field->data[start_row + i], &buf[i * field->size], field->size * sizeof(bool));
    }
}



