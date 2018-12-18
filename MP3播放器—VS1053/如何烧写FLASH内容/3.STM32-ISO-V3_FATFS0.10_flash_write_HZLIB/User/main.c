#include "stm32f10x.h"
#include "..\User\spi_flash\fatfs_flash_spi.h"
#include "..\User\usart\bsp_usart1.h"
#include "..\User\led\bsp_led.h" 
#include "ff.h"

//使用之前：
//1.保证已经把字库文件(songti.xbf, HZLIB.bin, UNIGBK.BIN)拷贝到外部Flash文件系统中，
//	如果没有请先运行(2.STM32-ISO-V3_USB Mass Storage_flash)工程
//2.使用该工程写字库文件需要一定时间请耐心等候保存字库写入成功
//3.使用该工程可以在串口调试助手端查看字库写入进度，建议打开串口调试助手查看
//4.开发板上三个LED灯会指示字库写入情况，每成功写入一个字库文件对应一个LED就亮。

FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT myres;
FIL myfsrc;
UINT mybr;

void fatfs_to_flash(void);

int main(void)
{
	USART1_Config();
	LED_GPIO_Config();
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	printf("this is a HZLIB write test demo \r\n");
	fatfs_to_flash();
	while(1);

}

void fatfs_to_flash(void)
{
	uint32_t write_addr=0,j=0;
	uint8_t tempbuf[256]={0};	
	myres=f_mount(&fs,"0:",1);
/*****************************  emWin程序用到的中文字库 ***************************************/
#if 1
	write_addr=60*4096;	
	
	printf("1.f_mount_xbf res=%d\n",myres);
	myres = f_open(&myfsrc,"0:songti.xbf",FA_OPEN_EXISTING | FA_READ);
	printf("2.f_open_xbf res=%d\n",myres);
	
	for(j=0;j<348;j++)//擦除扇区，起始位置60*4096共1392KB
	{
		SPI_FLASH_SectorErase(write_addr+j*4096);
	}
	j=0;
	while(myres == FR_OK) 
	{
		myres = f_read( &myfsrc, tempbuf, 256, &mybr);//读取数据	 
		//printf("%d.f_read_xbf res=%d\n",j,myres);
		if(myres!=FR_OK)break;			 //执行错误  
		SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //拷贝数据到外部flash上    
		write_addr+=256;				
		j++;
		if(mybr !=256)break;
	} 
	
	f_close(&myfsrc);
	
	SPI_FLASH_BufferRead(tempbuf,60*4096,100);			//读取数据，打印验证
	printf("readbuf_xbf\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n如果tempbuf不全为 FF ，那么说明字库拷贝成功！！！\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED1_ON;
#endif
/*****************************  裸机程序用到的中文字库 ***************************************/
#if 1
	write_addr=4096;	
	
	printf("1.f_mount_bin res=%d\n",myres);
	myres = f_open(&myfsrc,"0:HZLIB.bin",FA_OPEN_EXISTING | FA_READ);

	printf("2.f_open_bin res=%d\n",myres);
	
	for(j=0;j<53;j++)//擦除扇区，起始位置4096共212KB
	{
		SPI_FLASH_SectorErase(write_addr+j*4096);
	}
	j=0;
	while(myres == FR_OK) 
	{
		myres = f_read( &myfsrc, tempbuf, 256, &mybr);//读取数据	 
		//printf("%d.f_read_bin res=%d\n",j,myres);
		if(myres!=FR_OK)break;			 //执行错误  
		SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //拷贝数据到外部flash上    
		write_addr+=256;				
		j++;
		if(mybr !=256)break;
	} 
	
	f_close(&myfsrc);
	
	SPI_FLASH_BufferRead(tempbuf,4096,100);			//读取数据，打印验证
	printf("readbuf_bin\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n如果tempbuf不全为 FF ，那么说明字库拷贝成功！！！\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED2_ON;
#endif	

/************************* emWin程序中关于FatFs文件系统用到的中文字库 ***********************/
#if 1
	write_addr=465*4096;
	printf("1.f_mount_gbk res=%d\n",myres);
	myres = f_open(&myfsrc,"0:UNIGBK.BIN",FA_OPEN_EXISTING | FA_READ);
	printf("2.f_open_gbk res=%d,filesize=%d\n",myres,(int)myfsrc.fsize);
	
	for(j=0;j<43;j++)//擦除扇区，起始位置465*4096共172KB
	{
		SPI_FLASH_SectorErase(write_addr+j*4096);
	}
	j=0;
	while(myres == FR_OK) 
	{
		myres = f_read( &myfsrc, tempbuf, 256, &mybr);//读取数据	 
		//printf("%d.f_read_gbk res=%d\n",j,myres);
		if(myres!=FR_OK)break;			 //执行错误  
		SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //拷贝数据到外部flash上    
		write_addr+=256;				
		j++;
		if(mybr !=256)break;
	} 
	
	f_close(&myfsrc);
	SPI_FLASH_BufferRead(tempbuf,465*4096+174344/2,100);			//读取数据，打印验证
	printf("readbuf_UNIGBK.BIN\n");
	for(j=0;j<100;j++)
		printf("%X",tempbuf[j]);
	printf("\n如果tempbuf不全为 FF ，那么说明字库拷贝成功！！！\n");
	if((tempbuf[0]!=0xff)&&(tempbuf[1]!=0xff))LED3_ON;
#endif		
	myres=f_mount(&fs,"0:",0);
}
/*********************************************END OF FILE**********************/
