//
// Created by Cory Todd on 4/4/2020.
//
#pragma once

#include "zforce_proxy/zforce_proxy_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Simplified return codes
 */
typedef enum {
    zforce_ok,                        ///< Success, etc. Pick your synonym.
    zforce_error_init_failed,         ///< Could not initialize zForce library
    zforce_error_already_connected,   ///< Library is already connected
    zforce_error_connection_error,    ///< Failed to connect, see error code for more details
    zforce_error_configuration_error, ///< Failed to configure, see error code for more details
    zforce_error_message_read,        ///< Error reading message
} zforce_error_t;

/**
 * @brief Initialize this library
 *
 * @retval zforce_ok library read
 */
zforce_error_t zforce_initialize(void);

/**
 * @brief Create a new connection
 *
 * @retval zforce_ok connection succeeded
 * @retval zforce_connection_error check zForce SDK for error code
 */
zforce_error_t zforce_connect();

/**
 * @brief Configure the hardware's operation mode
 *
 * @retval zforce_ok connection succeeded
 * @retval zforce_configuration_error check SDK for error code meaning
 */
zforce_error_t zforce_configure(void);

/**
 * @brief Fetch and dump the next available device message
 *
 * @retval zforce_ok message read and printed okay
 */
zforce_error_t zforce_process_next_message(void);

/**
 * @brief Cleanup this library
 */
void zforce_deinitialize(void);

/**
 * @brief Returns the version of this library
 * @details
 * The semver number is stored as four 1-byte values
 *
 * MSB{Major, Minor, Path, Tweak}LSB
 *
 * @param[out] pVersion receives version
 */
void zforce_get_version(int *pVersion);

#ifdef __cplusplus
}
#endif // __cplusplus