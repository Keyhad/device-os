/*
 * Copyright (c) 2018 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLE_HAL_H
#define BLE_HAL_H

#include "hal_platform.h"

#if HAL_PLATFORM_BLE

#include <stdint.h>
#include <stddef.h>

#include "ble_hal_defines.h"
#include "ble_hal_impl.h"

/**
 * @addtogroup ble_api
 *
 * @brief
 *   This module provides BLE HAL APIs.
 * @{
 */

#define BLE_API_VERSION 1

typedef enum hal_ble_ant_type_t {
    BLE_ANT_DEFAULT  = 0,
    BLE_ANT_INTERNAL = 1,
    BLE_ANT_EXTERNAL = 2,
} hal_ble_ant_type_t;

typedef enum hal_ble_role_t {
    BLE_ROLE_INVALID    = 0,
    BLE_ROLE_PERIPHERAL = 1,
    BLE_ROLE_CENTRAL    = 2
} hal_ble_role_t;

typedef enum hal_ble_adv_evt_type_t {
    BLE_ADV_CONNECTABLE_SCANNABLE_UNDIRECRED_EVT        = 0,
    BLE_ADV_CONNECTABLE_UNDIRECTED_EVT                  = 1,
    BLE_ADV_CONNECTABLE_DIRECTED_EVT                    = 2,
    BLE_ADV_NON_CONNECTABLE_NON_SCANABLE_UNDIRECTED_EVT = 3,
    BLE_ADV_NON_CONNECTABLE_NON_SCANABLE_DIRECTED_EVT   = 4,
    BLE_ADV_SCANABLE_UNDIRECTED_EVT                     = 5,
    BLE_ADV_SCANABLE_DIRECTED_EVT                       = 6
} hal_ble_adv_evt_type_t;

typedef enum hal_ble_adv_fp_t {
    BLE_ADV_FP_ANY            = 0x00,   /**< Allow scan requests and connect requests from any device. */
    BLE_ADV_FP_FILTER_SCANREQ = 0x01,   /**< Filter scan requests with whitelist. */
    BLE_ADV_FP_FILTER_CONNREQ = 0x02,   /**< Filter connect requests with whitelist. */
    BLE_ADV_FP_FILTER_BOTH    = 0x03    /**< Filter both scan and connect requests with whitelist. */
} hal_ble_adv_fp_t;

typedef enum hal_ble_scan_fp_t {
    BLE_SCAN_FP_ACCEPT_ALL                      = 0x00,  /**< Accept all advertising packets except directed advertising packets
                                                                   not addressed to this device. */
    BLE_SCAN_FP_WHITELIST                       = 0x01,  /**< Accept advertising packets from devices in the whitelist except directed
                                                                   packets not addressed to this device. */
    BLE_SCAN_FP_ALL_NOT_RESOLVED_DIRECTED       = 0x02,  /**< Accept all advertising packets specified in @ref BLE_GAP_SCAN_FP_ACCEPT_ALL.
                                                                   In addition, accept directed advertising packets, where the advertiser's
                                                                   address is a resolvable private address that cannot be resolved. */
    BLE_SCAN_FP_WHITELIST_NOT_RESOLVED_DIRECTED = 0x03   /**< Accept all advertising packets specified in @ref BLE_GAP_SCAN_FP_WHITELIST.
                                                                   In addition, accept directed advertising packets, where the advertiser's
                                                                   address is a resolvable private address that cannot be resolved. */
} hal_ble_scan_fp_t;

typedef enum hal_ble_service_type_t {
    BLE_SERVICE_TYPE_INVALID   = 0,
    BLE_SERVICE_TYPE_PRIMARY   = 1,
    BLE_SERVICE_TYPE_SECONDARY = 2
} hal_ble_service_type_t;

typedef enum hal_ble_uuid_type_t {
    BLE_UUID_TYPE_16BIT          = 0,
    BLE_UUID_TYPE_128BIT         = 1,
    BLE_UUID_TYPE_128BIT_SHORTED = 2
} hal_ble_uuid_type_t;

/* BLE device address */
typedef struct hal_ble_addr_t {
    uint8_t addr[BLE_SIG_ADDR_LEN];
    ble_sig_addr_type_t addr_type;
    uint8_t reserved;
} hal_ble_addr_t;

