#include "spi_flash_py25q32.h"

/**********************************************************************************
 * 函数功能: 模块初始化
 */
uint8_t BSP_PY25Q32_Init(void)
{
    BSP_PY25Q32_Reset();
    return BSP_PY25Q32_GetStatus();
}

static void BSP_PY25Q32_Reset(void)
{
    uint8_t cmd[2] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};

    PY25Q32_Enable();
    /* Send the reset command */
    HAL_SPI_Transmit(&hspi1, cmd, 2, PY25Q32_TIMEOUT_VALUE); // 1
    PY25Q32_Disable();
}

/************************************************************a**********************
 * 函数功能: 获取设备状态
 */
static uint8_t BSP_PY25Q32_GetStatus(void)
{
    uint8_t cmd[] = {READ_STATUS_REG1_CMD};
    uint8_t status;

    PY25Q32_Enable();
    /* Send the read status command */
    HAL_SPI_Transmit(&hspi1, cmd, 1, PY25Q32_TIMEOUT_VALUE);
    /* Reception of the data */
    HAL_SPI_Receive(&hspi1, &status, 1, PY25Q32_TIMEOUT_VALUE);
    PY25Q32_Disable();

    /* Check the value of the register */
    if ((status & PY25Q32_FSR_BUSY) != 0)
    {
        return PY25Q32_BUSY;
    }
    else
    {
        return PY25Q32_OK;
    }
}

/**********************************************************************************
 * 函数功能: 写使能
 */
uint8_t BSP_PY25Q32_WriteEnable(void)
{
    uint8_t cmd[] = {WRITE_ENABLE_CMD};
    uint32_t tickstart = HAL_GetTick();

    /*Select the FLASH: Chip Select low */
    PY25Q32_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi1, cmd, 1, PY25Q32_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    PY25Q32_Disable();

    /* Wait the end of Flash writing */
    while (BSP_PY25Q32_GetStatus() == PY25Q32_BUSY)
        ;
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > PY25Q32_TIMEOUT_VALUE)
        {
            return PY25Q32_TIMEOUT;
        }
    }

    return PY25Q32_OK;
}

/**********************************************************************************
 * 函数功能: 获取设备ID
 */
void BSP_PY25Q32_Read_ID(uint8_t *ID)
{
    uint8_t cmd[4] = {READ_ID_CMD, 0x00, 0x00, 0x00};

    PY25Q32_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi1, cmd, 4, PY25Q32_TIMEOUT_VALUE);
    /* Reception of the data */
    HAL_SPI_Receive(&hspi1, ID, 2, PY25Q32_TIMEOUT_VALUE);
    PY25Q32_Disable();
}

/**********************************************************************************
 * 函数功能: 读数据
 * 输入参数: 缓存数组指针、读地址、字节数
 */
uint8_t BSP_PY25Q32_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);

    PY25Q32_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi1, cmd, 4, PY25Q32_TIMEOUT_VALUE);
    /* Reception of the data */
    if (HAL_SPI_Receive(&hspi1, pData, Size, PY25Q32_TIMEOUT_VALUE) != HAL_OK)
    {
        return PY25Q32_ERROR;
    }
    PY25Q32_Disable();
    return PY25Q32_OK;
}

/**********************************************************************************
 * 函数功能: 写数据
 * 输入参数: 缓存数组指针、写地址、字节数
 */
uint8_t BSP_PY25Q32_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[4];
    uint32_t end_addr, current_size, current_addr;
    uint32_t tickstart = HAL_GetTick();

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= WriteAddr)
    {
        current_addr += PY25Q32_PAGE_SIZE;
    }
    current_size = current_addr - WriteAddr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
        current_size = Size;
    }

    /* Initialize the adress variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* Perform the write page by page */
    do
    {
        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t)(current_addr >> 16);
        cmd[2] = (uint8_t)(current_addr >> 8);
        cmd[3] = (uint8_t)(current_addr);

        /* Enable write operations */
        BSP_PY25Q32_WriteEnable();

        PY25Q32_Enable();
        /* Send the command */
        if (HAL_SPI_Transmit(&hspi1, cmd, 4, PY25Q32_TIMEOUT_VALUE) != HAL_OK)
        {
            return PY25Q32_ERROR;
        }

        /* Transmission of the data */
        if (HAL_SPI_Transmit(&hspi1, pData, current_size, PY25Q32_TIMEOUT_VALUE) != HAL_OK)
        {
            return PY25Q32_ERROR;
        }
        PY25Q32_Disable();
        /* Wait the end of Flash writing */
        while (BSP_PY25Q32_GetStatus() == PY25Q32_BUSY)
            ;
        {
            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > PY25Q32_TIMEOUT_VALUE)
            {
                return PY25Q32_TIMEOUT;
            }
        }

        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + PY25Q32_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : PY25Q32_PAGE_SIZE;
    } while (current_addr < end_addr);

    return PY25Q32_OK;
}

/**********************************************************************************
 * 函数功能: 扇区擦除
 * 输入参数: 地址
 */
uint8_t BSP_PY25Q32_Erase_Block(uint32_t Address)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)(Address >> 16);
    cmd[2] = (uint8_t)(Address >> 8);
    cmd[3] = (uint8_t)(Address);

    /* Enable write operations */
    BSP_PY25Q32_WriteEnable();

    /*Select the FLASH: Chip Select low */
    PY25Q32_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi1, cmd, 4, PY25Q32_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    PY25Q32_Disable();

    /* Wait the end of Flash writing */
    while (BSP_PY25Q32_GetStatus() == PY25Q32_BUSY)
        ;
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > PY25Q32_SECTOR_ERASE_MAX_TIME)
        {
            return PY25Q32_TIMEOUT;
        }
    }
    return PY25Q32_OK;
}

/**********************************************************************************
 * 函数功能: 芯片擦除
 */
uint8_t BSP_PY25Q32_Erase_Chip(void)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = CHIP_ERASE_CMD;

    /* Enable write operations */
    BSP_PY25Q32_WriteEnable();

    /*Select the FLASH: Chip Select low */
    PY25Q32_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi1, cmd, 1, PY25Q32_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    PY25Q32_Disable();

    /* Wait the end of Flash writing */
    while (BSP_PY25Q32_GetStatus() != PY25Q32_BUSY)
        ;
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > PY25Q32_BULK_ERASE_MAX_TIME)
        {
            return PY25Q32_TIMEOUT;
        }
    }
    return PY25Q32_OK;
}
