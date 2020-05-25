<<<<<<< HEAD:34-TIMâ€”è¾“å…¥æ•èŽ·ä¹‹ç”µå®¹æŒ‰é”®æ£€æµ‹-V2/User/TPad/bsp_tpad.c

#include "bsp_tpad.h"

// µçÈÝ°´¼ü¿ÕÔØµÄÊ±ºò³äµçÊ±¼ä
uint16_t tpad_default_val;

static void TPAD_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ÊäÈë²¶»ñÍ¨µÀ GPIO ³õÊ¼»¯
	RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  TPAD_TIM_CH_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);	
}


///*
// * ×¢Òâ£ºTIM_TimeBaseInitTypeDef½á¹¹ÌåÀïÃæÓÐ5¸ö³ÉÔ±£¬TIM6ºÍTIM7µÄ¼Ä´æÆ÷ÀïÃæÖ»ÓÐ
// * TIM_PrescalerºÍTIM_Period£¬ËùÒÔÊ¹ÓÃTIM6ºÍTIM7µÄÊ±ºòÖ»Ðè³õÊ¼»¯ÕâÁ½¸ö³ÉÔ±¼´¿É£¬
// * ÁíÍâÈý¸ö³ÉÔ±ÊÇÍ¨ÓÃ¶¨Ê±Æ÷ºÍ¸ß¼¶¶¨Ê±Æ÷²ÅÓÐ.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ¶¼ÓÐ
// *	TIM_CounterMode			     TIMx,x[6,7]Ã»ÓÐ£¬ÆäËû¶¼ÓÐ
// *  TIM_Period               ¶¼ÓÐ
// *  TIM_ClockDivision        TIMx,x[6,7]Ã»ÓÐ£¬ÆäËû¶¼ÓÐ
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]²ÅÓÐ
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWMÐÅºÅ ÖÜÆÚºÍÕ¼¿Õ±ÈµÄ¼ÆËã--------------- */
// ARR £º×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷µÄÖµ
// CLK_cnt£º¼ÆÊýÆ÷µÄÊ±ÖÓ£¬µÈÓÚ Fck_int / (psc+1) = 72M/(psc+1)
// PWM ÐÅºÅµÄÖÜÆÚ T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// Õ¼¿Õ±ÈP=CCR/(ARR+1)

static void TPAD_TIM_Mode_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	// ¿ªÆô¶¨Ê±Æ÷Ê±ÖÓ,¼´ÄÚ²¿Ê±ÖÓCK_INT=72M
	TPAD_TIM_APBxClock_FUN(TPAD_TIM_CLK,ENABLE);

/*--------------------Ê±»ù½á¹¹Ìå³õÊ¼»¯-------------------------*/	
	// ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷µÄÖµ£¬ÀÛ¼ÆTIM_Period+1¸öÆµÂÊºó²úÉúÒ»¸ö¸üÐÂ»òÕßÖÐ¶Ï
	TIM_TimeBaseStructure.TIM_Period=TPAD_TIM_Period;	
	// Çý¶¯CNT¼ÆÊýÆ÷µÄÊ±ÖÓ = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= TPAD_TIM_Prescaler;	
	// Ê±ÖÓ·ÖÆµÒò×Ó £¬ÅäÖÃËÀÇøÊ±¼äÊ±ÐèÒªÓÃµ½
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ¼ÆÊýÆ÷¼ÆÊýÄ£Ê½£¬ÉèÖÃÎªÏòÉÏ¼ÆÊý
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// ÖØ¸´¼ÆÊýÆ÷µÄÖµ£¬Ã»ÓÃµ½²»ÓÃ¹Ü
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ³õÊ¼»¯¶¨Ê±Æ÷
	TIM_TimeBaseInit(TPAD_TIM, &TIM_TimeBaseStructure);

	/*--------------------ÊäÈë²¶»ñ½á¹¹Ìå³õÊ¼»¯-------------------*/		
	// ÅäÖÃÊäÈë²¶»ñµÄÍ¨µÀ£¬ÐèÒª¸ù¾Ý¾ßÌåµÄGPIOÀ´ÅäÖÃ
	TIM_ICInitStructure.TIM_Channel = TPAD_TIM_CHANNEL_x;
	// ÊäÈë²¶»ñÐÅºÅµÄ¼«ÐÔÅäÖÃ
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ÊäÈëÍ¨µÀºÍ²¶»ñÍ¨µÀµÄÓ³Éä¹ØÏµ£¬ÓÐÖ±Á¬ºÍ·ÇÖ±Á¬Á½ÖÖ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// ÊäÈëµÄÐèÒª±»²¶»ñµÄÐÅºÅµÄ·ÖÆµÏµÊý
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// ÊäÈëµÄÐèÒª±»²¶»ñµÄÐÅºÅµÄÂË²¨ÏµÊý
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ¶¨Ê±Æ÷ÊäÈë²¶»ñ³õÊ¼»¯
	TIM_ICInit(TPAD_TIM, &TIM_ICInitStructure);
	
	// Ê¹ÄÜ¼ÆÊýÆ÷
	TIM_Cmd(TPAD_TIM, ENABLE);
}

