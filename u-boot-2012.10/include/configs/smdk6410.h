/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * Configuation settings for the SAMSUNG SMDK6410(mDirac-III) board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_S3C6410		1	/* in a SAMSUNG S3C6410 SoC     */
#define CONFIG_S3C64XX		1	/* in a SAMSUNG S3C64XX Family  */
#define CONFIG_SMDK6410		1	/* on a SAMSUNG SMDK6410 Board  */

#define CONFIG_PERIPORT_REMAP
#define CONFIG_PERIPORT_BASE	0x70000000
#define CONFIG_PERIPORT_SIZE	0x13

#define CONFIG_SYS_IRAM_BASE    0x0c000000  /* Internal SRAM base address */
#define CONFIG_SYS_IRAM_SIZE    0x2000      /* 8 KB of internal SRAM memory */
#define CONFIG_SYS_IRAM_END     (CONFIG_SYS_IRAM_BASE + CONFIG_SYS_IRAM_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_IRAM_END - GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_SDRAM_BASE	0x50000000

/* input clock of PLL: SMDK6410 has 12MHz input clock */
#define CONFIG_SYS_CLK_FREQ         12000000

#if !defined(CONFIG_NAND_SPL) && (CONFIG_SYS_TEXT_BASE >= 0xc0000000)
#define CONFIG_ENABLE_MMU
#endif

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

/*
 * Architecture magic and machine type
 */
#define CONFIG_MACH_TYPE        1626

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/*
 * Size of malloc() pool
 */
 
/*
 * Hardware drivers
 */
#define CONFIG_DRIVER_DM9000
#define CONFIG_DM9000_BASE      0x18000300
#define DM9000_IO               CONFIG_DM9000_BASE
#define DM9000_DATA             (CONFIG_DM9000_BASE+0x4)

#define CONFIG_ETHADDR          08:08:10:12:10:27
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_IPADDR           192.168.50.165 
#define CONFIG_SERVERIP         192.168.50.162
#define CONFIG_GATEWAYIP        192.168.50.1

#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_MMC
#define CONFIG_SDHCI
#define CONFIG_MMC_SDMA
#define CONFIG_S3C6410_SDHCI
#define CONFIG_CMD_FAT


/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1          1	/* we use SERIAL 1 on SMDK6410	*/

#define CONFIG_SYS_HUSH_PARSER			/* use "hush" command parser	*/

#define CONFIG_CMDLINE_EDITING

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_NAND
#if defined(CONFIG_BOOT_ONENAND)
#define CONFIG_CMD_ONENAND
#endif
#define CONFIG_CMD_PING
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2

#define CONFIG_BOOTDELAY	3

#define CONFIG_ZERO_BOOTDELAY_CHECK

#if (CONFIG_COMMANDS & CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	1	/* which serial port to use	 */
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP				/* undef to save memory	      */
#define CONFIG_SYS_PROMPT		"SMDK6410 # "	/* Monitor Command Prompt     */
#define CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size    */
#define CONFIG_SYS_PBSIZE		384		/* Print Buffer Size          */
#define CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size  */

#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE	/* memtest works on	      */
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x10000000) /* 256MB in DRAM */

#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE	/* default load address	*/

#define CONFIG_SYS_HZ			1000

/**********************************
 Support Clock Settings
 **********************************
 Setting	SYNC	ASYNC
 ----------------------------------
 667_133_66	 X	  O
 533_133_66	 O	  O
 400_133_66	 X	  O
 400_100_50	 O	  O
 **********************************/

/*#define CONFIG_CLK_667_133_66*/
#define CONFIG_CLK_533_133_66
/*
#define CONFIG_CLK_400_100_50
#define CONFIG_CLK_400_133_66
#define CONFIG_SYNC_MODE
*/

/* SMDK6410 has 2 banks of DRAM, but we use only one in U-Boot */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE	/* SDRAM Bank #1	*/
#define PHYS_SDRAM_1_SIZE	0x10000000	/* 256 MB in Bank #1	*/

#define CONFIG_SYS_FLASH_BASE		0x10000000
#define CONFIG_SYS_MONITOR_BASE		0x00000000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks	*/
/* AM29LV160B has 35 sectors, AM29LV800B - 19 */
#define CONFIG_SYS_MAX_FLASH_SECT	40

#define CONFIG_AMD_LV800
#define CONFIG_SYS_FLASH_CFI		1	/* Use CFI parameters (needed?) */
/* Use drivers/cfi_flash.c, even though the flash is not CFI-compliant	*/
#define CONFIG_FLASH_CFI_DRIVER	1
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_FLASH_CFI_LEGACY
#define CONFIG_SYS_FLASH_LEGACY_512Kx16

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(5 * CONFIG_SYS_HZ) /* Timeout for Flash Erase	*/
#define CONFIG_SYS_FLASH_WRITE_TOUT	(5 * CONFIG_SYS_HZ) /* Timeout for Flash Write	*/


/*
 * SMDK6410 board specific data
 */

#define CONFIG_IDENT_STRING	" for SMDK6410"

/* base address for uboot */
#define CONFIG_SYS_PHY_UBOOT_BASE	(CONFIG_SYS_SDRAM_BASE + 0x0fe00000)
/* total memory available to uboot */
#define CONFIG_SYS_UBOOT_SIZE		(1024 * 1024)

/* Put environment copies after the end of U-Boot owned RAM */
#define CONFIG_NAND_ENV_DST	(CONFIG_SYS_UBOOT_BASE + CONFIG_SYS_UBOOT_SIZE)

