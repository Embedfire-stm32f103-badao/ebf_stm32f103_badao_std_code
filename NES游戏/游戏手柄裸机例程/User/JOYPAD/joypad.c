#include  "joypad.h"

//初始化手柄接口.	 
void JOYPAD_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(macJOYPAD_CLK_GPIO_CLK|macJOYPAD_LAT_GPIO_CLK|macJOYPAD_DAT_GPIO_CLK, ENABLE);	 //使能PB,PD端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = macJOYPAD_CLK_GPIO_PIN;				 //
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(macJOYPAD_CLK_GPIO_PORT, &GPIO_InitStructure); //初始化GPIO
 	GPIO_SetBits(macJOYPAD_CLK_GPIO_PORT,macJOYPAD_CLK_GPIO_PIN);	//上拉

 	GPIO_InitStructure.GPIO_Pin = macJOYPAD_LAT_GPIO_PIN;				 //
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(macJOYPAD_LAT_GPIO_PORT, &GPIO_InitStructure); //初始化GPIO
 	GPIO_SetBits(macJOYPAD_LAT_GPIO_PORT,macJOYPAD_LAT_GPIO_PIN);	//上拉
	
	
 	GPIO_InitStructure.GPIO_Pin  = macJOYPAD_DAT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 	GPIO_Init(macJOYPAD_DAT_GPIO_PORT, &GPIO_InitStructure); //初始化GPIO
	GPIO_SetBits(macJOYPAD_DAT_GPIO_PORT,macJOYPAD_DAT_GPIO_PIN);	//上拉

}

//手柄延迟函数
//t:要延迟的时间
void JOYPAD_Delay(uint16_t t)
{
	while(t--);
}
//读取手柄按键值.
//FC手柄数据输出格式:
//每给一个脉冲,输出一位数据,输出顺序:
//A->B->SELECT->START->UP->DOWN->LEFT->RIGHT.
//总共8位,对于有C按钮的手柄,按下C其实就等于A+B同时按下.
//按下是1,松开是0.
//返回值:
//[7]:右
//[6]:左
//[5]:下
//[4]:上
//[3]:Start
//[2]:Select
//[1]:B
//[0]:A
uint8_t JOYPAD_Read(void)
{
	__IO uint8_t temp=0;
 	uint8_t t;
	JOYPAD_LAT_HIGH;	//锁存当前状态
 	JOYPAD_Delay(80);
	JOYPAD_LAT_LOW; 
	for(t=0;t<8;t++)
	{
		temp>>=1;	 
		if(macJOYPAD_DAT_READ==0)temp|=0x80;//LOAD之后，就得到第一个数据
		JOYPAD_CLK_HIGH;			   	//每给一次脉冲，收到一个数据
		JOYPAD_Delay(80);
		JOYPAD_CLK_LOW;	
		JOYPAD_Delay(80); 
	}
	return temp;
}