/*===========================================================================================*/
/**
  * @brief  ¸´Î»µçÈÝ°´¼ü£¬·Åµç£¬ÖØÐÂ³äµç
  * @param  ÎÞ
  * @retval ÎÞ
  * ËµÃ÷£º
  * ¿ª·¢°åÉÏµçÖ®ºó£¬µçÈÝ°´¼üÄ¬ÈÏÒÑ¾­³äÂúÁËµç£¬ÒªÏë²âµÃµçÈÝ°´¼üµÄ³äµçÊ±¼ä
  * ¾Í±ØÐëÏÈ°ÑµçÈÝ°´¼üµÄµç·Åµô£¬·½·¨ÎªÈÃ½ÓµçÈÝ°´¼üµÄIOÊä³öµÍµçÆ½¼´¿É
  * ·ÅµçÍê±ÏÖ®ºó£¬ÔÙ°ÑÁ¬½ÓµçÈÝ°´¼üµÄIOÅäÖÃÎªÊäÈë£¬È»ºóÍ¨¹ýÊäÈë²¶»ñµÄ·½·¨
  * ²âÁ¿µçÈÝ°´¼üµÄ³äµçÊ±¼ä£¬Õâ¸ö³äµçÊ±¼äÊÇÃ»ÓÐÊÖÖ¸´¥ÃþµÄÇé¿öÏÂµÄ³äµçÊ±¼ä
  * ¶øÇÒÕâ¸ö¿ÕÔØµÄ³äµçÊ±¼ä·Ç³£ÎÈ¶¨£¬ÒòÎªµçÂ·°åµÄÓ²¼þÒÑ¾­È·¶¨ÁË
  * 
  * µ±ÓÐÊÖÖ¸´¥ÃþµÄÇé¿öÏÂ£¬³äµçÊ±¼ä»á±ä³¤£¬ÎÒÃÇÖ»ÐèÒª¶Ô±ÈÕâÁ½¸öÊ±¼ä¾Í¿ÉÒÔ
  * ÖªµÀµçÈÝ°´¼üÊÇ·ñÓÐÊÖÖ¸´¥Ãþ
  */
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // ÊäÈë²¶»ñÍ¨µÀ1 GPIO ³õÊ¼»¯
	RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  TPAD_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	// Á¬½ÓTPADµÄIOÅäÖÃÎªÊä³ö£¬È»ºóÊä³öµÍµçÆ½£¬ÑÓÊ±Ò»»á£¬È·±£µçÈÝ°´¼ü·ÅµçÍê±Ï
	GPIO_ResetBits(TPAD_TIM_CH_PORT,TPAD_TIM_CH_PIN);
  
	// ·ÅµçÊÇºÜ¿ìµÄ£¬Ò»°ãÊÇus¼¶±ð
	SysTick_Delay_Ms( 5 );
	
	// Á¬½ÓTPADµÄIOÅäÖÃÎªÊäÈë£¬ÓÃÓÚÊäÈë²¶»ñ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}

/**
  * @brief  »ñÈ¡¶¨Ê±Æ÷²¶»ñÖµ
  * @param  ÎÞ
  * @retval ¶¨Ê±Æ÷²¶»ñÖµ¡£Èç¹û³¬Ê±,ÔòÖ±½Ó·µ»Ø¶¨Ê±Æ÷µÄ¼ÆÊýÖµ¡£
  */