/* BLE UUID, little endian */
typedef struct hal_ble_uuid_t {
    union {
        uint16_t uuid16;
        uint8_t uuid128[BLE_SIG_UUID_128BIT_LEN];
    };
    hal_ble_uuid_type_t type;
    uint8_t reserved[3];
} hal_ble_uuid_t;

/* BLE advertising parameters */
typedef struct hal_ble_adv_params_t {
    uint16_t version;
    uint16_t size;
    uint16_t interval;                  /**< Advertising interval in 625 us units. */
    uint16_t timeout;                   /**< Advertising timeout in 10 ms units.*/
    hal_ble_adv_evt_type_t type;        /**< Advertising event type.*/
    uint8_t filter_policy;
    uint8_t inc_tx_power;
    uint8_t reserved;
} hal_ble_adv_params_t;

/* BLE scanning parameters */
typedef struct hal_ble_scan_params_t {
    uint16_t version;
    uint16_t size;
    uint16_t interval;                  /**< Scan interval in 625 us units. */
    uint16_t window;                    /**< Scan window in 625 us units. */
    uint16_t timeout;                   /**< Scan timeout in 10 ms units. */
    uint8_t active;
    uint8_t filter_policy;
} hal_ble_scan_params_t;

/* BLE connection parameters */
typedef struct hal_ble_conn_params_t {
    uint16_t version;
    uint16_t size;
    uint16_t min_conn_interval;         /**< Minimum Connection Interval in 1.25 ms units.*/
    uint16_t max_conn_interval;         /**< Maximum Connection Interval in 1.25 ms units.*/
    uint16_t slave_latency;             /**< Slave Latency in number of connection events.*/
    uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units.*/
} hal_ble_conn_params_t;

/* BLE service definition */
typedef struct hal_ble_svc_t {
    uint16_t version;
    uint16_t size;
    hal_ble_uuid_t uuid;
    hal_ble_attr_handle_t start_handle;
    hal_ble_attr_handle_t end_handle;
} hal_ble_svc_t;

typedef struct hal_ble_char_init_t {
    uint16_t version;
    uint16_t size;
    uint8_t properties;
    uint8_t reserved[3];
    const char* description;
    hal_ble_uuid_t uuid;
    hal_ble_attr_handle_t service_handle;
} hal_ble_char_init_t;

typedef struct hal_ble_char_handles_t {
    uint16_t version;
    uint16_t size;
    hal_ble_attr_handle_t decl_handle;
    hal_ble_attr_handle_t value_handle;
    hal_ble_attr_handle_t user_desc_handle;
    hal_ble_attr_handle_t cccd_handle;
    hal_ble_attr_handle_t sccd_handle;
} hal_ble_char_handles_t;

/* BLE characteristic definition */
typedef struct hal_ble_char_t {
    uint16_t version;
    uint16_t size;
    hal_ble_uuid_t uuid;
    uint8_t char_ext_props : 1;
    uint8_t properties;
    uint8_t reserved[2];
    hal_ble_char_handles_t charHandles;
} hal_ble_char_t;

typedef struct hal_ble_desc_init_t {
    uint16_t version;
    uint16_t size;
    uint8_t* descriptor;
    size_t len;
    hal_ble_uuid_t uuid;
    hal_ble_attr_handle_t char_handle;
} hal_ble_desc_init_t;

/* BLE events structure */
typedef struct hal_ble_gap_on_adv_stopped_evt_t {
    void* reserved;
} hal_ble_gap_on_adv_stopped_evt_t;

typedef struct hal_ble_gap_on_scan_result_evt_t {
    int8_t rssi;
    uint8_t reserved;
    struct {
        uint16_t connectable   : 1;     /**< Connectable advertising event type. */
        uint16_t scannable     : 1;     /**< Scannable advertising event type. */
        uint16_t directed      : 1;     /**< Directed advertising event type. */
        uint16_t extended_pdu  : 1;     /**< Received an extended advertising set. */
    } type;
    static_assert(sizeof(type) == sizeof(uint16_t), "Advertising event type length mismatch.");
    uint8_t* adv_data;
    uint8_t* sr_data;
    uint16_t adv_data_len;
    uint16_t sr_data_len;
    hal_ble_addr_t peer_addr;
} hal_ble_gap_on_scan_result_evt_t;

