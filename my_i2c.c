/*!
 * @file    my_i2c.c
 *
 * @brief
 *
 *  Created on: Apr 8, 2019
 *      Author: Roberto Baquerizo
 */


#include "my_i2c.h"

void InitI2C2( void )
{
    /* https://www.digikey.com/eewiki/display/microcontroller/I2C+Communication+with+the+TI+Tiva+TM4C123GXL */
    /* Enable I2C Bus 2 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

    /* Reset I2C Bus 2 */
    SysCtlPeripheralReset( SYSCTL_PERIPH_I2C2 );

    /* Enable GPIO Peripheral containing I2C2 (PN5 and PN4) */
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPION );

    /* Configure the pin muxing for I2C2 functions on port N4 and N5 */
    GPIOPinConfigure( GPIO_PN4_I2C2SDA );
    GPIOPinConfigure( GPIO_PN5_I2C2SCL );

    /* Select the I2C function for these pins */
    GPIOPinTypeI2CSCL( GPIO_PORTN_BASE, GPIO_PIN_5 );
    GPIOPinTypeI2C( GPIO_PORTN_BASE, GPIO_PIN_4 );


    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C2_BASE + I2C_O_FIFOCTL) = 80008000;
}

//read specified register on slave device
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg)
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C2_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C2_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C2_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C2_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C2_BASE));

    //return data pulled from the specified register
    return I2CMasterDataGet(I2C2_BASE);
}
