/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

// Get the node identifier for the button1
#define SW0_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led0) || !gpio_is_ready_dt(&led0)) {
		printf("Error: LED0 or LED1 device is not ready\n");
		return 0;
	}

	if (!device_is_ready(button.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}

	while (1) {
		bool val = gpio_pin_get_dt(&button);
		gpio_pin_set_dt(&led0,val);

		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
