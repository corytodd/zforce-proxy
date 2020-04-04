//
// Created by Cory Todd on 4/4/2020.
//
#pragma once

#include "zforce_proxy/zforce_proxy_common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 * @brief Returns the version of this library
 * @details
 * The semver number is stored as four 1-byte values
 *
 * MSB{Major, Minor, Path, Tweak}LSB
 *
 * @param[out] pVersion receives version
 */
void zforce_get_version(int* pVersion);

#ifdef __cplusplus
}
#endif // __cplusplus