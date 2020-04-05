//
// Created by Cory Todd on 4/4/2020.
//
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Must be included in this order, after system libraries
// clang-format off
#include <zForceCommon.h>
#include <OsAbstractionLayer.h>
#include <zForce.h>
#include <Queue.h>
#include <Connection.h>
#include <Device.h>
#include <Message.h>
// clang-format on

// Include proxy header last
#include "config.h"
#include "zforce_proxy/zforce_proxy.h"
#include "zforce_proxy/zforce_proxy_common.h"
#include "zforce_proxy/zforce_error_string.h"
#include "zforce_proxy/zforce_dump_message.h"

/********************
 *-- Defines
 ********************/

#define DEVICE_VID "0x1536" // Vendor ID of Device.
#define DEVICE_PID "0x0101" // Product ID of Device.
#define TIMEOUT_MS 1000     // Timeout in milliseconds for reading messages

/********************
 *-- Private types
 ********************/

/**
 * @brief Internal state
 */
typedef struct {
    bool isInitialized;        ///< Global library initialization flag
    bool isConnected;          ///< Global connection state flag
    Connection *zConnection;   ///< Device connection handle
    SensorDevice *zDevice;     ///< Device connection handle
    PlatformDevice *zPlatform; ///< Platform handle
} proxy_t;

/********************
 *-- Private state
 ********************/
static proxy_t l_state = {0};

/********************
 *-- Prototypes
 ********************/

/**
 * @brief Attempts to open a connection to the first available device
 * @details
 * There can only be one connection at a time.
 *
 * @param[out] pProxy receives instance handle
 */
static zforce_prepare_connection(proxy_t *pProxy);

/**
 * @briefs Check for and await message from device
 * @details
 * Wait up to this many milliseconds before giving up
 *
 * @param[in] wait wait time in milliseconds, -1 to wait forever
 * @param[out] pMsg receives message
 *
 * @retval true message received
 * @retval false message not received
 */
static bool zforce_check_message(int wait, Message* pMsg);

/********************
 *-- Global functions
 ********************/
zforce_error_t zforce_initialize(void) {

    zforce_error_t ret = zforce_ok;

    if (l_state.isInitialized) {
        return ret;
    }

    bool resultCode = zForce_Initialize(NULL);

    if (resultCode) {
        l_state.isInitialized = true;
    } else {
        LOG_ERROR("zForce initialization failed\n");
        ret = zforce_error_init_failed;
    }

    return ret;
}

zforce_error_t zforce_connect(void) {
    zforce_error_t ret = zforce_prepare_connection(&l_state);

    if (ret == zforce_ok) {
        l_state.isConnected = true;
    }

    return ret;
}