typedef struct hal_ble_gap_on_connected_evt_t {
    uint16_t conn_interval;             /**< Connection Interval in 1.25 ms units.*/
    uint16_t slave_latency;             /**< Slave Latency in number of connection events.*/
    uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units.*/
    uint8_t role;                       /**< It can be either BLE_ROLE_PERIPHERAL or BLE_ROLE_CENTRAL. */
    uint8_t reserved;
    hal_ble_addr_t peer_addr;
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gap_on_connected_evt_t;

typedef struct hal_ble_gap_on_disconnected_evt_t {
    uint8_t reason;
    uint8_t reserved[3];
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gap_on_disconnected_evt_t;

typedef struct hal_ble_gap_on_conn_params_evt_t {
    uint16_t conn_interval;             /**< Connection Interval in 1.25 ms units.*/
    uint16_t slave_latency;             /**< Slave Latency in number of connection events.*/
    uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units.*/
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gap_on_conn_params_evt_t;

typedef struct hal_ble_gatt_on_params_updated_evt_t {
    size_t att_mtu_size;
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gatt_on_params_updated_evt_t;

typedef struct hal_ble_gattc_on_svc_disc_evt_t {
    size_t count;
    hal_ble_svc_t* services;
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gattc_on_svc_disc_evt_t;

typedef struct hal_ble_gattc_on_char_disc_evt_t {
    size_t count;
    hal_ble_char_t* characteristics;
    hal_ble_conn_handle_t conn_handle;
} hal_ble_gattc_on_char_disc_evt_t;

typedef struct hal_ble_gatt_on_data_evt_t {
    size_t offset;
    size_t data_len;
    uint8_t* data;
    hal_ble_conn_handle_t conn_handle;
    hal_ble_attr_handle_t attr_handle;
} hal_ble_gatt_on_data_evt_t;

typedef enum hal_ble_evts_type_t {
    BLE_EVT_UNKNOWN = 0x00,
    BLE_EVT_ADV_STOPPED = 0x01,
    BLE_EVT_SCAN_RESULT = 0x02,
    BLE_EVT_CONNECTED = 0x03,
    BLE_EVT_DISCONNECTED = 0x04,
    BLE_EVT_CONN_PARAMS_UPDATED = 0x05,
    BLE_EVT_GATT_PARAMS_UPDATED = 0x06,
    BLE_EVT_SVC_DISCOVERED = 0x07,
    BLE_EVT_CHAR_DISCOVERED = 0x08,
    BLE_EVT_DATA_WRITTEN = 0x09,
    BLE_EVT_DATA_NOTIFIED = 0x0A,
    BLE_EVT_MAX = 0x7FFFFFFF
} hal_ble_evts_type_t;

typedef struct hal_ble_evts_t {
    uint16_t version;
    uint16_t size;
    hal_ble_evts_type_t type;
    union {
        hal_ble_gap_on_adv_stopped_evt_t adv_stopped;
        hal_ble_gap_on_scan_result_evt_t scan_result;
        hal_ble_gap_on_connected_evt_t connected;
        hal_ble_gap_on_disconnected_evt_t disconnected;
        hal_ble_gap_on_conn_params_evt_t conn_params_updated;
        hal_ble_gatt_on_params_updated_evt_t gatt_params_updated;
        hal_ble_gattc_on_svc_disc_evt_t svc_disc;
        hal_ble_gattc_on_char_disc_evt_t char_disc;
        hal_ble_gatt_on_data_evt_t data_rec;
    } params;
} hal_ble_evts_t;

typedef void (*on_ble_evt_cb_t)(const hal_ble_evts_t* event, void* context);
typedef void (*on_ble_scan_result_cb_t)(const hal_ble_gap_on_scan_result_evt_t* result, void* context);
typedef void (*on_ble_disc_service_cb_t)(const hal_ble_gattc_on_svc_disc_evt_t* event, void* context);
typedef void (*on_ble_disc_char_cb_t)(const hal_ble_gattc_on_char_disc_evt_t* event, void* context);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Acquires the lock for exclusive access to the BLE API.
 *
 * @param      reserved  The reserved
 *
 * @returns    0 on success, system_error_t on error.
 */
int hal_ble_lock(void* reserved);

/**
 * Releases the lock for exclusive access to the OpenThread API.
 *
 * @param      reserved  The reserved
 *
 * @returns    0 on success, system_error_t on error.
 */
int hal_ble_unlock(void* reserved);

/**
 * Initialize the BLE stack. This function must be called previous to any other BLE APIs.
 *
 * @param[in]   reserved    Reserved for future use.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_stack_init(void* reserved);

/**
 * Select the antenna for BLE radio.
 *
 * @param[in]   antenna The antenna type, either be BLE_ANT_INTERNAL or BLE_ANT_EXTERNAL.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_select_antenna(hal_ble_ant_type_t antenna);

/**
 * Set the callback on BLE events.
 *
 * @param[in]   callback    The callback function.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_set_callback_on_events(on_ble_evt_cb_t callback, void* context);

/**
 * Set local BLE identity address, which type must be either public or random.
 *
 * @param[in]   address Pointer to the BLE local identity address to be set.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_device_address(const hal_ble_addr_t* address);

/**
 * Get local BLE identity address, which type is either public or random static.
 *
 * @param[out]  address Pointer to address structure to be filled in.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_device_address(hal_ble_addr_t* address);

/**
 * Set the BLE device name.
 *
 * @param[in]   device_name Pointer to a UTF-8 encoded, <b>non NULL-terminated</b> string.
 * @param[in]   len         Length of the UTF-8, <b>non NULL-terminated</b> string pointed to by device_name
 *                          in octets (must be smaller or equal than @ref BLE_GAP_DEVNAME_MAX_LEN).
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_device_name(const char* device_name, size_t len);

/**
 * Get the BLE device name.
 *
 * @param[out]      device_name Pointer to an empty buffer where the UTF-8 <b>non NULL-terminated</b> string will be placed.
 *                              Set to NULL to obtain the complete device name length.
 * @param[in,out]   p_len       Length of the buffer pointed by device_name, complete device name length on output.
 *
 * @returns         0 on success, system_error_t on error.
 */
int hal_ble_gap_get_device_name(char* device_name, size_t len);

/**
 * Set GAP Appearance value.
 *
 * @param[in]   appearance  Appearance (16-bit), see @ref BLE_APPEARANCES.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_appearance(ble_sig_appearance_t appearance);

/**
 * Get GAP Appearance value.
 *
 * @param[out]  appearance  Pointer to appearance (16-bit) to be filled in, see @ref BLE_APPEARANCES.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_appearance(ble_sig_appearance_t* appearance);

/**
 * For Central, it set the connection parameters.
 * For Peripheral, it set GAP Peripheral Preferred Connection Parameters.
 *
 * @param[in]   ppcp    Pointer to a hal_ble_conn_params_t structure with the desired parameters.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_ppcp(const hal_ble_conn_params_t* ppcp, void* reserved);

/**
 * Get GAP Peripheral Preferred Connection Parameters.
 *
 * @param[in]   ppcp    Pointer to a hal_ble_conn_params_t structure where the parameters will be stored.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_ppcp(hal_ble_conn_params_t* ppcp, void* reserved);

/**
 * Set a BLE device whitelist. Only one whitelist can be used at a time and the whitelist is shared
 * between the BLE roles. The whitelist cannot be set if a BLE role is using the whitelist.
 *
 * @param[in]   addr_list   Pointer to a whitelist of peer addresses.
 * @param[in]   len         Length of the whitelist, maximum @ref BLE_GAP_WHITELIST_ADDR_MAX_COUNT.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_add_whitelist(const hal_ble_addr_t* addr_list, size_t len, void* reserved);

/**
 * Delete a BLE device whitelist.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_delete_whitelist(void* reserved);

/**
 * Set the TX Power for advertisement.
 *
 * @param[in]   value   Radio transmit power in dBm.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_tx_power(int8_t tx_power);

/**
 * Get the TX Power for advertisement.
 *
 * @returns     The current TX power.
 */
int hal_ble_gap_get_tx_power(int8_t* tx_power, void* reserved);

/**
 * Set the BLE advertising parameters. Changing the advertising parameter during
 * advertising will restart advertising using the new parameters.
 *
 * @param[in]   adv_params  Pointer to the advertising parameters to be set.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_advertising_parameters(const hal_ble_adv_params_t* adv_params, void* reserved);


/**
 * Get the current BLE advertising parameters.
 *
 * @param[in,out]   adv_params  Pointer to the advertising parameters to be filled.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_advertising_parameters(hal_ble_adv_params_t* adv_params, void* reserved);

/**
 * Set the BLE advertising data. It will update the advertising data immediately if success.
 * Changing the advertising data during advertising will restart advertising using the
 * previous advertising parameters and the new advertising data.
 *
 * @param[in]   buf     Pointer to the advertising data to be set.
 * @param[in]   len     Length of the advertising data.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_advertising_data(const uint8_t* buf, size_t len, void* reserved);

/**
 * Get the current BLE advertising data.
 *
 * @param[in,out]   buf     Pointer to the advertising data to be filled.
 * @param[in]       len     Length of the given buffer.
 *
 * @returns     The length of copied data, or system_error_t if negative value.
 */
ssize_t hal_ble_gap_get_advertising_data(uint8_t* buf, size_t len, void* reserved);

/**
 * Set the BLE scan response data.
 *
 * @param[in]   buf     Pointer to the scan response data to be set.
 * @param[in]   len     Length of the scan response data.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_scan_response_data(const uint8_t* buf, size_t len, void* reserved);

/**
 * Get the current BLE scan response data.
 *
 * @param[in,out]   buf     Pointer to the scan response data to be filled.
 * @param[in]       len     Length of the given buffer.
 *
 * @returns     The length of copied data, or system_error_t if negative value.
 */
ssize_t hal_ble_gap_get_scan_response_data(uint8_t* buf, size_t len, void* reserved);

/**
 * Start BLE advertising.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_start_advertising(void* reserved);

/**
 * Stop BLE advertising.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_stop_advertising(void);

/**
 * Check if BLE is advertising.
 *
 * @returns     true if it is advertising, otherwise, false.
 */
bool hal_ble_gap_is_advertising(void);

/**
 * Set the BLE scanning parameters.
 *
 * @param[in]   scan_params Pointer to the scanning parameters to be set.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_set_scan_parameters(const hal_ble_scan_params_t* scan_params, void* reserved);

/**
 * Get the BLE scanning parameters.
 *
 * @param[in,out]   scan_params Pointer to the scanning parameters to be filled.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_scan_parameters(hal_ble_scan_params_t* scan_params, void* reserved);

/**
 * Start scanning nearby BLE devices.
 *
 * @param[in] callback  The callback function to handle the scan results.
 * @param[in] context   The callback function context.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_start_scan(on_ble_scan_result_cb_t callback, void* context, void* reserved);

/**
 * Check if BLE is scanning nearby devices.
 *
 * @returns     true if it is scanning, otherwise false.
 */
bool hal_ble_gap_is_scanning(void);

/**
 * Stop scanning nearby BLE devices.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_stop_scan(void);

/**
 * Connect to a peer BLE device.
 *
 * @param[in]   address Pointer to the peer BLE identity address.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_connect(const hal_ble_addr_t* address, void* reserved);

/**
 * Check if BLE is connecting with peer device.
 *
 * @returns true if connecting, otherwise false.
 */
bool hal_ble_gap_is_connecting(const hal_ble_addr_t* address);

/**
 * Check if BLE is connected with peer device.
 *
 * @param[in]   address Pointer to the peer BLE identity address.
 *                      If NULL, it returns true as long as there has on-going connections.
 *
 * @returns true if connected, otherwise false.
 */
bool hal_ble_gap_is_connected(const hal_ble_addr_t* address);

/**
 * Cancel the ongoing procedure that connecting to the peer BLE device.
 *
 * @returns true if success, otherwise false.
 */
int hal_ble_gap_connect_cancel(const hal_ble_addr_t* address);

/**
 * Terminate BLE connection.
 *
 * @param[in]   conn_handle BLE connection handle. If BLE peripheral role, it is ignored.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_disconnect(hal_ble_conn_handle_t conn_handle, void* reserved);

/**
 * Update connection parameters. In the central role this will initiate a Link Layer connection parameter update procedure.
 * In the peripheral role, this will send the corresponding request and wait for the central to perform the procedure.
 *
 * @param[in]   conn_handle BLE connection handle.
 * @param[in]   conn_params Pointer to a hal_ble_conn_params_t structure with the desired parameters.
 *                          If NULL is provided on a peripheral role, the parameters in the PPCP characteristic of the GAP
 *                          service will be used instead. If NULL is provided on a central role and in response to a
 *                          connection parameters update request, the peripheral request will be rejected.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_update_connection_params(hal_ble_conn_handle_t conn_handle, const hal_ble_conn_params_t* conn_params, void* reserved);

/**
 * Get connection parameters.
 *
 * @param[in]       conn_handle BLE connection handle.
 * @param[in,out]   conn_params Pointer to a hal_ble_conn_params_t structure to be filled.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gap_get_connection_params(hal_ble_conn_handle_t conn_handle, hal_ble_conn_params_t* conn_params, void* reserved);

/**
 * Get the RSSI value of the specific BLE connection.
 *
 * @param[in]   conn_handle BLE connection handle.
 *
 * @returns     the RSSI value.
 */
int hal_ble_gap_get_rssi(hal_ble_conn_handle_t conn_handle, void* reserved);

/**
 * Add a BLE 128-bits UUID service.
 *
 * @param[in]   type    BLE service type, either BLE_SERVICE_TYPE_PRIMARY or BLE_SERVICE_TYPE_SECONDARY.
 * @param[in]   uuid    Pointer to BLE service UUID.
 * @param[out]  handle  Service handle.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_server_add_service(uint8_t type, const hal_ble_uuid_t* uuid, hal_ble_attr_handle_t* handle, void* reserved);

/**
 * Add a 128-bits UUID BLE Characteristic under a specific BLE Service.
 *
 * @note It is currently only possible to add a characteristic to the last added service (i.e. only sequential population is supported at this time).
 *
 * @param[in]   char_init       Pointer to the hal_ble_char_init_t structure.
 * @param[out]  char_handles    Pointer to the hal_ble_char_handles_t structure.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_server_add_characteristic(const hal_ble_char_init_t* char_init, hal_ble_char_handles_t* char_handles, void* reserved);

/**
 * Add descriptor under a specific BLE Characteristic.
 *
 * @note It is currently only possible to add a descriptor to the last added characteristic (i.e. only sequential population is supported at this time).
 *
 * @param[in]   desc_init   Pointer to the hal_ble_desc_init_t structure.
 * @param[out]  handle      Handle of the descriptor.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_server_add_descriptor(const hal_ble_desc_init_t* desc_init, hal_ble_attr_handle_t* handle, void* reserved);

/**
 * Set Characteristic value..
 *
 * @param[in]   value_handle    Characteristic value handle.
 * @param[in]   buf             Pointer to the buffer that contains the data to be set.
 * @param[in]   len             Length of the data to be set.
 *
 * @returns     Length of the data has been set.
 */
ssize_t hal_ble_gatt_server_set_characteristic_value(hal_ble_attr_handle_t value_handle, const uint8_t* buf, size_t len, void* reserved);

/**
 * Get Characteristic value.
 *
 * @param[in]   value_handle    Characteristic value handle.
 * @param[in]   buf             Pointer to the buffer that to be filled with the Characteristic value.
 * @param[in]   len             Length of the given buffer. Return the actual length of the value.
 *
 * @returns     The length of read data.
 */
ssize_t hal_ble_gatt_server_get_characteristic_value(hal_ble_attr_handle_t value_handle, uint8_t* buf, size_t len, void* reserved);

/**
 * Discover all BLE primary services.
 *
 * @param[in]   conn_handle BLE connection handle.
 * @param[in]   callback    The callback function to handle the discovered services.
 * @param[in]   context     The callback function context.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_discover_all_services(hal_ble_conn_handle_t conn_handle, on_ble_disc_service_cb_t callback, void* context, void* reserved);

/**
 * Discover BLE service with specific 128-bits service UUID.
 *
 * @param[in]   conn_handle BLE connection handle.
 * @param[in]   uuid        Pointer to the service UUID.
 * @param[in]   callback    The callback function to handle the discovered services.
 * @param[in]   context     The callback function context.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_discover_service_by_uuid(hal_ble_conn_handle_t conn_handle, const hal_ble_uuid_t* uuid, on_ble_disc_service_cb_t callback, void* context, void* reserved);

/**
 * Discover all BLE characteristics within specific handle range.
 *
 * @param[in]   conn_handle BLE connection handle.
 * @param[in]   service     The service to be discovered.
 * @param[in]   callback    The callback function to handle the discovered characteristics.
 * @param[in]   context     The callback function context.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_discover_characteristics(hal_ble_conn_handle_t conn_handle, const hal_ble_svc_t* service, on_ble_disc_char_cb_t callback, void* context, void* reserved);

/**
 * Discover all BLE characteristics within specific handle range.
 *
 * @param[in]   conn_handle BLE connection handle.
 * @param[in]   service     The service to be discovered.
 * @param[in]   uuid        The specific characteristic UUID.
 * @param[in]   callback    The callback function to handle the discovered characteristics.
 * @param[in]   context     The callback function context.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_discover_characteristics_by_uuid(hal_ble_conn_handle_t conn_handle, const hal_ble_svc_t* service, const hal_ble_uuid_t* uuid, on_ble_disc_char_cb_t callback, void* context, void* reserved);

/**
 * Check if the BLE discovery procedure is ongoing or not.
 *
 * @returns True if BLE is discovering services, characteristics or descriptors, otherwise false.
 */
bool hal_ble_gatt_client_is_discovering(hal_ble_conn_handle_t conn_handle);

/**
 * Set the desired ATT_MTU size.
 *
 * @param[in]   att_mtu     The desired ATT_MTU size.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_set_att_mtu(size_t att_mtu, void* reserved);

/**
 * Configure the Client Characteristic Configuration Descriptor.
 *
 * @param[in]   conn_handle     BLE connection handle.
 * @param[in]   cccd_handle     The peer device's Characteristic CCCD handle.
 * @param[in]   cccd_value      The CCCD value, it can be either BLE_SIG_CCCD_VAL_DISABLED, BLE_SIG_CCCD_VAL_NOTIFICATION or BLE_SIG_CCCD_VAL_INDICATION.
 *
 * @returns     0 on success, system_error_t on error.
 */
int hal_ble_gatt_client_configure_cccd(hal_ble_conn_handle_t conn_handle, hal_ble_attr_handle_t cccd_handle, ble_sig_cccd_value_t cccd_value, void* reserved);

/**
 * Write data to GATT server with a response required from peer device.
 *
 * @param[in]   conn_handle     BLE connection handle.
 * @param[in]   value_handle    The peer device's Characteristic value handle.
 * @param[in]   buf             Pointer to the buffer that contains the data to be written.
 * @param[in]   len             Length of the data to be written.
 *
 * @returns     The length of written data.
 */
ssize_t hal_ble_gatt_client_write_with_response(hal_ble_conn_handle_t conn_handle, hal_ble_attr_handle_t value_handle, const uint8_t* buf, size_t len, void* reserved);

/**
 * Write data to GATT server without a response required from peer device.
 *
 * @param[in]   conn_handle     BLE connection handle.
 * @param[in]   value_handle    The peer device's Characteristic value handle.
 * @param[in]   buf             Pointer to the buffer that contains the data to be written.
 * @param[in]   len             Length of the data to be written.
 *
 * @returns     The length of written data.
 */
ssize_t hal_ble_gatt_client_write_without_response(hal_ble_conn_handle_t conn_handle, hal_ble_attr_handle_t value_handle, const uint8_t* buf, size_t len, void* reserved);

/**
 * Read data from GATT server. The data is returned through a BLE event.
 *
 * @param[in]   conn_handle     BLE connection handle.
 * @param[in]   value_handle    The peer device's Characteristic value handle.
 * @param[in]   buf             Pointer to the buffer to be filled.
 * @param[in]   len             Length of the given buffer.
 *
 * @returns     The length of read data.
 */
ssize_t hal_ble_gatt_client_read(hal_ble_conn_handle_t conn_handle, hal_ble_attr_handle_t attr_handle, uint8_t* buf, size_t len, void* reserved);


#ifdef __cplusplus
} // extern "C"

namespace particle { namespace ble {

class BleLock {
public:
    BleLock()
            : locked_(false) {
        lock();
    }

    ~BleLock() {
        if (locked_) {
            unlock();
        }
    }

    BleLock(BleLock&& lock)
            : locked_(lock.locked_) {
        lock.locked_ = false;
    }

    void lock() {
        hal_ble_lock(nullptr);
        locked_ = true;
    }

    void unlock() {
        hal_ble_unlock(nullptr);
        locked_ = false;
    }

    BleLock(const BleLock&) = delete;
    BleLock& operator=(const BleLock&) = delete;

private:
    bool locked_;
};

} } /* particle::ble */

#endif /* __cplusplus */

#endif //HAL_PLATFORM_BLE


/**
 * @}
 */

#endif /* BLE_HAL_API_H */
