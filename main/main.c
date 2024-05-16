#include <stdio.h>
#include "i2c_tools.h"
#include "print_bits.h"
#include "esp_log.h"

static const char *TAG = "I2C";

void app_main(void)
{
int x = bit_test();
printf("bit test: \n%d",x);
    if (i2c_init() == 1)
    {
        ESP_LOGI(TAG, "ERROR Connecting to I2C");

    }
    else{

        ESP_LOGI(TAG, "SUCCESS! Connected to I2C");
        int ret = i2c_get(0x20,2);
        printf("got this back %d",ret);

        
    }
}