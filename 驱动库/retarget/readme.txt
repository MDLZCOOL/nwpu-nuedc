这段代码摘自：https://github.com/cnoviello/mastering-stm32/blob/master/nucleo-f030R8/system/src/retarget

感谢原作者

默认用于stm32，其他平台需要修改接口

需要说明的是，这段代码的原理是重载c标准库的输入输出函数，具体行为与编译器有关，所以能否使用取决于编译器，实测gcc-arm-none-eabi-10.3编译器可用

使用方法：在串口初始化之后调用 RetargetInit(&huartX); 即可