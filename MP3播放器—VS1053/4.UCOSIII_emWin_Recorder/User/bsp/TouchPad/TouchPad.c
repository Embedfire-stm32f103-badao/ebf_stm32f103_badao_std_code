#include "..\User\bsp\TouchPad\TouchPad.h"
#include "includes.h"

#define TPAD_ARR_MAX_VAL 	0XFFFF	//最大的ARR值

volatile  uint16_t tpad_default_val=0;//空载的时候(没有手按下),计数器需要的时间

void TPAD_Reset(void);
uint16_t  TPAD_Get_Val(void);
uint16_t  TPAD_Get_MaxVal(uint8_t n);
void TIMx_CHx_Cap_Init(uint16_t arr,uint16_t psc); 

//初始化触摸按键
//获得空载的时候触摸按键的取值.
//返回值:0,初始化成功;1,初始化失败
uint8_t TPAD_Init(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	TIMx_CHx_Cap_Init(TPAD_ARR_MAX_VAL,SystemCoreClock/1000000-1);//以1Mhz的频率计数 
	for(i=0;i<10;i++)//连续读取10次
	{				 
		buf[i]=TPAD_Get_Val();
		bsp_DelayUS(10*1000);	    
	}				    
	for(i=0;i<9;i++)//排序
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//取中间的8个数据进行平均
	tpad_default_val=temp/6;
	//printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
	return 0;		     	    					   
}


//复位一次
void TPAD_Reset(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	
	//设置GPIOA.1为推挽使出
	macPAD_TIM_GPIO_APBxClock_FUN ( macPAD_TIM_GPIO_CLK, ENABLE );	 //使能PA端口时钟
 	GPIO_InitStructure.GPIO_Pin = macPAD_TIM_CH_PIN;				 //PA1 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(macPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
 	GPIO_ResetBits ( macPAD_TIM_CH_PORT, macPAD_TIM_CH_PIN );						 //PA.1输出0,放电

	bsp_DelayUS(5*1000);

	TIM_SetCounter(macPAD_TIMx,0);		//归0
	TIM_ClearITPendingBit(macPAD_TIMx, TIM_IT_CC2|TIM_IT_Update); //清除中断标志
	
	//设置GPIOA.1为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //浮空输入
	GPIO_Init(macPAD_TIM_CH_PORT, &GPIO_InitStructure);

}


//得到定时器捕获值
//如果超时,则直接返回定时器的计数值.
uint16_t TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while(TIM_GetFlagStatus ( macPAD_TIMx, macPAD_TIM_IT_CCx ) == RESET)//等待捕获上升沿
	{
		if(TIM_GetCounter(macPAD_TIMx)>TPAD_ARR_MAX_VAL-500)return TIM_GetCounter(macPAD_TIMx);//超时了,直接返回CNT的值
	};	
	return macPAD_TIM_GetCaptureX ( macPAD_TIMx );	  
} 	 


//读取n次,取最大值
//n：连续获取的次数
//返回值：n次读数里面读到的最大读数值
uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//得到一次值
		if(temp>res)res=temp;
	};
	return res;
}  


//扫描触摸按键
//mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
//返回值:0,没有按下;1,有按下;										  
#define TPAD_GATE_VAL 	80	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
uint8_t TPAD_Scan(uint8_t mode)
{
	static uint8_t keyen=0;	//0,可以开始检测;>0,还不能开始检测	 
	uint8_t res=0;
	uint8_t sample=3;		//默认采样次数为3次	 
	uint16_t rval;
	if(mode)
	{
		sample=6;	//支持连按的时候，设置采样次数为6次
		keyen=0;	//支持连按	  
	}
	rval=TPAD_Get_MaxVal(sample); 
//	printf("scan_rval=%d\n",rval);
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//大于tpad_default_val+TPAD_GATE_VAL,有效
	{						
  		rval=TPAD_Get_MaxVal(sample);		 
		if((keyen==0)&&(rval>(tpad_default_val+TPAD_GATE_VAL)))//大于tpad_default_val+TPAD_GATE_VAL,有效
		{
			res=1;
		}	   
		//printf("r:%d\r\n",rval);			     	    					   
		keyen=5;				//至少要再过5次之后才能按键有效   
	}else if(keyen>2)keyen=2; 	//如果检测到按键松开,则直接将次数将为2,以提高响应速度
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 


//定时器2通道2输入捕获配置
void TIMx_CHx_Cap_Init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	

	//设置GPIOA.1为浮空输入
	macPAD_TIM_GPIO_APBxClock_FUN ( macPAD_TIM_GPIO_CLK, ENABLE );	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = macPAD_TIM_CH_PIN;				 //PA1 端口配置
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //浮空输入
	GPIO_Init(macPAD_TIM_CH_PORT, &GPIO_InitStructure);	 //设置为浮空输入
	
	//初始化TIM5
	macPAD_TIM_APBxClock_FUN ( macPAD_TIM_CLK, ENABLE );	 //使能TIM5时钟
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(macPAD_TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	//初始化通道2 
	TIM_ICInitStructure.TIM_Channel = macPAD_TIM_Channel_X; //CC1S=01 	选择输入端 IC2映射到TI5上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
	TIM_ICInit(TIM5, &TIM_ICInitStructure);//初始化I5 IC2

	TIM_Cmd ( macPAD_TIMx, ENABLE ); 	//使能定时器5
		
}


/*********************************************END OF FILE**********************/
