#ifndef HPS2_MPI_UTILS_H
#define HPS2_MPI_UTILS_H

#include "stdio.h"
#include "sandPile.h"
#include <stdbool.h>
void log_from_0_pid(char *msg, int rank, FILE *dst);

void send_result(int **result, uint16_t rows, uint16_t columns);
void receive_part(int *buf, uint16_t columns, int src_rank, bool is_sender_above);
void receive_result(int rank, uint16_t rows, sandPileFields *field, uint16_t start_row);
void send_part_async(int *row, uint16_t columns, int dest_rank, bool is_sender_above);

#endif //HPS2_MPI_UTILS_H
