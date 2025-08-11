0. 接口相关
接口为i2c或spi，仅需修改oled.h中的Send函数即可完成移植

1. 使用
平台初始化i2c/spi完成后调用OLED_Init()初始化OLED. 注意MCU启动比OLED上电快, 可等待20ms再初始化OLED
调用OLED_NewFrame()开始绘制新的一帧
调用OLED_DrawXXX()系列函数绘制图形到显存 调用OLED_Printxxx()系列函数绘制文本到显存
调用OLED_ShowFrame()将显存内容显示到OLED