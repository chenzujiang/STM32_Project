/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stm32_eval_sdio_sd.h"		/* Example: SDC contorl */
#include "my_include.h"

/* Definitions of physical drive number for each drive */
#define SDC		0	/* Example: Map SD card drive to drive number 0 */




/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SDC :
		stat = 0;

		// translate the reslut code here

		return stat;

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SDC :
		result = SD_Init();
		stat = result;
		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SDC :
		// translate the arguments here
		if(count == 1)  //调用单块读写
		{
			result = SD_ReadBlock(buff,sector * BLOCK_SIZE,BLOCK_SIZE);
			result = SD_WaitReadOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);
		}
		else if(count > 1)//调用多块读写
		{
			result = SD_ReadMultiBlocks(buff,sector * BLOCK_SIZE,BLOCK_SIZE,count);
			result = SD_WaitReadOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);
		}
		res = result;
		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SDC :
		// translate the arguments here
		if(count == 1) //如果为单个扇区
		{
			result = SD_WriteBlock((uint8_t *)buff,sector * BLOCK_SIZE,BLOCK_SIZE);
			result = SD_WaitWriteOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);
		}
		else if(count > 1) //如果为多个扇区
		{
			result = SD_WriteMultiBlocks((uint8_t *)buff,sector * BLOCK_SIZE,BLOCK_SIZE,count);
			result = SD_WaitWriteOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);
		}

		// translate the reslut code here
		res = result;
		return res;
	}

	return RES_PARERR;
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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SDC :

		// Process of the command for the ATA drive

		return res;
	}

	return RES_PARERR;
}

#endif

DWORD get_fattime (void)
{
	time_t now_tm;
	SYS_TM *P_tm;
	DWORD Dat = 0,val;
	now_tm = time(NULL);
	P_tm = localtime(&now_tm);
	
	val = P_tm->tm_year - 80;
	Dat = val << 25;
	
	val = P_tm->tm_mon + 1;
	Dat |= (val << 21);
	
	val = P_tm->tm_mday;
	Dat |= (val << 16);
	
	val = P_tm->tm_hour;
	Dat |= (val << 11);
	
	val = P_tm->tm_min;
	Dat |= (val << 5);
	
	val = P_tm->tm_sec;
	Dat |= (val >> 1);
	return Dat;
}



