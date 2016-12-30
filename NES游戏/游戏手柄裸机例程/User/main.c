/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_led.h"  
#include  "joypad.h"

static void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	uint8_t joykey=0;
	
	LED_GPIO_Config();
	JOYPAD_Init();
	/* USART1 配置模式为 115200 8-N-1，中断接收 */
	USARTx_Config();
	printf("游戏手柄\n");
	LED2_ON;
	while(1)
	{
		joykey=JOYPAD_Read();
		if(joykey)
		{
			//printf("joykey:%d\n",joykey);
			if(joykey&0x10)printf("上");
			if(joykey&0x20)printf("下");
			if(joykey&0x40)printf("左");
			if(joykey&0x80)printf("右");
			if(joykey&0x04)printf("选择");
			if(joykey&0x08)printf("开始");
			if(joykey&0x01)printf("A");
			if(joykey&0x02)printf("B");
			printf("\n");
		}
		Delay(0x2ffff);		
	}
}

/*********************************************END OF FILE**********************/
