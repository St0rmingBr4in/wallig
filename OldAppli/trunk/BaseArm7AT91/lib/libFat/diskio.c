/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#ifdef kUSE_FAT
#include "diskio.h"
#include "StorageDevices/ata.h"
#include "StorageDevices/mmc.h"
#include "StorageDevices/usb.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */

#define MMC		0
#define ATA		1
#define USB		2


static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_initialize();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;

	case MMC :
		result = MMC_disk_initialize();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;

	case USB :
		result = USB_disk_initialize();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_status();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;

	case MMC :
		result = MMC_disk_status();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;

	case USB :
		result = USB_disk_status();
		// translate the reslut code here
		stat = (DRESULT)result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_read(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;

	case MMC :
		result = MMC_disk_read(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;

	case USB :
		result = USB_disk_read(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		result = ATA_disk_write(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;

	case MMC :
		result = MMC_disk_write(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;

	case USB :
		result = USB_disk_write(buff, sector, count);
		// translate the reslut code here
		res = (DRESULT)result;
		return res;
	}
	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		// pre-process here

		result = ATA_disk_ioctl(ctrl, buff);
		// post-process here
		res = (DRESULT)result;
		return res;

	case MMC :
		// pre-process here

		result = MMC_disk_ioctl(ctrl, buff);
		// post-process here
		res = (DRESULT)result;
		return res;

	case USB :
		// pre-process here

		result = USB_disk_ioctl(ctrl, buff);
		// post-process here
		res = (DRESULT)result;
		return res;
	}
	return RES_PARERR;
}

#endif
