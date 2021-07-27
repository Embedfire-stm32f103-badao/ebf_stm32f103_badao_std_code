/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "string.h"	 


#ifndef FATFS_FLASH_SPI
	#define FATFS_FLASH_SPI				1
#endif

#ifndef TM_FATFS_CUSTOM_FATTIME
	#define TM_FATFS_CUSTOM_FATTIME		0
#endif

#ifndef FATFS_USE_SDIO
	#define FATFS_USE_SDIO			0
#endif

/* Include SD card files if is enabled */
#if FATFS_USE_SDIO == 1
	#include "bsp_sdio_sdcard.h"
	extern SD_CardInfo SDCardInfo;
#endif

#if FATFS_FLASH_SPI == 1
	#include "bsp_spi_flash.h"
#endif


/* Definitions of physical drive number for each media */
#define ATA			    0
#define SPI_FLASH		1

#define SD_BLOCK_SIZE            512 /* Block Size in Bytes */


__align(4) uint8_t align_buffer[SD_BLOCK_SIZE];						  


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS status = STA_NOINIT;
	FLASH_DEBUG_FUNC() ;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
    			FLASH_DEBUG("SD init");
				  status = SD_Init();
					if (status!=SD_OK )
					{
						status = STA_NOINIT;
					}
					else
					{
						status = RES_OK;
					}
			#endif
			break;
		
		case SPI_FLASH:
			#if	FATFS_FLASH_SPI ==1
			FLASH_DEBUG("SPI FLASH init");
			status = TM_FATFS_FLASH_SPI_disk_initialize();
			#endif
			break;

		default:
			status = STA_NOINIT;
	}
	
	return status;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{

	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
			#if	FATFS_FLASH_SPI ==1
			status = TM_FATFS_FLASH_SPI_disk_status();	/* SDIO communication */
			#endif
			break;

		default:
			status = STA_NOINIT;
	}
	
	return status;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1    

        if((uint32_t)buff%4 != 0)    //传入的buff数据地址不是四字节对齐，需要额外处理
          {
            uint8_t i;
            for(i=0;i<count;i++)
            {
            
              SD_ReadBlock(align_buffer, (uint64_t)sector*SD_BLOCK_SIZE+SD_BLOCK_SIZE*i, SD_BLOCK_SIZE);

                  /* Check if the Transfer is finished */
              SD_WaitReadOperation();  //循环查询dma传输是否结束
            
                /* Wait until end of DMA transfer */
              while(SD_GetStatus() != SD_TRANSFER_OK);
              
              memcpy(buff,align_buffer,SD_BLOCK_SIZE);
              
              buff+=SD_BLOCK_SIZE;
            }

              }
          else        //传入的buff数据地址四字节对齐，直接读取
            {  
                if (count > 1)
                {
                  SD_ReadMultiBlocks(buff, (uint64_t)sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);
                
                      /* Check if the Transfer is finished */
                     SD_WaitReadOperation();  //循环查询dma传输是否结束
                
                    /* Wait until end of DMA transfer */
                    while(SD_GetStatus() != SD_TRANSFER_OK);

                }
                else
                {
                  
                  SD_ReadBlock(buff, (uint64_t)sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE);

                      /* Check if the Transfer is finished */
                     SD_WaitReadOperation();  //循环查询dma传输是否结束
                
                    /* Wait until end of DMA transfer */
                    while(SD_GetStatus() != SD_TRANSFER_OK);

                }
             }
        
				status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
					status = TM_FATFS_FLASH_SPI_disk_read(buff, sector, count);	/* SDIO communication */
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	
	return status;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
      if((uint32_t)buff%4 != 0)   //若传入的buff地址不是4字节对齐，需要额外处理
      {
        uint8_t i;
        for(i=0;i<count;i++)
        {
          memcpy(align_buffer,buff,SD_BLOCK_SIZE);
          SD_WriteBlock(align_buffer,(uint64_t)sector*SD_BLOCK_SIZE+SD_BLOCK_SIZE*i,SD_BLOCK_SIZE);//单个sector的写操作
          
          /* Check if the Transfer is finished */
          SD_WaitWriteOperation();	                  //等待dma传输结束
          while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
          
          buff+=SD_BLOCK_SIZE;
        }
        
      }
      else                          //4字节对齐，不需要额外处理
      {    
					if (count > 1)
					{
						SD_WriteMultiBlocks((uint8_t *)buff, (uint64_t)sector*SD_BLOCK_SIZE,SD_BLOCK_SIZE, count);						
						/* Check if the Transfer is finished */
						SD_WaitWriteOperation();	   //等待dma传输结束
						while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
					}
					else
					{
						SD_WriteBlock((uint8_t *)buff,(uint64_t)sector*SD_BLOCK_SIZE,SD_BLOCK_SIZE);						
						/* Check if the Transfer is finished */
						SD_WaitWriteOperation();	   //等待dma传输结束
						while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
					}
        }
					status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
					status = TM_FATFS_FLASH_SPI_disk_write((BYTE *)buff, sector, count);	/* SDIO communication */
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				switch (cmd) 
				{				
					case GET_SECTOR_SIZE :     // Get R/W sector size (WORD) 
						*(WORD * )buff = 512;
					break;
					case GET_BLOCK_SIZE:      // Get erase block size in unit of sector (DWORD)
						*(DWORD * )buff = 1;
					break;
					case GET_SECTOR_COUNT:
						*(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
					case CTRL_SYNC :
					break;
				}
				status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
					status = TM_FATFS_FLASH_SPI_disk_ioctl(cmd, buff);	/* SDIO communication */
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif

__weak DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
