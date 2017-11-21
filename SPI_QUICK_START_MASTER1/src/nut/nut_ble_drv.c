/*
 * CFile1.c
 *
 * Created: 2017/9/24 13:14:31
 *  Author: long
 */ 

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "nut_ble.h"

#define BLE_PAIR_ENABLE true

typedef struct nut_serv_char
{
	uint16_t buf_size;
	uint8_t *buf;
	at_ble_characteristic_t *ble_char;
	//nut_char_init_handler_t init_handler;
}nut_serv_char_t;

typedef void (*nut_char_init_handler_t)(nut_serv_char_t *);

typedef struct nut_serv
{
	at_ble_uuid_t	serv_uuid;
	at_ble_handle_t	serv_handle;
	at_ble_characteristic_t nut_chars[NUT_CHARS_SIZE];
	nut_serv_char_t nut_serv_chars[NUT_CHARS_SIZE];
	// nut_char_init_handler_t nut_char_init_handlers[NUT_CHARS_SIZE];
}nut_serv_t;

nut_serv_t nut_inst;


static uint8_t nut_power_char_buf[NUT_POWER_CHAR_BUF_LEN];
static void nut_power_char_init(nut_serv_char_t * nut_serv_char)
{
	/* auto code hear */
	/*Configure SPOG Characteristic*/
	nut_serv_char->ble_char->uuid.type = AT_BLE_UUID_128;
	memcpy(&nut_serv_char->ble_char->uuid.uuid[0], NUT_POWER_CHAR_UUID, NUT_UUID_128_LEN);

	/* config Characteristic access properties*/
	nut_serv_char->ble_char->properties = AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_WRITE;
	
	nut_serv_char->ble_char->init_value = nut_serv_char->buf;
	nut_serv_char->ble_char->value_init_len = nut_serv_char->buf_size;
	nut_serv_char->ble_char->value_max_len = nut_serv_char->buf_size;

	/* Configure the NUT characteristic permission */
	if(BLE_PAIR_ENABLE){
		nut_serv_char->ble_char->value_permissions = (AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR);
	}else{
		nut_serv_char->ble_char->value_permissions = AT_BLE_ATTR_NO_PERMISSIONS;
	}
}

static uint8_t nut_accelerate_char_buf[NUT_ACCELERATE_CHAR_BUF_LEN];
static void nut_accelerate_char_init(nut_serv_char_t * nut_serv_char)
{
	/* auto code hear */
	/*Configure SPOG Characteristic*/
	nut_serv_char->ble_char->uuid.type = AT_BLE_UUID_128;
	memcpy(&nut_serv_char->ble_char->uuid.uuid[0], NUT_ACCELERATE_CHAR_UUID, NUT_UUID_128_LEN);

	/* config Characteristic access properties*/
	nut_serv_char->ble_char->properties = AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_WRITE;
	
	nut_serv_char->ble_char->init_value = nut_serv_char->buf;
	nut_serv_char->ble_char->value_init_len = nut_serv_char->buf_size;
	nut_serv_char->ble_char->value_max_len = nut_serv_char->buf_size;

	/* Configure the NUT characteristic permission */
	if(BLE_PAIR_ENABLE){
		nut_serv_char->ble_char->value_permissions = (AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR);
	}else{
		nut_serv_char->ble_char->value_permissions = AT_BLE_ATTR_NO_PERMISSIONS;
	}
}

static uint8_t nut_heartrate_char_buf[NUT_HEARTRATE_CHAR_BUF_LEN];
static void nut_heartrate_char_init(nut_serv_char_t * nut_serv_char)
{
	/* auto code hear */
	/*Configure SPOG Characteristic*/
	nut_serv_char->ble_char->uuid.type = AT_BLE_UUID_128;
	memcpy(&nut_serv_char->ble_char->uuid.uuid[0], NUT_HEARTRATE_CHAR_UUID, NUT_UUID_128_LEN);

	/* config Characteristic access properties*/
	nut_serv_char->ble_char->properties = AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_WRITE;
	
	nut_serv_char->ble_char->init_value = nut_serv_char->buf;
	nut_serv_char->ble_char->value_init_len = nut_serv_char->buf_size;
	nut_serv_char->ble_char->value_max_len = nut_serv_char->buf_size;

	/* Configure the NUT characteristic permission */
	if(BLE_PAIR_ENABLE){
		nut_serv_char->ble_char->value_permissions = (AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR);
	}else{
		nut_serv_char->ble_char->value_permissions = AT_BLE_ATTR_NO_PERMISSIONS;
	}
}

