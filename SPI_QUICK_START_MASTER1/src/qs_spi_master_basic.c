/**
 * \file
 *
 * \brief SAM SPI Quick Start for SAMB
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"

static uint8_t write_buffer_ADS1292R_wakeup[1]			= {0x02};
static uint8_t write_buffer_ADS1292R_standby[1]			= {0x04};
static uint8_t write_buffer_ADS1292R_reset[1]			= {0x06};
static uint8_t write_buffer_ADS1292R_start[1]			= {0x08};
static uint8_t write_buffer_ADS1292R_stop[1]			= {0x0a};
static uint8_t write_buffer_ADS1292R_offsetcal[1]		= {0x1a};
static uint8_t write_buffer_ADS1292R_rdatac[1]			= {0x10};
static uint8_t write_buffer_ADS1292R_sdatac[1]			= {0x11};
static uint8_t write_buffer_ADS1292R_rdata[1]			= {0x12};
//static uint8_t write_buffer_ADS1292R_regster_data[11]	= {0x02,0xa0,0xf0,0x40,0x60,0xe0,0x00,0x00,0xea,0x83,0x00};//no lead-off
static uint8_t write_buffer_ADS1292R_regster_data[11]	= {0x01,0xe0,0x14,0x40,0x60,0xe0,0x0c,0x00,0xea,0x83,0x00};//with lead-off£¬250SPS
static uint8_t write_buffer_ADS1292R_read_register[2]	= {0x20,0x00};
static uint8_t write_buffer_ADS1292R_write_register[2]	= {0x41,0x0a};
static uint8_t read_buffer_ADS1292R[9]					= {0x00};

static uint8_t write_buffer_SD_waite[1]					= {0xff};
static uint8_t write_buffer_SD_reset[6]					= {SD_RESET,0x00,0x00,0x00,0x00,0x95};
static uint8_t write_buffer_SD_cmd55[6]					= {SD_CMD55,0x00,0x00,0x00,0x00,0xff};
static uint8_t write_buffer_SD_acmd41[6]				= {SD_ACMD41,0x40,0x00,0x00,0x00,0xff};
static uint8_t SD_read_block_data[512];
static uint8_t SD_write_block_ADS1292R[512]				= {0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00};
static uint8_t SD_write_block_FXLS8471Q[512]			= {0xff,0xff,0xff,0xff,0x11,0x11,0x11,0x00};

static uint8_t SD_address[4]							= {0x00,0x00,0x80,0x00};//-2000h

static uint8_t write_buffer_FXLS8471Q_cmd[2]			= {0x00,0x00};
static uint8_t read_buffer_FXLS8471Q[FXLS8471Q_READ_LEN];

static uint16_t i										= 0;
static uint16_t repeat_sd								= 0;
static uint16_t timeout									= 0;
static uint32_t SD_address_index						= 0x00008000;
static uint16_t FXLS8471Q_index							= 7;
static uint16_t ADS1292R_index							= 7;

struct spi_module spi_master_instance_ADS1292R, spi_master_instance_SD;
struct spi_slave_inst slave_ADS1292R, slave_SD;

struct i2c_master_module i2c_master_instance;

struct i2c_master_packet packet = {
	.address     = FXLS8471Q_SLAVE_ADDR,
	.data_length = 1,
	.data        = write_buffer_FXLS8471Q_cmd,
};

static void init_led(void)
{
	struct gpio_config config_gpio;

	config_gpio.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(LED_G_PIN, &config_gpio);
	gpio_pin_set_config(LED_R_PIN, &config_gpio);
	gpio_pin_set_output_level(LED_G_PIN, LED_INACTIVE);
	gpio_pin_set_output_level(LED_R_PIN, LED_INACTIVE);
}


////////////////////////////////For board power up, DO NOT modify it!//////////////////////////
static void init_start(void)
{
	struct gpio_config config_gpio;

	config_gpio.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(EXT1_PIN_PSHOLD, &config_gpio);
	gpio_pin_set_output_level(EXT1_PIN_PSHOLD, true);
	
	gpio_pin_set_output_level(LED_G_PIN, LED_ACTIVE);
	gpio_pin_set_output_level(LED_R_PIN, LED_ACTIVE);
}
//////////////////////////////////////////////////////////////////////////////////////////////


static void init_ADS1292R_reset(void)
{
	//GPIO for ADS1292R
	struct gpio_config config_gpio;

	config_gpio.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(EXT1_PIN_ADSTART, &config_gpio);
	gpio_pin_set_config(EXT1_PIN_ADSRST, &config_gpio);	
	gpio_pin_set_output_level(EXT1_PIN_ADSTART, false);
	gpio_pin_set_output_level(EXT1_PIN_ADSRST, false);
	
	config_gpio.direction = GPIO_PIN_DIR_INPUT;
	config_gpio.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(EXT1_PIN_ADSDRDY, &config_gpio);
	
	//start_up sequence for ADS1292R
	gpio_pin_set_output_level(EXT1_PIN_ADSRST, true);
	for(i = 0; i < 0xFFFF; i++) {
	}
}

static void init_ADS1292R_config(void)	
{
	//SPI0 for ADS1292R
	struct spi_config config_spi_master_ADS1292R;
	struct spi_slave_inst_config slave_dev_config_ADS1292R;
	
	gpio_pin_set_output_level(EXT1_PIN_ADSRST, false);
	for(i = 0; i < 0x07FF; i++) {
	}
	gpio_pin_set_output_level(EXT1_PIN_ADSRST, true);
	for(i = 0; i < 0x0FFF; i++) {
	}
	
	spi_slave_inst_get_config_defaults(&slave_dev_config_ADS1292R);
	slave_dev_config_ADS1292R.ss_pin = CONF_SPI0_PIN_SSN;
	spi_attach_slave(&slave_ADS1292R, &slave_dev_config_ADS1292R);
	
	spi_get_config_defaults(&config_spi_master_ADS1292R);
	config_spi_master_ADS1292R.transfer_mode = CONF_SPI0_TRANSFER_MODE;
	config_spi_master_ADS1292R.clock_divider = 25;//set clock at 1Mhz
	config_spi_master_ADS1292R.pin_number_pad[0] = CONF_SPI0_PIN_SCK;
	config_spi_master_ADS1292R.pinmux_sel_pad[0] = CONF_SPI0_MUX_SCK;
	config_spi_master_ADS1292R.pin_number_pad[1] = CONF_SPI0_PIN_MOSI;
	config_spi_master_ADS1292R.pinmux_sel_pad[1] = CONF_SPI0_MUX_MOSI;
	config_spi_master_ADS1292R.pin_number_pad[2] = CONF_SPI0_PIN_SSN;
	config_spi_master_ADS1292R.pinmux_sel_pad[2] = CONF_SPI0_MUX_SSN;
	config_spi_master_ADS1292R.pin_number_pad[3] = CONF_SPI0_PIN_MISO;
	config_spi_master_ADS1292R.pinmux_sel_pad[3] = CONF_SPI0_MUX_MISO;
	spi_init(&spi_master_instance_ADS1292R, CONF_SPI0, &config_spi_master_ADS1292R);
	spi_enable(&spi_master_instance_ADS1292R);

	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, false);
	for(i = 0; i < 0xFF; i++) {
	}
	//send SDATAC(0x11)
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, true);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_write_buffer_wait(&spi_master_instance_ADS1292R, write_buffer_ADS1292R_sdatac, 1);
	for(i = 0; i < 0x0FFF; i++) {
	}
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, false);
	for(i = 0; i < 0xFF; i++) {
	}
	//write registers
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, true);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_write_buffer_wait(&spi_master_instance_ADS1292R, write_buffer_ADS1292R_write_register, 2);
	spi_write_buffer_wait(&spi_master_instance_ADS1292R, write_buffer_ADS1292R_regster_data, 11);
	for(i = 0; i < 0xFFFF; i++) {
	}
	spi_write_buffer_wait(&spi_master_instance_ADS1292R, write_buffer_ADS1292R_offsetcal, 1);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, false);
	for(i = 0; i < 0xFF; i++) {
	}
	//set start
	gpio_pin_set_output_level(EXT1_PIN_ADSTART, true);
	//send RDATAC(0x10)
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, true);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_write_buffer_wait(&spi_master_instance_ADS1292R, write_buffer_ADS1292R_rdatac, 1);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, false);
}

enum status_code init_sd(void)
{
	uint8_t sd_return_status[1] = {0};
	uint8_t sd_init_status = 1;

	//SPI1 for SD
	struct spi_config config_spi_master_SD;
	struct spi_slave_inst_config slave_dev_config_SD;
	
	spi_slave_inst_get_config_defaults(&slave_dev_config_SD);
	slave_dev_config_SD.ss_pin = CONF_SPI1_PIN_SSN;
	spi_attach_slave(&slave_SD, &slave_dev_config_SD);
	
	spi_get_config_defaults(&config_spi_master_SD);
	config_spi_master_SD.transfer_mode = CONF_SPI1_TRANSFER_MODE;	
	config_spi_master_SD.pin_number_pad[0] = CONF_SPI1_PIN_SCK;
	config_spi_master_SD.pinmux_sel_pad[0] = CONF_SPI1_MUX_SCK;
	config_spi_master_SD.pin_number_pad[1] = CONF_SPI1_PIN_MOSI;
	config_spi_master_SD.pinmux_sel_pad[1] = CONF_SPI1_MUX_MOSI;
	config_spi_master_SD.pin_number_pad[2] = CONF_SPI1_PIN_SSN;
	config_spi_master_SD.pinmux_sel_pad[2] = CONF_SPI1_MUX_SSN;
	config_spi_master_SD.pin_number_pad[3] = CONF_SPI1_PIN_MISO;
	config_spi_master_SD.pinmux_sel_pad[3] = CONF_SPI1_MUX_MISO;
	config_spi_master_SD.clock_divider = 129;//set clock at 200Khz
	spi_init(&spi_master_instance_SD, CONF_SPI1, &config_spi_master_SD);
	spi_enable(&spi_master_instance_SD);
	
	//set SD as SPI mode
	spi_select_slave(&spi_master_instance_SD, &slave_SD, false);
	for(i = 0; i < 0x0c; i++) {
		spi_write_buffer_wait(&spi_master_instance_SD, write_buffer_SD_waite, 1);//wait for at least 74 clock
	}
	for(i = 0; i < 0xFF; i++) {
	}
	write_command_sd(&spi_master_instance_SD, &slave_SD, write_buffer_SD_reset, sd_return_status);//reset
	if (sd_return_status[0] == 0x01)
	{
		write_sd_cmd8(&spi_master_instance_SD, &slave_SD, sd_return_status);//check version
		repeat_sd = 0;
		do
		{
			repeat_sd++;
			write_command_sd(&spi_master_instance_SD, &slave_SD, write_buffer_SD_cmd55, sd_return_status);//initial
			write_command_sd(&spi_master_instance_SD, &slave_SD, write_buffer_SD_acmd41, sd_return_status);
		} while (sd_return_status[0] != 0x00 && repeat_sd < 200);
		if (repeat_sd == 200)
		{
			sd_init_status = 0;//ERROR
		} 
		else
		{
			spi_disable(&spi_master_instance_SD);
			config_spi_master_SD.clock_divider = 25;//set clock at 1Mhz
			spi_init(&spi_master_instance_SD, CONF_SPI1, &config_spi_master_SD);
			spi_enable(&spi_master_instance_SD);
			sd_init_status = 1;//OK
		}
	} 
	else
	{
		sd_init_status = 0;//ERROR
	}
	for(i = 0; i < 0xff; i++) {
	}
	spi_select_slave(&spi_master_instance_SD, &slave_SD, false);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(&spi_master_instance_SD, write_buffer_SD_waite, 1);
	for(i = 0; i < 0xff; i++) {
	}
	if (sd_init_status == 0x01)
	{
		return STATUS_OK;
	}
	else
	{
		return STATUS_ERR_NOT_INITIALIZED;
	}
}

enum status_code init_FXLS8471Q(void)
{		
	struct i2c_master_config config_i2c_master;	
	i2c_master_get_config_defaults(&config_i2c_master);
	i2c_master_init(&i2c_master_instance, CONF_I2C_SLAVE_MODULE, &config_i2c_master);
	i2c_enable(i2c_master_instance.hw);
	
	//register initial
	packet.data_length = 2;	
					
	write_buffer_FXLS8471Q_cmd[0] = FXLS8471Q_CTRL_REG1;
	write_buffer_FXLS8471Q_cmd[1] = 0x00;
	packet.data = write_buffer_FXLS8471Q_cmd;		
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			return STATUS_ERR_TIMEOUT;
		}
	}
	
	write_buffer_FXLS8471Q_cmd[0] = FXLS8471Q_XYZ_DATA_CFG;
	write_buffer_FXLS8471Q_cmd[1] = 0x01;
	packet.data = write_buffer_FXLS8471Q_cmd;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			return STATUS_ERR_TIMEOUT;
		}
	}
	
	write_buffer_FXLS8471Q_cmd[0] = FXLS8471Q_CTRL_REG1;
	write_buffer_FXLS8471Q_cmd[1] = 0x15;
	packet.data = write_buffer_FXLS8471Q_cmd;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			return STATUS_ERR_TIMEOUT;
		}
	}
		

	return STATUS_OK;
}

enum status_code read_FXLS8471Q(struct i2c_master_module *const module)
{
	packet.data_length = 1;
	
	write_buffer_FXLS8471Q_cmd[0] = FXLS8471Q_STATUS;
	packet.data = write_buffer_FXLS8471Q_cmd;
	while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &packet) != STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			return STATUS_ERR_TIMEOUT;
		}
	}
	
	packet.data_length = FXLS8471Q_READ_LEN;
	packet.data = read_buffer_FXLS8471Q;
	while (i2c_master_read_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			return STATUS_ERR_TIMEOUT;
		}
	}

	return STATUS_OK;
}

static void read_sensor_data(void)
{
	for(i = 0; i < 0xf; i++) {
	}				
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, true);	
	for(i = 0; i < 0xF; i++) {
	}		
	spi_read_buffer_wait(&spi_master_instance_ADS1292R, read_buffer_ADS1292R, 9, 0);
	for(i = 0; i < 0xF; i++) {
	}
	spi_select_slave(&spi_master_instance_ADS1292R, &slave_ADS1292R, false);
			
	if((read_buffer_ADS1292R[0] & 0x06) == 0x00 && (read_buffer_ADS1292R[1] & 0x1f) == 0x00 && (read_buffer_ADS1292R[2] & 0xff) == 0x00) //lead off channel 2
//	if((read_buffer_ADS1292R[0] & 0x00) == 0x00 && (read_buffer_ADS1292R[1] & 0x1f) == 0x00 && (read_buffer_ADS1292R[2] & 0xff) == 0x00) //  13'0
	{
		gpio_pin_set_output_level(LED_R_PIN, LED_INACTIVE);			
		for (i = 0; i < 6; i++)
		{
			SD_write_block_ADS1292R[ADS1292R_index + i] = read_buffer_ADS1292R[3 + i];
		}
		ADS1292R_index = ADS1292R_index + 6;
			
/*		while (read_FXLS8471Q(&i2c_master_instance) != STATUS_OK) {
			if (timeout++ == TIMEOUT) {
				break;
			}
		}			
		if (read_buffer_FXLS8471Q[0] >= 0x08)
		{
			for (i = 0; i < 6; i++)
			{
				SD_write_block_FXLS8471Q[FXLS8471Q_index + i] = read_buffer_FXLS8471Q[1 + i];
			}
			FXLS8471Q_index = FXLS8471Q_index + 6;
		}*/	
	}
	else
	{
		gpio_pin_set_output_level(LED_R_PIN, LED_ACTIVE);//red
//		gpio_pin_set_output_level(LED_0_PIN2, LED_0_INACTIVE);
	}				
}

