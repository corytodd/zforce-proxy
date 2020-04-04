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

/********************
 *-- Private types
 ********************/

/**
 * @brief Internal state
 */
typedef struct {
    bool isInitialized;      ///< Global library initialization flag
    bool isConnected;        ///< Global connection state flag
    Connection *zConnection; ///< Device connection handle
} proxy_t;

/********************
 *-- Private state
 ********************/
static proxy_t l_state = {0};

/********************
 *-- Global functions
 ********************/
zforce_return_t zforce_initialize(void) {

    zforce_return_t ret = zforce_ok;

    if (l_state.isInitialized) {
        return ret;
    }

    bool resultCode = zForce_Initialize(NULL);

    if (resultCode) {
        l_state.isInitialized = true;
    } else {
        LOG_ERROR("zForce initialization failed\n");
        ret = force_init_failed;
    }

    return ret;
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
    ASSERT(pVersion, "pVersion is null");

    // MSB{ Major, Minor, Patch, Tweak}LSB
    *pVersion = PROJECT_VER_MAJOR << 24 | PROJECT_VER_MINOR << 16 | PROJECT_VER_PATCH << 8 | PROJECT_VER_TWEAK;
}