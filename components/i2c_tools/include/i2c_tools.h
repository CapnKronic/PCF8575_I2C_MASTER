#include "driver/i2c_master.h"

#define SCL 22
#define SDA 21
#define I2C_PORT 0
int i2c_init();
int i2c_detect();
i2c_master_bus_handle_t getHandle();
int i2c_get(int chip_addr, int length);