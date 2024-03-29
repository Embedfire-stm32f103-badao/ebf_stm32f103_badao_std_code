/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   DAC输出正弦波例程 PA4 PA5双通道输出（板子的PA4引脚复用会影响输出电压）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./dac/bsp_dac.h"




//【 ！！】注意事项：

//DAC通道与SPI-FLASH引脚共用，所以DAC和SPI-FLASH程序不能同时运行。

//！参考本章的硬件设计部分说明，实验前需要拆掉SPI-FLASH芯片附近的电阻R6。
//！不拆掉的话PA4引脚输出的正弦波不正常

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
			/*初始化DAC，开始DAC转换*/
			DAC_Mode_Init();
	
  	  while(1);	 
}

/*********************************************END OF FILE**********************/

