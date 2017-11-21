/**
 * \file
 *
 * \brief SAM B11 SPI configuration
 *
 * Copyright (C) 2015-2016 Atmel Corporation. All rights reserved.
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


#ifndef CONF_SPI_H_INCLUDED
#  define CONF_SPI_H_INCLUDED

//SPI0 for ADS1292R
#  define CONF_SPI_MASTER_ENABLE     true
#  define CONF_SPI_SLAVE_ENABLE      false

#  define CONF_SPI0_TRANSFER_MODE     SPI_TRANSFER_MODE_1

#  define CONF_SPI0                   EXT2_SPI_MODULE

#  define CONF_SPI0_PIN_SCK           EXT2_SPI_PIN_PAD3
#  define CONF_SPI0_PIN_MOSI          EXT2_SPI_PIN_PAD1
#  define CONF_SPI0_PIN_SSN           EXT2_SPI_PIN_PAD0
#  define CONF_SPI0_PIN_MISO          EXT2_SPI_PIN_PAD2

#  define CONF_SPI0_MUX_SCK           EXT2_SPI_MUX_PAD3
#  define CONF_SPI0_MUX_MOSI          EXT2_SPI_MUX_PAD1
#  define CONF_SPI0_MUX_SSN           EXT2_SPI_MUX_PAD0
#  define CONF_SPI0_MUX_MISO          EXT2_SPI_MUX_PAD2

//SPI1 for SD
#  define CONF_SPI1_TRANSFER_MODE     SPI_TRANSFER_MODE_0

#  define CONF_SPI1                   EXT1_SPI_MODULE

#  define CONF_SPI1_PIN_SCK           EXT1_SPI_PIN_PAD3
#  define CONF_SPI1_PIN_MOSI          EXT1_SPI_PIN_PAD1
#  define CONF_SPI1_PIN_SSN           EXT1_SPI_PIN_PAD0
#  define CONF_SPI1_PIN_MISO          EXT1_SPI_PIN_PAD2

#  define CONF_SPI1_MUX_SCK           EXT1_SPI_MUX_PAD3
#  define CONF_SPI1_MUX_MOSI          EXT1_SPI_MUX_PAD1
#  define CONF_SPI1_MUX_SSN           EXT1_SPI_MUX_PAD0
#  define CONF_SPI1_MUX_MISO          EXT1_SPI_MUX_PAD2

#  define SD_RESET					  0x40+0
#  define SD_INIT					  0x40+1
#  define SD_READ_CSD				  0x40+9
#  define SD_READ_CID				  0x40+10
#  define SD_STOP_TRANSMISSION		  0x40+12
#  define SD_SEND_STATUS			  0x40+13
#  define SD_SET_BLOCKEN			  0x40+16
#  define SD_READ_BLOCK				  0x40+17
#  define SD_READ_MULTI_BLOCK		  0x40+18
#  define SD_WRITE_BLOCK			  0x40+24
#  define SD_WRITE_MULTI_BLOCK		  0x40+25

#  define SD_CMD55					  0x40+55
#  define SD_ACMD41					  0x40+41


#endif /* CONF_SPI_H_INCLUDED */