uint16_t TPAD_Get_Val(void)
{		
	// Ã¿´Î²¶»ñµÄÊ±ºò£¬±ØÐëÏÈ¸´Î»·Åµç
	TPAD_Reset();
	
	// µ±µçÈÝ°´¼ü¸´Î»·ÅµçÖ®ºó£¬¼ÆÊýÆ÷Çå0¿ªÊ¼¼ÆÊý
  TIM_SetCounter (TPAD_TIM,0);
	// Çå³ýÏà¹ØµÄ±êÖ¾Î»
	TIM_ClearITPendingBit (TPAD_TIM, TPAD_TIM_IT_CCx | TIM_IT_Update);
	
	// µÈ´ý²¶»ñÉÏÉýÑØ£¬µ±µçÈÝ°´¼ü³äµçµ½1.8V×óÓÒµÄÊ±ºò£¬¾Í»á±»ÈÏÎªÊÇÉÏÉýÑØ
	while(TIM_GetFlagStatus (TPAD_TIM, TPAD_TIM_IT_CCx) == RESET)
	{
		// Èç¹û³¬Ê±ÁË,Ö±½Ó·µ»ØCNTµÄÖµ
		// Ò»°ã³äµçÊ±¼ä¶¼ÊÇÔÚms¼¶±ðÒÔÄÚ£¬ºÜÉÙ»á³¬¹ý¶¨Ê±Æ÷µÄ×î´ó¼ÆÊýÖµ
		if (TIM_GetCounter(TPAD_TIM) > TPAD_TIM_Period-100)
		{
			return TIM_GetCounter (TPAD_TIM);
		} 			
	}
	
	// »ñÈ¡²¶»ñ±È½Ï¼Ä´æÆ÷µÄÖµ
	return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
} 


void TPAD_TIM_Init(void)
{
	TPAD_TIM_GPIO_Config();
	TPAD_TIM_Mode_Config();		
}


/**
  * @brief  ³õÊ¼»¯´¥Ãþ°´¼ü£¬»ñµÃ¿ÕÔØµÄÊ±ºò´¥Ãþ°´¼üµÄ³äµçÊ±¼ä
  * @param  ÎÞ
  * @retval 0£º³É¹¦£¬1£ºÊ§°Ü
  * @note   ¿ÕÔØÖµÒ»°ãºÜÎÈ¶¨£¬ÓÉÓ²¼þµçÂ·¾ö¶¨£¬¸Ãº¯ÊýÖ»ÐèÒªµ÷ÓÃÒ»´Î¼´¿É
  *         ¶øÇÒÕâ¸ö¿ÕÔØµÄ³äµçÊ±¼äÃ¿¸öÓ²¼þ¶¼²»Ò»Ñù£¬×îºÃÊµ¼Ê²âÊÔÏÂ
  */
uint8_t TPAD_Init(void)
{
	uint16_t temp;
	
	// µçÈÝ°´¼üÓÃµ½µÄÊäÈë²¶»ñµÄIOºÍ²¶»ñÄ£Ê½²ÎÊý³õÊ¼»¯
	TPAD_TIM_Init();
	
	temp = TPAD_Get_Val();
	
	// µçÈÝ°´¼ü¿ÕÔØµÄ³äµçÊ±¼ä·Ç³£ÎÈ¶¨£¬²»Í¬µÄÓ²¼þ³äµçÊ±¼ä²»Ò»Ñù
	// ÐèÒªÊµ¼Ê²âÊÔËùµÃ£¬°ÔµÀ ÉÏµÄµçÈÝ°´¼ü¿ÕÔØ³äµçÊ±¼äÎÈ¶¨ÔÚ218
	// Èç¹ûÄã¾õµÃµ¥´Î²âÁ¿²»×¼È·£¬Äã¿ÉÒÔ¶à´Î²âÁ¿È»ºóÈ¡¸öÆ½¾ùÖµ
	if( (TPAD_DEFAULT_VAL_MIN<temp) && (temp<TPAD_DEFAULT_VAL_MAX) )
	{
		tpad_default_val = temp;
		// µ÷ÊÔµÄÊ±ºò¿ÉÒÔ°Ñ²¶»ñµÄÖµ´òÓ¡³öÀ´£¬¿´¿´Ä¬ÈÏµÄ³äµçÊ±¼äÊÇ¶àÉÙ
    printf("µçÈÝ°´¼üÄ¬ÈÏ³äµçÊ±¼äÎª: %d us\n",tpad_default_val);
		return 0;  // ³É¹¦
	}
	else
  {
		return 1; // Ê§°Ü
  }
}

