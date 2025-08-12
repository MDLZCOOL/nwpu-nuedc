#ifndef __AD9959_H
#define __AD9959_H

#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>

#define SDIO0_H DL_GPIO_setPins(AD9959_PORT, AD9959_SD0_PIN)
#define SDIO0_L DL_GPIO_clearPins(AD9959_PORT, AD9959_SD0_PIN)
#define SDIO1_H DL_GPIO_setPins(AD9959_PORT, AD9959_SD1_PIN)
#define SDIO1_L DL_GPIO_clearPins(AD9959_PORT, AD9959_SD1_PIN)
#define SDIO2_H DL_GPIO_setPins(AD9959_PORT, AD9959_SD2_PIN)
#define SDIO2_L DL_GPIO_clearPins(AD9959_PORT, AD9959_SD2_PIN)
#define SDIO3_H DL_GPIO_setPins(AD9959_PORT, AD9959_SD3_PIN)
#define SDIO3_L DL_GPIO_clearPins(AD9959_PORT, AD9959_SD3_PIN)
#define PDC_H DL_GPIO_setPins(AD9959_PORT, AD9959_PCD_PIN)
#define PDC_L DL_GPIO_clearPins(AD9959_PORT, AD9959_PCD_PIN)
#define RST_H DL_GPIO_setPins(AD9959_PORT, AD9959_RST_PIN)
#define RST_L DL_GPIO_clearPins(AD9959_PORT, AD9959_RST_PIN)
#define SCLK_H DL_GPIO_setPins(AD9959_PORT, AD9959_SCLK_PIN)
#define SCLK_L DL_GPIO_clearPins(AD9959_PORT, AD9959_SCLK_PIN)
#define CS_H DL_GPIO_setPins(AD9959_PORT, AD9959_CS_PIN)
#define CS_L DL_GPIO_clearPins(AD9959_PORT, AD9959_CS_PIN)
#define UPDATE_H DL_GPIO_setPins(AD9959_PORT, AD9959_UP_PIN)
#define UPDATE_L DL_GPIO_clearPins(AD9959_PORT, AD9959_UP_PIN)
#define PS0_H DL_GPIO_setPins(AD9959_PORT, AD9959_P0_PIN)
#define PS0_L DL_GPIO_clearPins(AD9959_PORT, AD9959_P0_PIN)
#define PS1_H DL_GPIO_setPins(AD9959_PORT, AD9959_P1_PIN)
#define PS1_L DL_GPIO_clearPins(AD9959_PORT, AD9959_P1_PIN)
#define PS2_H DL_GPIO_setPins(AD9959_PORT, AD9959_P2_PIN)
#define PS2_L DL_GPIO_clearPins(AD9959_PORT, AD9959_P2_PIN)
#define PS3_H DL_GPIO_setPins(AD9959_PORT, AD9959_P3_PIN)
#define PS3_L DL_GPIO_clearPins(AD9959_PORT, AD9959_P3_PIN)

#define CSR_ADD 0x00   // CSR  Channel Select Register(通道选择寄存器)                1 Bytes
#define FR1_ADD 0x01   // FR1  Function Register 1(功能寄存器1)                       3 Bytes
#define FR2_ADD 0x02   // FR2  Function Register 2(功能寄存器2)                       2 Bytes
#define CFR_ADD 0x03   // CFR  Channel Function Register(通道功能寄存器)              3 Bytes
#define CFTW0_ADD 0x04 // CFTW0 Channel Frequency Tuning Word 0(通道频率转换字寄存器)  4 Bytes
#define CPOW0_ADD 0x05 // CPW0 Channel Phase Offset Word 0(通道相位转换字寄存器)      2 Bytes
#define ACR_ADD 0x06   // ACR  Amplitude Control Register(幅度控制寄存器)             3 Bytes
#define LSRR_ADD 0x07  // LSR  Linear Sweep Ramp Rate(通道线性扫描寄存器)             2 Bytes
#define RDW_ADD 0x08   // RDW  LSR Rising Delta Word(通道线性向上扫描寄存器)          4 Bytes
#define FDW_ADD 0x09   // FDW  LSR Falling Delta Word(通道线性向下扫描寄存器)         4 Bytes
#define CW1 0x0A       // CW1  Channel Word 1(通道字寄存器1)                         4 Bytes
#define CW2 0x0B       // CW2  Channel Word 2(通道字寄存器2)                         4 Bytes
#define CW3 0x0C       // CW3  Channel Word 3(通道字寄存器3)                         4 Bytes
#define CW4 0x0D       // CW4  Channel Word 4(通道字寄存器4)                         4 Bytes

void Init_AD9959(void);
void delay_9959(uint32_t length);
void InitIO_9959(void);
void InitReset(void);
void AD9959_IO_Update(void);
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);
void ReadData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);
void Write_Frequence(uint8_t Channel, uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void Write_Phase(uint8_t Channel, uint16_t Phase);

uint32_t Get_Freq(void);
uint8_t Get_Amp(void);
void Sweep_Frequency(uint8_t Channel, uint32_t Start_Freq, uint32_t Stop_Freq, uint32_t Step, uint32_t time,
                     uint8_t NO_DWELL);
void Channel_Select(uint8_t Channel);
void Stop_AD9959(void);
void AD9959_error(void);
void SET_2FSK(uint8_t Channel, double f_start, double f_stop);
void SET_2ASK(uint8_t Channel, double f, uint16_t A_start, uint16_t A_stop);
void Freq2Word(double f, uint8_t *fWord);
void Amp2Word(uint16_t A, uint8_t *AWord);
void Phase2Word(uint16_t Phase, uint8_t *PWord);
#endif
