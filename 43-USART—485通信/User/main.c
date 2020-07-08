/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   485通讯例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"   
#include "./key/bsp_key.h"  
#include "./485/bsp_485.h"


static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 

	char *pbuf;
	uint16_t len;
	
	LED_GPIO_Config();	
	
  /*初始化USART1*/
  Debug_USART_Config();
	
	/*初始化485使用的串口，使用中断模式接收*/
	RS485_Config();
	
	LED_BLUE;
	
	Key_GPIO_Config();
	
	printf("\r\n 欢迎使用野火F103-霸道  STM32  开发板。\r\n");
  printf("\r\n 野火F103-霸道 485通讯实验例程\r\n");
	
	printf("\r\n 实验步骤：\r\n");

	printf("\r\n 1.使用导线连接好两个485通讯设备\r\n");
	printf("\r\n 2.使用跳线帽连接好:5v --- C/4-5V,485-D --- PA2,485-R ---PA3 \r\n");
	printf("\r\n 3.若使用两个野火开发板进行实验，给两个开发板都下载本程序即可。\r\n");
	printf("\r\n 4.准备好后，按下其中一个开发板的KEY1键，会使用485向外发送0-255的数字 \r\n");
	printf("\r\n 5.若开发板的485接收到256个字节数据，会把数据以16进制形式打印出来。 \r\n");
	
  while(1)
  {
		/*按一次按键发送一次数据*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			uint16_t i;
			
			LED_BLUE;
			
			RS485_TX_EN();
			
			for(i=0;i<=0xff;i++)
			{
			RS485_SendByte(i);	 //发送数据
			}
			
			/*加短暂延时，保证485发送数据完毕*/
			Delay(0xFFF);
			RS485_RX_EN();
			
			LED_GREEN;
			
			printf("\r\n发送数据成功！\r\n"); //使用调试串口打印调试信息到终端

		}
		else
		{		
			LED_BLUE;
			
			pbuf = get_rebuff(&len);
			if(len>=256)
			{
				LED_GREEN;
				printf("\r\n接收到长度为%d的数据\r\n",len);	
				RS485_DEBUG_ARRAY((uint8_t*)pbuf,len);
				clean_rebuff();
			}
		}
  }

}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/
