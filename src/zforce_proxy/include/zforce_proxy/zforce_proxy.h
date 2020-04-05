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
    zforce_no_message,                ///< Timed out setting configuration
    zforce_error_timeout,             ///< Operation timed out
    zforce_device_nxe                 ///< Device could not be found
} zforce_error_t;

/**
 * @brief types of messages exported by this library
 */
typedef enum {
    zmessage_touch ///< A touch event
} zmessage_types_t;

/**
 * @brief Touch event characteristics
 */
typedef enum {
    down_event,    ///< New touch object
    move_event,    ///< Touch is moving
    up_event,      ///< Touch is no longer detected
    invalid_event, ///< Invalid event reported by device
    ghost_event    ///< Ghost touch
} zevent_t;

/**
 * @brief A single, flat object containing touch event information
 */
#pragma pack(push, 2)
typedef struct {
    bool hasX;      ///< Is set to true if the coordinate system has a X axis.
    bool hasSizeX;  ///< True if sizeX has a value
    bool hasY;      ///< Is set to true if the coordinate system has a Y axis.
    bool hasSizeY;  ///< True if sizeY has a value
    bool hasZ;      ///< Is set to true if the coordinate system has a Z axis.
    bool hasSizeZ;  ///< True if sizeZ has a value
    zevent_t event; ///< Event type
    uint32_t id;    ///< Id of this message
    uint32_t sizeX; ///< Size of touch along X axis
    uint32_t sizeY; ///< Size of touch along Y axis
    uint32_t sizeZ; ///< Size of touch along Z axis
    uint32_t X;     ///< X axis value.
    uint32_t Y;     ///< Y axis value.
    uint32_t Z;     ///< Z axis value.
} ztouch_message_t;
#pragma pack(pop)

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
 * @retval zforce_device_nxe no device connected
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
 * @param[in] filter report messages only of this type
 * @param[out] pMsg receives message
 * @retval zforce_ok message read and printed okay
 * @retval zforce_no_message no message available
 */
zforce_error_t zforce_process_next_message(zmessage_types_t filter, ztouch_message_t *pMsg);

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