/**
  * @brief  ¶ÁÈ¡Èô¸É´Î¶¨Ê±Æ÷²¶»ñÖµ£¬²¢·µ»Ø×î´óÖµ¡£
  * @param  num £º¶ÁÈ¡´ÎÊý
  * @retval ¶ÁÈ¡µ½µÄ×î´ó¶¨Ê±Æ÷²¶»ñÖµ
  */
uint16_t TPAD_Get_MaxVal( uint8_t num )
{
	uint16_t temp=0, res=0;
	
	while(num--)
	{
		temp = TPAD_Get_Val();
		if( temp > res ) 
			res = temp;
	}	
	return res;	
} 

/**
  * @brief  °´¼üÉ¨Ãèº¯Êý
  * @param  ÎÞ
  * @retval 1£º°´¼üÓÐÐ§£¬0£º°´¼üÎÞÐ§
  */
uint8_t TPAD_Scan(void)
{
	// keyen£º°´¼ü¼ì²âÊ¹ÄÜ±êÖ¾
	// 0£º¿ÉÒÔ¿ªÊ¼¼ì²â
	// >0£º»¹²»ÄÜ¿ªÊ¼¼ì²â£¬±íÊ¾°´¼üÒ»Ö±±»°´ÏÂ
	// ×¢Òâ£ºkeytn Õâ¸ö±äÁ¿ÓÉ static ÐÞÊÎ£¬Ïàµ±ÓÚÒ»¸öÈ«¾Ö±äÁ¿£¬µ«ÊÇÒòÎªÊÇÔÚº¯ÊýÄÚ²¿¶¨Òå£¬
	// ËùÒÔÊÇÏàµ±ÓÚÕâ¸öº¯ÊýµÄÈ«¾Ö±äÁ¿£¬Ã¿´ÎÐÞ¸ÄÖ®Ç°±£ÁôµÄÊÇÉÏÒ»´ÎµÄÖµ
	static uint8_t keyen=0;	

	uint8_t res=0,sample=3; 
	uint16_t scan_val;		

	// ¸ù¾ÝsampleÖµ²ÉÑù¶à´Î£¬²¢È¡×î´óÖµ£¬Ð¡µÄÒ»°ãÊÇ¸ÉÈÅ»òÕßÊÇÎó´¥Ãþ
	scan_val = TPAD_Get_MaxVal(sample); 
	
  // µ±É¨ÃèµÄÖµ´óÓÚ¿ÕÔØÖµ¼ÓÉÏÄ¬ÈÏµÄÃÅÏÞÖµÖ®ºó£¬±íÊ¾°´¼ü°´ÏÂ
	// Õâ¸öTPAD_GATE_VAL¸ù¾ÝÓ²¼þ¾ö¶¨£¬ÐèÒªÊµ¼Ê²âÊÔ
	if(scan_val > (tpad_default_val+TPAD_GATE_VAL))
	{				
    // ÔÙ´Î¼ì²â£¬ÀàËÆÓÚ»úÐµ°´¼üµÄÈ¥¶¶
		scan_val = TPAD_Get_MaxVal(sample);		
		if( ( keyen == 0 )&& (scan_val > (tpad_default_val+TPAD_GATE_VAL)))
				res = 1;	// ÓÐÐ§µÄ°´¼ü

		// Èç¹û°´¼üÒ»Ö±±»°´ÏÂ£¬keyenµÄÖµ»áÒ»Ö±ÔÚkeyenµÄ³õÊ¼ÖµºÍkeyen-1Ö®¼äÑ­»·£¬ÓÀÔ¶²»»áµÈÓÚ0
		keyen = 2;
	}
	
	// µ±°´¼üÃ»ÓÐ±»°´ÏÂ»òÕßkeyen>0Ê±£¬»áÖ´ÐÐkeyen--
	if( keyen > 0)
		keyen--;
  
	return res;
}
/*********************************************END OF FILE**********************/
=======

