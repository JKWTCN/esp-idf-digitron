#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

esp_err_t spi_write(spi_device_handle_t spi, uint8_t *data, uint8_t len);
void digitron_clear(spi_device_handle_t spi);
void digitron_display_error();
void digitron_display_int(int num);
void digitron_init(void);
#pragma once
