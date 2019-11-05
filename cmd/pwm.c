/*
 * Control PWM pins on the fly
 *
 * Copyright (c) 2019 GROOVE X, Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <pwm.h>
#include <asm/io.h>
#include <command.h>
#include <errno.h>

__weak int name_to_pwm(const char *name)
{
	return simple_strtoul(name, NULL, 10);
}

enum pwm_cmd {
	PWM_INIT,
	PWM_ENABLE,
	PWM_SET,
};

static int do_pwm_set(int pwm, int duty, int period) {
    return pwm_config(pwm, duty, period);
}

static int do_pwm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int pwm;
	int duty, period;
	enum pwm_cmd sub_cmd;
	const char *str_cmd, *str_pwm = NULL;
	int ret;

	if (argc < 2)
 show_usage:
		return CMD_RET_USAGE;
	str_cmd = argv[1];
	argc -= 2;
	argv += 2;

	if (argc > 0)
		str_pwm = *argv;

	if (!str_pwm)
		goto show_usage;

	/* parse the behavior */
	switch (*str_cmd) {
		case 'i': sub_cmd = PWM_INIT;   break;
		case 'e': sub_cmd = PWM_ENABLE; break;
		case 's': sub_cmd = PWM_SET;    break;
		default:  goto show_usage;
	}

	/* turn the pwm name into a pwm number */
	pwm = name_to_pwm(str_pwm);
	if (pwm < 0) {
		printf("pwm: failed to lookup pin '%s'\n", str_pwm);
		goto show_usage;
	}

	/* finally, let's do it */
	switch (sub_cmd) {
	case PWM_INIT:
		ret = pwm_init(pwm, 0, 0);
		if (ret) {
			printf("pwm: failed to init pwm\n");
			return ret;
		}
		break;
	case PWM_ENABLE:
		ret = pwm_enable(pwm);
		if (ret) {
			printf("pwm: failed to enable pwm\n");
			return ret;
		}
		break;
	case PWM_SET:
		if (argc < 3) {
			goto show_usage;
		} else if (!argv[1] || !argv[2]) {
			goto show_usage;
		}
		duty = simple_strtoul(argv[1], NULL, 10);
		period = simple_strtoul(argv[2], NULL, 10);
		ret = do_pwm_set(pwm, duty, period);
		if (ret) {
			printf("pwm: failed to config pwm\n");
			return ret;
		}
		printf("pwm: no. %d: duty=%d, period=%d\n", pwm, duty, period);
		break;
	default:
		goto show_usage;
	}
	return 0;
}

U_BOOT_CMD(pwm, 5, 0, do_pwm,
	   "control pwm pins",
	   "<init|enable|set> <pin>\n"
	   "    - init/enable/set the specified pin");
