/*
 * l3gd20_add.c
 *
 *  Created on: 31.01.2021
 *      Author: oxford
 */


#include "l3gd20_add.h"
#include "l3gd20h_reg.h"

void l3gd20_init(void) {
	HAL_Delay(10);

	dev_ctx.write_reg = l3gd20_write;
	dev_ctx.read_reg = l3gd20_read;
	dev_ctx.handle = &SENSOR_BUS;

	int32_t error_check;
	error_check = l3gd20h_dev_id_get(&dev_ctx, &whoamI);

	if (error_check != 0) {
		uint32_t size;
		size = sprintf((char*) tx_buffer,
				"Error. Gyroscope address cannot be read! \n");
		tx_com(tx_buffer, size);
	}

	if (whoamI != L3GD20H_ID) {
		uint32_t size;
		size = sprintf((char*) tx_buffer,
				"Error. Gyroscope not found! whoamI = %d, gyroscope = %d \n",
				whoamI, L3GD20H_ID);
		tx_com(tx_buffer, size);
		return;
	}

   /* Set Output Data Rate / Power mode */
	error_check = l3gd20h_gy_data_rate_set(&dev_ctx, L3GD20H_50Hz);

	if (error_check != 0) {
		uint32_t size;
		size = sprintf((char*) tx_buffer,
				"Error. Writing register 'ctrl1' fault! \n");
		tx_com(tx_buffer, size);
		return;
	}
	/* Enable Device Data Format */
	error_check = l3gd20h_dev_data_format_set(&dev_ctx,
			L3GD20H_LSB_LOW_ADDRESS);

	if (error_check != 0) {
		uint32_t size;
		size = sprintf((char*) tx_buffer,
				"Error. Writing register 'BLE' fault! \n");
		tx_com(tx_buffer, size);
		return;
	}
  /* Enable Block Data Update */
   error_check = l3gd20h_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

   if (error_check !=0)
       {
       	uint32_t size;
       		size = sprintf((char*)tx_buffer, "Error. Writing register 'ctrl4.bdu' fault! \n");
       		tx_com(tx_buffer, size);
       		return;
       }
 /* Set full scale */
   error_check =  l3gd20h_gy_full_scale_set(&dev_ctx, L3GD20H_2000dps);

   if (error_check !=0)
          {
          	uint32_t size;
          		size = sprintf((char*)tx_buffer, "Error. Writing register 'ctrl4.fs' fault! \n");
          		tx_com(tx_buffer, size);
          		return;
          }

  /*  Informing about success of initializing */
  uint32_t size;
  size = sprintf((char*)tx_buffer, "Initializing successful! \r\n");
  tx_com(tx_buffer, size);

  HAL_Delay(10);
 }


/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  buffor    pointer to data to write in register reg
 * @param  length    number of consecutive register to write
 *
 */

int32_t l3gd20_write(void *handle, uint8_t reg, uint8_t *buffor, uint16_t length)
{
    HAL_GPIO_WritePin(GYR_CS_PORT, GYR_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Transmit(handle, buffor, length, 1000);
    HAL_GPIO_WritePin(GYR_CS_PORT, GYR_CS_PIN, GPIO_PIN_SET);

    return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
int32_t l3gd20_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
	/* Read command */
	reg |= 0x80;
	HAL_GPIO_WritePin(GYR_CS_PORT, GYR_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Receive(handle, bufp, len, 1000);
    HAL_GPIO_WritePin(GYR_CS_PORT, GYR_CS_PIN, GPIO_PIN_SET);

    return 0;
}

/*
 * @brief  Read angular data from device
 *
 */
int32_t l3gd20_ReadAngularData(void)
{

	 /* Read device status register */
	int32_t error_check;

	error_check = l3gd20h_dev_status_get(&dev_ctx, &status);

	    if (error_check !=0)
	    {
	    	uint32_t size;
	    		size = sprintf((char*)tx_buffer, "Error. Device is busy! \r\n");
	    		tx_com(tx_buffer, size);
	    }

	    if ( status.zyxda ) {
	      /* Read imu data */
	      memset(raw_angular_data, 0x00, 3 * sizeof(int16_t));
	      memset(angular_data, 0x00, 3 * sizeof(float));

	      error_check = l3gd20h_angular_rate_raw_get(&dev_ctx, raw_angular_data, raw_data);

	      if (error_check !=0)
	      	    {
	      	    	uint32_t size;
	      	    		size = sprintf((char*)tx_buffer, "Reading angular data fault! \r\n");
	      	    		tx_com(tx_buffer, size);
	      	    }


	      angular_data[0] = l3gd20h_from_fs2000_to_mdps(raw_angular_data[0]);
	      angular_data[1] = l3gd20h_from_fs2000_to_mdps(raw_angular_data[1]);
	      angular_data[2] = l3gd20h_from_fs2000_to_mdps(raw_angular_data[2]);

	      sprintf((char *)tx_buffer, "[mdps]:%4.2f\t%4.2f\t%4.2f\r\n",angular_data[0], angular_data[1], angular_data[2]);
	      tx_com(tx_buffer, strlen((char const *)tx_buffer));
	    }
return 0;
}

/*
 * @brief  Read temperature data from device
 *
 */
int32_t l3gd20_ReadTemperature(void) {

	/* Read device status register */
	int32_t error_check;
	error_check = l3gd20h_dev_status_get(&dev_ctx, &status);

	if (error_check != 0) {
		uint32_t size;
		size = sprintf((char*) tx_buffer, "Error. Device is busy! \r\n");
		tx_com(tx_buffer, size);
	}

		gyro_temperature = 0;

		error_check = l3gd20h_temperature_raw_get(&dev_ctx, &gyro_temperature);

		if (error_check != 0) {
			uint32_t size;
			size = sprintf((char*) tx_buffer,
					"Error. Temperature reading fault! \r\n");
			tx_com(tx_buffer, size);
		}

		uint32_t size;
	size = sprintf((char*) tx_buffer, "Gyroscope temp. is %d Celsius \r\n",gyro_temperature);
	tx_com(tx_buffer, size);



	return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  length        number of byte to send
 *
 */
void tx_com(uint8_t *tx_buffer, uint16_t length)
{
	HAL_UART_Transmit_DMA(&UART_BUS ,tx_buffer, length);
}
