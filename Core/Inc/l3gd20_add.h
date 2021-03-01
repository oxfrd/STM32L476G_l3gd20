/*
 * l3gd20_add.h
 *
 *  Created on: 31.01.2021
 *      Author: oxford
 */

#ifndef INC_L3GD20_ADD_H_
#define INC_L3GD20_ADD_H_

#include <stdio.h>
#include <string.h>
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "l3gd20h_reg.h"
#include "stm32l4xx_hal.h"

#define SENSOR_BUS hspi2
#define GYR_CS_PORT GYRO_CS_GPIO_Port
#define GYR_CS_PIN GYRO_CS_Pin
#define UART_BUS huart2

stmdev_ctx_t dev_ctx;
uint8_t whoamI;
uint8_t rst;
uint8_t tx_buffer[1000];
l3gd20h_status_reg_t status;

int16_t raw_angular_data [3];
float angular_data [3];
uint8_t gyro_temperature;
uint8_t raw_data[6];





void l3gd20_init(void);
int32_t l3gd20_write(void *handle, uint8_t reg, uint8_t *buffor, uint16_t length);
int32_t l3gd20_read(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len);
int32_t l3gd20_ReadAngularData(void);
int32_t l3gd20_ReadTemperature(void);
void tx_com(uint8_t *tx_buffer, uint16_t length);

#endif /* INC_L3GD20_ADD_H_ */