#include "bsp_tpad.h"

// µçÈÝ°´¼ü¿ÕÔØµÄÊ±ºò³äµçÊ±¼ä
uint16_t tpad_default_val;

static void TPAD_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ÊäÈë²¶»ñÍ¨µÀ GPIO ³õÊ¼»¯
	RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  TPAD_TIM_CH_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);	
}


///*
// * ×¢Òâ£ºTIM_TimeBaseInitTypeDef½á¹¹ÌåÀïÃæÓÐ5¸ö³ÉÔ±£¬TIM6ºÍTIM7µÄ¼Ä´æÆ÷ÀïÃæÖ»ÓÐ
// * TIM_PrescalerºÍTIM_Period£¬ËùÒÔÊ¹ÓÃTIM6ºÍTIM7µÄÊ±ºòÖ»Ðè³õÊ¼»¯ÕâÁ½¸ö³ÉÔ±¼´¿É£¬
// * ÁíÍâÈý¸ö³ÉÔ±ÊÇÍ¨ÓÃ¶¨Ê±Æ÷ºÍ¸ß¼¶¶¨Ê±Æ÷²ÅÓÐ.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ¶¼ÓÐ
// *	TIM_CounterMode			     TIMx,x[6,7]Ã»ÓÐ£¬ÆäËû¶¼ÓÐ
// *  TIM_Period               ¶¼ÓÐ
// *  TIM_ClockDivision        TIMx,x[6,7]Ã»ÓÐ£¬ÆäËû¶¼ÓÐ
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]²ÅÓÐ
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWMÐÅºÅ ÖÜÆÚºÍÕ¼¿Õ±ÈµÄ¼ÆËã--------------- */
// ARR £º×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷µÄÖµ
// CLK_cnt£º¼ÆÊýÆ÷µÄÊ±ÖÓ£¬µÈÓÚ Fck_int / (psc+1) = 72M/(psc+1)
// PWM ÐÅºÅµÄÖÜÆÚ T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// Õ¼¿Õ±ÈP=CCR/(ARR+1)

static void TPAD_TIM_Mode_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	// ¿ªÆô¶¨Ê±Æ÷Ê±ÖÓ,¼´ÄÚ²¿Ê±ÖÓCK_INT=72M
	TPAD_TIM_APBxClock_FUN(TPAD_TIM_CLK,ENABLE);

/*--------------------Ê±»ù½á¹¹Ìå³õÊ¼»¯-------------------------*/	
	// ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷µÄÖµ£¬ÀÛ¼ÆTIM_Period+1¸öÆµÂÊºó²úÉúÒ»¸ö¸üÐÂ»òÕßÖÐ¶Ï
	TIM_TimeBaseStructure.TIM_Period=TPAD_TIM_Period;	
	// Çý¶¯CNT¼ÆÊýÆ÷µÄÊ±ÖÓ = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= TPAD_TIM_Prescaler;	
	// Ê±ÖÓ·ÖÆµÒò×Ó £¬ÅäÖÃËÀÇøÊ±¼äÊ±ÐèÒªÓÃµ½
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ¼ÆÊýÆ÷¼ÆÊýÄ£Ê½£¬ÉèÖÃÎªÏòÉÏ¼ÆÊý
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// ÖØ¸´¼ÆÊýÆ÷µÄÖµ£¬Ã»ÓÃµ½²»ÓÃ¹Ü
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ³õÊ¼»¯¶¨Ê±Æ÷
	TIM_TimeBaseInit(TPAD_TIM, &TIM_TimeBaseStructure);

	/*--------------------ÊäÈë²¶»ñ½á¹¹Ìå³õÊ¼»¯-------------------*/		
	// ÅäÖÃÊäÈë²¶»ñµÄÍ¨µÀ£¬ÐèÒª¸ù¾Ý¾ßÌåµÄGPIOÀ´ÅäÖÃ
	TIM_ICInitStructure.TIM_Channel = TPAD_TIM_CHANNEL_x;
	// ÊäÈë²¶»ñÐÅºÅµÄ¼«ÐÔÅäÖÃ
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ÊäÈëÍ¨µÀºÍ²¶»ñÍ¨µÀµÄÓ³Éä¹ØÏµ£¬ÓÐÖ±Á¬ºÍ·ÇÖ±Á¬Á½ÖÖ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// ÊäÈëµÄÐèÒª±»²¶»ñµÄÐÅºÅµÄ·ÖÆµÏµÊý
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// ÊäÈëµÄÐèÒª±»²¶»ñµÄÐÅºÅµÄÂË²¨ÏµÊý
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ¶¨Ê±Æ÷ÊäÈë²¶»ñ³õÊ¼»¯
	TIM_ICInit(TPAD_TIM, &TIM_ICInitStructure);
	
	// Ê¹ÄÜ¼ÆÊýÆ÷
	TIM_Cmd(TPAD_TIM, ENABLE);
}

