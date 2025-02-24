#include <unistd.h>
#include "BME688.h"
#include "FT232H_i2c.h"
#include <iostream>
#include "FT232H_BME688_Driver.h"
#include <yarp/os/Network.h>
#include <yarp/os/Property.h>

int main(void)
{
    yarp::os::Network network;
    FT232H_BME688_Driver driver;
    yarp::os::Property prop;
    driver.open(prop);

    usleep(10000 * 1000);

    driver.close();

    return 0;
}

