/*
 * tmp102_task.c
 *
 *  Created on: Apr 8, 2019
 *      Author: rober
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "task.h"
#include "priorities.h"
#include "timers.h"

#include "uart.h"
#include "logger_task.h"
#include "tmp102_task.h"
#include "my_i2c.h"

#define MY_STACK_SIZE 256

extern xQueueHandle q_pLoggerQueue;

TimerHandle_t tmp102_timer_handle;

static void init_1hz( void *params )
{

    tmp102_timer_handle = xTimerCreate( "TMP102_1Hz", pdMS_TO_TICKS(1000), pdTRUE, (void*)0, tmp102_task_callback );

    if( tmp102_timer_handle == NULL)
    {
        while(1);
    }

    if((xTimerStart(tmp102_timer_handle, 0)) != pdTRUE)
    {
        while(1);
    }

    /* The only work of this task was to initialize a timer and start it. We are suspending the task now*/
    vTaskSuspend(NULL);
}

void tmp102_task_callback( TimerHandle_t timer )
{
    static log_msg_t msg_out;
    msg_out.src = pcTimerGetTimerName( tmp102_timer_handle );
    float temp = -5.5;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500);
    //Timer handle for 1Hz Temperature Sensor task
    if( tmp102_timer_handle == timer )
    {
        msg_out.tickcount = xTaskGetTickCount();
        msg_out.type = MSG_GET_TEMP;
        msg_out.src = pcTimerGetTimerName( timer );
        memcpy( msg_out.msg, "GET_TEMP", sizeof( msg_out.msg ) );
        tmp102_get_temp( &temp );

        msg_out.data.temperature = temp;

        //Enqueue the worker queue with a new msg
        if( xQueueSend( q_pLoggerQueue, &msg_out, xMaxBlockTime ) != pdPASS )
        {
            puts("ERROR - TMP102 SENSOR TASK - QUEUE SEND\n");
        }
    }
}

void tmp102_get_temp( float *temp )
{
    uint8_t buff[2] = {0};
    buff[0] = I2CReceive( TMP102_SLAVE_ADDR, TMP102_REG_TEMPERATURE );
    buff[1] = I2CReceive( TMP102_SLAVE_ADDR, TMP102_REG_TEMPERATURE + 1);

    /* We get MSB(15:8) in buff[0] and LSB(7:4) in buff[1] */
    uint16_t temp_raw = 0;
    temp_raw = (((uint16_t)buff[0]) << 4) | (buff[1] >> 4) & 0xFFF;
    if(temp_raw & 0x800)
    {
        temp_raw = ((~temp_raw) + 1) & 0xFFF;
        *temp = (-1) * (float)temp_raw * 0.0625;
    }
    else
    {
        *temp = ((float)temp_raw)*0.0625;
    }

    return;
}

uint8_t temp_task_init( void )
{
    configASSERT( loggerTaskInitDone == 1 );

    if( pdTRUE != xTaskCreate( init_1hz, (const portCHAR *)"TMP102_1Hz", MY_STACK_SIZE, NULL,
                                 tskIDLE_PRIORITY + PRIO_TMP_TASK, NULL ) )
    {
        return 1;
    }

    return 0;
}
