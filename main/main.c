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
        int ret =i2c_detect();
        ret = i2c_dump(0x21,2);
       int *data =1111111111111111;

      ret =  i2c_set(0x21, data);
        printf("got this back %d",ret);

        
    }
}