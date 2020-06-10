#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "stm32f10x.h"
 
/******************** PAD 引脚配置参数定义，只限TIM2、3、4或5**************************/
#define             macPAD_TIMx                                TIM5
#define             macPAD_TIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macPAD_TIM_CLK                             RCC_APB1Periph_TIM5

#define             macPAD_TIM_Channel_X                       TIM_Channel_2
#define             macPAD_TIM_IT_CCx                          TIM_IT_CC2
#define             macPAD_TIM_GetCaptureX                     TIM_GetCapture2

#define             macPAD_TIM_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macPAD_TIM_GPIO_CLK                        RCC_APB2Periph_GPIOA
#define             macPAD_TIM_CH_PORT                         GPIOA
#define             macPAD_TIM_CH_PIN                          GPIO_Pin_1

/************************** PAD 函数声明********************************/
uint8_t   TPAD_Init(void);
uint8_t   TPAD_Scan(uint8_t mode);

#endif /* __TOUCHPAD_H_ */