static void configure_gpio_callbacks(void)
{
	gpio_init();

	gpio_register_callback(EXT1_PIN_ADSDRDY, read_sensor_data,
	GPIO_CALLBACK_FALLING);

	gpio_enable_callback(EXT1_PIN_ADSDRDY);
}

void send_send();
int nut_bord_init(void);

extern uint8_t already_send;

void main(void)
{

	uint8_t  delay			= 0;
	uint8_t  rep_1292R		= 0;
	uint8_t  rep_FXLS8471Q	= 0;
		
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	init_led();
	init_start();//For power up
 	init_ADS1292R_reset();
 	//init_FXLS8471Q();		
 	init_sd();	
 	init_ADS1292R_config();
 	configure_gpio_callbacks();

	//nut_bord_init();
	
	gpio_pin_set_output_level(LED_G_PIN, LED_ACTIVE);//green
	//for(i = 0; i < 0xffff; i++) {
	//}
	//
	
	while (true) {		
		//ble_event_task(BLE_EVENT_TIMEOUT);
		//void check_wait_conn_ok();
		//check_wait_conn_ok();
		//send_send();

		if (ADS1292R_index >= 506)
		{
			send_send();
			//char databuf[] = {'1', '2', '3', '4', '\0'};
			//uint16_t datalen = 4;
			//at_ble_status_t nut_serv_ecg_send_data(uint8_t *databuf, uint16_t datalen);
			//nut_serv_ecg_send_data((uint8_t *)databuf, datalen);

			// gpio_pin_set_output_level(LED_R_PIN, LED_ACTIVE);	
			// ADS1292R_index = 7;
			// do 
			// {
			// 	rep_1292R++;
			// } while ((sd_write_sector(&spi_master_instance_SD, &slave_SD, SD_address, SD_write_block_ADS1292R) == STATUS_ERR_BAD_DATA) && rep_1292R < 5);
			// rep_1292R = 0;
			// for(i = 0; i < 0xff; i++) {
			// }
			// SD_address_index = SD_address_index + 1;
			// SD_address[0] = SD_address_index >> 24;
			// SD_address[1] = SD_address_index >> 16;
			// SD_address[2] = SD_address_index >> 8;
			// SD_address[3] = SD_address_index;
			// gpio_pin_set_output_level(LED_R_PIN, LED_INACTIVE);					
		}
/*		if(FXLS8471Q_index >= 506)
		{
			FXLS8471Q_index = 7;
			do
			{
				rep_FXLS8471Q++;
			} while ((sd_write_sector(&spi_master_instance_SD, &slave_SD, SD_address, SD_write_block_FXLS8471Q) == STATUS_ERR_BAD_DATA) || rep_FXLS8471Q < 5);
			rep_FXLS8471Q = 0;
			for(i = 0; i < 0xff; i++) {
			}
			SD_address_index = SD_address_index + 1;
			SD_address[0] = SD_address_index >> 24;
			SD_address[1] = SD_address_index >> 16;
			SD_address[2] = SD_address_index >> 8;
			SD_address[3] = SD_address_index;	
		}*/					
	}
}
