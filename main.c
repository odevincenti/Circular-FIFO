/***************************************************************************//**
  @file     main.c
  @brief    Circular FIFO usage example
  @author   Olivia De Vincenti
 ******************************************************************************/

#include <stdio.h>
#include "fifo.h"

#define BULK_READ_SIZE  8

int main (void)
{
    // Request FIFO
    fifo_id_t id = FIFO_GetId();

    // Write string to buffer
    fifo_value_t w_arr[] = "Hello World!";
    size_t r = FIFO_WriteToBuffer(id, &w_arr[0], sizeof w_arr);
    if (r == sizeof w_arr){
        printf("Write OK\n");
    } else {
        printf("Write ERR: wrote %u values\n", r);
    }

    // Read from buffer, 8 values at a time
    fifo_value_t r_arr[BULK_READ_SIZE];
    for (uint8_t i = 0; i < (sizeof w_arr)/(sizeof r_arr) + 1; i++){
        for (uint8_t i = 0; i < BULK_READ_SIZE + 1; i++){ r_arr[i] = 0; }
        r = FIFO_ReadFromBuffer(id, &r_arr[0], sizeof r_arr);
        if (r == sizeof r_arr){
            printf("Read %u values: ", i + 1);
        } else {
            printf("Buffer Empty: ");
        }
        printf("%.*s\n", BULK_READ_SIZE, r_arr);
    }

    // Write string to buffer
    fifo_value_t w2_arr[] = "Hello there - GENERAL KENOBI";
    r = FIFO_WriteToBuffer(id, &w2_arr[0], sizeof w2_arr);
    if (r == sizeof w2_arr){
        printf("Write 2 OK\n");
    } else {
        printf("Write ERR: wrote %u values\n", r);
    }    

    // Read whole string
    fifo_value_t r2_arr[BULK_READ_SIZE * 4];
    for (uint8_t i = 0; i < sizeof r2_arr + 1; i++){ r2_arr[i] = 0; }

    for (uint8_t i = 0; i < (sizeof w2_arr)/(sizeof r2_arr) + 1; i++){
        r = FIFO_ReadFromBuffer(id, &r2_arr[0], sizeof r2_arr);
        if (r == sizeof r2_arr){
            printf("Read %u values: ", i);
        } else {
            printf("Buffer Empty: ");
        }
        printf("%.*s\n", sizeof r2_arr, r2_arr);
    }

    return 0;
}
