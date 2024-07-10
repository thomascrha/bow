#ifndef LOG_H
#define LOG_H

#include "config.h"

void bow_log_message(int importance, const char *file, int line, ...);
void bow_log_level_set(int importance);
void bow_log_level_set_from_string(const char *importance);

#define bow_log_debug(...) bow_log_message(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#define bow_log_info(...) bow_log_message(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)

#define bow_log_warning(...) bow_log_message(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)

#define bow_log_error(...) bow_log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#define bow_log_panic(...)  \
	bow_log_message(LOG_PANIC, __FILE__, __LINE__, __VA_ARGS__);  \
	exit(2)

#endif // LOG_H
