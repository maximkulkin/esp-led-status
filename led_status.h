#pragma once

#include "led_status_private.h"

typedef void * led_status_t;

led_status_t led_status_init(uint8_t gpio, bool active_high);
void led_status_done(led_status_t status);

// Set looped pattern that will be executing until changed.
// Passing NULL as pattern disables blinking.
void led_status_set(led_status_t status, led_status_pattern_t *pattern);

// Execute given pattern once and then return to pattern set by led_status_set().
void led_status_signal(led_status_t status, led_status_pattern_t *pattern);
