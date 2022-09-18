/***************************************************************************//**
  @file     fifo.c
  @brief    Software circular FIFO implementation
  @author   Olivia De Vincenti
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include "fifo.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
	fifo_value_t    queue[MAX_FIFO_SIZE];
	size_t          head;
	size_t          tail;
    bool            is_buffer_full;
} fifo_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void init_fifo(fifo_id_t id);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static fifo_t FIFO_Array[FIFO_MAX_N];   // FIFO Array
static fifo_id_t fifo_n = 0;            // Amount of active FIFO queues

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

fifo_id_t FIFO_GetId(void)
{
#ifdef FIFO_DEVELOPMENT_MODE
    if (fifo_n >= FIFO_MAX_N)
    {
        return FIFO_INVALID_ID;
    }
    else
#endif // FIFO_DEVELOPMENT_MODE
    {
        init_fifo(fifo_n);
        return fifo_n++;
    }
}

bool FIFO_IsBufferFull(fifo_id_t id){

    return FIFO_Array[id].is_buffer_full;
}

bool FIFO_IsBufferEmpty(fifo_id_t id){

    return FIFO_Array[id].head == FIFO_Array[id].tail && !FIFO_Array[id].is_buffer_full;
}

size_t FIFO_WriteToBuffer(fifo_id_t id, fifo_value_t* data, size_t data_size){
    
    size_t i;

    for (i = 0; i < data_size && i < MAX_FIFO_SIZE; i++){

        if (FIFO_PushToBuffer(id, *(data + i)) == FIFO_BUFFER_FULL){      // Push value to buffer, if buffer is full:
            break;              // Stop writing
        }
    }

    // printf("WRITE: %s\n", FIFO_Array[id].queue);

    return i;
}

size_t FIFO_ReadFromBuffer(fifo_id_t id, fifo_value_t* data_ptr, uint16_t data_size){

    size_t i;

    for (i = 0; i < data_size && i < MAX_FIFO_SIZE; i++){

        if (FIFO_PullFromBuffer(id, data_ptr + i) == FIFO_BUFFER_EMPTY){      // Pull value to buffer, if buffer is empty:
            break;              // Stop reading
        }
    }

    return i;
}

bool FIFO_PushToBuffer(fifo_id_t id, fifo_value_t data){

    if (!FIFO_Array[id].is_buffer_full){                        // If buffer is not full

        *((fifo_value_t*)(&FIFO_Array[id].queue[0] + FIFO_Array[id].head)) = data;   // Write data

        if (FIFO_Array[id].head + 1 < MAX_FIFO_SIZE){           // Overflow?
            FIFO_Array[id].head++;                              // Advance head
        } else {
            FIFO_Array[id].head = 0;                            // Reset head
        }

        if (FIFO_Array[id].head == FIFO_Array[id].tail ){       // If buffer is now full
            FIFO_Array[id].is_buffer_full = FIFO_BUFFER_FULL;   // Set flag
        }

    }

    return FIFO_Array[id].is_buffer_full;       // Return buffer state
}

bool FIFO_PullFromBuffer(fifo_id_t id, fifo_value_t* data_ptr){

    if (FIFO_Array[id].head == FIFO_Array[id].tail && !FIFO_Array[id].is_buffer_full){    // If buffer is empty

        return FIFO_BUFFER_EMPTY;       // Return buffer empty state and don't write in data_ptr*
    
    } else {                            // If buffer is not empty

        *(data_ptr) = *((fifo_value_t*) (&FIFO_Array[id].queue[0] + FIFO_Array[id].tail));      // Write tail value to data_ptr

        if (FIFO_Array[id].tail + 1 < MAX_FIFO_SIZE){           // Overflow?
            FIFO_Array[id].tail++;                              // Advance tail
        } else {
            FIFO_Array[id].tail = 0;                            // Reset tail
        }

        FIFO_Array[id].is_buffer_full = !FIFO_BUFFER_FULL;       // Buffer is now not full
        return !FIFO_BUFFER_EMPTY;                               // Return buffer not empty state

    }
}

size_t FIFO_GetBufferLength(fifo_id_t id){
    if (FIFO_Array[id].is_buffer_full){
        return MAX_FIFO_SIZE;
    } else {
        return FIFO_Array[id].head - FIFO_Array[id].tail;
    }
}

void FIFO_Reset(fifo_id_t id){
    init_fifo(id);
}

void FIFO_ClearBuffer(fifo_id_t id){
    uint8_t i;
    for (i = 0; i < MAX_FIFO_SIZE - 1; i++){
        FIFO_Array[id].queue[i] = 0;
    }
    FIFO_Reset(id);
}

void FIFO_FreeId(fifo_id_t id){
    if (fifo_n > 0){
        fifo_n--;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void init_fifo(fifo_id_t id){
    FIFO_Array[id].head = 0;
    FIFO_Array[id].tail = 0;
    FIFO_Array[id].is_buffer_full = false;
}

/******************************************************************************/