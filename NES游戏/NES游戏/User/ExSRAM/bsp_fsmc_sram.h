/**
  ******************************************************************************
  * @file FSMC/SRAM/fsmc_sram.h 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Header for fsmc_sram.c file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// 64MB =0X0400 0000
// BANK1 0X6000 0000 ~ 0X63FF FFFF
// BANK2 0X6400 0000 ~ 0X67FF FFFF
// BANK3 0X6800 0000 ~ 0X6CFF FFFF
// BANK4 0X6C00 0000 ~ 0X6FFF FFFF
// #define Bank1_SRAM1_ADDR    ((uint32_t)0x60000000)
// #define Bank1_SRAM2_ADDR    ((uint32_t)0x64000000)
#define Bank1_SRAM3_ADDR    ((uint32_t)0x68000000)
// #define Bank1_SRAM4_ADDR    ((uint32_t)0x6C000000)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FSMC_SRAM_Init(void);

#endif /* __FSMC_SRAM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


