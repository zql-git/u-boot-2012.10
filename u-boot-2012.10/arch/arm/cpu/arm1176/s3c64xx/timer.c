/*
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, ARM Ltd. <philippe.robin@arm.com>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/proc-armv/ptrace.h>

#include <asm/arch/s3c6410.h>

#include <div64.h>

DECLARE_GLOBAL_DATA_PTR;

#define PRESCALER    167


static ulong timestamp;
static ulong lastdec;
int timer_load_val = 0;


static s3c64xx_timers *s3c64xx_get_base_timers(void)
{
    return (s3c64xx_timers *)ELFIN_TIMER_BASE;
}

/* macro to read the 16 bit timer */
static inline ulong read_timer(void)
{
    s3c64xx_timers *const timers = s3c64xx_get_base_timers();

    return timers->TCNTO4;
}


int timer_init(void) 
{
	s3c64xx_timers *const timers = s3c64xx_get_base_timers();

	/* use PWM Timer 4 because it has no output */
	/* prescaler for Timer 4 is 16 */
	timers->TCFG0 = 0x0f00;
	if (timer_load_val == 0) {
		/*
		 * for 10 ms clock period @ PCLK with 4 bit divider = 1/2
		 * (default) and prescaler = 16. Should be 10390
		 * @33.25MHz and 15625 @ 50 MHz
		 */
		timer_load_val = get_PCLK() / (2 * 16 * 100);
	}

	/* load value for 10 ms timeout */
	lastdec = timers->TCNTB4 = timer_load_val;
	/* auto load, manual update of Timer 4 */
	timers->TCON = (timers->TCON & ~0x00700000) | TCON_4_AUTO | TCON_4_UPDATE;
	/* auto load, start Timer 4 */
	timers->TCON = (timers->TCON & ~0x00700000) | TCON_4_AUTO | COUNT_4_ON;
	timestamp = 0;

	return (0);
}
 
/*
 * timer without interrupts
 */

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
    get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
    /* We overrun in 100s */
     return (ulong)(timer_load_val * 100);
}
void reset_timer_masked(void)
{
	/* reset time */
	lastdec = read_timer();
	timestamp = 0;
}

ulong get_timer_masked(void)
{
    ulong now = read_timer();

	if (lastdec >= now) {
		/* normal mode */
		timestamp += lastdec - now;
	}
	else {
		/* we have an overflow ... */
		timestamp += lastdec + timer_load_val - now;
	}
	lastdec = now;

	return timestamp;
}

ulong get_timer(ulong base)
{
    return get_timer_masked() - base;
}

void __udelay(unsigned long usec)
{
    ulong tmo, tmp;

    if (usec >= 1000) {     /* if "big" number, spread normalization to seconds */
        tmo = usec / 1000;  /* start to normalize for usec to ticks per sec */
        tmo *= CONFIG_SYS_HZ;      /* find number of "ticks" to wait to achieve target */
        tmo /= 1000;        /* finish normalize. */
    }
    else {              /* else small number, don't kill it prior to HZ multiply */
        tmo = usec * CONFIG_SYS_HZ;
        tmo /= (1000 * 1000);
    }

    tmp = get_timer(0);     /* get current timestamp */
    if ((tmo + tmp + 1) < tmp)  /* if setting this fordward will roll time stamp */
        reset_timer_masked();   /* reset "advancing" timestamp to 0, set lastdec value */
    else
        tmo += tmp;     /* else, set advancing stamp wake up time */

    while (get_timer_masked()<tmo)  /* loop till event */
         /*NOP*/;
}

