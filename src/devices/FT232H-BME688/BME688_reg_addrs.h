#ifndef BME688_REG_ADDRS_H
#define BME688_REG_ADDRS_H

#define BME688_CTRL_MEAS_REG_ADDR   0x74
#define BME688_CTRL_HUM_REG_ADDR    0x72
#define BME688_CTRL_GAS_1_REG_ADDR  0x71
#define BME688_CONFIG_REG_ADDR      0x75

// temperature field 0 data registers
#define BME688_TEMP_MSB0_REG_ADDR   0x22
#define BME688_TEMP_LSB0_REG_ADDR    0x23
#define BME688_TEMP_XLSB0_REG_ADDR   0x24

// temperature calibration parameters
#define BME688_TEMP_CALIB_PAR_T1_MSB_REG_ADDR    0xEA
#define BME688_TEMP_CALIB_PAR_T1_LSB_REG_ADDR    0xE9

#define BME688_TEMP_CALIB_PAR_T2_MSB_REG_ADDR    0x8B
#define BME688_TEMP_CALIB_PAR_T2_LSB_REG_ADDR    0x8A

#define BME688_TEMP_CALIB_PAR_T3_REG_ADDR        0x8C

// pressure field 0 data registers
#define BME688_PRESS_MSB0_REG_ADDR  0x1F
#define BME688_PRESS_LSB0_REG_ADDR  0x20
#define BME688_PRESS_XLSB0_REG_ADDR 0x21


// pressure calibration parameters
#define BME688_PRESS_CALIB_PAR_P1_MSB_REG_ADDR   0x8F
#define BME688_PRESS_CALIB_PAR_P1_LSB_REG_ADDR   0x8E

#define BME688_PRESS_CALIB_PAR_P2_MSB_REG_ADDR   0x91
#define BME688_PRESS_CALIB_PAR_P2_LSB_REG_ADDR   0x90

#define BME688_PRESS_CALIB_PAR_P3_REG_ADDR       0x92

#define BME688_PRESS_CALIB_PAR_P4_MSB_REG_ADDR   0x95
#define BME688_PRESS_CALIB_PAR_P4_LSB_REG_ADDR   0x94

#define BME688_PRESS_CALIB_PAR_P5_MSB_REG_ADDR   0x97
#define BME688_PRESS_CALIB_PAR_P5_LSB_REG_ADDR   0x96

#define BME688_PRESS_CALIB_PAR_P6_REG_ADDR       0x99

#define BME688_PRESS_CALIB_PAR_P7_REG_ADDR       0x98

#define BME688_PRESS_CALIB_PAR_P8_MSB_REG_ADDR   0x9D
#define BME688_PRESS_CALIB_PAR_P8_LSB_REG_ADDR   0x9C

#define BME688_PRESS_CALIB_PAR_P9_MSB_REG_ADDR   0x9F
#define BME688_PRESS_CALIB_PAR_P9_LSB_REG_ADDR   0x9E

#define BME688_PRESS_CALIB_PAR_P10_REG_ADDR      0xA0


// humidity field 0 data registers
#define BME688_HUM_MSB0_REG_ADDR    0x25
#define BME688_HUM_LSB0_REG_ADDR    0x26

// humidity calibration parameters
#define BME688_HUM_CALIB_PAR_H1_LSB_REG_ADDR     0xE2
#define BME688_HUM_CALIB_PAR_H1_MSB_REG_ADDR     0xE3

#define BME688_HUM_CALIB_PAR_H2_LSB_REG_ADDR     0xE2
#define BME688_HUM_CALIB_PAR_H2_MSB_REG_ADDR     0xE1

#define BME688_HUM_CALIB_PAR_H3_REG_ADDR         0xE4

#define BME688_HUM_CALIB_PAR_H4_REG_ADDR         0xE5

#define BME688_HUM_CALIB_PAR_H5_REG_ADDR         0xE6

#define BME688_HUM_CALIB_PAR_H6_REG_ADDR         0xE7

#define BME688_HUM_CALIB_PAR_H7_REG_ADDR         0xE8


#endif
