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
    zforce_ok,                  ///< Success, etc. Pick your synonym.
    zforce_init_failed,         ///< Could not initialize zForce library
    zforce_already_connected,   ///< Library is already connected
    zforce_connection_error,    ///< Failed to connect, see error code for more details
    zforce_configuration_error, ///< Failed to configure, see error code for more details
} zforce_return_t;

/**
 * @brief Initialize this library
 *
 * @retval zforce_ok library read
 */
zforce_return_t zforce_initialize(void);

/**
 * @brief Create a new connection
 *
 * @retval zforce_ok connection succeeded
 * @retval zforce_connection_error check zForce SDK for error code
 */
zforce_return_t zforce_connect();

/**
 * @brief Configure the hardware's operation mode
 *
 * @retval zforce_ok connection succeeded
 * @retval zforce_configuration_error check SDK for error code meaning
 */
zforce_return_t zforce_configure(void);

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