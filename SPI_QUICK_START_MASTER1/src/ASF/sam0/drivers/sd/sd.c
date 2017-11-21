/*
 * sd.c
 *
 * Created: 2017/6/30 18:40:17
 *  Author: Administrator
 */

#include <asf.h>

static uint8_t write_buffer_SD_waite[1]					= {0xff};
static uint8_t write_buffer_SD_start_byte[1]			= {0xfe};
static uint8_t write_buffer_SD_read_block[1]			= {SD_READ_BLOCK};
static uint8_t write_buffer_SD_write_block[1]			= {SD_WRITE_BLOCK};
static uint8_t sd_data[6]								= {0xff,0xff,0xff,0xff,0xff,0xff};//R1,X,X,X,X,X
static uint8_t sd_cmd8[6]								= {0x48,0x00,0x00,0x01,0xaa,0x87};
static uint16_t i										= 0;

//****************************************************************************

void write_command_sd(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_cmd, uint8_t *sd_data)
{	
	spi_select_slave(module, slave, false);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	spi_select_slave(module, slave, true);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_write_buffer_wait(module, sd_cmd, 6);
	do
	{
		spi_read_buffer_wait(module, sd_data, 1, 0xff);
	} while (sd_data[0] == 0xff);
	for(i = 0; i < 0xff; i++) {
	}
	spi_select_slave(module, slave, false);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	for(i = 0; i < 0xff; i++) {
	}
}

//****************************************************************************

void write_sd_cmd8(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_data)
{	
	spi_select_slave(module, slave, false);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	spi_select_slave(module, slave, true);
	for(i = 0; i < 0xFF; i++) {
	}
	spi_write_buffer_wait(module, sd_cmd8, 6);
	do
	{
		spi_read_buffer_wait(module, sd_data, 1, 0xff);
	} while (sd_data[0] == 0xff);
	spi_read_buffer_wait(module, sd_data+1, 5, 0xff);
	for(i = 0; i < 0xff; i++) {
	}
	spi_select_slave(module, slave, false);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	for(i = 0; i < 0xff; i++) {
	}
}

//****************************************************************************

enum status_code sd_write_sector(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_address, uint8_t *sd_write_block_data)
{
	uint8_t status_sd,temp = 0;
	
	spi_select_slave(module, slave, false);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	spi_select_slave(module, slave, true);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_write_block, 1);
	spi_write_buffer_wait(module, sd_address, 4);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	do
	{
		spi_read_buffer_wait(module, sd_data, 1, 0xff);
	} while (sd_data[0] == 0xff);
	if (sd_data[0] == 0x00)
	{
		for(i = 0; i < 0x08; i++) {
			spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
		}
		spi_write_buffer_wait(module, write_buffer_SD_start_byte, 1);//start byte
		spi_write_buffer_wait(module, sd_write_block_data, 512);//512 data
		spi_write_buffer_wait(module, write_buffer_SD_waite, 1);//crc
		spi_write_buffer_wait(module, write_buffer_SD_waite, 1);//crc
		do 
		{
			temp++;
			spi_read_buffer_wait(module, sd_data, 1, 0xff);
		} while ((sd_data[0] & 0x1f) != 0x05 && temp < 50);
		if (temp == 50)
		{
			status_sd = 0x00;//ERROR
		} 
		else
		{
			do
			{
				spi_read_buffer_wait(module, sd_data, 1, 0xff);
			} while (sd_data[0] == 0xff);
			status_sd = 0x01;//OK
		}	
	}
	else
	{
		status_sd = 0x00;//ERROR
	}
	for(i = 0; i < 0xff; i++) {
	}
	spi_select_slave(module, slave, false);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	for(i = 0; i < 0xff; i++) {
	}
	if (status_sd == 0x01)
	{
		return STATUS_OK;
	} 
	else
	{
		return STATUS_ERR_BAD_DATA;
	}
}
//****************************************************************************

enum status_code sd_read_sector(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_address, uint8_t *sd_read_block_data)
{
	uint8_t status_sd;
	uint8_t sd_buffer[1] = {0x00};
	
	spi_select_slave(module, slave, false);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	spi_select_slave(module, slave, true);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_read_block, 1);
	spi_write_buffer_wait(module, sd_address, 4);
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	do
	{
		spi_read_buffer_wait(module, sd_data, 1, 0xff);
	} while (sd_data[0] == 0xff);
	if (sd_data[0] == 0x00)
	{
		do
		{
			spi_read_buffer_wait(module, sd_data, 1, 0xff);
		} while (sd_data[0] != write_buffer_SD_start_byte[0] && sd_data[0] != 0x07);//start byte or ecc error
		spi_read_buffer_wait(module, sd_read_block_data, 512, 0xff);//512 data
		spi_read_buffer_wait(module, sd_buffer, 1, 0xff);//crc
		spi_read_buffer_wait(module, sd_buffer, 1, 0xff);//crc
		if (sd_data[0] = write_buffer_SD_start_byte[0])
		{
			status_sd = 0x01;//OK
		} 
		else
		{
			status_sd = 0x00;//ERROR
		}
	}
	else
	{
		status_sd = 0x00;//ERROR
	}
	for(i = 0; i < 0xff; i++) {
	}
	spi_select_slave(module, slave, false);
	for(i = 0; i < 0xff; i++) {
	}
	spi_write_buffer_wait(module, write_buffer_SD_waite, 1);
	for(i = 0; i < 0xff; i++) {
	}
	if (status_sd == 0x01)
	{
		return STATUS_OK;
	}
	else
	{
		return STATUS_ERR_BAD_DATA;
	}
}