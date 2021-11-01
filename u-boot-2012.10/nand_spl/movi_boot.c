#include <common.h>

#ifdef CONFIG_MOVINAND
#if defined(CONFIG_SMDK6410)
#include <asm/arch/s3c6410.h>
#endif
#include <movi.h>
#include <asm/io.h>

void movi_bl2_copy(void)
{
	__attribute__((noreturn)) void (*uboot)(void);
#if defined(CONFIG_S3C6400)
	CopyMovitoMem(MOVI_BL2_POS, MOVI_BL2_BLKCNT, (uint *)BL2_BASE, CONFIG_SYS_CLK_FREQ, MOVI_INIT_REQUIRED);
#else
	writel(readl(SDHCI_CONTROL4) | (0x3 << 16), SDHCI_CONTROL4);
	CopyMovitoMem(HSMMC_CHANNEL, MOVI_BL2_POS, MOVI_BL2_BLKCNT, (uint *)BL2_BASE, MOVI_INIT_REQUIRED);
#endif
	/*
	 * Jump to U-Boot image
	 */
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;
	(*uboot)();
}
#endif
