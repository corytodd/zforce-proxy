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
    zforce_ok,        ///< Success, etc. Pick your synonym.
    force_init_failed ///< Could not initialize zForce library
} zforce_return_t;

/**
 * @brief Initialize this library
 * @retval zforce_ok library read
 */
zforce_return_t zforce_initialize(void);

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