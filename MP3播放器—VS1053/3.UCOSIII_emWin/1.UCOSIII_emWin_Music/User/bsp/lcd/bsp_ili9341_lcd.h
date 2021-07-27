#ifndef      __BSP_ILI9341_LCD_H
#define	     __BSP_ILI9341_LCD_H
#include "stm32f10x.h"
/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6C00 0000
RAM基地址 = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/

/******************************* ILI9341 显示屏的 FSMC 参数定义 ***************************/
#define      macFSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x6C000000 )	     //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define      macFSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x6D000000 )      //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址  

//方便emWin使用
#define 		macFSMC_ILI9341_REG								*(__IO uint16_t *)(0x6C000000)
#define 		macFSMC_ILI9341_RAM								*(__IO uint16_t *)(0x6D000000)

#define      macFSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM4



/******************************* ILI9341 显示屏8080通讯引脚定义 ***************************/
#define      macILI9341_CS_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_CS_CLK                RCC_APB2Periph_GPIOG   
#define      macILI9341_CS_PORT               GPIOG
#define      macILI9341_CS_PIN                GPIO_Pin_12

#define      macILI9341_DC_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_DC_CLK                RCC_APB2Periph_GPIOE   
#define      macILI9341_DC_PORT               GPIOE
#define      macILI9341_DC_PIN                GPIO_Pin_2

#define      macILI9341_RST_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      macILI9341_RST_CLK               RCC_APB2Periph_GPIOG 
#define      macILI9341_RST_PORT              GPIOG
#define      macILI9341_RST_PIN               GPIO_Pin_11

#define      macILI9341_BK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_BK_CLK                RCC_APB2Periph_GPIOG
#define      macILI9341_BK_PORT               GPIOG
#define      macILI9341_BK_PIN                GPIO_Pin_6



/*************************************** 调试预用 ******************************************/
#define      macDEBUG_DELAY()                



/***************** ILI934 显示屏全屏默认（扫描方向为1时）最大宽度和最大高度 *********************/
#define      macILI9341_Default_Max_Width		  240     //起始点的X坐标
#define      macILI9341_Default_Max_Heigth		320     //起始点的Y坐标


/******************************* 定义 ILI934 常用命令 ********************************/
#define      macCMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define      macCMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define      macCMD_SetPixel		 		          0x2C	     //填充像素


/********************************** 声明 ILI934 函数 ***************************************/
#define			LCD_BK_EN		  GPIO_ResetBits(macILI9341_BK_PORT, macILI9341_BK_PIN)
#define			LCD_BK_DIS		GPIO_SetBits(macILI9341_BK_PORT, macILI9341_BK_PIN)

void ILI9341_Init ( void );
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
#endif /* __BSP_ILI9341_ILI9341_H */
