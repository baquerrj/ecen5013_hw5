/*!
 * @file	common_task.h
 *
 * @brief
 *
 *  Created on: Apr 8, 2019
 *      Author: Roberto Baquerizo
 */

#ifndef LOGGER_TASK_H_
#define LOGGER_TASK_H_

typedef enum
{
    MSG_BEGIN = 0,
    MSG_TOGGLE_LED,
    MSG_LOG_STRING,
    MSG_GET_TEMP,
    MSG_MAX
} log_msg_e;

typedef struct
{
    TickType_t tickcount;
    char msg[25];
    log_msg_e type;
    union data
    {
        float temperature;
        uint32_t toggle_count;
    } data;
} log_msg_t;


/*!
 * @brief Logger Task function
 *
 * @param params - FreeRTOS TaskHandle_t params
 * @returns void
 */
void logger_task( void *params );

/*!
 * @brief Initialize logger task by calling FreeRTOS mechanisms
 *          xTaskCreate and xQueueCreate
 * @oaram void
 * @returns 0 if successful
 */
uint8_t logger_task_init( void );


#endif /* LOGGER_TASK_H_ */
