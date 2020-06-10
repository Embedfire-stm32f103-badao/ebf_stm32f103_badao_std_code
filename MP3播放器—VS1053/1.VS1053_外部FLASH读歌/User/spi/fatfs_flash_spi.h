#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

#include "diskio.h"
#include "integer.h"

//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF



/*SPI接口*/
#define FLASH_SPI                           SPI1
#define FLASH_SPI_CLK                       RCC_APB2Periph_SPI1

#define FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA

#define FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA

#define FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA

#define FLASH_CS_PIN                        GPIO_Pin_4                  /* PA.04 */
#define FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA


#define SPI_FLASH_CS_LOW()      GPIO_ResetBits(FLASH_CS_GPIO_PORT , FLASH_CS_PIN)				//输出低电平 GPIO_ResetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()     GPIO_SetBits(FLASH_CS_GPIO_PORT , FLASH_CS_PIN)			  //设置为高电平 GPIO_SetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         0
#define FLASH_DEBUG_FUNC_ON    0

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define FLASH_DEBUG_FUNC()               do{\
                                         if(FLASH_DEBUG_FUNC_ON)\
                                         printf("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)



DSTATUS TM_FATFS_FLASH_SPI_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_SPI_disk_status(void) ;
DRESULT TM_FATFS_FLASH_SPI_disk_ioctl(BYTE cmd, char *buff) ;
DRESULT TM_FATFS_FLASH_SPI_disk_read(BYTE *buff, DWORD sector, UINT count) ;
DRESULT TM_FATFS_FLASH_SPI_disk_write(BYTE *buff, DWORD sector, UINT count) ;

void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, uint32_t NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
int GetGBKCode_from_EXFlash(unsigned char* pBuffer,const unsigned char * c);
#endif /* __SPI_FLASH_H */

