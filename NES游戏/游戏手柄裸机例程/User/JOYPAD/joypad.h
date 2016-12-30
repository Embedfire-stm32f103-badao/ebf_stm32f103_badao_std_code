#ifndef __JOYPAD_H__
#define __JOYPAD_H__	 
 
#include "stm32f10x.h"

//手柄连接引脚
#define macJOYPAD_CLK_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define macJOYPAD_CLK_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define macJOYPAD_CLK_GPIO_PIN		  GPIO_Pin_3			          /* 连接到SCL时钟线的GPIO */

#define macJOYPAD_LAT_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define macJOYPAD_LAT_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define macJOYPAD_LAT_GPIO_PIN		  GPIO_Pin_2			          /* 连接到SCL时钟线的GPIO */

#define macJOYPAD_DAT_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define macJOYPAD_DAT_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define macJOYPAD_DAT_GPIO_PIN		  GPIO_Pin_8			          /* 连接到SCL时钟线的GPIO */

#define JOYPAD_CLK_HIGH				GPIO_SetBits(macJOYPAD_CLK_GPIO_PORT,macJOYPAD_CLK_GPIO_PIN)
#define JOYPAD_CLK_LOW			GPIO_ResetBits(macJOYPAD_CLK_GPIO_PORT,macJOYPAD_CLK_GPIO_PIN)

#define JOYPAD_LAT_HIGH				GPIO_SetBits(macJOYPAD_LAT_GPIO_PORT,macJOYPAD_LAT_GPIO_PIN)
#define JOYPAD_LAT_LOW			GPIO_ResetBits(macJOYPAD_LAT_GPIO_PORT,macJOYPAD_LAT_GPIO_PIN)

#define macJOYPAD_DAT_READ	GPIO_ReadInputDataBit(macJOYPAD_DAT_GPIO_PORT,macJOYPAD_DAT_GPIO_PIN)


void JOYPAD_Init(void);			//初始化	
void JOYPAD_Delay(uint16_t t);		//延时
uint8_t JOYPAD_Read(void);			//读取键值	 				    
#endif //end of __JOYPAD_H__

