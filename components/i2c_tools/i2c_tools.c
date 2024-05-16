#include "i2c_tools.h"
#include "esp_log.h"
#include "print_bits.h"
#define I2C_TOOL_TIMEOUT_VALUE_MS (50)
i2c_master_bus_handle_t bus_handle;
i2c_master_bus_handle_t getHandle()
{
    return bus_handle;
}
static uint32_t i2c_frequency = 100 * 1000;

static const char *TAG = "I2C";

static esp_err_t i2c_get_port(int port, i2c_port_t *i2c_port)
{
    if (port >= I2C_NUM_MAX)
    {
        ESP_LOGE(TAG, "Wrong port number: %d", port);
        return ESP_FAIL;
    }
    *i2c_port = port;
    return ESP_OK;
}

static int i2c_config()
{
    i2c_port_t i2c_port = I2C_NUM_0;
    if (bus_handle)
    {

        // re-init the bus
        if (i2c_del_master_bus(bus_handle) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed at delete");

            return 1;
        }
    }

    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = i2c_port,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    if (i2c_new_master_bus(&i2c_bus_config, &bus_handle) != ESP_OK)
    {
        return 1;
    }
    i2c_detect();

    return 0;
}

int i2c_init()
{

    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));

    return i2c_config();
}

int i2c_detect()
{
    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16)
    {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++)
        {
            fflush(stdout);
            address = i + j;
            esp_err_t ret = i2c_master_probe(bus_handle, address, I2C_TOOL_TIMEOUT_VALUE_MS);
            if (ret == ESP_OK)
            {
                printf("%02x ", address);
            }
            else if (ret == ESP_ERR_TIMEOUT)
            {
                printf("UU ");
            }
            else
            {
                printf("-- ");
            }
        }
        printf("\r\n");
    }

    return 0;
}

int i2c_get(int chip_addr, int len)
{
    int data_addr = -1;
    uint8_t *data = malloc(len);

    i2c_device_config_t i2c_dev_conf = {
        .scl_speed_hz = i2c_frequency,
        .device_address = chip_addr,
    };
    i2c_master_dev_handle_t dev_handle;
    if (i2c_master_bus_add_device(bus_handle, &i2c_dev_conf, &dev_handle) != ESP_OK)
    {
                ESP_LOGE(TAG, "ERROR at i2c get");

        return 1;
    }

    esp_err_t ret = i2c_master_transmit_receive(dev_handle, (uint8_t *)&data_addr, 1, data, len, I2C_TOOL_TIMEOUT_VALUE_MS);
    if (ret == ESP_OK)
    {
        // SHOW(uint16_t,data);
        for (int i = 0; i < len; i++)
        {
            printf("0x%02x ", data[i]);
            if ((i + 1) % 16 == 0)
            {
                printf("\r\n");
            }
        }
        if (len % 16)
        {
            printf("\r\n");
        }
    }
    else if (ret == ESP_ERR_TIMEOUT)
    {
        ESP_LOGW(TAG, "Bus is busy");
    }
    else
    {
        ESP_LOGW(TAG, "Read failed");
    }
    free(data);
    if (i2c_master_bus_rm_device(dev_handle) != ESP_OK)
    {
        return 1;
    }
    return 0;
}

// static struct {
//     struct arg_int *chip_address;
//     struct arg_int *register_address;
//     struct arg_int *data;
//     struct arg_end *end;
// } i2cset_args;

// static int do_i2cset_cmd(int argc, char **argv)
// {
//     int nerrors = arg_parse(argc, argv, (void **)&i2cset_args);
//     if (nerrors != 0) {
//         arg_print_errors(stderr, i2cset_args.end, argv[0]);
//         return 0;
//     }

//     /* Check chip address: "-c" option */
//     int chip_addr = i2cset_args.chip_address->ival[0];
//     /* Check register address: "-r" option */
//     int data_addr = 0;
//     if (i2cset_args.register_address->count) {
//         data_addr = i2cset_args.register_address->ival[0];
//     }
//     /* Check data: "-d" option */
//     int len = i2cset_args.data->count;

//     i2c_device_config_t i2c_dev_conf = {
//         .scl_speed_hz = i2c_frequency,
//         .device_address = chip_addr,
//     };
//     i2c_master_dev_handle_t dev_handle;
//     if (i2c_master_bus_add_device(bus_handle, &i2c_dev_conf, &dev_handle) != ESP_OK) {
//         return 1;
//     }

//     uint8_t *data = malloc(len + 1);
//     data[0] = data_addr;
//     for (int i = 0; i < len; i++) {
//         data[i + 1] = i2cset_args.data->ival[i];
//     }
//     esp_err_t ret = i2c_master_transmit(dev_handle, data, len + 1, I2C_TOOL_TIMEOUT_VALUE_MS);
//     if (ret == ESP_OK) {
//         ESP_LOGI(TAG, "Write OK");
//     } else if (ret == ESP_ERR_TIMEOUT) {
//         ESP_LOGW(TAG, "Bus is busy");
//     } else {
//         ESP_LOGW(TAG, "Write Failed");
//     }

//     free(data);
//     if (i2c_master_bus_rm_device(dev_handle) != ESP_OK) {
//         return 1;
//     }
//     return 0;
// }

