/*
 * (C) Copyright 2012 SAMSUNG Electronics
 * Jaehoon Chung <jh80.chung@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/s3c6410.h>

#ifdef CONFIG_MMC_CHANNEL
#define MMC_CHANNEL CONFIG_MMC_CHANNEL
#else
#define    MMC_CHANNEL 0
#endif

#define MMC_REGS_BASE        (ELFIN_HSMMC_BASE + 0x100000*MMC_CHANNEL)

static char *S3C_NAME = "SAMSUNG SDHCI";
static void sdhc_set_gpio()
{
    u32 reg;
#if (MMC_CHANNEL == 0)
    reg = readl(GPGCON) & 0xf0000000;
    writel(reg | 0x02222222, GPGCON);

    reg = readl(GPGPUD) & 0xfffff000;
    writel(reg, GPGPUD);
#elif (MMC_CHANNEL == 1)
    writel(0x00222222, GPHCON0);
    writel(0x00000000, GPHCON1);

    reg = readl(GPHPUD) & 0xfffff000;
    writel(reg, GPHPUD);
#else
    printf("#####err: SDMMC channel is not defined!\n");
#endif
}

static void s3c_sdhci_set_control_reg(struct sdhci_host *host)
{
	unsigned long val, ctrl;
	/*
	 * SELCLKPADDS[17:16]
	 * 00 = 2mA
	 * 01 = 4mA
	 * 10 = 7mA
	 * 11 = 9mA
	 */
	sdhci_writel(host, SDHCI_CTRL4_DRIVE_MASK(0x3), SDHCI_CONTROL4);

	val = sdhci_readl(host, SDHCI_CONTROL2);
	val &= SDHCI_CTRL2_SELBASECLK_SHIFT;

	val |=	SDHCI_CTRL2_ENSTAASYNCCLR |
		SDHCI_CTRL2_ENCMDCNFMSK |
		SDHCI_CTRL2_ENFBCLKRX |
		SDHCI_CTRL2_ENCLKOUTHOLD;

	sdhci_writel(host, val, SDHCI_CONTROL2);

	/*
	 * FCSEL3[31] FCSEL2[23] FCSEL1[15] FCSEL0[7]
	 * FCSel[1:0] : Rx Feedback Clock Delay Control
	 *	Inverter delay means10ns delay if SDCLK 50MHz setting
	 *	01 = Delay1 (basic delay)
	 *	11 = Delay2 (basic delay + 2ns)
	 *	00 = Delay3 (inverter delay)
	 *	10 = Delay4 (inverter delay + 2ns)
	 */
	val = SDHCI_CTRL3_FCSEL0 | SDHCI_CTRL3_FCSEL1;
	sdhci_writel(host, val, SDHCI_CONTROL3);

	/*
	 * SELBASECLK[5:4]
	 * 00/01 = HCLK
	 * 10 = EPLL
	 * 11 = XTI or XEXTCLK
	 */
	ctrl = sdhci_readl(host, SDHCI_CONTROL2);
	ctrl &= ~SDHCI_CTRL2_SELBASECLK_MASK(0x3);
	ctrl |= SDHCI_CTRL2_SELBASECLK_MASK(0x2);
	sdhci_writel(host, ctrl, SDHCI_CONTROL2);
}

int s3c_sdhci_init(u32 regbase, int index, int bus_width)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
	if (!host) {
		printf("sdhci__host malloc fail!\n");
		return 1;
	}
	sdhc_set_gpio();
	host->name = S3C_NAME;
	host->ioaddr = (void *)regbase;

	host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE |
		SDHCI_QUIRK_BROKEN_R1B | SDHCI_QUIRK_32BIT_DMA_ADDR;
	host->voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

	host->set_control_reg = &s3c_sdhci_set_control_reg;
	//host->set_clock = set_mmc_clk;
	host->index = index;

	host->host_caps = MMC_MODE_HC;

	add_sdhci(host, 52000000, 400000);
	return 0;
}

