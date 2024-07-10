#include <stdio.h>
#include <stdarg.h>

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "log.h"

static int current_log_level = LOG_INFO;

static const char *verbosity_names[] = {
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR",
	"PANIC",
};

static char* get_level_colour(int importance) {
	switch (importance) {
		case LOG_DEBUG:
			return BLUE;
		case LOG_INFO:
			return GREEN;
		case LOG_WARNING:
			return YELLOW;
		case LOG_ERROR:
			return RED;
		case LOG_PANIC:
			return BOLD_RED;
		default:
			return GREEN;
	}
}

void bow_log_message(int importance, const char *file, int line, ...) {
	if (importance < current_log_level) {
		return;
	}

	// get the current time and format it as a string
	struct timespec current_unix_time;
	if (clock_gettime(CLOCK_REALTIME, &current_unix_time) != 0) {
		fprintf(stderr, "clock_gettime() failed: %s\n", strerror(errno));
		current_unix_time.tv_sec = 0;
		current_unix_time.tv_nsec = 0;
	}

    char datetime[20];
    struct tm *datetime_info;
    datetime_info = localtime(&(current_unix_time.tv_sec));
    strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", datetime_info);

	// colour the importance level
	char importance_coloured[20];
	sprintf(importance_coloured, "%s%s%s", get_level_colour(importance), verbosity_names[importance], RESET);

	// print the message minus the actual message
    fprintf(stderr, "%s %s %s:%d: ", datetime, importance_coloured, file, line);

	// unpack the variadic arguments and print the message
	va_list args;
	va_start(args, line);
	vfprintf(stderr, va_arg(args, const char *), args);
	va_end(args);
	fprintf(stderr, "\n");
}

void bow_log_set_level(int importance) {
	current_log_level = importance;
}
