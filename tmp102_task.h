/*
 * tmp102_task.h
 *
 *  Created on: Apr 8, 2019
 *      Author: rober
 */

#ifndef TMP102_TASK_H_
#define TMP102_TASK_H_

#include "timers.h"

#define TMP102_SLAVE_ADDR           (0x48)


/* Register address */
#define TMP102_REG_TEMPERATURE          (0x00)
#define TMP102_REG_CONFIGURATION        (0x01)
#define TMP102_REG_TLOW                 (0x02)
#define TMP102_REG_THIGH                (0x03)



/*!
 * @brief Timer Callback for Temperature Sensor Task
 *
 * @param timer - FreeRTOS TimerHandle_t associated with task
 * @returns void
 */
void tmp102_task_callback( TimerHandle_t timer );

/*!
 * @brief Reads temperature data from TMP102 sensor registers
 *          and writes decoded value to input variable
 * @param temp - pointer to write decoded temperature value to
 * @returns void
 */
void tmp102_get_temp( float *temp );

/*!
 * @brief Initialize 1Hz Temperature Sensor task
 *
 * @param void
 * @returns 0 if successful
 */
uint8_t temp_task_init( void );

#endif /* TMP102_TASK_H_ */
