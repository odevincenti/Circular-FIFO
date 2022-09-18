/***************************************************************************//**
  @file     test.c
  @brief    Main to test circular FIFO
  @author   Olivia De Vincenti
 ******************************************************************************/

#include <stdio.h>
#include "fifo.h"

#define BULK_READ_SIZE  8

int main (void)
{
    fifo_id_t id = FIFO_GetId();
    fifo_value_t w_arr[] = "Hello World! I am testing the circular FIFO, now I want to see what happens when I exceed the 256 value limit. I hope everything goes OK and nothing breaks down, if that happens it would be a problem because I'm hungry and tired and I deserve a break from the computer.";
    size_t r = FIFO_WriteToBuffer(id, &w_arr[0], sizeof w_arr);
    if (r == sizeof w_arr){
        printf("Write OK\n");
    } else {
        printf("Write ERR: wrote %u values\n", r);
    }
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

    for (int i = 0; i < BULK_READ_SIZE + 1; i++){ r_arr[i] = 0; }
    fifo_value_t w2_arr[] = "Hello there, GENERAL KENOBI";
    r = FIFO_WriteToBuffer(id, &w2_arr[0], sizeof w2_arr);
    if (r == sizeof w2_arr){
        printf("Write 2 OK\n");
    } else {
        printf("Write ERR: wrote %u values\n", r);
    }    

    fifo_value_t r2_arr[BULK_READ_SIZE * 4];
    r = FIFO_ReadAll(id, &r2_arr[0]);
    printf("Read all: %s\n", r2_arr);
    printf("Read %u values", r);

    return 0;
}
