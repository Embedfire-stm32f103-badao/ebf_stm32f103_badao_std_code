#ifndef __VS1053_H__
#define __VS1053_H__

#include "stm32f10x.h"

//////////////////////////////////////////////////////////////
/*定义SPI2作为VS1053的硬件接口*/
#define VS_XCS								GPIO_Pin_12								/*定义VS1053的片选管脚*/
#define VS_SCLK								GPIO_Pin_13								/*定义VS1053的时钟管脚*/
#define VS_MISO								GPIO_Pin_14								/*定义VS1053的MISO管脚*/
#define VS_MOSI								GPIO_Pin_15								/*定义VS1053的MOSI管脚*/
#define VS_SPIGPIO_PORT				GPIOB											/* GPIO端口 */
#define VS_SPIGPIO_CLK				RCC_APB2Periph_GPIOB			/* GPIO端口时钟 */
#define VS_SPI								SPI2
#define VS_SPI_CLK						RCC_APB1Periph_SPI2


#define VS_XDCS								GPIO_Pin_6								/*定义VS1053的片选管脚*/
#define VS_GPIO_XDCS_PORT			GPIOE											/* GPIO端口 */
#define VS_GPIO_XDCS_CLK			RCC_APB2Periph_GPIOE			/* GPIO端口时钟 */

#define VS_GPIO_DREQ_PORT			GPIOE									/* GPIO端口 */
#define VS_GPIO_DREQ_CLK			RCC_APB2Periph_GPIOE					/* GPIO端口时钟 */
#define VS_DREQ								GPIO_Pin_3								/*定义VS1053的DREQ管脚*/

#define VS_GPIO_RST_PORT			GPIOC									/* GPIO端口 */
#define VS_GPIO_RST_CLK				RCC_APB2Periph_GPIOC					/* GPIO端口时钟 */
#define VS_RST								GPIO_Pin_3							/*定义VS1053的RST管脚*/

#define VS_DREQ_IN						GPIO_ReadInputDataBit(VS_GPIO_DREQ_PORT,VS_DREQ)

#define VS_XDCS_SET						GPIO_SetBits(VS_GPIO_XDCS_PORT,VS_XDCS)
#define VS_XDCS_CLR						GPIO_ResetBits(VS_GPIO_XDCS_PORT,VS_XDCS)

#define VS_XCS_SET						GPIO_SetBits(VS_SPIGPIO_PORT,VS_XCS)
#define VS_XCS_CLR						GPIO_ResetBits(VS_SPIGPIO_PORT,VS_XCS)

#define VS_RST_SET						GPIO_SetBits(VS_GPIO_RST_PORT,VS_RST)
#define VS_RST_CLR						GPIO_ResetBits(VS_GPIO_RST_PORT,VS_RST)
//////////////////////////////////////////////////////////////

__packed typedef struct 
{							  
	u8 mvol;		//主音量,范围:0~254
	u8 bflimit;		//低音频率限定,范围:2~15(单位:10Hz)
	u8 bass;		//低音,范围:0~15.0表示关闭.(单位:1dB)
	u8 tflimit;		//高音频率限定,范围:1~15(单位:Khz)
	u8 treble;		//高音,范围:0~15(单位:1.5dB)(原本范围是:-8~7,通过函数修改了);
	u8 effect;		//空间效果设置.0,关闭;1,最小;2,中等;3,最大.
	u8 speed;			//播放速度：0，1:正常播放，2:2倍速度，3:3倍速度...
}_vs1053_obj;

__packed typedef struct
{
	u8 input;						//输入通道选择.0：MICP，1：LINE1
	u8 samplerate;			//采样速率选择（x*8K）：1：8K，2：16K，3：24K...
	u8 channel;					//声道：1：双声道，2：左声道，3：右声道
	u8 agc;							//增益：1~64
}_recorder_obj;

extern _vs1053_obj vsset;		//VS1053设置


#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS1053寄存器定义
#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f 

#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 		 

#define I2S_CONFIG			0XC040
#define GPIO_DDR			0XC017
#define GPIO_IDATA			0XC018
#define GPIO_ODATA			0XC019



u16  VS_RD_Reg(u8 address);				//读寄存器
u16  VS_WRAM_Read(u16 addr);	    	//读RAM
void VS_WRAM_Write(u16 addr,u16 val);	//写RAM
void VS_WR_Data(u8 data);				//写数据
void VS_WR_Cmd(u8 address,u16 data);	//写命令
void VS_stop(void);
u8   VS_HD_Reset(void);			    	//硬复位
void VS_Soft_Reset(void);           	//软复位
u16 VS_Ram_Test(void);             		//RAM测试	    
void VS_Sine_Test(void);            	//正弦测试
												
void VS_SPI_SpeedHigh(void);
void VS_SPI_SpeedLow(void);
u8 	 VS_SPI_ReadWriteByte(u8 data);
void VS_Init(void);						//初始化VS10XX	 
void VS_Set_Speed(u8 t);				//设置播放速度
u16  VS_Get_HeadInfo(void);     		//得到比特率
u32 VS_Get_ByteRate(void);				//得到字节速率
u16 VS_Get_EndFillByte(void);			//得到填充字节
u8 	 VS_Send_MusicData(u8* buf);		//向VS10XX发送32字节
void VS_Restart_Play(void);				//重新开始下一首歌播放	  
void VS_Reset_DecodeTime(void);			//重设解码时间
u16  VS_Get_DecodeTime(void);   		//得到解码时间

void VS_Load_Patch(u16 *patch,u16 len);	//加载用户patch
//u8 	 VS_Get_Spec(u16 *p);       		//得到分析数据	   
//void VS_Set_Bands(u16 *buf,u8 bands);	//设置中心频率
void VS_Set_Vol(u8 volx);				//设置主音量   
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble);//设置高低音
void VS_Set_Effect(u8 eft);				//设置音效
void VS_Set_All(void);
/*--------以下是录音功能------------*/
 //RIFF块
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;这里固定为"RIFF",即0X46464952
    u32 ChunkSize ;		   	//集合大小;文件总大小-8
    u32 Format;	   			//格式;WAVE,即0X45564157
}ChunkRIFF ;
//fmt块
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;这里固定为"fmt ",即0X20746D66
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:20.
    u16 AudioFormat;	  	//音频格式;0X10,表示线性PCM;0X11表示IMA ADPCM
	u16 NumOfChannels;		//通道数量;1,表示单声道;2,表示双声道;
	u32 SampleRate;			//采样率;0X1F40,表示8Khz
	u32 ByteRate;			//字节速率; 
	u16 BlockAlign;			//块对齐(字节); 
	u16 BitsPerSample;		//单个采样数据大小;4位ADPCM,设置为4
//	u16 ByteExtraData;		//附加的数据字节;2个; 线性PCM,没有这个参数
//	u16 ExtraData;			//附加的数据,单个采样数据块大小;0X1F9:505字节  线性PCM,没有这个参数
}ChunkFMT;	   
//fact块 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;这里固定为"fact",即0X74636166;
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4.
    u32 NumOfSamples;	  	//采样的数量; 
}ChunkFACT;
//data块 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;这里固定为"data",即0X61746164
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);文件大小-60.
}ChunkDATA;

//wav头
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff块
	ChunkFMT fmt;  	//fmt块
	//ChunkFACT fact;	//fact块 线性PCM,没有这个结构体	 
	ChunkDATA data;	//data块		 
}__WaveHeader; 

void recoder_enter_rec_mode(_recorder_obj *recset);
//void recoder_wav_init(__WaveHeader* wavhead);
/*-----------  结束录音功能 -------------------*/
#endif
