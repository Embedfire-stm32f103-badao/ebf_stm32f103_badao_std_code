#include "..\User\bsp\lcd\bsp_ili9341_lcd.h"
#include "..\User\bsp\lcd\bsp_xpt2046_lcd.h"
#include <stdio.h> 
#include <string.h>



/******************************* 声明 XPT2046 相关的静态函数 ***************************/
static void                   XPT2046_GPIO_SPI_Config               ( void );

static void                   XPT2046_DelayUS                       ( __IO uint32_t ulCount );
static void                   XPT2046_WriteCMD                      ( uint8_t ucCmd );
static uint16_t               XPT2046_ReadCMD                       ( void );

/**
  * @brief  XPT2046 初始化函数
  * @param  无
  * @retval 无
  */	
void XPT2046_Init ( void )
{
	XPT2046_GPIO_SPI_Config ();		
}



/**
  * @brief  配置 XPT2046 的模拟SPI
  * @param  无
  * @retval 无
  */	
static void XPT2046_GPIO_SPI_Config ( void ) 
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
	

  /* 开启GPIO时钟 */
  RCC_APB2PeriphClockCmd ( macXPT2046_SPI_GPIO_CLK | macXPT2046_INT_GPIO_CLK, ENABLE );

  /* 模拟SPI GPIO初始化 */          
  GPIO_InitStructure.GPIO_Pin=macXPT2046_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(macXPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_MOSI_PIN;
  GPIO_Init(macXPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_MISO_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
  GPIO_Init(macXPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = macXPT2046_SPI_CS_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
  GPIO_Init(macXPT2046_SPI_CS_PORT, &GPIO_InitStructure); 
   
	GPIO_InitStructure.GPIO_Pin = macXPT2046_INT_GPIO_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(macXPT2046_INT_GPIO_PORT, &GPIO_InitStructure); 
  /* 拉低片选，选择XPT2046 */
   macXPT2046_CS_ENABLE();
}




/**
  * @brief  用于 XPT2046 的简单微秒级延时函数
  * @param  nCount ：延时计数值，单位为微妙
  * @retval 无
  */	
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;


	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  
	      
		while ( uc -- );     //延1微秒	

	}
	
}



/**
  * @brief  XPT2046 的写入命令
  * @param  ucCmd ：命令
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 无
  */
static void XPT2046_WriteCMD ( uint8_t ucCmd ) 
{
	uint8_t i;


	macXPT2046_MOSI_0();
	
	macXPT2046_CLK_LOW();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? macXPT2046_MOSI_1() : macXPT2046_MOSI_0();
		
	  XPT2046_DelayUS ( 5 );
		
		macXPT2046_CLK_HIGH();

	  XPT2046_DelayUS ( 5 );

		macXPT2046_CLK_LOW();
	}
	
}


/**
  * @brief  XPT2046 的读取命令
  * @param  无
  * @retval 读取到的数据
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;
	


	macXPT2046_MOSI_0();

	macXPT2046_CLK_HIGH();

	for ( i=0;i<12;i++ ) 
	{
		macXPT2046_CLK_LOW();    
	
		usTemp = macXPT2046_MISO();
		
		usBuf |= usTemp << ( 11 - i );
	
		macXPT2046_CLK_HIGH();
		
	}
	
	return usBuf;

}

#define SAMP_CNT 4
#define SAMP_CNT_DIV2 2
/*
*********************************************************************************************************
*	函 数 名: XPT2046_ReadAdc_Fliter
*	功能说明: 选择一个模拟通道，启动ADC，并返回ADC采样结果
*	形    参：_ucCh = 0x90 表示Y通道； 0xd0 表示X通道
*	返 回 值: 滤波后的12位ADC值
*********************************************************************************************************
*/
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel)
{
  u8 i, j, min;
  u16 temp;
  u16 tempXY[SAMP_CNT];
  
  static uint16_t adc_x = 0,adc_y = 0;

  if(macXPT2046_INT_Read()== macXPT2046_INT_ActiveLevel) 
  {
    for(i = 0;i < SAMP_CNT;i++)
    {
      XPT2046_WriteCMD(channel);
      tempXY[i] = XPT2046_ReadCMD();     
    }
    // 降序排列
    for(i=0; i<SAMP_CNT-1; i++)
    {
      min=i;
      for (j=i+1; j<SAMP_CNT; j++)
      {
        if (tempXY[min] > tempXY[j]) min=j;
      }
      temp = tempXY[i];
      tempXY[i] = tempXY[min];
      tempXY[min] = temp;
    }
   
    // 设定阈值
    if((tempXY[SAMP_CNT_DIV2]-tempXY[SAMP_CNT_DIV2-1]) > 5)
    {
      /* 若两个中间值相差太远，则舍弃这个新数据，返回上一次的触摸数据*/
      if(channel == macXPT2046_CHANNEL_Y )
        return adc_x; //x通道
      else
        return adc_y; //y通道
    
    }
    
   // 求中间值的均值   
   if(channel == macXPT2046_CHANNEL_Y)
   {
     adc_x = (tempXY[SAMP_CNT_DIV2]+tempXY[SAMP_CNT_DIV2-1]) / 2;
      return adc_x;
    }      
    else
    {
      adc_y = (tempXY[SAMP_CNT_DIV2]+tempXY[SAMP_CNT_DIV2-1]) / 2;
      return adc_y;
    }
       
  }
  else
  {
    return 0;   //没有触摸，返回0
  }
}
















