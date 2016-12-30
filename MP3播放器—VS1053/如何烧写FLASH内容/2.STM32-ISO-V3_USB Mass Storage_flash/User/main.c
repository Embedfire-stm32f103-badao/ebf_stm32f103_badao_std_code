/**
  ******************************************************************************
  * @file    main.c
  * @author  秉火
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "stm32f10x.h"
#include "..\User\spi_flash\fatfs_flash_spi.h"
#include "..\User\usart\bsp_usart1.h"	
#include "..\User\led\bsp_led.h"  
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"

static void USB_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	 /* USART1 config */
	USART1_Config();
	
  LED_GPIO_Config();
  
	/*初始化*/
	Set_System();
  	
	/*设置USB时钟为48M*/
	Set_USBClock();
 	
	/*配置USB中断(包括SDIO中断)*/
	USB_Interrupts_Config();
 
	/*USB初始化*/
 	USB_Init();
 
 	while (bDeviceState != CONFIGURED);	 //等待配置完成
	   
	printf("\r\n 野火 ISO STM32 USB MASS STORAGE 实验\r\n");
	 
  while (1)
  {
    LED2_TOGGLE;
    USB_Delay(0x0FFFFF);
  }
}

/* -------------------------------------end of file -------------------------------------------- */
