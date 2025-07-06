#include <string.h>
#include "consts.h"
#include "generals.h"
#include "diagnostic.h"
#include "unk3.h"
#include "unk4.h"

typedef char *char_p;

// DATA: 007E6
const char_p unk_007e6[] = {
	&mode_ram[0],
	&mode_ram[10],
	&mode_ram[20],
	&mode_ram[30]
};

// DATA: 007FC
const char_p unk_007fc = &mode_ram[90];
// DATA: 007FE
const char_p table_start_ptr = &mode_ram[0];
// DATA: 00800
const char_p table_end_ptr = &mode_ram[10];
// DATA: 00802
const char_p table_step_ptr = &mode_ram[20];

// FUNCTION: GY454XE  Re 08BC4
void f_08BC4(char m, char sm) {
	int v0;
	char v1[40];
	char num[10];

	v0 = 0;
	do memcpy((void*)v1[v0 * 10], unk_007e6[v0], 10);
	while (++v0 < 3);
	memcpy(&v1[30], unk_007fc, 10);
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
		case MODE_MATRIX:
			table_mode = TABLE_MATRIX;
			f_0ABDE();
			break;
		case MODE_VECTOR:
			table_mode = TABLE_VECTOR;
			f_0ABDE();
			break;
		case MODE_EQN:
			if (!submode) submode = SMODE_EQN_SIMUL2;
			f_0ABFC();
			break;
		case MODE_TABLE:
			// Load in the default Start, End and Step values, I guess
			num_fromdigit(num, 1);
			f_0448A(table_start_ptr, num);
			f_0448A(table_step_ptr, num);
			num_fromdigit(num, 5);
			f_0448A(table_end_ptr, num);
			break;
	}
	return;
}

// FUNCTION: GY454XE  Re 08CD6
char mode_menu(void) {
	char m;
	char i;

	m = show_mode_menu();
	if (m) {
		d_08125 = 0;
		f_0B05A();
		switch (m) {
			case MODE_BASE_N:
				f_08BC4(MODE_BASE_N, SMODE_BASE_N_DEC);
				break;
			case MODE_STAT:
				i = f_09118(0x29, 1);
				f_08BC4(MODE_STAT, i);
				break;
			case MODE_MATRIX:
				i = f_09268(0x27, 1);
				break;
			case MODE_VECTOR:
				i = f_09268(0x28, 1);
				break;
			case MODE_EQN:
				f_08BC4(MODE_EQN, f_08D94());
				i = submode;
			default:
				i = 0;
				f_08BC4(m, 0);
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
char show_mode_menu(void) {
	char v0;
	char v1;

	v1 = 42;
	if (display_menu(&v1, -1) == 3) return v1;
	else return 0;
}

// 08D94 - STUB
char f_08D94(void) {
	return 0;
}

// FUNCTION: GY454XE  Re 08DBA
char setup_menu(void) {
	char v0;
	char v1;

	if (d_080FE & (1 << 6)) return 0;
	v1 = 2;
	if (display_menu(&v1, 0) == 3) {
		v0 = 0xff;
		// SWITCH: GY454XE  Re 01E5C
		switch (v1) {
			default: goto j_08e04;
			// CASE: GY454XE  Re 08E0A
			case 1:   // Probably a leftover from ES series when MathIO result format couldn't be changed
			case 22:  // MathIO > MathO
				f_08EBA(1);
				setup_decimalo = 0;
				goto j_08e06;
			// CASE: GY454XE  Re 08E18
			case 23:  // MathIO > LineO
				f_08EBA(1);
				setup_decimalo = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08E26
			case 2:  // LineIO
				f_08EBA(0);
				setup_decimalo = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08E34
			case 3:  // Deg
				setup_angle_unit = ANGLE_DEG;
				goto j_08e06;
			// CASE: GY454XE  Re 08E3C
			case 4:  // Rad
				setup_angle_unit = ANGLE_RAD;
				goto j_08e06;
			// CASE: GY454XE  Re 08E44
			case 5:  // Gra
				setup_angle_unit = ANGLE_GRA;
				goto j_08e06;
			// CASE: GY454XE  Re 08E4C
			case 6:  // Fix
			case 7:  // Sci
			case 8:  // Norm
				v0 = f_08F00(v1);
				goto j_08e06;
			// CASE: GY454XE  Re 08E56
			case 9:  // ab/c
				setup_frac_result = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08E5E
			case 10:  // d/c
				setup_frac_result = 0;
				goto j_08e06;
			// CASE: GY454XE  Re 08E66
			case 12:  // CMPLX > a+bi
				setup_cmplx_result = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08E6E
			case 13:  // CMPLX > r∠θ
				setup_cmplx_result = 0;
				goto j_08e06;
			// CASE: GY454XE  Re 08E76
			case 14:  // STAT > ON
				setup_stat_freq = 1;
				setup_stat();
				goto j_08e06;
			// CASE: GY454XE  Re 08E7A
			case 15:  // STAT > OFF
				setup_stat_freq = 0;
				setup_stat();
				goto j_08e06;
			// CASE: GY454XE  Re 08E86
			case 16:  // Rdec > ON
				setup_rdec = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08E90
			case 17:  // Rdec > OFF
				setup_rdec = 0;
				goto j_08e06;
			// CASE: GY454XE  Re 08E9A
			case 18:  // Disp > Dot
				setup_decimal_mark = 1;
				goto j_08e06;
			// CASE: GY454XE  Re 08EA4
			case 19:  // Disp > Comma
				setup_decimal_mark = 0;
				goto j_08e06;
			// CASE: GY454XE  Re 08EAE
			case 11:  // <CONT>
				set_contrast(0);
				v0 = 0;
				goto j_08e06;
		}
	} else
j_08e04:
		v0 = 0;
j_08e06:
	return v0;
}

// FUNCTION: GY454XE  Re 08EBA
void f_08EBA(char a) {
	setup_mathi = a;
	if (!(mode == MODE_EQN || mode == MODE_INEQ || mode == MODE_RATIO) || (d_080FE & 0xf) != 3) {
		if (table_mode & (1 << 7)) f_0AF16();
		f_0B8B8(2);
		f_0B05A();
		clear_result();
		if (table_mode != 1) f_044B6();
	}

	return;
}

// 08F00 - STUB
void f_08F00(char a) {
	return;
}

// FUNCTION: GY454XE  Re 08FA2
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
