//
// Created by Cory Todd on 4/4/2020.
//

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <zforce_proxy/zforce_proxy.h>

/**
 * @brief CLI run states
 */
typedef enum {
    state_startup,  ///< Application has just startup
    state_init,     ///< Application is initializing
    state_config,   ///< Application is configuring
    state_idle,     ///< Application is idle
    state_shutdown, ///< Application is shutting down
    state_stopped,  ///< Application is stopped
    state_error     ///< A stopped error state
} state_t;

typedef struct {
    state_t state;
    void *handle;
    int return_code;
} cli_t;

/**
 * @brief Local state for CLI
 * @details
 * Unfortunately we need to keep this global so the signal handler can work
 */
static cli_t l_cli = {0};

/**
 * @brief Handle ctrl+c signal to shutdown application
 */
static void cli_signal_handler(int sig);

int main(int argc, char **argv) {

    int version = 0;
    zforce_get_version(&version);

    LOG_INFO("zForce Proxy Version: %d.%d.%d.%d", (version >> 24) & 0xFF, (version >> 16) & 0xFF, (version >> 8) & 0xFF,
             version & 0xFF);

    // Install the Control-C handler.
    signal(SIGINT, cli_signal_handler);

    zforce_error_t ret;

    while (l_cli.state != state_stopped) {
        switch (l_cli.state) {

        case state_startup:
            // Configure cli
            LOG_INFO("Starting up...");
            ret = zforce_initialize();
            l_cli.state = ret == zforce_ok ? state_init : state_error;
            break;

        case state_init:
            // Configure library
            LOG_INFO("Entering initialization state...");
            ret = zforce_connect();
            l_cli.state = ret == zforce_ok ? state_config : state_error;
            break;

        case state_config:
            // Configure connection and device
            LOG_INFO("Entering configuration state...");
            ret = zforce_configure();

            if (ret == zforce_ok) {
                l_cli.state = state_idle;
                LOG_INFO("Starting message processing loop");
            } else {
                l_cli.state = state_error;
            }
            break;

        case state_idle:
            // Process message loop
            ret = zforce_process_next_message();
            l_cli.state = ret == zforce_ok ? state_idle : state_error;
            break;

        case state_shutdown:
            LOG_INFO("Shutting down...");
            zforce_deinitialize();
            l_cli.state = state_stopped;
            break;

        case state_error:
            LOG_ERROR("An error condition was triggered.");
            l_cli.return_code = -1;
            l_cli.state = state_shutdown;
            break;

        default:
            break;
        }
    }

    LOG_INFO("Shutdown complete.");

    return l_cli.return_code;
}

static void cli_signal_handler(int sig) {
    (void)sig;
    l_cli.state = state_shutdown;
    exit(-1);
}