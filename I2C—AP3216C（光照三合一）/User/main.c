/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ap3216c测试，测试信息通过USART1打印在电脑的超级终端
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "./ap3216c/ap3216c.h"
#include "bsp_SysTick.h"  
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include <string.h>


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /**/
  float ALS;
  uint16_t PS;
  uint16_t IR;
  uint8_t IntStatus;
  
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
  
  printf("\r\n 欢迎使用野火 STM32 F429 开发板。\r\n");	
  printf("\r\n 这是一个三合一光照传感器测试例程 \r\n");
  
  printf(" 芯片初始化中.....\n");
  /* 初始化 光照传感器 */
  ap3216c_init();

  while(1)    
  {
    IntStatus = ap3216c_get_IntStatus();    // 先读状态位，读ADC数据位会清除状态位（默认设置）
    ALS = ap3216c_read_ambient_light();
    PS = ap3216c_read_ps_data();
    IR = ap3216c_read_ir_data();

    printf("\n光照强度是：%.2fLux\n红外强度是：%d\n", ALS, IR);

    if (PS == 55555)    // IR 太强 PS 数据无效
      printf("IR 太强 PS 数据无效\n");
    else
    {
      printf("接近距离是：%d\n", PS & 0x3FF);
    }
    
    if (AP_INT_Read() == 0)
      printf("有中断产生\n");
    
    if ((PS >> 15) & 1)
      printf("物体接近\n");
    else
      printf("物体远离\n");
    
    if (IntStatus & 0x1)
      printf("ALS 产生中断\n");
    
    if (IntStatus >> 1 & 0x1)
      printf("PS 产生中断\n");
    
    SysTick_Delay_Ms(400);
  }
}


/*********************************************END OF FILE**********************/
