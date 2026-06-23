#pragma once

constexpr static uint8_t LSM303_ADDR_A = 0x19; // I2C Address
constexpr static uint8_t LSM303_ADDR_M = 0x1E;

/* +== Accelerometer Registers ==+ */
constexpr static uint8_t CTRL_REG1_A = 0x20;
constexpr static uint8_t CTRL_REG2_A = 0x21;
constexpr static uint8_t CTRL_REG3_A = 0x22;
constexpr static uint8_t CTRL_REG4_A = 0x23;
constexpr static uint8_t CTRL_REG5_A = 0x24;
constexpr static uint8_t CTRL_REG6_A = 0x25;

constexpr static uint8_t REFERENCE_A = 0x26;
constexpr static uint8_t STATUS_REG_A = 0x27;
constexpr static uint8_t OUT_X_L_A = 0x28;
constexpr static uint8_t OUT_X_H_A = 0x29;

constexpr static uint8_t OUT_Y_L_A = 0x2A;
constexpr static uint8_t OUT_Y_H_A = 0x2B;

constexpr static uint8_t OUT_Z_L_A = 0x2C;
constexpr static uint8_t OUT_Z_H_A = 0x2D;

constexpr static uint8_t FIFO_CTRL_REG_A = 0x2E;
constexpr static uint8_t FIFO_SRC_A      = 0x2F;

constexpr static uint8_t INT1_CFG_A = 0x30;
constexpr static uint8_t INT1_SRC_A = 0x31;
constexpr static uint8_t INT1_THS_A = 0x32;
constexpr static uint8_t INT1_DURATION_A = 0x33;

constexpr static uint8_t INT2_CFG_A = 0x34;
constexpr static uint8_t INT2_SRC_A = 0x35;
constexpr static uint8_t INT2_THS_A = 0x36;
constexpr static uint8_t INT2_DURATION_A = 0x37;

constexpr static uint8_t CLICK_CFG_A = 0x38;
constexpr static uint8_t CLICK_SRC_A = 0x39;
constexpr static uint8_t CLICK_THS_A = 0x3A;

constexpr static uint8_t TIME_LIMIT_A = 0x3B;
constexpr static uint8_t TIME_LATENCY_A = 0x3C;
constexpr static uint8_t TIME_WINDOW_A = 0x3D;

/* +== Magnetometer Registers ==+ */
constexpr static uint8_t OFFSET_X_REG_L_M = 0x45;
constexpr static uint8_t OFFSET_X_REG_H_M = 0x46;

constexpr static uint8_t OFFSET_Y_REG_L_M = 0x47;
constexpr static uint8_t OFFSET_Y_REG_H_M = 0x48;

constexpr static uint8_t OFFSET_Z_REG_L_M = 0x49;
constexpr static uint8_t OFFSET_Z_REG_H_M = 0x4A;

constexpr static uint8_t WHO_AM_I_M = 0x4F;

constexpr static uint8_t CFG_REG_A_M = 0x60;
constexpr static uint8_t CFG_REG_B_M = 0x61;
constexpr static uint8_t CFG_REG_C_M = 0x62;

constexpr static uint8_t INT_CTRL_REG_M = 0x63;
constexpr static uint8_t INT_SOURCE_REG_M = 0x64;
constexpr static uint8_t INT_THS_L_REG_M = 0x65;
constexpr static uint8_t INT_THS_H_REG_M = 0x66;

constexpr static uint8_t STATUS_REG_M = 0x67;

constexpr static uint8_t OUTX_L_REG_M = 0x68;
constexpr static uint8_t OUTX_H_REG_M = 0x69;

constexpr static uint8_t OUTY_L_REG_M = 0x6A;
constexpr static uint8_t OUTY_H_REG_M = 0x6B;

constexpr static uint8_t OUTZ_L_REG_M = 0x6C;
constexpr static uint8_t OUTZ_H_REG_M = 0x6D;