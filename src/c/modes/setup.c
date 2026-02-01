#include <string.h>
#include "../consts.h"
#include "../generals.h"
#include "../diagnostic.h"
#include "setup.h"
#include "menufuncs.h"
#include "../unk/unk4.h"
#include "../unk/unk4_1.h"

static void f_08EBA(char a);
static char show_eqn_select(void);
static char num_fmt_prompt(char a);

// FUNCTION: GY454XE  Re 08BC4
// FUNCTION: GY455XE  Im 094E8
// FUNCTION: GY460XF  Im 08E2A
void mode_init(char m, char sm) {
	int v0;
	char _m;
	char _sm;
	char v1[40];
	char num[10];

	_m = mode;
	_sm = submode;
	v0 = 0;
	do memcpy((void *)v1[v0 * 10], unk_007e6[v0], 10);
	while (++v0 < 3);
	memcpy(&v1[30], unk_007e6[11], 10);
	if (mode != m) {
		d_080FD = 0;
		memzero(mode_ram, (char *)&magic_string - (char*)mode_ram);
	}
	f_044B6();
	mode = m;
	submode = sm;
	clear_result();
	switch (m) {
		case MODE_STAT:
			setup_stat();
			table_mode = TABLE_STAT_TABLE;
			if (!submode) submode = SMODE_STAT_1VAR;
			break;
#if ENABLE_MATRIX == 1
		case MODE_MATRIX:
			table_mode = TABLE_MATRIX;
			table_eqn_init();
			break;
#endif
		case MODE_VECTOR:
			table_mode = TABLE_VECTOR;
			table_eqn_init();
			break;
		case MODE_EQN:
			if (!submode) submode = SMODE_EQN_SIMUL2;
			table_eqn_setup();
			break;
#if ENABLE_RATIO == 1
		case MODE_RATIO:
			if (!submode) ratio_mode = 1;
			submode = SMODE_EQN_POLY2;
			table_ratio_setup();
			break;
#endif
#if ENABLE_INEQ == 1
		case MODE_INEQ:
			if (!submode) {
				submode = SMODE_EQN_POLY2;
				ineq_mode = 0;
			}
			table_ineq_setup();
			if (_m == MODE_INEQ && submode == _sm) {
				do memcpy((void *)v1[v0 * 10], unk_007e6[v0], 10);
				while (++v0 < 3);
				if (submode == SMODE_EQN_POLY3) memcpy(unk_007e6[11], &v1[30], 10);
			}
			break;
#endif
		case MODE_TABLE:
			// Load in the default Start, End and Step values
			num_fromdigit(num, 1);
			smart_num_cpy(table_param_ptrs[0], num);
			smart_num_cpy(table_param_ptrs[2], num);
			num_fromdigit(num, 5);
			smart_num_cpy(table_param_ptrs[1], num);
			break;
	}
	return;
}

// FUNCTION: GY454XE  Re 08CD6
// FUNCTION: GY455XE  Im 095FA
// FUNCTION: GY460XF  Im 08FE0
char mode_menu(void) {
	char m;
	char i;

	m = show_mode_menu();
	if (m) {
		arrow_state = 0;
		f_0B05A();
		switch (m) {
			default:
				i = 0;
				mode_init(m, 0);
				break;
			case MODE_STAT:
				i = stat_display_menu(STAT_SMENU_START, 1);
				mode_init(MODE_STAT, i);
				break;
#if ENABLE_MATRIX == 1
			case MODE_MATRIX:
				i = show_menu_matvct(MATRIX_SMENU_START, 1);
				break;
#endif
#if ENABLE_VECTOR == 1
			case MODE_VECTOR:
				i = show_menu_matvct(VECTOR_SMENU_START, 1);
				break;
#endif
			case MODE_EQN:
				mode_init(MODE_EQN, show_eqn_select());
				i = submode;
				break;
#if ENABLE_RATIO == 1
			case MODE_RATIO:
				i = show_menu_ratio(RATIO_SMENU_START, 1);
				break;
#endif
#if ENABLE_INEQ == 1
			case MODE_INEQ:
				i = show_menu_ineq(INEQ_SMENU_START, 1);
				break;
#endif
			case MODE_BASE_N:
				mode_init(MODE_BASE_N, SMODE_BASE_N_DEC);
				i = 0;
				break;
		}
		if (!i) {
			table_mode = 1;
			f_0B8B8(2);
		}
	}
	return m;
}

