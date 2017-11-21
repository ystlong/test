/*
 * sd.h
 *
 * Created: 2017/7/2 20:35:47
 *  Author: Administrator
 */ 


#ifndef SD_H_INCLUDED
#define SD_H_INCLUDED

#include <conf_spi.h>
#include <spi.h>

#ifdef __cplusplus
extern "C" {
#endif

void write_command_sd(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_cmd, uint8_t *sd_data);
void write_sd_cmd8(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_data);
enum status_code sd_write_sector(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_address, uint8_t *sd_write_block_data);
enum status_code sd_read_sector(struct spi_module *const module, struct spi_slave_inst *const slave, uint8_t *sd_address, uint8_t *sd_read_block_data);

#ifdef __cplusplus
}
#endif

#endif /* SD_H_ */