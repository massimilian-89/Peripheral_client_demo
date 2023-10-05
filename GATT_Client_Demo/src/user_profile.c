/**
 ****************************************************************************************
 *
 * @file user_profile.c
 *
 * @brief Profile project source code.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup GATT_CLIENT_DEMO
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_profile.h"
#include "user_periph_setup.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/


void user_app_init(void)
{
		arch_printf("\n\n\rinit");
    default_app_on_init();
}


// Function to start advertising

void user_app_adv_start(void)
{
   
    struct gapm_start_advertise_cmd* cmd;
    cmd = app_easy_gap_undirected_advertise_get_active();
    app_easy_gap_undirected_advertise_start();
	

}


// Callback function when advertising completes
void user_app_adv_undirect_complete(uint8_t status)
{
    // If advertising was canceled then update advertising data and start advertising again
    if (status == GAP_ERR_CANCELED)
    {
        user_app_adv_start();
    }
} 


// Callback function when a connection is established
void user_app_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
    if (app_env[connection_idx].conidx != GAP_INVALID_CONIDX)
    {
        app_connection_idx = connection_idx;
				arch_printf("\n\n\rconnected \r\n");
				connection_flag=1;
    }
    else
    {
	
        // No connection has been established, restart advertising
        user_app_adv_start();
	
		
    }

    default_app_on_connection(connection_idx, param);
		user_gatt_discover_all_services(connection_idx, 1);	
}


// Callback function when a disconnection occurs
void user_app_disconnect(struct gapc_disconnect_ind const *param)
{
	


		arch_printf("\n\n\rDisconnect");
    ADV_status_flag=0;
	 connection_flag=0;
		wdg_freeze(); 
	
}




void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    switch(msgid)
    {
				case GATTC_CMP_EVT:
				{
					
				struct gattc_cmp_evt const* evt = (struct gattc_cmp_evt const *)(param);
				
					switch(evt->operation)
						{
							case GATTC_SDP_DISC_SVC_ALL:
							{
								
								struct gattc_sdp_svc_ind const *disc_svc = (struct gattc_sdp_svc_ind const *)(param);
								

							}break;
							}
						}
						
						case GAPC_PARAM_UPDATED_IND:
						{

									

							} 
							break;
							
					 case GATTC_SDP_SVC_IND:
						 
						{
							
						struct gattc_sdp_svc_ind const *disc_svc = (struct gattc_sdp_svc_ind const *)(param);
							
						uint8_t con_idx = KE_IDX_GET(src_id);
						handle_svc_ind(con_idx, disc_svc);
						handle_matching_characteristic(char_properties, matched_char.con_id, disc_svc);
							
						}break;
						
					case GATTC_READ_IND:
											{
													struct gattc_read_ind const *read_ind = (struct gattc_read_ind const *)param;
												
													arch_printf("read command received...");
													arch_printf("\n\n\rhandle: 0x%04X\n", read_ind->handle);
													arch_printf("\n\n\roffset: 0x%04X\n", read_ind->offset);
													arch_printf("\n\n\rlength: 0x%04X\n", read_ind->length);
													
													arch_printf("\n\n\rvalue: ");
												
													for (uint16_t i = 0; i < read_ind->length; i++) {
															arch_printf("0x%02X \n", read_ind->value[i]);
													}
													
													arch_printf("\n");
													
											}break;
											
					 case GATTC_EVENT_IND:
						 
								{
										struct gattc_event_ind const *notify_ind = (struct gattc_event_ind const *)param;
										
										arch_printf("notification received...");

										arch_printf("\n\n\rvalue: ");
										for (uint16_t i = 0; i < notify_ind->length; i++) {
												arch_printf("0x%02X \n", notify_ind->value[i]);
										}

								}
								break;						
					

						default:
								break;
				}
}



// Function to compare UUIDs
static bool compare_uuid(const uint8_t* uuid1, const uint8_t* uuid2) {
    return memcmp(uuid1, uuid2, 16) == 0;
}


// Function to check if a characteristic is readable
static bool is_characteristic_readable(const gattc_chars_t* gatt_char) {
    return gatt_char->c.properties & ATT_CHAR_PROP_READ;
}

// Function to check if a characteristic is writable
static bool is_characteristic_writable(const gattc_chars_t* gatt_char) {
    return gatt_char->c.properties & ATT_CHAR_PROP_WRITE;
}

// Function to check if a characteristic supports notifications
static bool is_characteristic_notifiable(const gattc_chars_t* gatt_char) {
    return gatt_char->c.properties & ATT_CHAR_PROP_NOTIFY;
}



void buttonPress_isr(void) 
	
{
    // Increment the counter
    switchCounter_value++;


    // Print the updated value
    arch_printf("\n Writing to value:\t 0x%x\r\n", switchCounter_value);
    
    // Write the updated value to the characteristic
    user_ble_gatt_write(GATTC_WRITE, matched_char.con_id, matched_char.handle, &switchCounter_value, sizeof(switchCounter_value));
    

}

// Function to handle characteristic write when writable and button pressed

void setButtonPress_ir(void)
{
	
	
		GPIO_EnableIRQ(GPIO_SW_PORT, GPIO_SW_PIN, GPIO0_IRQn, INPUT_LEVEL, EDGE_LEVEL, DEBOUNCE_MS);
		GPIO_SetIRQInputLevel(GPIO0_IRQn, GPIO_IRQ_INPUT_LEVEL_LOW);
		GPIO_RegisterCallback(GPIO0_IRQn, buttonPress_isr);

}


// Function to handle characteristic write when writable and button pressed

static void handle_writable_characteristic(uint8_t con_idx, uint16_t handle) 
{
    setButtonPress_ir();
}


// Timer interrupt handler and characteristic read handler

void timer_and_read_handler(void) {
    user_gatt_read_simple(matched_char.con_id, matched_char.handle);
    app_easy_timer(1000, timer_and_read_handler);
}


// Function to handle matching characteristics

static void handle_matching_characteristic(g_support  char_properties, uint8_t con_idx, struct gattc_sdp_svc_ind const* disc_svc) {
   
	
    if (char_properties.is_writable) {
			
        handle_writable_characteristic(con_idx, matched_char.handle);
			
    } else if (char_properties.is_readable) {
				
        timer_and_read_handler() ;

    } else if (char_properties.supports_notifications) {

        user_ble_gatt_write(GATTC_WRITE,matched_char.con_id, matched_char.handle, &ccc_value, sizeof(ccc_value));
				
        arch_printf("Subscribing for notification...\n");
			
    }

    
}




// Function to handle services and match with the UUID

static void handle_svc_ind(uint8_t con_idx, struct gattc_sdp_svc_ind const* disc_svc) {
    // Initialize other variables
    bool uuid_matched = false;
    uint8_t MyUUID[] = USER_UUID;
	
    // Reset the struct properties
    char_properties.is_readable = false;
    char_properties.is_writable = false;
    char_properties.supports_notifications = false;

    // Allocate memory for service_info_t structure
    service_info_t* svc = ke_malloc(user_ble_get_svc_size(disc_svc), KE_MEM_NON_RETENTION);

    dbg_block_printf("%s: conn_idx=%04x start_h=%04x end_h=%04x\r\n", __func__, con_idx,
                     disc_svc->start_hdl, disc_svc->end_hdl);

    // Parse the discovered service and populate svc
    user_gatt_parse_service(disc_svc, svc, con_idx);

    // Main loop
    for (uint16_t i = 0; i < svc->num_chars; i++) {
        gattc_chars_t* gatt_char = &svc->items[i];

        if (compare_uuid(MyUUID, gatt_char->uuid.uuid.uuid128)) {
					
            // UUID match found, set the flag to true and break out of the loop
            uuid_matched = true;
            arch_printf("\r\nUUID MATCHED");
						matched_char.handle = gatt_char->handle;
            matched_char.con_id = con_idx;
            arch_printf("\nRead: %s\r\n", (gatt_char->c.properties & ATT_CHAR_PROP_READ) ? "Yes" : "No");
            arch_printf("\nWrite: %s\r\n", (gatt_char->c.properties & ATT_CHAR_PROP_WRITE) ? "Yes" : "No");
            arch_printf("\nNotify: %s\r\n", (gatt_char->c.properties & ATT_CHAR_PROP_NOTIFY) ? "Yes" : "No");
            arch_printf("\r\nFOUND UUID in C array style:{\t");

            for (int j = 0; j < 16; j++) {
                arch_printf("0x%02X", gatt_char->uuid.uuid.uuid128[j]);
                if (j < 15) {
                    arch_printf(", ");
                }
            }

            if (is_characteristic_readable(gatt_char)) {
                char_properties.is_readable = true;
                arch_printf("\r\nCharacteristic is READABLE.\r\n");
            }

            if (is_characteristic_writable(gatt_char)) {
                char_properties.is_writable = true;
                arch_printf("\r\nCharacteristic is WRITABLE.\r\n");
            }

            if (is_characteristic_notifiable(gatt_char)) {
                char_properties.supports_notifications = true;
                arch_printf("\r\nCharacteristic supports NOTIFICATIONS.\r\n");
            }

            break; // Exit the loop when a match is found
        }
    }

    if (!uuid_matched) {
       
        arch_printf("\r\nUUID NOT FOUND\r\n");
    }

    // Free allocated memory
    ke_free(svc);


}


/// @} GATT_CLIENT_DEMO
