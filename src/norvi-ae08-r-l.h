#pragma once
#include <Arduino.h>
#ifdef ESP32

#define GSM_RX_PIN (33u)
#define GSM_TX_PIN (32u)

#define GSM_RESET_PIN (21u)

// Input inversion
#define INPUT_INVERSION

// Input pin values
//#define INPUT_PIN_1 (13u) // New pin?
#define INPUT_PIN_1 (39u) // Old pin?
#define INPUT_PIN_2 (34u)
#define INPUT_PIN_3 (35u)
#define INPUT_PIN_4 (14u)

#define OUTPUT_PIN_1 (12u)
#define OUTPUT_PIN_2 (2u)
#define OUTPUT_PIN_3 (27u)
#define OUTPUT_PIN_4 (4u)

#define WAN_SPI_CS_PIN (5u)
#define WAN_MISO_PIN (19u)
#define WAN_MOSI_PIN (23u)
#define WAN_SCLK_PIN (18u)
#define WAN_RESET_PIN (22u)
#define WAN_INIT_PIN (2u)

#define OLED_I2C_SCL_PIN (17u)
#define OLED_I2C_SDA_PIN (16u)

#define ANALOG_BTN_PIN (36u)

#endif