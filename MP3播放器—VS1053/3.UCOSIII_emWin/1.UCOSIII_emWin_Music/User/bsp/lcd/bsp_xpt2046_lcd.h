#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H


#include "stm32f10x.h"



/******************************* XPT2046 触摸屏中断引脚定义 ***************************/
#define             macXPT2046_INT_GPIO_CLK                        RCC_APB2Periph_GPIOF  
#define             macXPT2046_INT_GPIO_PORT                       GPIOF
#define             macXPT2046_INT_GPIO_PIN                        GPIO_Pin_9
#define             macXPT2046_INT_Read()                          GPIO_ReadInputDataBit ( macXPT2046_INT_GPIO_PORT, macXPT2046_INT_GPIO_PIN )
#define 						macXPT2046_INT_ActiveLevel											0


/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             macXPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG

#define             macXPT2046_SPI_CS_PIN		                        GPIO_Pin_10
#define             macXPT2046_SPI_CS_PORT		                      GPIOF

#define	            macXPT2046_SPI_CLK_PIN	                        GPIO_Pin_7
#define             macXPT2046_SPI_CLK_PORT	                        GPIOG

#define	            macXPT2046_SPI_MOSI_PIN	                        GPIO_Pin_11
#define	            macXPT2046_SPI_MOSI_PORT	                      GPIOF

#define	            macXPT2046_SPI_MISO_PIN	                        GPIO_Pin_6
#define	            macXPT2046_SPI_MISO_PORT	                      GPIOF


#define             macXPT2046_CS_ENABLE()                          GPIO_ResetBits ( macXPT2046_SPI_CS_PORT, macXPT2046_SPI_CS_PIN )    
#define             macXPT2046_CS_DISABLE()                         GPIO_SetBits   ( macXPT2046_SPI_CS_PORT, macXPT2046_SPI_CS_PIN )  

#define             macXPT2046_CLK_HIGH()                           GPIO_SetBits   ( macXPT2046_SPI_CLK_PORT, macXPT2046_SPI_CLK_PIN )    
#define             macXPT2046_CLK_LOW()                            GPIO_ResetBits ( macXPT2046_SPI_CLK_PORT, macXPT2046_SPI_CLK_PIN ) 

#define             macXPT2046_MOSI_1()                             GPIO_SetBits   ( macXPT2046_SPI_MOSI_PORT, macXPT2046_SPI_MOSI_PIN ) 
#define             macXPT2046_MOSI_0()                             GPIO_ResetBits ( macXPT2046_SPI_MOSI_PORT, macXPT2046_SPI_MOSI_PIN )

#define             macXPT2046_MISO()                               GPIO_ReadInputDataBit ( macXPT2046_SPI_MISO_PORT, macXPT2046_SPI_MISO_PIN )



/******************************* XPT2046 触摸屏参数定义 ***************************/
#define	            macXPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            macXPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字




/******************************** XPT2046 触摸屏函数声明 **********************************/
void     XPT2046_Init( void );
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel); 


#endif /* __BSP_TOUCH_H */

