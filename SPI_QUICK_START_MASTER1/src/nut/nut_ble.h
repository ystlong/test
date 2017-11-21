
#ifndef NUT_BLE_H
#define NUT_BLE_H 1

/** @brief Custom serial chat UUID length */
#define NUT_UUID_128_LEN				(16)
#define NUT_CHARS_SIZE					(5)

/* Service */
#define NUT_SERVICE_UUID              ("\x0b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x00\x12\xbc\x0a")
/* 电量 */
#define NUT_POWER_CHAR_UUID           ("\x1b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x01\x12\xbc\x0a")
/* 加速度计 */
#define NUT_ACCELERATE_CHAR_UUID      ("\x2b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x01\x12\xbc\x0a")
/* 心率 */
#define NUT_HEARTRATE_CHAR_UUID       ("\x3b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x01\x12\xbc\x0a")
/* 心电 */
#define NUT_ECG_CHAR_UUID             ("\x4b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x01\x12\xbc\x0a")
/* 命令 */
#define NUT_CMD_CHAR_UUID             ("\x5b\xc5\xd5\xa5\x02\x00\xa6\x85\xe5\x11\x35\x39\x01\x12\xbc\x0a")

#define NUT_SERVICE_BUF_LEN           (150)
#define NUT_POWER_CHAR_BUF_LEN        (150)
#define NUT_ACCELERATE_CHAR_BUF_LEN   (150)
#define NUT_HEARTRATE_CHAR_BUF_LEN    (150)
#define NUT_ECG_CHAR_BUF_LEN          (150)
#define NUT_CMD_CHAR_BUF_LEN          (150)


/** @brief NUT_FAST_ADV between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). */
#define NUT_FAST_ADV						(1600)

/** @brief NUT_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x028F in seconds, 0x0000 disables time-out.*/
#define NUT_ADV_TIMEOUT						(655)


void nut_event_callback_init();
void nut_ble_init();
void nut_prf_dev_adv();

#endif