#ifdef CONFIG_ENABLE_MMU
#define CONFIG_SYS_MAPPED_RAM_BASE	0xc0000000
#else
#define CONFIG_SYS_MAPPED_RAM_BASE	CONFIG_SYS_SDRAM_BASE
#endif

/* NAND U-Boot load and start address */
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SYS_MAPPED_RAM_BASE + 0x0fe00000)

/* NAND configuration */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x70200010 
#define CONFIG_SYS_S3C_NAND_HWECC
#define CONFIG_NAND_BL1_8BIT_ECC


#define CONFIG_CMD_NAND_LOCK_UNLOCK

#define CONFIG_CMD_NAND_YAFFS       1

#define CONFIG_SYS_NAND_SKIP_BAD_DOT_I	1  /* ".i" read skips bad blocks	      */
#define CONFIG_SYS_NAND_WP		1
#define CONFIG_SYS_NAND_YAFFS_WRITE	1  /* support yaffs write		      */

#define CONFIG_SYS_NAND_BBT_2NDPAGE	1  /* bad-block markers in 1st and 2nd pages  */

#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_PHY_UBOOT_BASE	/* NUB load-addr      */
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST	/* NUB start-addr     */

#define CONFIG_SYS_NAND_U_BOOT_OFFS	(16 * 1024)	/* Offset to RAM U-Boot image */
#define CONFIG_SYS_NAND_U_BOOT_SIZE	(2 * 1024 * 1024)	/* Size of RAM U-Boot image   */

/* NAND chip page size		*/
#define CONFIG_SYS_NAND_PAGE_SIZE	4096//2048
/* NAND chip block size		*/
#define CONFIG_SYS_NAND_BLOCK_SIZE	(512 * 1024)  /* one block 512K*/
/* NAND chip page per block count  */
#define CONFIG_SYS_NAND_PAGE_COUNT	(128)          /*  one block  has 128 pages */
/* Location of the bad-block label */
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
/* Extra address cycle for > 128MiB */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE

/* Size of the block protected by one OOB (Spare Area in Samsung terminology) */
#define CONFIG_SYS_NAND_ECCSIZE		512
/* Number of ECC bytes per OOB - S3C6410 calculates 4 bytes ECC in 1-bit mode */
#define CONFIG_SYS_NAND_ECCBYTES	13
/* Size of a single OOB region */
#define CONFIG_SYS_NAND_OOBSIZE		128
/* ECC byte positions */
#define CONFIG_SYS_NAND_ECCPOS	{24,25,26,27,28,29,30,31,32,33,\
        34,35,36,37,38,39,40,41,42,43,\
        44,45,46,47,48,49,50,51,52,53,\
        54,55,56,57,58,59,60,61,62,63,\
        64,65,66,67,68,69,70,71,72,73,\
        74,75,76,77,78,79,80,81,82,83,\
        84,85,86,87,88,89,90,91,92,93,\
        94,95,96,97,98,99,100,101,102,103,\
        104,105,106,107,108,109,110,111,112,113,\
        114,115,116,117,118,119,120,121,122,123,\
        124,125,126,127}

/* Boot configuration (define only one of next 3) */
/*----------------------------------------------------------------------------*/
//#define CONFIG_BOOT_NAND
#define CONFIG_BOOT_MOVINAND
/*----------------------------------------------------------------------------*/


/* None of these are currently implemented. Left from the original Samsung
 * version for reference
#define CONFIG_BOOT_NOR
#define CONFIG_BOOT_MOVINAND
#define CONFIG_BOOT_ONENAND
*/

#define CONFIG_NAND
#define CONFIG_NAND_S3C64XX

#ifdef	CONFIG_BOOT_MOVINAND
#define CONFIG_MOVINAND
#define CONFIG_BOOTCOMMAND "fatload mmc 0:1 50008000 u-boot-nand.bin; nand erase.chip;nand write.uboot 50008000 0 0x200000;fatload mmc 0:1 0x50008000 zImage;nand write.e 0x50008000 0x500000 0x500000;fatload mmc 0:1 0x50008000 rootfs.yaffs2; nand write.yaffs2 0x50008000 0x01e00000 $filesize;"

#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 1024*1024)
#define CONFIG_ENV_SIZE		    0x4000	/* Total Size of Environment Sector 16K*/
#define CONFIG_ENV_OFFSET		0x00047000  /* 284K */

#else
#define CONFIG_BOOTCOMMAND "nand read 0x50018000 0x500000 0x400000;bootm 0x50018000"
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 512 * 1024)
#define CONFIG_ENV_SIZE		    0x80000	    /* Total Size of Environment Sector */ //512K Bytes
#define CONFIG_ENV_OFFSET		0x100000    /* 1M */
#endif
/* Unimplemented or unsupported. See comment above.
#define CONFIG_ONENAND
#define CONFIG_MOVINAND
*/

/* Settings as above boot configuration */
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_BOOTARGS		"root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200"

#if 0//!defined(CONFIG_ENABLE_MMU)
#define CONFIG_CMD_USB			1
#define CONFIG_USB_S3C64XX
#define CONFIG_USB_OHCI_NEW		1
#define CONFIG_SYS_USB_OHCI_REGS_BASE		0x74300000
#define CONFIG_SYS_USB_OHCI_SLOT_NAME		"s3c6410"
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	3
#define CONFIG_SYS_USB_OHCI_CPU_INIT		1

#define CONFIG_USB_STORAGE	                1
#endif
#define CONFIG_DOS_PARTITION	            1

#if defined(CONFIG_USB_OHCI_NEW) && defined(CONFIG_ENABLE_MMU)
# error "usb_ohci.c is currently broken with MMU enabled."
#endif

#endif	/* __CONFIG_H */
