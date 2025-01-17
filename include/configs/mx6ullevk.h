/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6UL 14x14 EVK board.
 */
#ifndef __MX6ULLEVK_CONFIG_H
#define __MX6ULLEVK_CONFIG_H


#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/mach-imx/gpio.h>

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#define PHYS_SDRAM_SIZE	SZ_512M

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configs */
#ifdef CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR

/* NAND pin conflicts with usdhc2 */
#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_SYS_FSL_USDHC_NUM	1
#else
#define CONFIG_SYS_FSL_USDHC_NUM	2
#endif
#endif

/* I2C configs */
#ifdef CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_SPEED		100000
#endif

/* PWM configs */
#define CONFIG_PWM_IMX
#define CONFIG_IMX6_PWM_PER_CLK	24000000

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootdelay=0\0" \
	"image=/kernel/normal/zImage\0" \
	"console=ttymxc0\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"fdt_file=/kernel/normal/imx6ull-14x14-evk.dtb\0" \
	"fdt_addr=0x83000000\0" \
	"boot_fdt=try\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=${mmcroot} ti=${ti}\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"gpio input GPIO1_7; " \
		"if test $? = 0; then " \
			"env set ti 1; " \
		"else " \
			"env set ti 0; " \
		"fi; " \
		"pwm init 0; " \
		"pwm set 0 100000 100000; " \
		"pwm enable 0; " \
		"pwm init 2; " \
		"pwm set 2 100000 100000; " \
		"pwm enable 2; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"loadbootenv_addr=0x80000000\0" \
	"bootdev=1\0" \
	"partid=auto\0" \
	"bootenv=uEnv.txt\0" \
	"custombootcmd=echo Failed to load uEnv.txt; run loadimage; run mmcboot\0" \
	"uEnvtxt_existence_test=test -e mmc ${bootdev}:${partid} /${bootenv}\0" \
	"loadbootenv=load mmc $bootdev:$partid ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from uEnv.txt; env import -t ${loadbootenv_addr} ${filesize}\0" \
	"uenvboot=if run uEnvtxt_existence_test; then run loadbootenv; run importbootenv; fi; run custombootcmd\0" \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; " \
	"run uenvboot"

/* Miscellaneous configurable options */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x8000000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* environment organization */
#define CONFIG_SYS_MMC_ENV_DEV		1	/* USDHC2 */
#define CONFIG_SYS_MMC_ENV_PART		3	/* user area */
#define CONFIG_MMCROOT			"/dev/mmcblk1p3"  /* USDHC2 */

#define CONFIG_ENV_SIZE			SZ_8K
#define CONFIG_ENV_OFFSET		(12 * SZ_64K)

#define CONFIG_IMX_THERMAL

#define CONFIG_IOMUX_LPSR

#define CONFIG_SOFT_SPI

#ifdef CONFIG_FSL_QSPI
#define CONFIG_SYS_FSL_QSPI_AHB
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED	40000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define FSL_QSPI_FLASH_NUM		1
#define FSL_QSPI_FLASH_SIZE		SZ_32M
#endif

#endif
