#pragma once

#include <main.h>

#define DIN_H HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_SET)
#define DIN_L HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_RESET)
#define SCLK_H HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET)
#define SCLK_L HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET)
#define CLR_H HAL_GPIO_WritePin(CLR_GPIO_Port, CLR_Pin, GPIO_PIN_SET)
#define CLR_L HAL_GPIO_WritePin(CLR_GPIO_Port, CLR_Pin, GPIO_PIN_RESET)
#define SYNC_H HAL_GPIO_WritePin(SYNC_GPIO_Port, SYNC_Pin, GPIO_PIN_SET)
#define SYNC_L HAL_GPIO_WritePin(SYNC_GPIO_Port, SYNC_Pin, GPIO_PIN_RESET)
#define LDAC_H HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_SET)
#define LDAC_L HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_RESET)

void dac8562_delay(unsigned int nCount);
void dac8562_write(uint8_t cmd, uint16_t data);
void dac8562_set_voltage(uint16_t test);
