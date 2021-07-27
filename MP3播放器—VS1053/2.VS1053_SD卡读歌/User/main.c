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
#include "bsp_exti.h"
FATFS fs;													/* Work area (file system object) for logical drives */

char song_pt = 0;
uint8_t song_list[][20] = {"0:TestFile_1.mp3", 	//歌曲的数量会自动计算出来，用户只需要在此添加歌曲名即可
													 "0:TestFile_2.mp3"};
						
char song_number_max = 0;
/**
  * @brief  计算歌曲数量
  * @param  无
  * @retval 无
 */							 
void song_list_init(void)
{
	song_number_max=sizeof(song_list)/sizeof(song_list[0]);
}

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
	f_mount(&fs,"0:",1);

	/* 初始化LED */
	LED_GPIO_Config();
	
	/* 初始化按键 */
	EXTI_Key_Config();

	/* 初始化VS1053的IO口 */
	VS_Init();
	
	/* 打印测试结果 */
	printf("vs1053:%4X\n",VS_Ram_Test());
	
	/* 延时100毫秒 */
	Delay_ms(100);
	
	/* 正弦测试  */
	VS_Sine_Test();	
	
	/* 硬复位MP3 */
	VS_HD_Reset();
	
	/* 软复位VS10XX */
	VS_Soft_Reset();
	
	/* 计算歌曲数量 */
	song_list_init();
	while(1)
	{
		// 测试歌曲放在SD卡根目录下
		vs1053_player_song(song_list[song_pt]);
		printf("MusicPlay End\n");
	}
}

/******************* end of file**************************/

