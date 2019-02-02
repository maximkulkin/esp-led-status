#pragma once

#include "led_status_private.h"

typedef void * led_status_t;

led_status_t led_status_init(int gpio);
void led_status_done(led_status_t status);

void led_status_set(led_status_t status, led_status_pattern_t *pattern);
