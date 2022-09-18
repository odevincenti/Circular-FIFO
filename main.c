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

    /**************************** EX 1 *****************************/
    // Write string to buffer
    fifo_value_t w_arr[] = "Hello World!";
    size_t r = FIFO_WriteToBuffer(id, &w_arr[0], sizeof w_arr);
    if (r == sizeof w_arr){
        printf("Write OK\n");
    } else {
        printf("Write 0 ERR, buffer full: wrote %u values\n", r);
    }

    // Read from buffer, 8 values at a time
    fifo_value_t r_arr[BULK_READ_SIZE];
    for (uint8_t i = 0; i < (sizeof w_arr)/(sizeof r_arr) + 1; i++){
        for (uint8_t i = 0; i < BULK_READ_SIZE + 1; i++){ r_arr[i] = 0; }
        r = FIFO_ReadFromBuffer(id, &r_arr[0], sizeof r_arr);
        if (r == sizeof r_arr){
            printf("Read %u: ", i + 1);
            printf("%.*s\n", BULK_READ_SIZE, r_arr);
        } else {
            printf("Read %u: ", i + 1);
            printf("%.*s\n", BULK_READ_SIZE, r_arr);
            printf("Buffer Empty\n");
        }
    }

    /**************************** EX 2 *****************************/
    // Write string to buffer
    fifo_value_t w1_arr[] = "Hey there demons, it's me, ya boi";
    if(FIFO_WriteToBuffer(id, &w1_arr[0], sizeof w1_arr) != sizeof w1_arr){
        printf("Write 1 ERR, buffer full");
    }
    printf("Write 1: %s\n", w1_arr);

    // Read all values in buffer
    fifo_value_t r1_arr[sizeof w1_arr];
    r = FIFO_ReadAll(id, &r1_arr[0]);
    printf("Read all: %s\n", r1_arr);
    printf("Read %u values\n", r);

    /**************************** EX 3 *****************************/
    // Write string to buffer
    fifo_value_t w2_arr[] = "Hello there - GENERAL KENOBI";
    r = FIFO_WriteToBuffer(id, &w2_arr[0], sizeof w2_arr);
    printf("Write 2: %s\n", w2_arr);

    // Read whole string
    fifo_value_t r2_arr[BULK_READ_SIZE * 4];
    for (uint8_t i = 0; i < sizeof r2_arr + 1; i++){ r2_arr[i] = 0; }

    r = FIFO_ReadFromBuffer(id, &r2_arr[0], sizeof r2_arr);
    if (r == sizeof r2_arr){
        printf("%.*s\n", sizeof r2_arr, r2_arr);
    } else {
        printf("%.*s\n", sizeof r2_arr, r2_arr);
        printf("Buffer Empty");
    }

    return 0;
}
