#include <common.h>
#include <dm.h>
#include <env.h>
#include <errno.h>
#include <image.h>
#include <init.h>
#include <malloc.h>
#include <net.h>
#include <spl.h>
#include <serial.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/clk_synthesizer.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mem.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <asm/omap_common.h>
#include <asm/omap_sec_common.h>
#include <asm/omap_mmc.h>
#include <miiphy.h>
#include <cpsw.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <env_internal.h>
#include <watchdog.h>
#include "board.h"

DECLARE_GLOBAL_DATA_PTR;

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;

#define GPIO0_RISINGDETECT	(AM33XX_GPIO0_BASE + OMAP_GPIO_RISINGDETECT)
#define GPIO1_RISINGDETECT	(AM33XX_GPIO1_BASE + OMAP_GPIO_RISINGDETECT)

#define GPIO0_IRQSTATUS1	(AM33XX_GPIO0_BASE + OMAP_GPIO_IRQSTATUS1)
#define GPIO1_IRQSTATUS1	(AM33XX_GPIO1_BASE + OMAP_GPIO_IRQSTATUS1)

#define GPIO0_IRQSTATUSRAW	(AM33XX_GPIO0_BASE + 0x024)
#define GPIO1_IRQSTATUSRAW	(AM33XX_GPIO1_BASE + 0x024)

#ifndef CONFIG_DM_SERIAL
struct serial_device *default_serial_console(void)
{
	return &eserial1_device;
}
#endif

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
static const struct ddr_data ddr3_AM335x_LINUX_MODULE_ddr_data	= {
	.datardsratio0 =	0x00000040,
	.datawdsratio0 =	0x00000080,
	.datafwsratio0 =	0x000000EB,
	.datawrsratio0 = 	0x000000C0
};

static const struct cmd_control ddr3_AM335x_LINUX_MODULE_cmd_ctrl_data	= {
	.cmd0csratio = 	0x00000100,
	.cmd0iclkout = 	0x00000001,
	.cmd1csratio = 	0x00000100,
	.cmd1iclkout = 	0x00000001,
	.cmd2csratio = 	0x00000100,
	.cmd2iclkout = 	0x00000001
};

static struct emif_regs ddr3_AM335x_LINUX_MODULE_emif_reg_data	= {
	.sdram_config = 	0x61A05332,
	.ref_ctrl = 	0x00000C30,
	.sdram_tim1 = 	0x0AAAD4DB,
	.sdram_tim2 = 	0x246B7FDA,
	.sdram_tim3 = 	0x50FFE67F,
	.zq_config = 	0x50074BE1,
	.emif_ddr_phy_ctlr_1 = 	0x00100208
};

const struct ctrl_ioregs ddr3_AM335x_LINUX_MODULE_ioregs_data	= {
	.cm0ioctl =	0x0000018B,
	.cm1ioctl =	0x0000018B,
	.cm2ioctl =	0x0000018B,
	.dt0ioctl =	0x0000018B,
	.dt1ioctl =	0x0000018B
};

#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
#ifdef CONFIG_SPL_SERIAL_SUPPORT
	/* break into full u-boot on 'c' */
	if (serial_tstc() && serial_getc() == 'c')
		return 1;
#endif

#ifdef CONFIG_SPL_ENV_SUPPORT
	env_init();
	env_load();
	if (env_get_yesno("boot_os") != 1)
		return 1;
#endif

	return 0;
}
#endif

const struct dpll_params *get_dpll_ddr_params(void)
{
	int ind = get_sys_clk_index();

	return &dpll_ddr3_400MHz[ind];
}

const struct dpll_params *get_dpll_mpu_params(void)
{
	int ind = get_sys_clk_index();
	int freq = am335x_get_efuse_mpu_max_freq(cdev);

	switch (freq) {
	case MPUPLL_M_1000:
		return &dpll_mpu_opp[ind][5];
	case MPUPLL_M_800:
		return &dpll_mpu_opp[ind][4];
	case MPUPLL_M_720:
		return &dpll_mpu_opp[ind][3];
	case MPUPLL_M_600:
		return &dpll_mpu_opp[ind][2];
	case MPUPLL_M_500:
		return &dpll_mpu_opp100;
	case MPUPLL_M_300:
		return &dpll_mpu_opp[ind][0];
	}

	return &dpll_mpu_opp[ind][0];
}

void set_uart_mux_conf(void)
{
#if CONFIG_CONS_INDEX == 1
	enable_uart0_pin_mux();
#elif CONFIG_CONS_INDEX == 2
	enable_uart1_pin_mux();
#elif CONFIG_CONS_INDEX == 3
	enable_uart2_pin_mux();
#elif CONFIG_CONS_INDEX == 4
	enable_uart3_pin_mux();
#elif CONFIG_CONS_INDEX == 5
	enable_uart4_pin_mux();
#elif CONFIG_CONS_INDEX == 6
	enable_uart5_pin_mux();
#endif
}

void set_mux_conf_regs(void)
{
	enable_board_pin_mux();
}
void sdram_init(void)
{
	config_ddr(400	,
		&ddr3_AM335x_LINUX_MODULE_ioregs_data,
		&ddr3_AM335x_LINUX_MODULE_ddr_data,
		&ddr3_AM335x_LINUX_MODULE_cmd_ctrl_data,
		&ddr3_AM335x_LINUX_MODULE_emif_reg_data,
		0);
}
#endif

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
#if defined(CONFIG_HW_WATCHDOG)
	hw_watchdog_init();
#endif

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
#if defined(CONFIG_MTD_RAW_NAND)
	gpmc_init();
#endif


	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
		// puts("late init");
	return 0;
}
#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	return 0;
}
#endif

#ifdef CONFIG_TI_SECURE_DEVICE
void board_fit_image_post_process(void **p_image, size_t *p_size)
{
	secure_boot_verify_image(p_image, p_size);
}
#endif

int board_mmc_init(struct bd_info *bis)
{
	return omap_mmc_init(0, 0, 0, -1, -1);
}