/*===========================================================================================*/
/**
  * @brief  ¸´Î»µçÈÝ°´¼ü£¬·Åµç£¬ÖØÐÂ³äµç
  * @param  ÎÞ
  * @retval ÎÞ
  * ËµÃ÷£º
  * ¿ª·¢°åÉÏµçÖ®ºó£¬µçÈÝ°´¼üÄ¬ÈÏÒÑ¾­³äÂúÁËµç£¬ÒªÏë²âµÃµçÈÝ°´¼üµÄ³äµçÊ±¼ä
  * ¾Í±ØÐëÏÈ°ÑµçÈÝ°´¼üµÄµç·Åµô£¬·½·¨ÎªÈÃ½ÓµçÈÝ°´¼üµÄIOÊä³öµÍµçÆ½¼´¿É
  * ·ÅµçÍê±ÏÖ®ºó£¬ÔÙ°ÑÁ¬½ÓµçÈÝ°´¼üµÄIOÅäÖÃÎªÊäÈë£¬È»ºóÍ¨¹ýÊäÈë²¶»ñµÄ·½·¨
  * ²âÁ¿µçÈÝ°´¼üµÄ³äµçÊ±¼ä£¬Õâ¸ö³äµçÊ±¼äÊÇÃ»ÓÐÊÖÖ¸´¥ÃþµÄÇé¿öÏÂµÄ³äµçÊ±¼ä
  * ¶øÇÒÕâ¸ö¿ÕÔØµÄ³äµçÊ±¼ä·Ç³£ÎÈ¶¨£¬ÒòÎªµçÂ·°åµÄÓ²¼þÒÑ¾­È·¶¨ÁË
  * 
  * µ±ÓÐÊÖÖ¸´¥ÃþµÄÇé¿öÏÂ£¬³äµçÊ±¼ä»á±ä³¤£¬ÎÒÃÇÖ»ÐèÒª¶Ô±ÈÕâÁ½¸öÊ±¼ä¾Í¿ÉÒÔ
  * ÖªµÀµçÈÝ°´¼üÊÇ·ñÓÐÊÖÖ¸´¥Ãþ
  */
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // ÊäÈë²¶»ñÍ¨µÀ1 GPIO ³õÊ¼»¯
	RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  TPAD_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	// Á¬½ÓTPADµÄIOÅäÖÃÎªÊä³ö£¬È»ºóÊä³öµÍµçÆ½£¬ÑÓÊ±Ò»»á£¬È·±£µçÈÝ°´¼ü·ÅµçÍê±Ï
	GPIO_ResetBits(TPAD_TIM_CH_PORT,TPAD_TIM_CH_PIN);
  
	// ·ÅµçÊÇºÜ¿ìµÄ£¬Ò»°ãÊÇus¼¶±ð
	SysTick_Delay_Ms( 5 );
	
	// Á¬½ÓTPADµÄIOÅäÖÃÎªÊäÈë£¬ÓÃÓÚÊäÈë²¶»ñ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}

/**
  * @brief  »ñÈ¡¶¨Ê±Æ÷²¶»ñÖµ
  * @param  ÎÞ
  * @retval ¶¨Ê±Æ÷²¶»ñÖµ¡£Èç¹û³¬Ê±,ÔòÖ±½Ó·µ»Ø¶¨Ê±Æ÷µÄ¼ÆÊýÖµ¡£
  */
