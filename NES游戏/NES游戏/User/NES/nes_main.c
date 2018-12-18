//////////////////////////////////////////////////////////////////////////////////	 
//本程序移植自网友ye781205的NES模拟器工程		  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "nes_main.h" 
#include "nes_ppu.h"
#include "nes_mapper.h"
#include "nes_apu.h"

#include "malloc.h"
#include "ff.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "joypad.h"
#include "bsp_lcd.h"
#include  "VS1053.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"  
#include "bsp_key.h" 


int MapperNo;			//map编号
int NES_scanline;		//nes扫描线
int VROM_1K_SIZE;
int VROM_8K_SIZE;

u8 PADdata;   			//手柄1键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 PADdata1;   			//手柄2键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 *NES_RAM;			//保持1024字节对齐
u8 *NES_SRAM;  
NES_header *RomHeader; 	//rom文件头
MAPPER *NES_Mapper;		 
MapperCommRes *MAPx;  


u8* spr_ram;			//精灵RAM,256字节
ppu_data* ppu;			//ppu指针
u8* VROM_banks;
u8* VROM_tiles;

apu_t *apu; 			//apu指针
u8 *wave_buffers;


u8 *nesapusbuf[NES_APU_BUF_NUM];		//音频缓冲帧
u8* romfile;							//nes文件指针,指向整个nes文件的起始地址.
//////////////////////////////////////////////////////////////////////////////////////

 
//加载ROM
//返回值:0,成功
//    1,内存错误
//    3,map错误
u8 nes_load_rom(void)
{  
    u8* p;  
	u8 i;
	u8 res=0;
	p=(u8*)romfile;	
	if(strncmp((char*)p,"NES",3)==0)
	{  
		RomHeader->ctrl_z=p[3];
		RomHeader->num_16k_rom_banks=p[4];
		RomHeader->num_8k_vrom_banks=p[5];
		RomHeader->flags_1=p[6];
		RomHeader->flags_2=p[7]; 
		if(RomHeader->flags_1&0x04)p+=512;		//有512字节的trainer:
		if(RomHeader->num_8k_vrom_banks>0)		//存在VROM,进行预解码
		{		
			VROM_banks=p+16+(RomHeader->num_16k_rom_banks*0x4000);
#if	NES_RAM_SPEED==1	//1:内存占用小 0:速度快	 
			VROM_tiles=VROM_banks;	 
#else  
			VROM_tiles=mymalloc(SRAMEX,RomHeader->num_8k_vrom_banks*8*1024);//这里可能申请多达1MB内存!!!
			if(VROM_tiles==0)VROM_tiles=VROM_banks;//内存不够用的情况下,尝试VROM_titles与VROM_banks共用内存			
			compile(RomHeader->num_8k_vrom_banks*8*1024/16,VROM_banks,VROM_tiles);  
#endif	
		}else 
		{
			VROM_banks=mymalloc(SRAMIN,8*1024);
			VROM_tiles=mymalloc(SRAMEX,8*1024);
			if(!VROM_banks||!VROM_tiles)res=1;
		}  	
		VROM_1K_SIZE = RomHeader->num_8k_vrom_banks * 8;
		VROM_8K_SIZE = RomHeader->num_8k_vrom_banks;  
		MapperNo=(RomHeader->flags_1>>4)|(RomHeader->flags_2&0xf0);
		if(RomHeader->flags_2 & 0x0E)MapperNo=RomHeader->flags_1>>4;//忽略高四位，如果头看起来很糟糕 
		printf("use map:%d\r\n",MapperNo);
		for(i=0;i<255;i++)  // 查找支持的Mapper号
		{		
			if (MapTab[i]==MapperNo)break;		
			if (MapTab[i]==-1)res=3; 
		} 
		if(res==0)
		{
			switch(MapperNo)
			{
				case 1:  
					MAP1=mymalloc(SRAMIN,sizeof(Mapper1Res)); 
					if(!MAP1)res=1;
					break;
				case 4:  
				case 6: 
				case 16:
				case 17:
				case 18:
				case 19:
				case 21: 
				case 23:
				case 24:
				case 25:
				case 64:
				case 65:
				case 67:
				case 69:
				case 85:
				case 189:
					MAPx=mymalloc(SRAMIN,sizeof(MapperCommRes)); 
					if(!MAPx)res=1;
					break;  
				default:
					break;
			}
		}
	} 
	return res;	//返回执行结果
} 
//释放内存 
void nes_sram_free(void)
{ 
	u8 i;
	myfree(SRAMIN,NES_RAM);		
	myfree(SRAMIN,NES_SRAM);	
	myfree(SRAMIN,RomHeader);	
	myfree(SRAMIN,NES_Mapper);
	myfree(SRAMIN,spr_ram);		
	myfree(SRAMIN,ppu);	
	myfree(SRAMIN,apu);	
	myfree(SRAMIN,wave_buffers);
	for(i=0;i<NES_APU_BUF_NUM;i++)myfree(SRAMEX,nesapusbuf[i]);//释放APU BUFs 
	myfree(SRAMEX,romfile);	  
	if((VROM_tiles!=VROM_banks)&&VROM_banks&&VROM_tiles)//如果分别为VROM_banks和VROM_tiles申请了内存,则释放
	{
		myfree(SRAMIN,VROM_banks);
		myfree(SRAMEX,VROM_tiles);		 
	}
	switch (MapperNo)//释放map内存
	{
		case 1: 			//释放内存
			myfree(SRAMIN,MAP1);
			break;	 	
		case 4: 
		case 6: 
		case 16:
		case 17:
		case 18:
		case 19:
		case 21:
		case 23:
		case 24:
		case 25:
		case 64:
		case 65:
		case 67:
		case 69:
		case 85:
		case 189:
			myfree(SRAMIN,MAPx);break;	 		//释放内存 
		default:break; 
	}
	NES_RAM=0;	
	NES_SRAM=0;
	RomHeader=0;
	NES_Mapper=0;
	spr_ram=0;
	ppu=0;
	apu=0;
	wave_buffers=0;
	for(i=0;i<NES_APU_BUF_NUM;i++)nesapusbuf[i]=0; 
	VROM_banks=0;
	VROM_tiles=0; 
	MAP1=0;
	MAPx=0;
} 
//为NES运行申请内存
//romsize:nes文件大小
//返回值:0,申请成功
//       1,申请失败
u8 nes_sram_malloc(u32 romsize)
{
	u16 i=0;
	for(i=0;i<64;i++)//为NES_RAM,查找1024对齐的内存
	{
		NES_SRAM=mymalloc(SRAMIN,i*32);
		NES_RAM=mymalloc(SRAMIN,0X800);	//申请2K字节,必须1024字节对齐
		if((u32)NES_RAM%1024)			//不是1024字节对齐
		{
			myfree(SRAMIN,NES_RAM);		//释放内存,然后重新尝试分配
			myfree(SRAMIN,NES_SRAM); 
		}else 
		{
			myfree(SRAMIN,NES_SRAM); 	//释放内存
			break;
		}
	}	 
 	NES_SRAM=mymalloc(SRAMIN,0X2000);
	RomHeader=mymalloc(SRAMIN,sizeof(NES_header));
	NES_Mapper=mymalloc(SRAMIN,sizeof(MAPPER));
	spr_ram=mymalloc(SRAMIN,0X100);		
	ppu=mymalloc(SRAMIN,sizeof(ppu_data));  
	apu=mymalloc(SRAMIN,sizeof(apu_t));		//sizeof(apu_t)=  12588
	wave_buffers=mymalloc(SRAMIN,APU_PCMBUF_SIZE);
	for(i=0;i<NES_APU_BUF_NUM;i++)
	{
		nesapusbuf[i]=mymalloc(SRAMEX,APU_PCMBUF_SIZE+10);//申请内存
	}
 	romfile=mymalloc(SRAMEX,romsize);			//申请游戏rom空间,等于nes文件大小 
	if(i==64||!NES_RAM||!NES_SRAM||!RomHeader||!NES_Mapper||!spr_ram||!ppu||!apu||!wave_buffers||!nesapusbuf[NES_APU_BUF_NUM-1]||!romfile)
	{
		nes_sram_free();
		return 1;
	}
	memset(NES_SRAM,0,0X2000);				//清零
	memset(RomHeader,0,sizeof(NES_header));	//清零
	memset(NES_Mapper,0,sizeof(MAPPER));	//清零
	memset(spr_ram,0,0X100);				//清零
	memset(ppu,0,sizeof(ppu_data));			//清零
	memset(apu,0,sizeof(apu_t));			//清零
	memset(wave_buffers,0,APU_PCMBUF_SIZE);	//清零
	for(i=0;i<NES_APU_BUF_NUM;i++)memset(nesapusbuf[i],0,APU_PCMBUF_SIZE+10);//清零 
	memset(romfile,0,romsize);				//清零 
	return 0;
} 
////频率设置
////PLL,倍频数
//void nes_clock_set(u8 PLL)
//{
////	u16 tPLL=PLL;
//	u8 temp=0;	 
//	RCC->CFGR&=0XFFFFFFFC;	//修改时钟频率为内部8M	   
//	RCC->CR&=~0x01000000;  	//PLLOFF  
// 	RCC->CFGR&=~(0XF<<18);	//清空原来的设置
// 	PLL-=2;//抵消2个单位
//	RCC->CFGR|=PLL<<18;   	//设置PLL值 2~16
//	RCC->CFGR|=1<<16;	  	//PLLSRC ON 
//	FLASH->ACR|=0x12;	  	//FLASH 2个延时周期
// 	RCC->CR|=0x01000000;  	//PLLON
//	while(!(RCC->CR>>25));	//等待PLL锁定
//	RCC->CFGR|=0x02;		//PLL作为系统时钟	 
//	while(temp!=0x02)    	//等待PLL作为系统时钟设置成功
//	{   
//		temp=RCC->CFGR>>2;
//		temp&=0x03;
//	}  
//} 

