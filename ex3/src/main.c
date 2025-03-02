#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define RECEIVE_TIMEOUT 100
#define RECEIVE_BUFF_SIZE 10

static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

const struct device *uart= DEVICE_DT_GET(DT_NODELABEL(uart20));

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data) {
	switch (evt->type) {

		case UART_RX_RDY: {
			if ((evt->data.rx.len) == 1) {
				if (evt->data.rx.buf[evt->data.rx.offset] == '0') {
					gpio_pin_toggle_dt(&led0);
				} else if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
					gpio_pin_toggle_dt(&led1);
				} else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
					gpio_pin_toggle_dt(&led2);
				}
			break;
			}
		}
		case UART_RX_DISABLED:
			uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
			break;

		default:
			break;
	}
}

const struct uart_config uart_cfg = {
	.baudrate = 115200,
	.parity = UART_CFG_PARITY_NONE,
	.stop_bits = UART_CFG_STOP_BITS_1,
	.data_bits = UART_CFG_DATA_BITS_8,
	.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

int main(void) {
	int ret;

	int err = uart_configure(uart, &uart_cfg);

	if (err == -ENOSYS) {
		return -ENOSYS;
	}

    if (!device_is_ready(uart)){
		printk("UART device not ready\r\n");
		return 1 ;
	}

	if (!device_is_ready(led0.port)){
		printk("GPIO device is not ready\r\n");
		return 1;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ; 
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ;
	}

	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret) {
		return 1;
	}

	static uint8_t tx_buf[] =   {"nRF Connect SDK Fundamentals Course\r\n"
		"Press 0-2 on your keyboard to toggle LEDS 0-2 on your development kit\r\n"};

	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
	if (ret) {
		return 1;
	}

	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret) {
		return 1;
	}
}