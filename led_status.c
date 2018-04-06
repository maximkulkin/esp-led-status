#include <stdlib.h>

#include <etstimer.h>
#include <esplibs/libmain.h>

#include "led_status_private.h"

typedef struct {
    int gpio;
    ETSTimer timer;

    led_status_pattern_t *pattern;
    bool state;
    int n;
} led_status_t;


static void led_status_tick(led_status_t *status) {
    status->state = !status->state;
    status->n = (status->n + 1) % status->pattern->n;

    gpio_write(status->gpio, status->state);

    sdk_os_timer_arm(&status->timer, status->pattern->delay[status->n], 0);
}

led_status_t *led_status_init(int gpio) {
    led_status_t *status = malloc(sizeof(led_status_t));
    status->gpio = gpio;
    sdk_os_timer_setfn(&status->timer, (void(*)(void*))led_status_tick, status);

    gpio_enable(status->gpio, GPIO_OUTPUT);

    return status;
}

void led_status_done(led_status_t *status) {
    sdk_os_timer_disarm(&status->timer);
    gpio_disable(status->gpio);
    free(status);
}

void led_status_set(led_status_t *status, led_status_pattern_t *pattern) {
    if (!pattern || pattern->n == 0) {
        sdk_os_timer_disarm(&status->timer);
        return;
    }

    status->pattern = pattern;

    status->state = false;
    status->n = 0;

    led_status_tick(status);
}