// FUNCTION: GY454XE  Re 08D6E
// FUNCTION: GY455XE  Im 09692
// FUNCTION: GY460XF  Im 09078
char show_mode_menu(void) {
	char v0;
	char v1;

	v1 = MODE_MENU_START;
	if (display_menu(&v1, -1) == 3) return v1;
	else return 0;
}

// FUNCTION: GY454XE  Re 08D94
// FUNCTION: GY455XE  Im 096B8
// FUNCTION: GY460XF  Im 0909E
static char show_eqn_select(void) {
	char loc_m1;

	loc_m1 = EQN_SMENU_START;
	if (display_menu(&loc_m1, 0) == 3) return loc_m1;
	else return 0;
}

// FUNCTION: GY454XE  Re 08DBA
// FUNCTION: GY455XE  Im 096DE
// FUNCTION: GY460XF  Im 090C4
char setup_menu(void) {
	char v0;
	char v1;

	if (d_080FE & (1 << 6)) return 0;
	v1 = 2;
	if (display_menu(&v1, 0) == 3) {
		v0 = 0xff;
		// SWITCH: GY454XE  Re 01E5C
		// SWITCH: GY455XE  Im 01E5C
		// SWITCH: GY460XF  Im 01BB4
		switch (v1) {
			default: goto j_08e04;
			// CASE: GY454XE  Re 08E0A
			// CASE: GY460XF  Im 09114
			case 1:   // Probably a leftover from ES series when MathIO result format couldn't be changed
			case 22:  // MathIO > MathO
				f_08EBA(1);
				setup_decimalo = 0;
				break;
			// CASE: GY454XE  Re 08E18
			// CASE: GY460XF  Im 09122
			case 23:  // MathIO > LineO
				f_08EBA(1);
				setup_decimalo = 1;
				break;
			// CASE: GY454XE  Re 08E26
			// CASE: GY460XF  Im 09130
			case 2:  // LineIO
				f_08EBA(0);
				setup_decimalo = 1;
				break;
			// CASE: GY454XE  Re 08E34
			// CASE: GY460XF  Im 0913E
			case 3:  // Deg
				setup_angle_unit = ANGLE_DEG;
				break;
			// CASE: GY454XE  Re 08E3C
			// CASE: GY460XF  Im 09146
			case 4:  // Rad
				setup_angle_unit = ANGLE_RAD;
				break;
			// CASE: GY454XE  Re 08E44
			// CASE: GY460XF  Im 0914E
			case 5:  // Gra
				setup_angle_unit = ANGLE_GRA;
				break;
			// CASE: GY454XE  Re 08E4C
			// CASE: GY460XF  Im 09156
			case 6:  // Fix
			case 7:  // Sci
			case 8:  // Norm
				v0 = num_fmt_prompt(v1);
				break;
			// CASE: GY454XE  Re 08E56
			// CASE: GY460XF  Im 09160
			case 9:  // ab/c
				setup_frac_result = 1;
				break;
			// CASE: GY454XE  Re 08E5E
			// CASE: GY460XF  Im 09168
			case 10:  // d/c
				setup_frac_result = 0;
				break;
			// CASE: GY454XE  Re 08E66
			// CASE: GY460XF  Im 09170
			case 12:  // CMPLX > a+bi
				setup_cmplx_result = 1;
				break;
			// CASE: GY454XE  Re 08E6E
			// CASE: GY460XF  Im 09178
			case 13:  // CMPLX > r∠θ
				setup_cmplx_result = 0;
				break;
			// CASE: GY454XE  Re 08E76
			// CASE: GY460XF  Im 09180
			case 14:  // STAT > ON
				setup_stat_freq = 1;
				setup_stat();
				break;
			// CASE: GY454XE  Re 08E7A
			// CASE: GY460XF  Im 09184
			case 15:  // STAT > OFF
				setup_stat_freq = 0;
				setup_stat();
				break;
			// CASE: GY454XE  Re 08E86
			// CASE: GY460XF  Im 09190
			case 16:  // Rdec > ON
				setup_rdec = 1;
				break;
			// CASE: GY454XE  Re 08E90
			// CASE: GY460XF  Im 0919A
			case 17:  // Rdec > OFF
				setup_rdec = 0;
				break;
			// CASE: GY454XE  Re 08E9A
			// CASE: GY460XF  Im 091A4
			case 18:  // Disp > Dot
				setup_decimal_mark = 1;
				break;
			// CASE: GY454XE  Re 08EA4
			// CASE: GY460XF  Im 091AE
			case 19:  // Disp > Comma
				setup_decimal_mark = 0;
				break;
			// CASE: GY454XE  Re 08EAE
			// CASE: GY460XF  Im 091B8
			case 11:  // <CONT>
				set_contrast(0);
				v0 = 0;
				break;
		}
	} else
j_08e04:
		v0 = 0;
j_08e06:
	return v0;
}

