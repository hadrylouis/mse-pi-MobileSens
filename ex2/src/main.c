/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

int main(void) {
	while (1) {
		printk("Hello World!\n");
		k_msleep(1000);
	}
}