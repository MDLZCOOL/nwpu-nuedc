#include "ina226.h"

/*
**************************************************
* 说明：读取BUS电压，并转换为浮点数据
**************************************************
*/
float INA226_GetBusV(uint16_t ina226Address)
{
    uint16_t regData;
    float fVoltage;
    regData = INA226_GetBusVReg(ina226Address);
    fVoltage = regData * 0.00125f; /*电压的LSB = 1.25mV*/
    return fVoltage * 10.492f / 10.500f;
}
/*
**************************************************
* 说明：读取电流，并转换为浮点数据
**************************************************
*/
float INA226_GetCurrent(uint16_t ina226Address)
{
    uint16_t regData;
    float fCurrent;
    regData = INA226_GetCurrentReg(ina226Address);
    if (regData >= 0x8000)
        regData = 0;
    fCurrent = regData * 0.0002f; /*电流的LSB = 0.2mA，由用户配置*/
    return fCurrent / 1.010f * 0.98572f;
}
/*
**************************************************
* 说明：读取功率，并转换为浮点数据
**************************************************
*/
float INA226_GetPower(uint16_t ina226Address)
{
    uint16_t regData;
    float fPower;
    regData = INA226_GetPowerReg(ina226Address);
    fPower = regData * 0.005f; /*功率的LSB = 电流的LSB*25*/
    return fPower;
}

uint8_t INA226_SetConfig(uint16_t ConfigWord, uint16_t ina226Address)
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_CONFIG;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetConfig(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_CONFIG};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetShuntV(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_SHUNTV};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetBusVReg(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_BUSV};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t INA226_SetCalibrationReg(uint16_t ConfigWord, uint16_t ina226Address)
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_CALIB;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetCalibrationReg(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_CALIB};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetPowerReg(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_POWER};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetCurrentReg(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_CURRENT};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetManufID(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_MANUF_ID};
    uint8_t ReceivedTable[2];

    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t INA226_GetDieID(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_DIE_ID};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t INA226_SetMaskEnable(uint16_t ConfigWord, uint16_t ina226Address)
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_MASK;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetMaskEnable(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_MASK};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t INA226_SetAlertLimit(uint16_t ConfigWord, uint16_t ina226Address)
{
    uint8_t SentTable[3];
    SentTable[0] = INA226_ALERTL;
    SentTable[1] = (ConfigWord & 0xFF00) >> 8;
    SentTable[2] = (ConfigWord & 0x00FF);
    return HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 3, INA226_I2C_TIMEOUT);
}

uint16_t INA226_GetAlertLimit(uint16_t ina226Address)
{
    uint8_t SentTable[1] = {INA226_ALERTL};
    uint8_t ReceivedTable[2];
    HAL_I2C_Master_Transmit(&INA226_COM_PORT, ina226Address, SentTable, 1, INA226_I2C_TIMEOUT);
    if (HAL_I2C_Master_Receive(&INA226_COM_PORT, ina226Address, ReceivedTable, 2, INA226_I2C_TIMEOUT) != HAL_OK)
        return 0xFF;
    else
        return ((uint16_t)ReceivedTable[0] << 8 | ReceivedTable[1]);
}
