/*
 * libstatsdc
 * See the COPYING file for use and distribution legalese.
 *
 */

#include "statsdc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct _statsdc {
	char *host;
	char *port;
	int socket;
	char *last_error;
};

enum _statsdc_send_type {
	_STATSDC_SEND_TYPE_COUNTER,
	_STATSDC_SEND_TYPE_TIMING
};

/*
 * Protocol:
 *	key:delta|c
 *	key:delta|c|@rate
 *	key:value|ms
 *	key:value|ms|@rate
 *
 */
static
void
_statsdc_send(statsdc_t sdc, char *key, int delta, enum _statsdc_send_type type, float sample_rate) {
	char *packet = NULL;
	char *typekey = NULL;
	unsigned int packetlen=0;

	if (sdc->socket < 1) return;

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

	if (packet == NULL) return;
	packetlen = strlen(packet);

	send(sdc->socket, packet, packetlen, 0);

	free(packet);
}


statsdc_t statsdc_init(char *host, char *port) {
	statsdc_t sdc = NULL;
	if (host == NULL || port == NULL) return NULL;
	sdc = (statsdc_t)malloc(sizeof(struct _statsdc));
	if (sdc == NULL) return NULL;

	sdc->host = strdup(host);
	sdc->port = strdup(port);
	sdc->socket = 0;

	statsdc_reconnect(sdc);

	return sdc;
}

void statsdc_reconnect(statsdc_t sdc) {

	struct addrinfo hints = {}, *ai = NULL;

	if (sdc->socket == 0) {
		sdc->socket = socket(AF_INET, SOCK_DGRAM, 0);
	}

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	getaddrinfo(sdc->host, sdc->port, &hints, &ai);
	if (ai != NULL) {
		connect(sdc->socket, ai->ai_addr, ai->ai_addrlen);
		freeaddrinfo(ai);
	}
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
	free(sdc);
}

void statsdc_update(statsdc_t sdc, char *key, int delta, float sample_rate) {
	if (sdc == NULL || key == NULL) return;
	if (sample_rate < 1 && ((float)rand() / RAND_MAX) >= sample_rate) return;

	_statsdc_send(sdc, key, delta, _STATSDC_SEND_TYPE_COUNTER, sample_rate);
}

void statsdc_timing(statsdc_t sdc, char *key, int value, float sample_rate) {
	if (sdc == NULL || key == NULL) return;
	if (sample_rate < 1 && ((float)rand() / RAND_MAX) >= sample_rate) return;

	_statsdc_send(sdc, key, value, _STATSDC_SEND_TYPE_TIMING, sample_rate);
}
