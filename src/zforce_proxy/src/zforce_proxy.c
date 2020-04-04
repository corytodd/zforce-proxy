//
// Created by Cory Todd on 4/4/2020.
//
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Must be included in this order, after system libraries
#include <zForceCommon.h>
#include <OsAbstractionLayer.h>
#include <zForce.h>

// Include proxy header last
#include "zforce_proxy/zforce_proxy_common.h"
#include "config.h"

void zforce_get_version(int* pVersion) {
    ASSERT(pVersion, "pVersion is null");

    // MSB{ Major, Minor, Patch, Tweak}LSB
    *pVersion = PROJECT_VER_MAJOR << 24 |
                PROJECT_VER_MINOR << 16 |
                PROJECT_VER_PATCH << 8 |
                PROJECT_VER_TWEAK;
}