//开始nes游戏
//pname:nes游戏路径
//返回值:
//0,正常退出
//1,内存错误
//2,文件错误
//3,不支持的map
u8 nes_load(const char* pname)
{
	FIL *file; 
	UINT br;
	u8 res=0;   
	file=mymalloc(SRAMIN,sizeof(FIL));  
	if(file==0)return 1;						//内存申请失败.  
	res=f_open(file,(char*)pname,FA_READ);
	if((res!=FR_OK)||(file->fsize>1024*960))	//打开文件失败
	{
		myfree(SRAMIN,file);
		return 2;
	}	 
	res=nes_sram_malloc(file->fsize);			//申请内存 
	if(res==0)
	{
		f_read(file,romfile,file->fsize,&br);	//读取nes文件
		res=nes_load_rom();						//加载ROM
		if(res==0) 					
		{  
			Mapper_Init();						//map初始化
			cpu6502_init();						//初始化6502,并复位	  	 
			PPU_reset();						//ppu复位
			apu_init(); 						//apu初始化 
			nes_sound_open(0,APU_SAMPLE_RATE);	//初始化播放设备
			nes_emulate_frame();				//进入NES模拟器主循环 
			nes_sound_close();					//关闭声音输出
		}
	}
	f_close(file);
	myfree(SRAMIN,file);//释放内存
	nes_sram_free();	//释放内存
	return res;
} 
u8 nes_xoff=0;	//显示在x轴方向的偏移量(实际显示宽度=256-2*nes_xoff)
//设置游戏显示窗口
void nes_set_window(void)
{	
	u16 xoff=0,yoff=0; 
	u16 lcdwidth,lcdheight;
	lcdwidth=240;
	lcdheight=240;
	nes_xoff=(256-240)/2;	//得到x轴方向的偏移量
	xoff=0; 
	yoff=(320-lcdheight)/2;//屏幕高度 
	ILI9341_OpenWindow(xoff,yoff,lcdwidth,lcdheight);//让NES始终在屏幕的正中央显示
}
//extern void KEYBRD_FCPAD_Decode(uint8_t *fcbuf,uint8_t mode);
//读取游戏手柄数据
void nes_get_gamepadval(void)
{  
	PADdata=JOYPAD_Read();	//读取手柄1的值
	//PADdata1=0;				//没有手柄2,故不采用. 
}  

