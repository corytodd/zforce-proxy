//
// Created by Cory Todd on 4/4/2020.
//

#include <stdio.h>

#include <zforce_proxy/zforce_proxy.h>

int main(int argc, char **argv) {

    int version = 0;
    zforce_get_version(&version);

    printf("zForce Proxy Version: %d.%d.%d.%d", (version >> 24) & 0xFF, (version >> 16) & 0xFF, (version >> 8) & 0xFF,
           version & 0xFF);

    return 0;
}