esp-led-status
==============

Library for [ESP-OPEN-RTOS](https://github.com/SuperHouse/esp-open-rtos)
to communicate device status through different LED blink patterns.

Patterns are defined as a list of delays in milliseconds with positive values
being periods when LED is on and negative values - periods when LED is off.

```c
// 1000ms ON, 1000ms OFF
led_status_pattern_t waiting_wifi = LED_STATUS_PATTERN({1000, -1000});

// one short blink every 3 seconds
led_status_pattern_t normal_mode = LED_STATUS_PATTERN({100, -2900});

// three short blinks
led_status_pattern_t three_short_blinks = LED_STATUS_PATTERN({100, -100, 100, -100, 100, -700});


#define STATUS_LED_PIN 13

static led_status_t status = led_status_init(STATUS_LED_PIN, 1);
led_status_set(status, &normal_mode);

// execute one time signal
led_status_signal(status, &three_short_blinks);
```

License
=======

MIT licensed. See the bundled [LICENSE](https://github.com/maximkulkin/esp-led-status/blob/master/LICENSE) file for more details.
