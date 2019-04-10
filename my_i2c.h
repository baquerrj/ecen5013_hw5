/*!
 * @file    my_i2c.h
 *
 * @brief
 *
 *  Created on: Apr 8, 2019
 *      Author: Roberto Baquerizo
 */


#ifndef MY_I2C_H_
#define MY_I2C_H_

#include <stdint.h>
#include <stdbool.h>

//#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"

#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


void InitI2C2( void );


//read specified register on slave device
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);



#endif /* MY_I2C_H_ */