uint16_t TPAD_Get_Val(void)
{		
	// Ã¿´Î²¶»ñµÄÊ±ºò£¬±ØÐëÏÈ¸´Î»·Åµç
	TPAD_Reset();
	
	// µ±µçÈÝ°´¼ü¸´Î»·ÅµçÖ®ºó£¬¼ÆÊýÆ÷Çå0¿ªÊ¼¼ÆÊý
  TIM_SetCounter (TPAD_TIM,0);
	// Çå³ýÏà¹ØµÄ±êÖ¾Î»
	TIM_ClearITPendingBit (TPAD_TIM, TPAD_TIM_IT_CCx | TIM_IT_Update);
	
	// µÈ´ý²¶»ñÉÏÉýÑØ£¬µ±µçÈÝ°´¼ü³äµçµ½1.8V×óÓÒµÄÊ±ºò£¬¾Í»á±»ÈÏÎªÊÇÉÏÉýÑØ
	while(TIM_GetFlagStatus (TPAD_TIM, TPAD_TIM_IT_CCx) == RESET)
	{
		// Èç¹û³¬Ê±ÁË,Ö±½Ó·µ»ØCNTµÄÖµ
		// Ò»°ã³äµçÊ±¼ä¶¼ÊÇÔÚms¼¶±ðÒÔÄÚ£¬ºÜÉÙ»á³¬¹ý¶¨Ê±Æ÷µÄ×î´ó¼ÆÊýÖµ
		if (TIM_GetCounter(TPAD_TIM) > TPAD_TIM_Period-100)
		{
			return TIM_GetCounter (TPAD_TIM);
		} 			
	}
	
	// »ñÈ¡²¶»ñ±È½Ï¼Ä´æÆ÷µÄÖµ
	return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
} 


void TPAD_TIM_Init(void)
{
	TPAD_TIM_GPIO_Config();
	TPAD_TIM_Mode_Config();		
}


/**
  * @brief  ³õÊ¼»¯´¥Ãþ°´¼ü£¬»ñµÃ¿ÕÔØµÄÊ±ºò´¥Ãþ°´¼üµÄ³äµçÊ±¼ä
  * @param  ÎÞ
  * @retval 0£º³É¹¦£¬1£ºÊ§°Ü
  * @note   ¿ÕÔØÖµÒ»°ãºÜÎÈ¶¨£¬ÓÉÓ²¼þµçÂ·¾ö¶¨£¬¸Ãº¯ÊýÖ»ÐèÒªµ÷ÓÃÒ»´Î¼´¿É
  *         ¶øÇÒÕâ¸ö¿ÕÔØµÄ³äµçÊ±¼äÃ¿¸öÓ²¼þ¶¼²»Ò»Ñù£¬×îºÃÊµ¼Ê²âÊÔÏÂ
  */
uint8_t TPAD_Init(void)
{
	uint16_t temp;
	
	// µçÈÝ°´¼üÓÃµ½µÄÊäÈë²¶»ñµÄIOºÍ²¶»ñÄ£Ê½²ÎÊý³õÊ¼»¯
	TPAD_TIM_Init();
	
	temp = TPAD_Get_Val();
	
	// µçÈÝ°´¼ü¿ÕÔØµÄ³äµçÊ±¼ä·Ç³£ÎÈ¶¨£¬²»Í¬µÄÓ²¼þ³äµçÊ±¼ä²»Ò»Ñù
	// ÐèÒªÊµ¼Ê²âÊÔËùµÃ£¬°ÔµÀ ÉÏµÄµçÈÝ°´¼ü¿ÕÔØ³äµçÊ±¼äÎÈ¶¨ÔÚ218
	// Èç¹ûÄã¾õµÃµ¥´Î²âÁ¿²»×¼È·£¬Äã¿ÉÒÔ¶à´Î²âÁ¿È»ºóÈ¡¸öÆ½¾ùÖµ
	if( (TPAD_DEFAULT_VAL_MIN<temp) && (temp<TPAD_DEFAULT_VAL_MAX) )
	{
		tpad_default_val = temp;
		// µ÷ÊÔµÄÊ±ºò¿ÉÒÔ°Ñ²¶»ñµÄÖµ´òÓ¡³öÀ´£¬¿´¿´Ä¬ÈÏµÄ³äµçÊ±¼äÊÇ¶àÉÙ
		printf("µçÈÝ°´¼üÄ¬ÈÏ³äµçÊ±¼äÎª: %d us\n",tpad_default_val);
		return 0;  // ³É¹¦
	}
	else
  {
		return 1; // Ê§°Ü
  }
}

