#include "message.h"
#include "json.h"
#include "log.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bow_message_parse(char *json, struct bow_message *message) {
    bow_log_debug("Parsing message: %s", json);
    struct json_value_s *root = json_parse(json, strlen(json));
    if (root == NULL) {
        bow_log_error("json_parse failed");
        return EXIT_FAILURE;
    }

    struct json_object_s *object = (struct json_object_s *)root->payload;

    if (object == NULL) {
        printf("object is null\n");
        bow_log_error("json_value_as_object failed");
        free(root);
        return EXIT_FAILURE;
    }

    bow_log_debug("Loaded object");

    char *keys[] = {"type", "expression"};
    char *optional_keys[] = {"config"};

    message->expression = NULL;
    message->type = NULL;
    message->config = NULL;

    bow_log_debug("Iterating through object");
    struct json_object_element_s *element = object->start;
    printf("Here\n");
    if (element == NULL || element->name == NULL || element->value == NULL || element->next == NULL) {
        bow_log_error("element is null");
        free(root);
        return EXIT_FAILURE;
    }
    printf("Here2\n");
    while (element != NULL) {
        printf("Here3\n");
        struct json_string_s *name = element->name;
        printf("Here4\n");
        struct json_value_s *value = element->value;
        printf("Here5\n");
        if (name == NULL || value == NULL) {
            bow_log_error("name or value is null");
            free(root);
            return EXIT_FAILURE;
        }
        printf("Here6\n");
        if (value->payload == NULL) {
            bow_log_error("value->payload is null");
            free(root);
            return EXIT_FAILURE;
        }
        printf("Here6.5\n");
        struct json_string_s *string = (struct json_string_s *)value->payload;
        printf("Here7\n");
        if (string == NULL) {
            bow_log_error("string is null");
            free(root);
            return EXIT_FAILURE;
        }
        printf("Here8\n");

        for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]) + sizeof(optional_keys) / sizeof(optional_keys[0]); i++) {
            char *key = i < sizeof(keys) / sizeof(keys[0]) ? keys[i] : optional_keys[i - sizeof(keys) / sizeof(keys[0])];
            if (0 == strcmp(name->string, key)) {
                if (0 == strcmp(name->string, "expression")) {
                    message->expression = (char *)string->string;
                } else if (0 == strcmp(name->string, "type")) {
                    message->type = (char *)string->string;
                } else if (0 == strcmp(name->string, "config")) {
                    message->config = (char *)string->string;
                }
            }
        }

        element = element->next;
    }

    if (message->expression == NULL || message->type == NULL) {
        bow_log_error("missing required fields");
        free(root);
        return EXIT_FAILURE;
    }

    if (message->config != NULL) {
        bow_log_info("config: %s", message->config);
    }

    free(root);
    return 0;
}
