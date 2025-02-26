![YARP logo](https://raw.githubusercontent.com/robotology/yarp/master/doc/images/yarp-robot-24.png "yarp-device-FT232H-BME688")
Yarp device for FT232H-BME688
=====================

This repository contains the YARP plugin for the BME688 temperature, pressure, humidity, and gas sensor when using the FT232H breakout as the middle-man. This YARP device currently only supports extracting temperature, pressure, and humidity values, support for reading the gas measurement is currently not supported. **Note:** This device has only been tested on LTS Ubuntu Distributions.

<h1 align="center"> Documentation </h1>

This YARP device includes and uses the following shared libraries provided by FTDI: [libftd2xx version 1.4.32 for Linux](https://ftdichip.com/drivers/d2xx-drivers/) and [libmpsse version 1.0.7 for Linux](https://ftdichip.com/software-examples/mpsse-projects/libmpsse-i2c-examples/)

<h1 align="center"> Installation </h1>

### Edit the uDev Rules for the FT232H
1) Create the following new file for the udev rules: `/etc/udev/rules.d/11-ftdi.rules`
2) Fill it with the following contents:
```
# /etc/udev/rules.d/11-ftdi.rules
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6001", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6011", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6010", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6014", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6015", GROUP="plugdev", MODE="0666"
```
**Note:** These rules will take effect the next time you plug in the FT232H board

### Build with pure CMake commands

~~~
git clone https://github.com/ami-iit/yarp-device-FT232H-BME688.git
cd yarp-device-FT232H-BME688
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=<install_prefix>
make
make install
~~~
To make the device detectable, you can **either**:
- Add `<installation_path>/share/yarp` to the `YARP_DATA_DIRS` environment variable.
- If YARP has been installed using [robotology-superbuild](https://github.com/robotology/robotology-superbuild), one can use `<directory-where-you-downloaded-robotology-superbuild>/build/install` for the `<installation_path>`


### Edit your LD_LIBRARY_PATH env variable
1) Edit your `.bashrc` to append the full path of the `lib` folder provided by the YARP device to the `LD_LIBRARY_PATH` environment variable
    - To get this path do:  `cd lib` and then `pwd`
2) Then restart your terminal


<h1 align="center"> Running the device </h1>

1) Make sure yarpserver is running
2) Plug the BME688 sensor into the FT232H breakout
3) Connect the FT232H breakout to your computer
4) Type: `yarpdev --device ft232h_bme688`
5) While the device is running, open a new terminal and type: `yarp read ... /FT232H_BME688/data`

**Note: data is sent from the YARP device in the following order:** `temperature pressure humidity`

Units are as follows:
- Temperature: `°C`
- Pressure: `hPa`
- Humitidy: `%`


<h1 align="center"> Troubleshooting the device </h1>

Some users may receive the following error when running the YARP device: `[ERROR] Runtime error: No channels found`. To fix this error, follow these steps:
- Unplug the FT232H device from your computer and plug it back in
- run `sudo rmmod ftdi_sio`
- then run `sudo rmmod usbserial`
- then re-run the YARP device


License
---------

:construction: This repository is currently work in progress. :construction:

Maintainers
--------------
This repository is maintained by:

| | |
|:---:|:---:|
| [<img src="https://github.com/nicktrem.png" width="40">](https://github.com/nicktrem) | [@nicktrem](https://github.com/nicktrem) |
