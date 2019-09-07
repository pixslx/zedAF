#pragma once

#define SD_EN_PIN PA0
#define SD_DIR_PIN PA1
#define SD_STEP_PIN PA2
#define SD_CS_PIN PA3

#define BMP_CS_PIN PB10

const String VERSION = "11";

const long SERIAL_BOUD_RATE = 115200;

const double MAX_SPEED = 1000.0;
const int INITIAL_SPEED_DIVIDER = 2;
const double ACCELERATION = 200.0;
const bool INITIAL_HALF_STEP = false;
const int FULL_STEP_MICROSTEPS = 8;
const bool INITIAL_TEMP_COMPENSATION = false;
const int INWARD_DIRECTION = 1;
const int OUTWARD_DIRECTION = 0;
