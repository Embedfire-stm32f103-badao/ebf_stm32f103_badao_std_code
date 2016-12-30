#include "stm32f10x.h"
#include "ff.h"
#include "..\User\spi_flash\fatfs_flash_spi.h"
#include "..\User\usart\bsp_usart1.h"
#include "..\User\led\bsp_led.h"  

FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res_flash; 
UINT br, bw;            					/* File R/W count */
BYTE buffer[1024]={0};       		  /* file copy buffer */
BYTE textFileBuffer[] = "欢迎使用野火STM32-V3 ISO开发板  今天是个好日子，新建文件系统测试文件";


static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

int main(void)
{
	//LED初始化
	LED_GPIO_Config();
	
	//擦除整片外部Flash，慎重使用，擦除过程需要一定时间不要中途停止。
//	TM_FATFS_FLASH_SPI_disk_initialize();
//	SPI_FLASH_BulkErase();

	/* 初始化串口1*/
	USART1_Config();
	printf("\r\n this is a fatfs test demo \r\n");
/*************************   flash 文件系统   *********************************************/
	//在外部Flash挂载文件系统
	res_flash = f_mount(&fs,"0:",1);
	printf("\r\n f_mount res_flash=%d \r\n",res_flash);
	
	//如果没有文件系统就格式化创建创建文件系统
	if(res_flash ==FR_NO_FILESYSTEM)
	{
		res_flash=f_mkfs("0:",0,4096);							//格式化
		printf("\r\nmkfs res_flash=%d",res_flash);
		res_flash = f_mount(&fs,"0:",0);						//格式化后，先取消挂载
		res_flash = f_mount(&fs,"0:",1);						//重新挂载
	}
/**************************  flash   *****************************************/		
	//文件系统测试，写测试
	//打开文件，如果文件不参加则创建它
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_CREATE_ALWAYS | FA_WRITE );
	 
	if ( res_flash == FR_OK )
	{
		LED3_ON;
		res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &bw);
		f_close(&fnew);      
	}
	//读测试
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_OPEN_EXISTING | FA_READ); 	 
	res_flash = f_read(&fnew, buffer, sizeof(buffer), &br); 
	printf("\r\n %s ", buffer);
	/* Close open files */
	res_flash = f_close(&fnew);	
	//不再使用文件系统，取消挂载文件系统
	res_flash = f_mount(&fs,"0:",0);
	while(1)
	{
		LED2_TOGGLE;
		Delay(0xfffff);
	}
}

/*********************************************END OF FILE**********************/