static uint8_t nut_ecg_char_buf[NUT_ECG_CHAR_BUF_LEN];
static void nut_ecg_char_init(nut_serv_char_t * nut_serv_char)
{
	/* auto code hear */
	/*Configure SPOG Characteristic*/
	nut_serv_char->ble_char->uuid.type = AT_BLE_UUID_128;
	memcpy(&nut_serv_char->ble_char->uuid.uuid[0], NUT_ECG_CHAR_UUID, NUT_UUID_128_LEN);

	/* config Characteristic access properties*/
	nut_serv_char->ble_char->properties = AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_WRITE;
	
	nut_serv_char->ble_char->init_value = nut_serv_char->buf;
	nut_serv_char->ble_char->value_init_len = nut_serv_char->buf_size;
	nut_serv_char->ble_char->value_max_len = nut_serv_char->buf_size;

	/* Configure the NUT characteristic permission */
	if(BLE_PAIR_ENABLE){
		nut_serv_char->ble_char->value_permissions = (AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR);
	}else{
		nut_serv_char->ble_char->value_permissions = AT_BLE_ATTR_NO_PERMISSIONS;
	}
}

static uint8_t nut_cmd_char_buf[NUT_CMD_CHAR_BUF_LEN];
static void nut_cmd_char_init(nut_serv_char_t * nut_serv_char)
{
	/* auto code hear */
	/*Configure SPOG Characteristic*/
	nut_serv_char->ble_char->uuid.type = AT_BLE_UUID_128;
	memcpy(&nut_serv_char->ble_char->uuid.uuid[0], NUT_CMD_CHAR_UUID, NUT_UUID_128_LEN);

	/* config Characteristic access properties*/
	nut_serv_char->ble_char->properties = AT_BLE_CHAR_NOTIFY | AT_BLE_CHAR_WRITE;
	
	nut_serv_char->ble_char->init_value = nut_serv_char->buf;
	nut_serv_char->ble_char->value_init_len = nut_serv_char->buf_size;
	nut_serv_char->ble_char->value_max_len = nut_serv_char->buf_size;

	/* Configure the NUT characteristic permission */
	if(BLE_PAIR_ENABLE){
		nut_serv_char->ble_char->value_permissions = (AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_REQ_AUTHN_NO_AUTHR);
	}else{
		nut_serv_char->ble_char->value_permissions = AT_BLE_ATTR_NO_PERMISSIONS;
	}
}

