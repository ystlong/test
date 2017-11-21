/*
 * CFile1.c
 *
 * Created: 2017/9/24 12:56:07
 *  Author: long
 */ 
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "nut_ble.h"


/**@brief start and end handle */
#define START_HANDLE					(0x0001)
#define END_HANDLE						(0xffff)
extern at_ble_connected_t conn_params;
// at_ble_uuid_t	serv_uuid;
// at_ble_handle_t	serv_handle;
// at_ble_characteristic_t	endpoint_chars;

/**
 * @brief Connection handler invoked by ble manager
 */
static at_ble_status_t nut_prf_connected_state_handler(void *params)
{
	// printf("-----nut_prf_connected_state_handler--------\n");
	 at_ble_status_t status;
	// /** Connection related information*/
	memcpy((uint8_t *)&conn_params, params, sizeof(at_ble_connected_t));
	
	 // if(!app_csc_info.devicedb){		
	 // app_csc_info.discover_role = DISCOVER_SERVICE;	
	 // serv_uuid.type = AT_BLE_UUID_128;
	 // memcpy(&serv_uuid.uuid[0], NUT_SERVICE_UUID, NUT_UUID_128_LEN);
	 // /* Discover Remote Service by service UUID */
	 // status = at_ble_primary_service_discover_by_uuid(conn_params.handle,START_HANDLE, END_HANDLE, &serv_uuid);
	 // if(status != AT_BLE_SUCCESS){
	 // 	DBG_LOG("Failed to start service discovery. status = %d", status);
	 // } else {
	 // 	DBG_LOG_DEV("Started service discovery");
	 // }		
	 // }
	return AT_BLE_SUCCESS;

}
extern uint8_t already_send;
extern uint8_t conn_ok;
char * conn_ok_cmd = "c_ok";
char * conn_leave_cmd = "c_le";

int cmp_cmd(char *buf, char *cmd, int cmd_len)
{
	// dangerous code
	for(int i=0; i<cmd_len; i++){
		if (*(buf+i) != *(cmd+i)) {
			return 0;
		}
	}
	return 1;
}

/**
 * @brief invoked by ble manager on receiving notification
 */
at_ble_status_t nut_prf_write_handler(void *params)
{
	at_ble_characteristic_changed_t charct;
	memcpy((uint8_t*)&charct, params, sizeof(at_ble_characteristic_changed_t));
	uint16_t char_len = charct.char_len;
	char *buf = &charct.char_new_value[0];
	buf[char_len] = '\0';
	printf("%s", buf);
	// DBG_LOG("recive write\n\r");
	//at_ble_status_t nut_serv_ecg_send_data(uint16_t connhandle, uint8_t *databuf, uint16_t datalen);
	//nut_serv_ecg_send_data(charct.conn_handle, buf, char_len);
	already_send = 1;
	// equl cmd c_ok
	if (cmp_cmd(buf, conn_ok_cmd, 4)){
		conn_ok = 1;
	}else if(cmp_cmd(buf, conn_leave_cmd, 4)){
		conn_ok = 0;
	}
	return AT_BLE_SUCCESS;
}

/* GAP events */
static const ble_event_callback_t nut_gap_handle[] = {
	// case AT_BLE_UNDEFINED_EVENT:
	NULL,
	// case AT_BLE_SCAN_INFO:
	NULL,
	// case AT_BLE_SCAN_REPORT:
	NULL,
	// case AT_BLE_ADV_REPORT:
	NULL,
	// case AT_BLE_RAND_ADDR_CHANGED:
	NULL,
	// case AT_BLE_CONNECTED:
	nut_prf_connected_state_handler,
	// NULL,
	// case AT_BLE_DISCONNECTED:
	NULL,
	// case AT_BLE_CONN_PARAM_UPDATE_DONE:
	NULL,
	// case AT_BLE_CONN_PARAM_UPDATE_REQUEST:
	NULL,
	// case AT_BLE_PAIR_DONE:
	NULL,
	// case AT_BLE_PAIR_REQUEST:
	NULL,
	// case AT_BLE_SLAVE_SEC_REQUEST:
	NULL,
	// case AT_BLE_PAIR_KEY_REQUEST:
	NULL,
	// case AT_BLE_ENCRYPTION_REQUEST:
	NULL,
	// case AT_BLE_ENCRYPTION_STATUS_CHANGED:
	NULL,
	// case AT_BLE_RESOLV_RAND_ADDR_STATUS:
	NULL,
	// case AT_BLE_SIGN_COUNTERS_IND:
	NULL,
	// case AT_BLE_PEER_ATT_INFO_IND:
	NULL,
	// case AT_BLE_CON_CHANNEL_MAP_IND:
	NULL
};


/*gatt server event handle index*/
static const ble_event_callback_t nut_gatt_server_handle[] = {
	// case AT_BLE_NOTIFICATION_CONFIRMED:
	NULL,
	// case AT_BLE_INDICATION_CONFIRMED:
	NULL,
	// case AT_BLE_CHARACTERISTIC_CHANGED:
	nut_prf_write_handler,
	// NULL,
	// case AT_BLE_CHARACTERISTIC_CONFIGURATION_CHANGED:
	NULL,
	// case AT_BLE_SERVICE_CHANGED_INDICATION_SENT:
	NULL,
	// case AT_BLE_WRITE_AUTHORIZE_REQUEST:
	NULL,
	// case AT_BLE_MTU_CHANGED_INDICATION:
	NULL,
	// case AT_BLE_MTU_CHANGED_CMD_COMPLETE:
	NULL,
	// case AT_BLE_CHARACTERISTIC_WRITE_CMD_CMP:
	NULL,
	// case AT_BLE_READ_AUTHORIZE_REQUEST:
	NULL
};


/* gatt client event handler*/
static const ble_event_callback_t nut_gatt_client_handle[] = {
	// case AT_BLE_PRIMARY_SERVICE_FOUND:
	NULL,
	// case AT_BLE_INCLUDED_SERVICE_FOUND:
	NULL,
	// case AT_BLE_CHARACTERISTIC_FOUND:
	NULL,
	// case AT_BLE_DESCRIPTOR_FOUND:
	NULL,
	// case AT_BLE_DISCOVERY_COMPLETE:
	NULL,
	// case AT_BLE_CHARACTERISTIC_READ_BY_UUID_RESPONSE:
	NULL,
	// case AT_BLE_CHARACTERISTIC_READ_MULTIBLE_RESPONSE:
	NULL,
	// case AT_BLE_CHARACTERISTIC_WRITE_RESPONSE:
	NULL,
	// case AT_BLE_NOTIFICATION_RECIEVED:
	// nut_prf_notification_handler,
	NULL,
	// case AT_BLE_INDICATION_RECIEVED:
	NULL
};


void nut_event_callback_init()
{
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
	BLE_GAP_EVENT_TYPE,
	nut_gap_handle);
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
	BLE_GATT_CLIENT_EVENT_TYPE,
	nut_gatt_client_handle);
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
	BLE_GATT_SERVER_EVENT_TYPE,
	nut_gatt_server_handle);
}
