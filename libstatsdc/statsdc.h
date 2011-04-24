/*
 * libstatsdc
 * See the COPYING file for use and distribution legalese.
 *
 */

#ifndef _STATSDC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _statsdc * statsdc_t;

statsdc_t statsdc_init(char *host, char * port);
void statsdc_free(statsdc_t sdc);

void statsdc_reconnect(statsdc_t sdc);
void statsdc_update(statsdc_t sdc, char *key, int delta, float sample_rate);
void statsdc_timing(statsdc_t sdc, char *key, int value, float sample_rate);

void statsdc_increment(statsdc_t sdc, char *key) {
	statsdc_update(sdc, key, 1, 1);
}

void statsdc_decrement(statsdc_t sdc, char *key) {
	statsdc_update(sdc, key, -1, 1);
}

#ifdef __cplusplus
}
#endif

#endif
