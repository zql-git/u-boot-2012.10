#ifndef __MOVI_H__
#define __MOVI_H__

#include <config.h>

#define HSMMC_CHANNEL		0
#define MOVI_INIT_REQUIRED	0

#if defined(CONFIG_S3C6400) || defined(CONFIG_S3C6410) || defined(CONFIG_S3C6430)
#define	TCM_BASE		0x0C004000

#define BL2_BASE		(CONFIG_SYS_PHY_UBOOT_BASE)

#elif defined(CONFIG_S3C2450) || defined(CONFIG_S3C2416)
#define	TCM_BASE		0x40004000
#define BL2_BASE		0x33E00000
#else
# error TCM_BASE or BL2_BASE is not defined
#endif

/* TCM function for bl2 load */
#if defined(CONFIG_S3C6400)
#define CopyMovitoMem(a,b,c,d,e)	(((int(*)(uint, ushort, uint *, uint, int))(*((uint *)(TCM_BASE + 0x8))))(a,b,c,d,e))
#elif defined(CONFIG_S3C6410) || defined(CONFIG_S3C6430)
#define CopyMovitoMem(a,b,c,d,e)	(((int(*)(int, uint, ushort, uint *, int))(*((uint *)(TCM_BASE + 0x8))))(a,b,c,d,e))
#elif defined(CONFIG_S3C2450) || defined(CONFIG_S3C2416)
#define CopyMovitoMem(a,b,c,d)		(((int(*)(uint, ushort, uint *, int))(*((uint *)(TCM_BASE + 0x8))))(a,b,c,d))
#endif

/* size information */
#if defined(CONFIG_S3C6400)
#define SS_SIZE			(4 * 1024)
#define eFUSE_SIZE		(2 * 1024)	// 1.5k eFuse, 0.5k reserved
#else
#define SS_SIZE			(8 * 1024)
#define eFUSE_SIZE		(1 * 1024)	// 0.5k eFuse, 0.5k reserved`
#endif

/* movinand definitions */
#define MOVI_BLKSIZE		512

#ifdef CONFIG_BOOT_MOVINAND
#define MOVI_TOTAL_BLKCNT	*((volatile unsigned int*)(TCM_BASE - 0x4))
#define MOVI_HIGH_CAPACITY	*((volatile unsigned int*)(TCM_BASE - 0x8))
#else
#define MOVI_TOTAL_BLKCNT	7864320 // 7864320 // 3995648 // 1003520 /* static movinand total block count: for writing to movinand when nand boot */
#define MOVI_HIGH_CAPACITY	0
#endif

/* partition information */
#define PART_SIZE_BL		(300 * 1024)
#define PART_SIZE_KERNEL	(4 * 1024 * 1024)
#define PART_SIZE_ROOTFS	(8 * 1024 * 1024)

#define MOVI_LAST_BLKPOS	(MOVI_TOTAL_BLKCNT - (eFUSE_SIZE / MOVI_BLKSIZE))
#define MOVI_BL1_BLKCNT		(SS_SIZE / MOVI_BLKSIZE)
#define MOVI_ENV_BLKCNT		(CONFIG_ENV_SIZE / MOVI_BLKSIZE)
#define MOVI_BL2_BLKCNT		(PART_SIZE_BL / MOVI_BLKSIZE)
#define MOVI_ZIMAGE_BLKCNT	(PART_SIZE_KERNEL / MOVI_BLKSIZE)
#define MOVI_BL2_POS		(MOVI_LAST_BLKPOS - MOVI_BL1_BLKCNT - MOVI_BL2_BLKCNT - MOVI_ENV_BLKCNT)
#define MOVI_ROOTFS_BLKCNT	(PART_SIZE_ROOTFS / MOVI_BLKSIZE)

extern void movi_bl2_copy(void);

#endif /*__MOVI_H__*/