zforce_error_t zforce_configure(void) {
    ASSERT(l_state.isInitialized, "Library is not initialized\n");
    ASSERT(l_state.isConnected, "Device is not connected\n");
    ASSERT(l_state.zConnection, "Missing device connection\n");
    ASSERT(l_state.zDevice, "Missing device handle\n");

    // Enable everything except the OS HID touch device mode
    if (!l_state.zDevice->SetOperationModes(
            l_state.zDevice, DetectionMode | SignalsMode | LedLevelsMode | DetectionHidMode | GesturesMode,
            DetectionMode)) {
        LOG_ERROR("SetOperationModes error (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
        return zforce_error_configuration_error;
    }

    Message* message = NULL;
    if(!zforce_check_message(TIMEOUT_MS, message)){
        LOG_ERROR("Timed out setting configuration (SetOperationModes)\n");
        return zforce_error_timeout;
    }

    // Flip the y axis like a sane person
    if(!l_state.zDevice->SetReverseTouchActiveArea(l_state.zDevice, false, true)) {
        LOG_ERROR("SetReverseTouchActiveArea error (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
        return zforce_error_configuration_error;
    }

    if(!zforce_check_message(TIMEOUT_MS, message)){
        LOG_ERROR("Timed out setting configuration (SetReverseTouchActiveArea)\n");
        return zforce_error_timeout;
    }

    return zforce_ok;
}
zforce_error_t zforce_process_next_message(zmessage_types_t filter, ztouch_message_t *pMsg) {
    ASSERT(pMsg, "Received invalid message output handle\n");
    ASSERT(l_state.isInitialized, "Library is not initialized\n");
    ASSERT(l_state.isConnected, "Device is not connected\n");
    ASSERT(l_state.zConnection, "Missing device connection\n");
    ASSERT(l_state.zDevice, "Missing device handle\n");
    ASSERT(l_state.zPlatform, "Missing platform device\n");

    Message *message = l_state.zConnection->DeviceQueue->Dequeue(l_state.zConnection->DeviceQueue, TIMEOUT_MS);

    if (message == NULL) {
        return zforce_error_timeout;
    }

    DumpMessage(message);

    switch (message->MessageType) {
    case EnableMessageType:
        LOG_INFO("Device enabled, message loop has started\n");
        break;

    case OperationModesMessageType:
        if (!l_state.zDevice->GetResolution(l_state.zDevice)) {
            LOG_ERROR("GetResolution error (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
            return zforce_error_message_read;
        }
        break;

    case ResolutionMessageType:
        if (!l_state.zPlatform->GetMcuUniqueIdentifier(l_state.zPlatform)) {
            LOG_ERROR("GetMcuUniqueIdentifier error (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
            return zforce_error_message_read;
        }
        break;

    case McuUniqueIdentifierMessageType:
        if (!l_state.zDevice->SetEnable(l_state.zDevice, true, 0)) {
            LOG_ERROR("SetEnable error (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
            return zforce_error_message_read;
        }
        break;

    case TouchMessageType:
        if((filter & zmessage_touch) == zmessage_touch) {
            TouchMessage *pTouch = (TouchMessage *)message;

            pMsg->event = (zevent_t)pTouch->Event;
            pMsg->hasSizeX = pTouch->HasSizeX;
            pMsg->hasSizeY = pTouch->HasSizeY;
            pMsg->hasSizeZ = pTouch->HasSizeZ;
            pMsg->sizeX = pTouch->SizeX;
            pMsg->sizeY = pTouch->SizeY;
            pMsg->sizeZ = pTouch->SizeZ;

            pMsg->hasX = pTouch->HasX;
            pMsg->hasY = pTouch->HasY;
            pMsg->hasZ = pTouch->HasZ;
            pMsg->X = pTouch->X;
            pMsg->Y = pTouch->Y;
            pMsg->Z = pTouch->Z;
        }
        break;

    default:
        /* Do nothing */
        break;
    }

    message->Destructor(message);

    return zforce_ok;
}

void zforce_deinitialize(void) {

    // First we have to disconnect
    if (l_state.isConnected) {
        l_state.zConnection->Disconnect(l_state.zConnection);
        l_state.isConnected = false;
    }

    // Then we have to release the connection
    if (NULL != l_state.zConnection) {
        l_state.zConnection->Destructor(l_state.zConnection);
    }

    // Deinitialize the library to cleanup
    if (l_state.isInitialized) {
        zForce_Uninitialize();
        l_state.isInitialized = false;
    }
}

void zforce_get_version(int *pVersion) {
    ASSERT(pVersion, "pVersion is null\n");

    // MSB{ Major, Minor, Patch, Tweak}LSB
    *pVersion = PROJECT_VER_MAJOR << 24 | PROJECT_VER_MINOR << 16 | PROJECT_VER_PATCH << 8 | PROJECT_VER_TWEAK;
}

/********************
 *-- Static functions
 ********************/
static zforce_error_t zforce_prepare_connection(proxy_t *pProxy) {
    ASSERT(pProxy && pProxy->isInitialized, "Library is not initialized. Call zforce_initialize()\n");

    if (pProxy->isConnected) {
        return zforce_error_already_connected;
    }

    // First create the connection
    Connection *newConnection = Connection_New("hidpipe://vid=" DEVICE_VID ","
                                               "pid=" DEVICE_PID ","
                                               "index=0",
                                               "asn1://", "Streaming");
    if (newConnection == NULL) {
        LOG_ERROR("Unable to connect: (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
        return zforce_error_connection_error;
    }
    LOG_INFO("Connection created\n");

    // Then open the connection
    bool connectionAttemptResult = newConnection->Connect(newConnection);
    if (!connectionAttemptResult) {
        LOG_ERROR("Unable to connect to device: (%d) %s\n", zForceErrno, ErrorString(zForceErrno));
        return zforce_error_connection_error;
    }
    LOG_INFO("Connection opened\n");

    // Wait for Connection response to arrive within 1000 seconds.
    ConnectionMessage *connectionMessage =
        newConnection->ConnectionQueue->Dequeue(newConnection->ConnectionQueue, 1000000);
    if (NULL == connectionMessage) {
        LOG_ERROR("No Connection Message Received: %s\n", ErrorString(zForceErrno));
        return zforce_error_connection_error;
    }
    LOG_INFO("Devices: %d\n", newConnection->NumberOfDevices);

    // Get the device handle
    PlatformDevice *platformDevice = (PlatformDevice *)newConnection->FindDevice(newConnection, Platform, 0);
    if (NULL == platformDevice) {
        LOG_ERROR("No Platform device found\n");
        return zforce_error_connection_error;
    }
    LOG_INFO("Found platform handle\n");

    // Find the first Sensor type device (Core/Air/Plus).
    SensorDevice *sensorDevice = (SensorDevice *)newConnection->FindDevice(newConnection, Sensor, 0);
    if (NULL == sensorDevice) {
        LOG_ERROR("No Sensor device found\n");
        return zforce_error_connection_error;
    }

    char *deviceTypeString = NULL;
    DeviceType deviceType = sensorDevice->DeviceType & ~Sensor;

    switch (deviceType) {
    case Platform:
        deviceTypeString = "Platform";
        break;
    case Core:
        deviceTypeString = "Core";
        break;
    case Air:
        deviceTypeString = "Air";
        break;
    case Plus:
        deviceTypeString = "Plus";
        break;
    case Lighting:
        deviceTypeString = "Lighting";
        break;
    default:
        deviceTypeString = "Unknown";
        break;
    }

    LOG_INFO("Found device: %s\n", deviceTypeString);

    pProxy->zConnection = newConnection;
    pProxy->zDevice = sensorDevice;
    pProxy->zPlatform = platformDevice;

    return zforce_ok;
}

static bool zforce_check_message(int wait, Message* pMsg) {
    pMsg = l_state.zConnection->DeviceQueue->Dequeue(l_state.zConnection->DeviceQueue, wait);
    return pMsg != NULL;
}