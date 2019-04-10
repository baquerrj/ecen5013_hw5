## ECEN5013 Homework 5
# Task Creation and I2C on TI TIVA

TIVA TM4C1294XL FreeRTOS Application consisting of three tasks:
	1) Temperature Task gets temperature reading from TMP102 external sensor at 1Hz
	2) LED task toggles LEDs D1 and D2 at 10Hz
	3) Logger Task receives messages from temperature and LED tasks and logs to UART
Application makes of 2.1.4.178 TivaWare C Firmware Package and FreeRTOS v8.2.3

Built off from TIVA uart_echo example and freetos_demo for ek-tm4c123gxl