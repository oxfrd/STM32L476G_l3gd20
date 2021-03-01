# STM32L476G_l3gd20

Library for gyroscope l3gd20 converted from: https://github.com/STMicroelectronics/STMems_Standard_C_drivers/tree/master/l3gd20h_STdC
thats why sometimes l3gd20h can be seen in functions.

Only polling data checked. Interrupts and filters wasn't tested but it should work.
Code in folder Core.

Connection between MCU and gyroscope through 4-wire SPI (on disciovery L476 board it's SPI2).

Necesarry initialization ( l3_gd20_init() )

