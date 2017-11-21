/**
 * \file
 *
 * \brief BLE Startup Template
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 * Support</a>
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Startup Template
 */
/*- Includes ---------------------------------------------------------------*/
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "button.h"
#include "test_ble.h"
#include "nut/nut_ble.h"

/* timer callback function */
static void timer_callback_fn(void)
{
	/* Add timer callback functionality here */
}

static void button_cb(void)
{
	/* Add button callback functionality here */
}


#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN1                LED0_PIN1
#define LED_0_PIN2                LED0_PIN2
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
#define LED0_GPIO                 LED0_PIN
#define LED0                      LED0_PIN

static void init_led(void)
{
	//struct gpio_config config_gpio;
//
	//config_gpio.direction = GPIO_PIN_DIR_OUTPUT;
	//gpio_pin_set_config(LED_0_PIN1, &config_gpio);
	//gpio_pin_set_config(LED_0_PIN2, &config_gpio);
	//gpio_pin_set_output_level(LED_0_PIN1, LED_0_INACTIVE);
	//gpio_pin_set_output_level(LED_0_PIN2, LED_0_INACTIVE);
}

static void delay(int n)
{
	//for(int i=0; i<100000*n; i++);
}

void lll_led(int n)
{
	//if(n<0 || n >10){
		//n = 1;
	//}
	//for(int j=0; j<n; j++){
		//gpio_pin_set_output_level(LED_0_PIN1, LED_0_INACTIVE);
		//delay(1);
		//gpio_pin_set_output_level(LED_0_PIN1, LED_0_ACTIVE);
		//delay(1);
	//}
}



uint8_t already_send = 0;
uint8_t conn_ok = 0;

void check_wait_conn_ok()
{
	while(!conn_ok)
	{
		ble_event_task(BLE_EVENT_TIMEOUT);
	}
}


void send_send()
{
	char databuf[] = {'a', 'b', 'c', 'd', '\0'};
	uint16_t datalen = 4;

	/* Write application task */
	//if (already_send == 1) {
		at_ble_status_t nut_serv_ecg_send_data(uint8_t *databuf, uint16_t datalen);
		nut_serv_ecg_send_data((uint8_t *)databuf, datalen);
		//already_send = 0;
	//}
}

int nut_bord_init(void)
{
	
	struct gpio_config config_gpio;
	
	config_gpio.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(EXT1_PIN_PSHOLD, &config_gpio);
	gpio_pin_set_output_level(EXT1_PIN_PSHOLD, true);
	
	platform_driver_init();
	acquire_sleep_lock();

	/* Initialize serial console */
	serial_console_init();
	init_led();
	lll_led(2);
	
	
	// gpio_pin_set_output_level(LED_0_PIN1, LED_0_ACTIVE);

	///* Hardware timer */
	//hw_timer_init();
	//
	///* button initialization */
	//gpio_init();
	//button_init();
	//button_register_callback(button_cb);
	//
	//hw_timer_register_callback(timer_callback_fn);

	DBG_LOG("Initializing BLE Application");

	lll_led(1);
	
	//at_ble_addr_t ble_addr;
	
	/* Initialize the buffer address and buffer length based on user input */
	//nut_prf_buf_init(&send_data[0], APP_TX_BUF_SIZE);
	
	/* initialize the BLE chip  and Set the Device Address */
	ble_device_init(NULL);
	
	lll_led(1);

	nut_ble_init();
	lll_led(1);

	nut_prf_dev_adv();
	lll_led(1);

	
	/* Set ULP mode */
	//ble_set_ulp_mode(BLE_ULP_MODE_SET);
	
	//
	//while(true)
	//{
		//lll_led(2);
		//
		///* BLE Event task */
		//ble_event_task(BLE_EVENT_TIMEOUT);
		//send_send();
		//
		////char databuf[] = {'a', 'b', 'c', 'd', '\0'};
		////uint16_t datalen = 4;
////
		/////* Write application task */
		////if (already_send == 1) {
			////at_ble_status_t nut_serv_ecg_send_data(uint8_t *databuf, uint16_t datalen);
			////nut_serv_ecg_send_data((uint8_t *)databuf, datalen);
			////already_send = 0;
		////}
	//}

}


// int main(void)
// {
// 	return nut_bord_init();
// }