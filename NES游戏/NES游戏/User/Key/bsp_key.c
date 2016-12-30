/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h" 

/// 不精确的延时
static void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口（PA）的时钟*/
	RCC_APB2PeriphClockCmd(macKEY1_GPIO_CLK|macKEY2_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = macKEY1_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	
	GPIO_Init(macKEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = macKEY2_GPIO_PIN; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	
	GPIO_Init(macKEY2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief   检测是否有按键按下
  * @param   具体的端口和端口位
  *		@arg GPIOx: x可以是（A...G） 
  *		@arg GPIO_PIN 可以是GPIO_PIN_x（x可以是1...16）
  *   @arg Down_state:按键按下时的电平，1为高电平，0为低电平
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
uint8_t Key1_Scan(void)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(macKEY1_GPIO_PORT,macKEY1_GPIO_PIN) == 1 ) 
	{	   
		/*延时消抖*/
		Key_Delay(5000);		
		if(GPIO_ReadInputDataBit(macKEY1_GPIO_PORT,macKEY1_GPIO_PIN) == 1 )  
		{	 
			/*等待按键释放 */
			while(GPIO_ReadInputDataBit(macKEY1_GPIO_PORT,macKEY1_GPIO_PIN) == 1);   
			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}

uint8_t Key2_Scan(void)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(macKEY2_GPIO_PORT,macKEY2_GPIO_PIN) == 1 ) 
	{	   
		/*延时消抖*/
		Key_Delay(5000);		
		if(GPIO_ReadInputDataBit(macKEY2_GPIO_PORT,macKEY2_GPIO_PIN) == 1 )  
		{	 
			/*等待按键释放 */
			while(GPIO_ReadInputDataBit(macKEY2_GPIO_PORT,macKEY2_GPIO_PIN) == 1);   
			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/
