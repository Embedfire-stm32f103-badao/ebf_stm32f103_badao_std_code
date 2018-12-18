#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"


#define               macKEY1_GPIO_CLK                      RCC_APB2Periph_GPIOA
#define               macKEY1_GPIO_PORT    	                GPIOA		   
#define               macKEY1_GPIO_PIN		                  GPIO_Pin_0

#define               macKEY2_GPIO_CLK                      RCC_APB2Periph_GPIOC
#define               macKEY2_GPIO_PORT    	                GPIOC			   
#define               macKEY2_GPIO_PIN		                  GPIO_Pin_13


/*******
 *按键按下标置
 KEY_ON 1
 KEY_OFF 0
********/
#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
uint8_t Key1_Scan(void);
uint8_t Key2_Scan(void);
#endif /* __LED_H */