/**
* \NUT service Configuration function
*/
at_ble_status_t nut_serv_init()
{
	/* Configure SPOG Service UUID info*/
	nut_inst.serv_uuid.type =	 AT_BLE_UUID_128;
	memcpy(&nut_inst.serv_uuid.uuid[0], NUT_SERVICE_UUID, NUT_UUID_128_LEN);
	
	/* Configure SPOG Service Handle, output, set by call fn*/
	nut_inst.serv_handle = 0;
	
	// init characeristic: power_char
	nut_inst.nut_serv_chars[0].ble_char = nut_inst.nut_chars + 0;
	nut_inst.nut_serv_chars[0].buf = nut_power_char_buf;
	nut_inst.nut_serv_chars[0].buf_size = NUT_POWER_CHAR_BUF_LEN;
	nut_power_char_init(nut_inst.nut_serv_chars + 0);

	// init characeristic: accelerate_char
	nut_inst.nut_serv_chars[1].ble_char = nut_inst.nut_chars + 1;
	nut_inst.nut_serv_chars[1].buf = nut_accelerate_char_buf;
	nut_inst.nut_serv_chars[1].buf_size = NUT_ACCELERATE_CHAR_BUF_LEN;
	nut_accelerate_char_init(nut_inst.nut_serv_chars + 1);

	// init characeristic: heartrate_char
	nut_inst.nut_serv_chars[2].ble_char = nut_inst.nut_chars + 2;
	nut_inst.nut_serv_chars[2].buf = nut_heartrate_char_buf;
	nut_inst.nut_serv_chars[2].buf_size = NUT_HEARTRATE_CHAR_BUF_LEN;
	nut_heartrate_char_init(nut_inst.nut_serv_chars + 2);

	// init characeristic: ecg_char
	nut_inst.nut_serv_chars[3].ble_char = nut_inst.nut_chars + 3;
	nut_inst.nut_serv_chars[3].buf = nut_ecg_char_buf;
	nut_inst.nut_serv_chars[3].buf_size = NUT_ECG_CHAR_BUF_LEN;
	nut_ecg_char_init(nut_inst.nut_serv_chars + 3);

	// init characeristic: cmd_char
	nut_inst.nut_serv_chars[4].ble_char = nut_inst.nut_chars + 4;
	nut_inst.nut_serv_chars[4].buf = nut_cmd_char_buf;
	nut_inst.nut_serv_chars[4].buf_size = NUT_CMD_CHAR_BUF_LEN;
	nut_cmd_char_init(nut_inst.nut_serv_chars + 4);
	return (at_ble_primary_service_define(
		&nut_inst.serv_uuid, &nut_inst.serv_handle, 
		NULL, 0, & nut_inst.nut_chars,  NUT_CHARS_SIZE));
}

void nut_ble_init()
{
	at_ble_status_t status;

	if (nut_serv_init() != AT_BLE_SUCCESS) {
		DBG_LOG("init service error");
	}
	nut_event_callback_init();
	
	status = ble_advertisement_data_set();
	if (status != AT_BLE_SUCCESS) {
		DBG_LOG("Advertisement data set failed reason %d",status);
	}
}


/**
* \nut device advertisement handler function
*/
void nut_prf_dev_adv(void)
{
	/* Start of advertisement */
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL,
	AT_BLE_ADV_FP_ANY, NUT_FAST_ADV, NUT_ADV_TIMEOUT, 0) == AT_BLE_SUCCESS){
		DBG_LOG("Device Started Advertisement");
	}else{
		DBG_LOG("Device Advertisement Failed");
	}
}

at_ble_connected_t conn_params;

/**
* \CSC service send data function
*/
at_ble_status_t nut_serv_send_data(at_ble_characteristic_t *characteristic, 
	uint16_t connhandle, uint8_t *databuf, uint16_t datalen)
{
	at_ble_status_t status;
	uint16_t value = 0;
	uint16_t length;
	length = sizeof(uint16_t);
	status = at_ble_characteristic_value_get(characteristic->client_config_handle, (uint8_t *)&value, &length);
	// DBG_LOG("check write status");
	if (status != AT_BLE_SUCCESS){
		DBG_LOG("at_ble_characteristic_value_get value get failed");
		return status;
	}
	if(value == 1){
		status = at_ble_characteristic_value_set(characteristic->char_val_handle, databuf, datalen);
		if (status != AT_BLE_SUCCESS){
			DBG_LOG("at_ble_characteristic_value_set value set failed");
			return status;
		}
		status = at_ble_notification_send(connhandle, characteristic->char_val_handle);
		if (status != AT_BLE_SUCCESS){
			DBG_LOG("at_ble_notification_send  failed");
			return status;
		}
	}/*else{
		printf("check value !=0: %d, status:%d\r\n", value, status);
	}*/
	return status;
}

at_ble_status_t nut_serv_ecg_send_data(uint8_t *databuf, uint16_t datalen)
{
	return nut_serv_send_data(nut_inst.nut_serv_chars[3].ble_char, conn_params.handle, databuf, datalen);
}