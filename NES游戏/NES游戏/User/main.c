/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   液晶触摸画板实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_lcd.h"
#include "palette.h"
#include "joypad.h"
#include "bsp_led.h"   
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "bsp_fsmc_sram.h"
#include "ff.h"
#include "VS1053.h"
#include "nes_main.h"	
#include "TouchPad.h"
#include "malloc.h"	
#include "bsp_key.h" 

#define FILE_NAME_LEN 			100	
#define MAX_FILE_NUM				15
#define _DF1S								0x81

static FATFS fs;
static char path[100]="0:";
static uint8_t  file_num = 0;	
static uint8_t sel_index;
static unsigned char filelist[MAX_FILE_NUM][FILE_NAME_LEN]={0};
static unsigned char showlist[MAX_FILE_NUM][FILE_NAME_LEN/2]={0};
//频率设置
//PLL,倍频数
void system_clock_set(u8 PLL)
{
	unsigned char temp=0;   
	//复位并配置向量表
	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断		 
	SCB->VTOR = 0x08000000;//设置NVIC的向量表偏移寄存器
	
 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(!(RCC->CR>>17));//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;				  //抵消2个单位（因为是从2开始的，设置0就是2）
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}  

static FRESULT scan_files (char* path) 
{ 
	FRESULT res;
	FILINFO fno; 
	DIR dir; 
	int i=0; 
	char *fn; 
	char file_name[FILE_NAME_LEN];	
	
	#if _USE_LFN 
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	fno.lfname = lfn; 
	fno.lfsize = sizeof(lfn); 
	#endif 	
	res = f_opendir(&dir, path);
	if (res == FR_OK) 
	{ 
		i = strlen(path); 
		while(1)
		{ 
			res = f_readdir(&dir, &fno); 
			if (res != FR_OK || fno.fname[0] == 0) break; 
			#if _USE_LFN 
			fn = *fno.lfname ? fno.lfname : fno.fname; 
			#else 
			fn = fno.fname; 
			#endif 
			if (*fn == '.') continue;	
			if (fno.fattrib & AM_DIR) 
			{ 
				sprintf(&path[i], "/%s", fn); 							
				res = scan_files(path);											
				if (res != FR_OK) 
					break; 																	
				path[i] = 0; 
			} 
			else 
			{ 
				//printf("%s/%s\r\n", path, fn);
				if(strstr(fn,".nes")||strstr(fn,".NES"))
				{
					if (strlen(path)+strlen(fn)<FILE_NAME_LEN)
					{
						sprintf(file_name, "%s/%s", path, fn);						
						memcpy(filelist[file_num],file_name,strlen(file_name));	
						memcpy(showlist[file_num],fn,strlen(fn));						
						file_num++;
						if(file_num>=20)break;
					}
				}//if(strstr(fn,".nes")||strstr(fn,".NES"))
			 }//else
		 } //while
	} 
	return res; 
}

static void LCD_showindex(uint8_t index)
{
	ILI9341_Clear(10,10,20,300,macBLACK);
	ILI9341_DrawCircle(20,index*20,5,macYELLOW,1);
}
void LCD_showlist(void)
{
	uint8_t i=0;
	//ILI9341_Clear(10,10,220,300,macBLACK);
	LCD_showindex(sel_index);
	for(i=0;i<file_num;++i)
	{
		ILI9341_DispString_EN_CH(30,i*20+12,showlist[i],macBLACK,macBLUE);
	}
}

void LCD_selectindex(void)
{
	uint8_t joykey=0;
	while(1)
	{
		joykey=JOYPAD_Read();
		switch(joykey)
		{
			case 0x20://"上"
				sel_index++;
				if(sel_index>file_num)
					sel_index=1;
				LCD_showindex(sel_index);
				break;
		  case 0x10://"下"
				sel_index--;
				if(sel_index==0)
					sel_index=file_num;
				LCD_showindex(sel_index);
				break;
			case 0x04://"选择"
			case 0x08://"开始"
					return ;
				break;
			default:
				break;
		}
		joykey=0;
		Delay_ms(500);
	}
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{	
	system_clock_set(16);//128M
	//EXSRAM 初始化
	FSMC_SRAM_Init();
	//LCD 初始化
	LCD_Init();  
	//JOYSTICK 初始化
	JOYPAD_Init();
	//LED 初始化
	LED_GPIO_Config();
	//KEY 初始化
	Key_GPIO_Config();
	//mount EXFlash FatFS
  f_mount(&fs,"0:",1);	

	//USART1 初始化	
	USARTx_Config();
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部SRAM内存池 
	//SYSTick初始化
	SysTick_Init();
	//VS1053 初始化	
	VS_Init();
	//开液晶背光
	LCD_BK_EN;
	
	file_num = 0;
	sel_index=1;
//	printf("NES Game Play\n");
	scan_files(path);
	//printf("NES file namber:%d\n",file_num);
		
	while (file_num)
  {	
		LCD_showlist();
		LCD_selectindex();
		ILI9341_Clear(0,0,240,320,macBLACK);
		nes_load(filelist[sel_index-1]);
		ILI9341_Clear(0,0,240,320,macBLACK);
//		nes_load("0:/SuperMarie.nes");
  }
	ILI9341_Clear(0,0,240,320,macBLACK);
	ILI9341_DispString_EN_CH(60,10,"无法找到NES游戏文件",macBLACK,macBLUE);
	while(1);
}



/* ------------------------------------------end of file---------------------------------------- */

