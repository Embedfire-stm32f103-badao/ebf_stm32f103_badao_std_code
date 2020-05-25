<<<<<<< HEAD:34-TIMâ€”è¾“å…¥æ•è·ä¹‹ç”µå®¹æŒ‰é”®æ£€æµ‹-V2/User/TPad/bsp_tpad.h
#ifndef __BSP_TPAD_H
#define __BSP_TPAD_H


#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"

/************Í¨ÓÃ¶¨Ê±Æ÷TIM²ÎÊı¶¨Òå£¬Ö»ÏŞTIM2¡¢3¡¢4¡¢5************/
// µ±Ê¹ÓÃ²»Í¬µÄ¶¨Ê±Æ÷µÄÊ±ºò£¬¶ÔÓ¦µÄGPIOÊÇ²»Ò»ÑùµÄ£¬ÕâµãÒª×¢Òâ
// ÎÒÃÇÕâÀïÄ¬ÈÏÊ¹ÓÃTIM5

#define            TPAD_TIM                   TIM5
#define            TPAD_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            TPAD_TIM_CLK               RCC_APB1Periph_TIM5
#define            TPAD_TIM_Period            0XFFFF
#define            TPAD_TIM_Prescaler         71

// TIM ÊäÈë²¶»ñÍ¨µÀGPIOÏà¹Øºê¶¨Òå
#define            TPAD_TIM_CH_GPIO_CLK       RCC_APB2Periph_GPIOA
#define            TPAD_TIM_CH_PORT           GPIOA
#define            TPAD_TIM_CH_PIN            GPIO_Pin_1
#define            TPAD_TIM_CHANNEL_x         TIM_Channel_2

// ÖĞ¶ÏÏà¹Øºê¶¨Òå
#define            TPAD_TIM_IT_CCx            TIM_IT_CC2
#define            TPAD_TIM_IRQ               TIM5_IRQn
#define            TPAD_TIM_INT_FUN           TIM5_IRQHandler

// »ñÈ¡²¶»ñ¼Ä´æÆ÷Öµº¯Êıºê¶¨Òå
#define            TPAD_TIM_GetCapturex_FUN                 TIM_GetCapture2
// ²¶»ñĞÅºÅ¼«ĞÔº¯Êıºê¶¨Òå
#define            TPAD_TIM_OCxPolarityConfig_FUN           TIM_OC2PolarityConfig

// µçÈİ°´¼ü±»°´ÏÂµÄÊ±ºòÃÅÏŞÖµ£¬ĞèÒª¸ù¾İ²»Í¬µÄÓ²¼şÊµ¼Ê²âÊÔ£¬
// ¼õĞ¡Õâ¸öÃÅÏŞÖµ¿ÉÒÔÌá¸ßÏìÓ¦ËÙ¶È
#define            TPAD_GATE_VAL              40
// µçÈİ°´¼ü¿ÕÔØµÄÊ±ºòµÄ×î´óºÍ×îĞ¡µÄ³äµçÊ±¼ä£¬²»Í¬µÄÓ²¼ş²»Ò»Ñù
#define            TPAD_DEFAULT_VAL_MIN       75
#define            TPAD_DEFAULT_VAL_MAX       200

#define            TPAD_ON                     1
#define            TPAD_OFF                    0

/**************************º¯ÊıÉùÃ÷********************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(void);

#endif	/* __BSP_TPAD_H */


=======
#ifndef __BSP_TPAD_H
#define __BSP_TPAD_H


#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"

/************Í¨ÓÃ¶¨Ê±Æ÷TIM²ÎÊı¶¨Òå£¬Ö»ÏŞTIM2¡¢3¡¢4¡¢5************/
// µ±Ê¹ÓÃ²»Í¬µÄ¶¨Ê±Æ÷µÄÊ±ºò£¬¶ÔÓ¦µÄGPIOÊÇ²»Ò»ÑùµÄ£¬ÕâµãÒª×¢Òâ
// ÎÒÃÇÕâÀïÄ¬ÈÏÊ¹ÓÃTIM5

#define            TPAD_TIM                   TIM5
#define            TPAD_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            TPAD_TIM_CLK               RCC_APB1Periph_TIM5
#define            TPAD_TIM_Period            0XFFFF
#define            TPAD_TIM_Prescaler         71

// TIM ÊäÈë²¶»ñÍ¨µÀGPIOÏà¹Øºê¶¨Òå
#define            TPAD_TIM_CH_GPIO_CLK       RCC_APB2Periph_GPIOA
#define            TPAD_TIM_CH_PORT           GPIOA
#define            TPAD_TIM_CH_PIN            GPIO_Pin_1
#define            TPAD_TIM_CHANNEL_x         TIM_Channel_2

// ÖĞ¶ÏÏà¹Øºê¶¨Òå
#define            TPAD_TIM_IT_CCx            TIM_IT_CC2
#define            TPAD_TIM_IRQ               TIM5_IRQn
#define            TPAD_TIM_INT_FUN           TIM5_IRQHandler

// »ñÈ¡²¶»ñ¼Ä´æÆ÷Öµº¯Êıºê¶¨Òå
#define            TPAD_TIM_GetCapturex_FUN                 TIM_GetCapture2
// ²¶»ñĞÅºÅ¼«ĞÔº¯Êıºê¶¨Òå
#define            TPAD_TIM_OCxPolarityConfig_FUN           TIM_OC2PolarityConfig

// µçÈİ°´¼ü±»°´ÏÂµÄÊ±ºòÃÅÏŞÖµ£¬ĞèÒª¸ù¾İ²»Í¬µÄÓ²¼şÊµ¼Ê²âÊÔ£¬
// ¼õĞ¡Õâ¸öÃÅÏŞÖµ¿ÉÒÔÌá¸ßÏìÓ¦ËÙ¶È
#define            TPAD_GATE_VAL              100
// µçÈİ°´¼ü¿ÕÔØµÄÊ±ºòµÄ×î´óºÍ×îĞ¡µÄ³äµçÊ±¼ä£¬²»Í¬µÄÓ²¼ş²»Ò»Ñù£¬°ÔµÀV1ÎÈ¶¨ÔÚ218
#define            TPAD_DEFAULT_VAL_MIN       210
#define            TPAD_DEFAULT_VAL_MAX       230

#define            TPAD_ON                     1
#define            TPAD_OFF                    0

/**************************º¯ÊıÉùÃ÷********************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(void);

#endif	/* __BSP_TPAD_H */


>>>>>>> f24a7e019daefba4bdcb9221812d099b96547227:34-TIMâ€”è¾“å…¥æ•è·ä¹‹ç”µå®¹æŒ‰é”®æ£€æµ‹-V1/User/TPad/bsp_tpad.h
