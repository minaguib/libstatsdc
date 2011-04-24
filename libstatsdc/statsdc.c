/*
 * libstatsdc
 * Copyright 2011 Mina Naguib
 * See the accompanying COPYING file for use and distribution legalese.
 *
 */

#include "statsdc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct _statsdc {
	char *host;
	char *port;
	int socket;
	char *last_error_string;
};

enum _statsdc_send_type {
	_STATSDC_SEND_TYPE_COUNTER,
	_STATSDC_SEND_TYPE_TIMING
};

static
void
_statsdc_error(statsdc_t sdc, const char *format, ...) {
	va_list ap;

	if (sdc->last_error_string != NULL) {
		free(sdc->last_error_string);
		sdc->last_error_string = NULL;
	}

	va_start(ap, format);
	vasprintf(&(sdc->last_error_string), format, ap);
	va_end(ap);

}

/*
 * Protocol:
 *	key:delta|c
 *	key:delta|c|@rate
 *	key:value|ms
 *	key:value|ms|@rate
 *
 */
static
int
_statsdc_send(statsdc_t sdc, char *key, int delta, enum _statsdc_send_type type, float sample_rate) {
	char *packet = NULL;
	char *typekey = NULL;
	ssize_t sent;
	unsigned int packetlen=0;

	if (sdc->socket < 1) {
		_statsdc_error(sdc, "Socket unavailable for sending");
		return 0;
	}

	if (type == _STATSDC_SEND_TYPE_COUNTER) {
		typekey = "c";
	}
	else if (type == _STATSDC_SEND_TYPE_TIMING) {
		typekey = "ms";
	}

	if (sample_rate < 1) {
		asprintf(&packet, "%s:%d|%s|@%f", key, delta, typekey, sample_rate);
	}
	else {
		asprintf(&packet, "%s:%d|%s", key, delta, typekey);
	}

	if (packet == NULL) {
		_statsdc_error(sdc, "Could not construct packet to send");
		return 0;
	}
	packetlen = strlen(packet);
	sent = send(sdc->socket, packet, packetlen, 0);
	free(packet);

	if (sent < 0) {
		_statsdc_error(sdc, "send() failure: %s", strerror(errno));
		return 0;
	}

	return 1;
}

statsdc_t statsdc_init(char *host, char *port) {
	statsdc_t sdc = NULL;
	if (host == NULL || port == NULL) return NULL;
	sdc = (statsdc_t)malloc(sizeof(struct _statsdc));
	if (sdc == NULL) return NULL;

	sdc->host = strdup(host);
	sdc->port = strdup(port);
	sdc->socket = 0;
	sdc->last_error_string = NULL;

	statsdc_reconnect(sdc);

	return sdc;
}

int statsdc_reconnect(statsdc_t sdc) {

	struct addrinfo hints = {}, *ai = NULL;
	int res;

	if (sdc->socket == 0) {
		sdc->socket = socket(AF_INET, SOCK_DGRAM, 0);
	}
	if (sdc->socket == 0) {
		_statsdc_error(sdc, "Failed to create a datagram socket");
		return 0;
	}

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	res = getaddrinfo(sdc->host, sdc->port, &hints, &ai);
	if (res != 0) {
		_statsdc_error(sdc, "Failed to getaddrinfo for %s:%s : %s", sdc->host, sdc->port, gai_strerror(res));
		if (ai != NULL) freeaddrinfo(ai);
		return 0;
	}
	else if (ai == NULL) {
		_statsdc_error(sdc, "No addrinfo available for %s:%s");
		return 0;
	}
	else {
		res = connect(sdc->socket, ai->ai_addr, ai->ai_addrlen);
		freeaddrinfo(ai);
	}

	if (res != 0) {
		_statsdc_error(sdc, "Error connecting to %s:%s : %s", sdc->host, sdc->port, strerror(errno));
		return 0;
	}

	return 1;
}

void statsdc_free(statsdc_t sdc) {
	if (sdc == NULL) return;
	if (sdc->host != NULL) {
		free(sdc->host);
		sdc->host = NULL;
	}
	if (sdc->port != NULL) {
		free(sdc->port);
		sdc->port = NULL;
	}
	if (sdc->last_error_string != NULL) {
		free(sdc->last_error_string);
		sdc->last_error_string = NULL;
	}
	free(sdc);
}

char * statsdc_last_error_string(statsdc_t sdc) {
	if (sdc == NULL) {
		return "Uninitialized statsdc_t";
	}
	else {
		return sdc->last_error_string;
	}
}

int statsdc_update(statsdc_t sdc, char *key, int delta, float sample_rate) {
	if (sdc == NULL) return 0;
	if (key == NULL) {
		_statsdc_error(sdc, "Empty key");
		return 0;
	}
	if (sample_rate < 1 && ((float)rand() / RAND_MAX) >= sample_rate) return 1;

	return _statsdc_send(sdc, key, delta, _STATSDC_SEND_TYPE_COUNTER, sample_rate);
}

int statsdc_timing(statsdc_t sdc, char *key, int value, float sample_rate) {
	if (sdc == NULL) return 0;
	if (key == NULL) {
		_statsdc_error(sdc, "Empty key");
		return 0;
	}
	if (sample_rate < 1 && ((float)rand() / RAND_MAX) >= sample_rate) return 1;

	return _statsdc_send(sdc, key, value, _STATSDC_SEND_TYPE_TIMING, sample_rate);
}
