/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   10 * 60 * 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

// Get the node identifier for the button1
#define SW0_NODE DT_ALIAS(sw0)

#define MAX_NUMBER_FACT 10

LOG_MODULE_REGISTER(MSE_PI_MobileSens,LOG_LEVEL_DBG);

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
	int i;
	long int factorial = 1;

	printk("Calculating the factorials of numbers from 1 to %d:\n", MAX_NUMBER_FACT);
	for (i = 1; i <= MAX_NUMBER_FACT; i++) {
		factorial = factorial * i;
		printk("The factorial of %2d = %ld\n", i, factorial);
	}
	printk("_______________________________________________________\n");
	/*Important note!
	Code in ISR runs at a high priority, therefore, it should be written with timing in mind.
	Too lengthy or too complex tasks should not be performed by an ISR, they should be deferred
	to a thread.
	*/

	// Try log messages
	int exercise_num = 1000;
	LOG_INF("nRF Connect SDK Fundamentals");
    LOG_INF("Exercise %d",exercise_num);    
    LOG_DBG("A log message in debug level");
    LOG_WRN("A log message in warning level!");
    LOG_ERR("A log message in Error level!");

	gpio_pin_toggle_dt(&led);
}

int main(void) {
	int ret;

	if (!device_is_ready(led.port)) {
		return -1;
	}

	if (!device_is_ready(button.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}
	/* STEP 3 - Configure the interrupt on the button's pin */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	/* STEP 6 - Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

	/* STEP 7 - Add the callback function by calling gpio_add_callback()   */
	gpio_add_callback(button.port, &button_cb_data);

	while (1) {
		/* STEP 8 - Remove the polling code */

		k_msleep(SLEEP_TIME_MS);
	}
}