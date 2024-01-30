#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"


static const int TXBUFSIZE = 2*1024;
static const int RXBUFSIZE = 2*1024;

#define UART    (UART_NUM_1)
#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)


void init(void) {

  const uart_config_t uart_config = {
    .baud_rate = 120000,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT,
  };

  // We don't receive any data.
  ESP_ERROR_CHECK(uart_driver_install(UART, RXBUFSIZE, TXBUFSIZE, 10, NULL, 0));
  ESP_ERROR_CHECK(uart_param_config(UART, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

static void txTask(void *arg)
{
  static const char *TX_TASK_TAG = "TX_TASK";
  static const char togglingBits[] = {
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
  };

  esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);

  for (;;) {
    uart_write_bytes(UART, togglingBits, sizeof(togglingBits));
  }
}

void app_main(void)
{
  init();
  xTaskCreate(txTask, "txTask", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}
