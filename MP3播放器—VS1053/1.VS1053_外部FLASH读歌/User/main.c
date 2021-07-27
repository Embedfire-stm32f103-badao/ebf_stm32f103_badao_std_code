/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
	*/
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h" 
#include "ff.h"
#include  "VS1053.h"	
#include "bsp_SysTick.h"

FATFS fs;													/* Work area (file system object) for logical drives */
int res;

/**
  * @brief  
  * @param  无
  * @retval 无
 */
int main(void)
{
	SysTick_Init();
	
	/* 初始化串口 */
	USART1_Config();
	res = f_mount(&fs,"1:",1);
  if(FR_OK != res)
  {
    printf("f mount Error (%d)\r\n", res);
    while(1);
  }

	/* 初始化LED */
	LED_GPIO_Config();

	VS_Init();
	printf("vs1053:%4X\n",VS_Ram_Test());
	Delay_ms(100);
	VS_Sine_Test();	
	VS_HD_Reset();
	VS_Soft_Reset();
	
	while(1)
	{
		// 测试歌曲放在外部FLASH W25Q64中
		vs1053_player_song("1:TestFile.mp3");
		printf("MusicPlay End\n");
	}
	
}


/******************* end of file**************************/

