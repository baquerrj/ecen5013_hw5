/*!
 * @file	logger_task.c
 *
 * @brief
 *
 *  Created on: Apr 8, 2019
 *      Author: Roberto Baquerizo
 */


#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "priorities.h"

#include "tmp102_task.h"
#include "logger_task.h"
#include "uart.h"


#define LOGGER_QUEUE_ITEMSIZE   (sizeof(log_msg_t))
#define LOGGER_QUEUE_LENGTH     (30)

#define MY_STACK_SIZE 256

volatile uint8_t loggerTaskInitDone = 0;

xQueueHandle q_pLoggerQueue;

void logger_task( void *params )
{
    //float temp = 5.2;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500);
    log_msg_t msg_in;
    while(1)
    {
        if(xQueueReceive( q_pLoggerQueue , &msg_in, xMaxBlockTime ))
        {
            switch( msg_in.type )
            {
                case MSG_GET_TEMP:
                {
                    printf( "MSG - %s ----- TICK: %u ----- TEMP: %f C\n",
                            msg_in.msg, msg_in.tickcount, msg_in.data.temperature );
                    break;
                }
                case MSG_TOGGLE_LED:
                {
                    printf( "MSG - %s ----- TICK: %u ----- TOGGLE COUNT: %u\n",
                            msg_in.msg, msg_in.tickcount, msg_in.data.toggle_count );
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            puts( "ERROR - LOGGER TASK - QUEUE RECV\n" );
        }
    }
}

uint8_t logger_task_init( void )
{
    /* Creating a Queue required for Logging the msg */
    q_pLoggerQueue = xQueueCreate(LOGGER_QUEUE_LENGTH, LOGGER_QUEUE_ITEMSIZE);

    /* Create the task*/
    if(xTaskCreate(logger_task, (const portCHAR *)"LoggerTask", MY_STACK_SIZE, NULL,
                       tskIDLE_PRIORITY + PRIO_WORKERTASK, NULL ) != pdTRUE)
    {
        return 1;
    }

    loggerTaskInitDone = 1;

    return 0;
}
