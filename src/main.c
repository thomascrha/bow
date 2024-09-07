#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"
#include "log.h"
#include "message.h"
#include "window.h"

int main(int argc, char *argv[]) {
    struct bow_config *bow_config = bow_setup_config();
    if (bow_config == NULL) {
        bow_log_error("Failed to setup bow config");
        bow_destroy_config(bow_config);
        return EXIT_FAILURE;
    }

    bow_log_info("Starting bow with log level %d buffer size %d", bow_config->log_level, bow_config->buffer_size);

    if (argc != 2) {
        bow_log_panic("Usage: %s <pipe_path>", argv[0]);
    };

    char *pipe_path = argv[1];

    if (access(pipe_path, F_OK) == -1) {
        mkfifo(pipe_path, 0666);
    }

    while (1) {
        FILE *pipe_fd = fopen(pipe_path, "r");
        if (pipe_fd == NULL) {
            perror("fopen");
            bow_destroy_config(bow_config);
            return EXIT_FAILURE;
        }

        char json_message_str[bow_config->buffer_size];
        if (fgets(json_message_str, bow_config->buffer_size, pipe_fd) == NULL) {
            bow_log_error("Input is larger than buffer size");
            continue;
        }
        fclose(pipe_fd);

        bow_log_info("Received string");
        bow_log_debug("%s", json_message_str);

        // gpointer message = g_new(struct bow_message, 1);
        // ((struct bow_message *)message)->expression = expression;
        // ((struct data *)data)->window_timeout = window_timeout;
        struct bow_message message;
        if (bow_message_parse(json_message_str, &message) != 0) {
            bow_log_error("Failed to parse message");
            continue;
        }

        bow_log_debug("expression: %s", message.expression);
        bow_log_debug("type: %s", message.type);
        bow_log_debug("config: %s", message.config);

        //      int code = bow_create_run_window(message, bow_config->window_timeout);
        //
        //      if (code != 0) {
        //          bow_log_error("bow_create_run_window() returned %d", code);
        // continue;
        //      }
    }

    bow_destroy_config(bow_config);
    return EXIT_SUCCESS;
}
