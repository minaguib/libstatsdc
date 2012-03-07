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
statsdc_t statsdc_init(const char *host, const char * port);

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
 * Asks the statsdc object to re-resolve DNS if any and connect
 * the socket to the destination address
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_reconnect(statsdc_t sdc);


/*
 * Set a common prefix for all keys. The character `.' will be added
 * between the concatenated strings. To unset a prefix use the NULL value as
 * prefix.
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_prefix(statsdc_t sdc, const char* prefix);

/*
 * Send a counter update
 *
 * key is the "key.name.to.impact" in the graphite tree
 * delta is a positive or negative number to impact the key by
 * sample_rate should be between 0.0 and 1.0
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_update(statsdc_t sdc, const char *key, long int delta, float sample_rate);

/*
 * Send a timing update
 *
 * key is the "key.name.to.impact" in the graphite tree
 * delta is a positive or negative number to impact the key by
 * sample_rate should be between 0.0 and 1.0
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_timing(statsdc_t sdc, const char *key, long int value, float sample_rate);

/*
 * Sugar: Sends a counter update of delta:1 and sample_rate:1
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_increment(statsdc_t sdc, const char *key);

/*
 * Sugar: Sends a counter update of delta:-1 and sample_rate:1
 *
 * Returns 1 on success, 0 on error
 */
int statsdc_decrement(statsdc_t sdc, const char *key);

#ifdef __cplusplus
}
#endif

#endif