/**
  * @brief  ¶ÁÈ¡Èô¸É´Î¶¨Ê±Æ÷²¶»ñÖµ£¬²¢·µ»Ø×î´óÖµ¡£
  * @param  num £º¶ÁÈ¡´ÎÊý
  * @retval ¶ÁÈ¡µ½µÄ×î´ó¶¨Ê±Æ÷²¶»ñÖµ
  */
uint16_t TPAD_Get_MaxVal( uint8_t num )
{
	uint16_t temp=0, res=0;
	
	while(num--)
	{
		temp = TPAD_Get_Val();
		if( temp > res ) 
			res = temp;
	}	
	return res;	
} 

/**
  * @brief  °´¼üÉ¨Ãèº¯Êý
  * @param  ÎÞ
  * @retval 1£º°´¼üÓÐÐ§£¬0£º°´¼üÎÞÐ§
  */
uint8_t TPAD_Scan(void)
{
	// keyen£º°´¼ü¼ì²âÊ¹ÄÜ±êÖ¾
	// 0£º¿ÉÒÔ¿ªÊ¼¼ì²â
	// >0£º»¹²»ÄÜ¿ªÊ¼¼ì²â£¬±íÊ¾°´¼üÒ»Ö±±»°´ÏÂ
	// ×¢Òâ£ºkeytn Õâ¸ö±äÁ¿ÓÉ static ÐÞÊÎ£¬Ïàµ±ÓÚÒ»¸öÈ«¾Ö±äÁ¿£¬µ«ÊÇÒòÎªÊÇÔÚº¯ÊýÄÚ²¿¶¨Òå£¬
	// ËùÒÔÊÇÏàµ±ÓÚÕâ¸öº¯ÊýµÄÈ«¾Ö±äÁ¿£¬Ã¿´ÎÐÞ¸ÄÖ®Ç°±£ÁôµÄÊÇÉÏÒ»´ÎµÄÖµ
	static uint8_t keyen=0;	

	uint8_t res=0,sample=3; 
	uint16_t scan_val;		

	// ¸ù¾ÝsampleÖµ²ÉÑù¶à´Î£¬²¢È¡×î´óÖµ£¬Ð¡µÄÒ»°ãÊÇ¸ÉÈÅ»òÕßÊÇÎó´¥Ãþ
	scan_val = TPAD_Get_MaxVal(sample); 
	
  // µ±É¨ÃèµÄÖµ´óÓÚ¿ÕÔØÖµ¼ÓÉÏÄ¬ÈÏµÄÃÅÏÞÖµÖ®ºó£¬±íÊ¾°´¼ü°´ÏÂ
	// Õâ¸öTPAD_GATE_VAL¸ù¾ÝÓ²¼þ¾ö¶¨£¬ÐèÒªÊµ¼Ê²âÊÔ
	if(scan_val > (tpad_default_val+TPAD_GATE_VAL))
	{				
    // ÔÙ´Î¼ì²â£¬ÀàËÆÓÚ»úÐµ°´¼üµÄÈ¥¶¶
		scan_val = TPAD_Get_MaxVal(sample);		
		if( ( keyen == 0 )&& (scan_val > (tpad_default_val+TPAD_GATE_VAL)))
				res = 1;	// ÓÐÐ§µÄ°´¼ü

		// Èç¹û°´¼üÒ»Ö±±»°´ÏÂ£¬keyenµÄÖµ»áÒ»Ö±ÔÚkeyenµÄ³õÊ¼ÖµºÍkeyen-1Ö®¼äÑ­»·£¬ÓÀÔ¶²»»áµÈÓÚ0
		keyen = 2;
	}
	
	// µ±°´¼üÃ»ÓÐ±»°´ÏÂ»òÕßkeyen>0Ê±£¬»áÖ´ÐÐkeyen--
	if( keyen > 0)
		keyen--;
  
	return res;
}
/*********************************************END OF FILE**********************/
>>>>>>> f24a7e019daefba4bdcb9221812d099b96547227:34-TIMâ€”è¾“å…¥æ•èŽ·ä¹‹ç”µå®¹æŒ‰é”®æ£€æµ‹-V1/User/TPad/bsp_tpad.c
