#include <stdlib.h>

#include <etstimer.h>
#include <esplibs/libmain.h>

#include "led_status_private.h"

#define ABS(x) (((x) < 0) ? -(x) : (x))

typedef struct {
    uint8_t gpio;
    uint8_t active;
    ETSTimer timer;

    int n;
    led_status_pattern_t *pattern;
    led_status_pattern_t *signal_pattern;
} led_status_t;


static void led_status_write(led_status_t *status, bool on) {
    gpio_write(status->gpio, on ? status->active : !status->active);
}

static void led_status_tick(led_status_t *status) {
    led_status_pattern_t *p = status->signal_pattern ? status->signal_pattern : status->pattern;
    if (!p) {
        sdk_os_timer_disarm(&status->timer);
        led_status_write(status, false);
        return;
    }

    led_status_write(status, p->delay[status->n] > 0);
    sdk_os_timer_arm(&status->timer, ABS(p->delay[status->n]), 0);

    status->n = (status->n + 1) % p->n;
    if (status->signal_pattern && status->n == 0) {
        status->signal_pattern = NULL;
    }
}

led_status_t *led_status_init(uint8_t gpio, uint8_t active_level) {
    led_status_t *status = malloc(sizeof(led_status_t));
    status->gpio = gpio;
    status->active = active_level;
    sdk_os_timer_setfn(&status->timer, (void(*)(void*))led_status_tick, status);

    gpio_enable(status->gpio, GPIO_OUTPUT);
    led_status_write(status, false);

    return status;
}

void led_status_done(led_status_t *status) {
    sdk_os_timer_disarm(&status->timer);
    gpio_disable(status->gpio);
    free(status);
}

void led_status_set(led_status_t *status, led_status_pattern_t *pattern) {
    status->pattern = pattern;

    if (!status->signal_pattern) {
        status->n = 0;
        led_status_tick(status);
    }
}

void led_status_signal(led_status_t *status, led_status_pattern_t *pattern) {
    if (!status->signal_pattern && !pattern)
        return;

    status->signal_pattern = pattern;
    status->n = 0;  // whether signal pattern is NULL or not, just reset the state
    led_status_tick(status);
}