// FUNCTION: GY454XE  Re 08EBA
// FUNCTION: GY455XE  Im 097DE
// FUNCTION: GY460XF  Im 091C4
static void f_08EBA(char mathi) {
	setup_mathi = mathi;
	if (!(mode == MODE_EQN || mode == MODE_INEQ || mode == MODE_RATIO) || (d_080FE & 0xf) != 3) {
		if (table_mode & (1 << 7)) f_0AF16();
		f_0B8B8(2);
		f_0B05A();
		clear_result();
		if (table_mode != 1) f_044B6();
	}

	return;
}

// FUNCTION: GY454XE  Re 08F00
// FUNCTION: GY455XE  Im 09824
// FUNCTION: GY460XF  Im 0920A
static char num_fmt_prompt(char val) {
	char n;
	char min;
	char max;
	char *prompt;
	char keycode;

	n = 0xff;
	min = K_0;
	max = K_9;
	while (1) {
		switch (val) {
			case 6:
				prompt = s_prompt_fix;
				break;
			case 7:
				prompt = s_prompt_sci;
				break;
			default:
				min = K_1;
				max = K_2;
				prompt = s_prompt_norm;
				break;
		}
		print_4lines_4str(prompt, s_blank_line, s_blank_line, s_blank_line);
		keycode = getkeycode(1);
		if (min <= keycode && keycode <= max) {
			switch (val) {
				case 8:
					setup_num_fmt = keycode == K_1 ? NUM_FMT_NORM1 : NUM_FMT_NORM2;
					break;
				case 6:
					setup_num_fmt = NUM_FMT_FIX;
					setup_num_fmt_n = keycode & 0xf;
					break;
				case 7:
					setup_num_fmt = NUM_FMT_SCI;
					setup_num_fmt_n = keycode & 0xf;
					break;
			}
			break;
		} else if (keycode == K_SETUP || keycode == K_AC) {
			n = 0;
			break;
		}
	}
	return n;
}

// FUNCTION: GY454XE  Re 08FA2
// FUNCTION: GY455XE  Im 098C6
// FUNCTION: GY455XE  Im 092AC
void set_contrast(char debug) {
	char max;
	char min;
	char val;
	char keycode;

	max = 0x1d;
	min = 4;
	val = setup_contrast;
	print_4lines(contrast_screen);
	do {
		if (debug) {
			min = 0;
			max = 0x1f;
			diag_print_hex(val);
		}
		keycode = getkeycode(1);
		if (keycode == K_LEFT) {
			if (val > min) {
				f_046AE();
				--val;
			} else f_046C4();
		} else if (keycode == K_RIGHT) {
			if (val < max) {
				f_046AE();
				++val;
			} else f_046C4();
		} else if (keycode == K_AC) return;
		set_contrast_sfr(val);
		setup_contrast = val;
	} while (1);
}
