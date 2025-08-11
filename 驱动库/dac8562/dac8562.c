#include "dac8562.h"

void dac8562_set_voltage(uint16_t test)
{
  CLR_L;
  LDAC_H;
  dac8562_write(0x28,0x0001);
  dac8562_write(0x20,0x0003);
  dac8562_write(0x38,0x0001);
  dac8562_write(0x18,test);
  dac8562_write(0x19,test);
  LDAC_L;
  dac8562_delay(0x01);
  LDAC_H;
}

void dac8562_write(uint8_t cmd,uint16_t data)
{
  SYNC_H;
  dac8562_delay(0x01);
  SYNC_L;
  SCLK_L;
  for(uint8_t s=0;s<8;s++)
  {
    if((cmd&0x80)==0x80){DIN_H;}
    else{DIN_L;}
    dac8562_delay(0x01);
    SCLK_H;
    dac8562_delay(0x01);
    cmd<<=1;
    SCLK_L;
    dac8562_delay(0x01);
  }
  for(uint8_t s=0;s<16;s++)
  {
    if((data&0x8000)==0x8000){DIN_H;}
    else{DIN_L;}
    dac8562_delay(0x01);
    SCLK_H;
    dac8562_delay(0x01);
    data<<=1;
    SCLK_L;
    dac8562_delay(0x01);
  }
}

void dac8562_delay(unsigned int nCount)
{
  for(; nCount != 0; nCount--);
}
