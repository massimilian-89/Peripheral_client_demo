/**
 ****************************************************************************************
 *
 * @file user_profile.h
 *
 * @brief Bare Bone application header file.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_PROFILE_H_
#define _USER_PROFILE_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "app.h"                       // application definitions
#include "co_error.h"                  // error code definitions
#include "arch_wdg.h"
#include "app_callback.h"
#include "app_default_handlers.h"
#include "custs1_task.h"
#include "custs1.h"
#include "rwip_config.h"
#include "gap.h"
#include "app_easy_timer.h"
#include "ke_task.h"
#include "co_bt.h"
#include "uart_utils.h"
#include "arch_console.h"
#include "user_ble_common.h"
#include "user_ble_gatt.h"


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

// Data Structures to save service type

typedef struct {
	
    bool is_readable;
    bool is_writable;
    bool supports_notifications;
	
} g_support;


// Data Structures to save matched UUID parameters

typedef struct {
	
    uint16_t handle;
    uint8_t con_id;
    uint8_t RES_data[20];       // Adjust the size as needed
	
} MatchedCharacteristic;

static MatchedCharacteristic matched_char;

typedef enum {
	
    CHAR_PROPERTY_NONE = 0,
    CHAR_PROPERTY_READABLE = 1 << 0,
    CHAR_PROPERTY_WRITABLE = 1 << 1,
    CHAR_PROPERTY_NOTIFIABLE = 1 << 2
	
} CharacteristicProperties;	

static g_support char_properties = { false, false, false };

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Application initialization function.
 ****************************************************************************************
*/
void user_app_init(void);

/**
 ****************************************************************************************
 * @brief Advertising function.
 ****************************************************************************************
*/
void user_app_adv_start(void);

/**
 ****************************************************************************************
 * @brief Connection function.
 * @param[in] connection_idx Connection Id index
 * @param[in] param Pointer to GAPC_CONNECTION_REQ_IND message
 ****************************************************************************************

*/
void user_app_connection(uint8_t connection_idx, 
                         struct gapc_connection_req_ind const *param);


/**
 ****************************************************************************************
 * @brief Undirect advertising completion function.
 * @param[in] status Command complete event message status
 ****************************************************************************************
*/
void user_app_adv_undirect_complete(uint8_t status);

/**
 ****************************************************************************************
 * @brief Disconnection function.
 * @param[in] param Pointer to GAPC_DISCONNECT_IND message
 ****************************************************************************************
*/
void user_app_disconnect(struct gapc_disconnect_ind const *param);

/**
 ****************************************************************************************
 * @brief Handles the messages that are not handled by the SDK internal mechanisms.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id);
													
static void handle_svc_ind(uint8_t con_idx, struct gattc_sdp_svc_ind const *disc_svc);
//static void handle_matching_characteristic(gattc_chars_t* gatt_char, uint8_t con_idx, struct gattc_sdp_svc_ind const* disc_svc);		
													
									
													
/**
 ****************************************************************************************
 * @brief Handles a writable characteristic.
 *
 * @param[in] con_idx Connection index.
 * @param[in] handle  Characteristic handle.
 ****************************************************************************************
 */
static void handle_writable_characteristic(uint8_t con_idx, uint16_t handle);

/**
 ****************************************************************************************
 * @brief Timer and read handler function.
 ****************************************************************************************
 */
static void timer_and_read_handler(void);

/**
 ****************************************************************************************
 * @brief Handles a matching characteristic.
 *
 * @param[in] char_properties Characteristic properties.
 * @param[in] con_idx         Connection index.
 * @param[in] disc_svc        SDP service indication.
 ****************************************************************************************
 */
static void handle_matching_characteristic(g_support char_properties, uint8_t con_idx, struct gattc_sdp_svc_ind const *disc_svc);

/**
 ****************************************************************************************
 * @brief Handles a service indication.
 *
 * @param[in] con_idx  Connection index.
 * @param[in] disc_svc SDP service indication.
 ****************************************************************************************
 */
static void handle_svc_ind(uint8_t con_idx, struct gattc_sdp_svc_ind const* disc_svc);

/**
 ****************************************************************************************
 * @brief Compares two UUIDs for equality.
 *
 * @param[in] uuid1 First UUID to compare.
 * @param[in] uuid2 Second UUID to compare.
 *
 * @return true if the UUIDs are equal, false otherwise.
 ****************************************************************************************
 */
static bool compare_uuid(const uint8_t *uuid1, const uint8_t *uuid2);

/**
 ****************************************************************************************
 * @brief Checks if a characteristic is readable.
 *
 * @param[in] gatt_char Pointer to the GATT characteristic.
 *
 * @return true if the characteristic is readable, false otherwise.
 ****************************************************************************************
 */
static bool is_characteristic_readable(const gattc_chars_t *gatt_char);

/**
 ****************************************************************************************
 * @brief Checks if a characteristic is writable.
 *
 * @param[in] gatt_char Pointer to the GATT characteristic.
 *
 * @return true if the characteristic is writable, false otherwise.
 ****************************************************************************************
 */
static bool is_characteristic_writable(const gattc_chars_t *gatt_char);

/**
 ****************************************************************************************
 * @brief Checks if a characteristic supports notifications.
 *
 * @param[in] gatt_char Pointer to the GATT characteristic.
 *
 * @return true if the characteristic supports notifications, false otherwise.
 ****************************************************************************************
 */
static bool is_characteristic_notifiable(const gattc_chars_t *gatt_char);

/**
 ****************************************************************************************
 * @brief Sets up the Button Press ISR.
 ****************************************************************************************
 */
static void setButtonPress_isr(void);


#endif //_USER_PROFILE_H_