//nes模拟器主循环
void nes_emulate_frame(void)
{  
	uint8_t frame_cnt; 
	u8 nes_frame=0;
	nes_set_window();//设置窗口
	while(1)
	{	
		// LINES 0-239
		PPU_start_frame();
		for(NES_scanline = 0; NES_scanline< 240; NES_scanline++)
		{
			run6502(113*256);
			NES_Mapper->HSync(NES_scanline);
			//扫描一行		  
			if(nes_frame==0)scanline_draw(NES_scanline);
			else do_scanline_and_dont_draw(NES_scanline); 
		}  
		NES_scanline=240;
		run6502(113*256);//运行1线
		NES_Mapper->HSync(NES_scanline); 
		start_vblank(); 
		if(NMI_enabled()) 
		{
			cpunmi=1;
			run6502(7*256);//运行中断
		}
		NES_Mapper->VSync();
		// LINES 242-261    
		for(NES_scanline=241;NES_scanline<262;NES_scanline++)
		{
			run6502(113*256);	  
			NES_Mapper->HSync(NES_scanline);		  
		}	   
		end_vblank(); 
		nes_get_gamepadval();	//每3帧查询一次USB
		apu_soundoutput();		//输出游戏声音	 
		frame_cnt++; 	
		nes_frame++;
		if(nes_frame>NES_SKIP_FRAME)
		{
			nes_frame=0;//跳帧  
		}
		if(frame_cnt>=10)
		{
 			if(Key2_Scan())
			{
 				break;
			}
			frame_cnt=0;
 		}
	}
	ILI9341_OpenWindow(0,0,240,320);//恢复屏幕窗口
}
//在6502.s里面被调用
void debug_6502(u16 reg0,u8 reg1)
{
	printf("6502 error:%x,%d\r\n",reg0,reg1);
}
////////////////////////////////////////////////////////////////////////////////// 	 
//nes,音频输出支持部分
vu16 nesbufpos=0;
vu8 nesplaybuf=0;		//即将播放的音频帧缓冲编号
vu8 nessavebuf=0;		//当前保存到的音频缓冲编号 
void nes_vs1053_feeddata(void);
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			     
	if(TIM6->SR&0X0001)//溢出中断
	{				    
		nes_vs1053_feeddata();//填充数据	  	   				     	    	
	}				   
	TIM6->SR&=~(1<<0);//清除中断标志位 	  									 
}
//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC->APB1ENR|=1<<4;//TIM6时钟使能    
 	TIM6->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM6->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟	  
	TIM6->DIER|=1<<0;   //允许更新中断	 
	TIM6->CR1|=0x01;    //使能定时器3
				
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
}

