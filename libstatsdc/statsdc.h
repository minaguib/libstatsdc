/*
 * libstatsdc
 * Copyright 2011 Mina Naguib
 * See the accompanying COPYING file for use and distribution legalese.
 *
 */

#ifndef _STATSDC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _statsdc * statsdc_t;

/*
 * Initialize a statsdc_t instance
 * Use this instance as the first argument to all other functions in libstatsdc
 * Call statsdc_free on it when you're done
 *
 * Returns statsdc_t on success, NULL on failure
 */
statsdc_t statsdc_init(char *host, char * port);

/*
 * Frees the statsdc object when you're done with it
 */
void statsdc_free(statsdc_t sdc);

/*
 * Retrieves the error string of the last error that occurred
 * Errors are typically denoted by libstatsdc functions returning 0
 */
char * statsdc_last_error_string(statsdc_t sdc);

/*
 * Asks the statsdc object to re-reslve DNS if any and connect
 * to the destination address
 * 
 * Returns 1 on success, 0 on error
 */
int statsdc_reconnect(statsdc_t sdc);

/*
 * Send a counter update
 * Returns 1 on success, 0 on error
 */
int statsdc_update(statsdc_t sdc, char *key, long int delta, float sample_rate);

/*
 * Send a timing update
 * Returns 1 on success, 0 on error
 */
int statsdc_timing(statsdc_t sdc, char *key, long int value, float sample_rate);

/*
 * Sugar: Sends a counter update of delta:1 and sample_rate:1
 * Returns 1 on success, 0 on error
 */
int statsdc_increment(statsdc_t sdc, char *key);

/*
 * Sugar: Sends a counter update of delta:-1 and sample_rate:1
 * Returns 1 on success, 0 on error
 */
int statsdc_decrement(statsdc_t sdc, char *key);

#ifdef __cplusplus
}
#endif

#endif
