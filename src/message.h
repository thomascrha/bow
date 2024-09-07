#ifndef MESSAGE_H
#define MESSAGE_H

struct bow_message {
	char* type;
	char* expression;
	char* config;
};

int bow_message_parse(char* json, struct bow_message* message);

#endif // MESSAGE_H

