#include <stdio.h>
#include "digitron.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void app_main(void)
{
    digitron_init();
    int i = 0;
    while (1)
    {
        digitron_display_int(i++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