//音频播放回调函数
void nes_vs1053_feeddata(void)
{  
	u8 n;
	u8 nbytes;
	u8 *p; 
	if(nesplaybuf==nessavebuf)return;//还没有收到新的音频数据
	if(VS_DREQ_IN!=0)//可以发送数据给VS10XX
	{		 
		p=nesapusbuf[nesplaybuf]+nesbufpos; 
		nesbufpos+=32; 
		if(nesbufpos>APU_PCMBUF_SIZE)
		{
			nesplaybuf++;
			if(nesplaybuf>(NES_APU_BUF_NUM-1))nesplaybuf=0; 	
			nbytes=APU_PCMBUF_SIZE+32-nesbufpos;
			nesbufpos=0; 
		}else nbytes=32;
		for(n=0;n<nbytes;n++)
		{
			if(p[n]!=0)break;	//判断是不是剩余所有的数据都为0? 
		}
		if(n==nbytes)return;	//都是0,则直接不写入VS1053了,以免引起哒哒声. 
		VS_XDCS_CLR;  
		for(n=0;n<nbytes;n++)
		{
			VS_SPI_ReadWriteByte(p[n]);	 			
		}
		VS_XDCS_SET;     				   
	} 
}
//NES模拟器声音从VS1053输出,模拟WAV解码的wav头数据.
const u8 nes_wav_head[]=
{
0X52,0X49,0X46,0X46,0XFF,0XFF,0XFF,0XFF,0X57,0X41,0X56,0X45,0X66,0X6D,0X74,0X20,
0X10,0X00,0X00,0X00,0X01,0X00,0X01,0X00,0X11,0X2B,0X00,0X00,0X11,0X2B,0X00,0X00,
0X01,0X00,0X08,0X00,0X64,0X61,0X74,0X61,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
//NES打开音频输出
int nes_sound_open(int samples_per_sync,int sample_rate) 
{
	u8 *p;
	u8 i; 
	p=mymalloc(SRAMIN,100);	//申请100字节内存
	if(p==NULL)return 1;	//内存申请失败,直接退出
//	printf("sound open:%d\r\n",sample_rate);
	for(i=0;i<sizeof(nes_wav_head);i++)//复制nes_wav_head内容
	{
		p[i]=nes_wav_head[i];
	}
	p[24]=sample_rate&0XFF;			//设置采样率
	p[25]=(sample_rate>>8)&0XFF;
	p[28]=sample_rate&0XFF;			//设置字节速率(8位模式,等于采样率)
	p[29]=(sample_rate>>8)&0XFF; 
	nesplaybuf=0;
	nessavebuf=0;	
	VS_HD_Reset();		   			//硬复位
	VS_Soft_Reset();  				//软复位 
	VS_Set_All();					//设置音量等参数 			 
	VS_Reset_DecodeTime();			//复位解码时间 	  	 
	while(VS_Send_MusicData(p));	//发送wav head
	while(VS_Send_MusicData(p+32));	//发送wav head
	TIM6_Int_Init(100-1,1280-1);	//1ms中断一次
	myfree(SRAMIN,p);				//释放内存
	return 1;
}
//NES关闭音频输出
void nes_sound_close(void) 
{ 
	TIM6->CR1&=~(1<<0);				//关闭定时器6
	VS_Set_Vol(0);					//设置音量为0 	
} 
//NES音频输出到VS1053缓存
void nes_apu_fill_buffer(int samples,u8* wavebuf)
{	 
 	u16	i;	
	u8 tbuf;
	for(i=0;i<APU_PCMBUF_SIZE;i++)
	{
		nesapusbuf[nessavebuf][i]=wavebuf[i]; 
	}
	tbuf=nessavebuf;
	tbuf++;
	if(tbuf>(NES_APU_BUF_NUM-1))tbuf=0;
	if(tbuf!=nesplaybuf)
		nessavebuf=tbuf; 
}	
