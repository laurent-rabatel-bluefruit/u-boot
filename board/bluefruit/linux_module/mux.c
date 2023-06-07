#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include "board.h"

static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(uart0_rxd), (MODE(0) | PULLUP_EN | RXACTIVE)},	/* UART0_RXD */
	{OFFSET(uart0_txd), (MODE(0) | PULLUDEN)},		/* UART0_TXD */
	{-1},
};

static struct module_pin_mux uart1_pin_mux[] = {
	{OFFSET(uart1_rxd), (MODE(0) | PULLUP_EN | RXACTIVE)},	/* UART1_RXD */
	{OFFSET(uart1_txd), (MODE(0) | PULLUDEN)},		/* UART1_TXD */
	{-1},
};

static struct module_pin_mux uart2_pin_mux[] = {
	{OFFSET(spi0_sclk), (MODE(1) | PULLUP_EN | RXACTIVE)},	/* UART2_RXD */
	{OFFSET(spi0_d0), (MODE(1) | PULLUDEN)},		/* UART2_TXD */
	{-1},
};

static struct module_pin_mux uart3_pin_mux[] = {
	{OFFSET(spi0_cs1), (MODE(1) | PULLUP_EN | RXACTIVE)},	/* UART3_RXD */
	{OFFSET(ecap0_in_pwm0_out), (MODE(1) | PULLUDEN)},	/* UART3_TXD */
	{-1},
};

static struct module_pin_mux uart4_pin_mux[] = {
	{OFFSET(gpmc_wait0), (MODE(6) | PULLUP_EN | RXACTIVE)},	/* UART4_RXD */
	{OFFSET(gpmc_wpn), (MODE(6) | PULLUDEN)},		/* UART4_TXD */
	{-1},
};

static struct module_pin_mux uart5_pin_mux[] = {
	{OFFSET(lcd_data9), (MODE(4) | PULLUP_EN | RXACTIVE)},	/* UART5_RXD */
	{OFFSET(lcd_data8), (MODE(4) | PULLUDEN)},		/* UART5_TXD */
	{-1},
};
#ifdef CONFIG_MTD_RAW_NAND
static struct module_pin_mux nand_pin_mux[] = {
	{OFFSET(gpmc_ad0),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD0  */
	{OFFSET(gpmc_ad1),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD1  */
	{OFFSET(gpmc_ad2),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD2  */
	{OFFSET(gpmc_ad3),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD3  */
	{OFFSET(gpmc_ad4),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD4  */
	{OFFSET(gpmc_ad5),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD5  */
	{OFFSET(gpmc_ad6),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD6  */
	{OFFSET(gpmc_ad7),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD7  */
#ifdef CONFIG_SYS_NAND_BUSWIDTH_16BIT
	{OFFSET(gpmc_ad8),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD8  */
	{OFFSET(gpmc_ad9),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD9  */
	{OFFSET(gpmc_ad10),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD10 */
	{OFFSET(gpmc_ad11),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD11 */
	{OFFSET(gpmc_ad12),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD12 */
	{OFFSET(gpmc_ad13),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD13 */
	{OFFSET(gpmc_ad14),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD14 */
	{OFFSET(gpmc_ad15),	(MODE(0) | PULLUDDIS | RXACTIVE)}, /* AD15 */
#endif
	{OFFSET(gpmc_wait0),	(MODE(0) | PULLUP_EN | RXACTIVE)}, /* nWAIT */
	{OFFSET(gpmc_wpn),	(MODE(7) | PULLUP_EN)},		   /* nWP */
	{OFFSET(gpmc_csn0),	(MODE(0) | PULLUP_EN)},		   /* nCS */
	{OFFSET(gpmc_wen),	(MODE(0) | PULLDOWN_EN)},	   /* WEN */
	{OFFSET(gpmc_oen_ren),	(MODE(0) | PULLDOWN_EN)},	   /* OE */
	{OFFSET(gpmc_advn_ale),	(MODE(0) | PULLDOWN_EN)},	   /* ADV_ALE */
	{OFFSET(gpmc_be0n_cle),	(MODE(0) | PULLDOWN_EN)},	   /* BE_CLE */
	{-1},
};
#endif

void enable_uart0_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}

void enable_uart1_pin_mux(void)
{
	configure_module_pin_mux(uart1_pin_mux);
}

void enable_uart2_pin_mux(void)
{
	configure_module_pin_mux(uart2_pin_mux);
}

void enable_uart3_pin_mux(void)
{
	configure_module_pin_mux(uart3_pin_mux);
}

void enable_uart4_pin_mux(void)
{
	configure_module_pin_mux(uart4_pin_mux);
}

void enable_uart5_pin_mux(void)
{
	configure_module_pin_mux(uart5_pin_mux);
}

void enable_board_pin_mux(void)
{
#if defined(CONFIG_MTD_RAW_NAND)
	//configure_module_pin_mux(nand_pin_mux);
#endif
}
