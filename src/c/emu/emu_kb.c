#if REAL == 0

#include "emu_kb.h"
#include "../unk/unk4_1.h"
#include "../consts.h"
#include <string.h>

static int emu_num_to_str(char *num, char *out);

// FUNCTION: GY455XE  Im 04DF6
// FUNCTION: GY460XF  Im 03F08
void emu_update_screen(void) {
	char *tmp;
	int i;
	int j;
	emu_kb kb;

	init_emu_kb(&kb);
	tmp = kb.screen_buf;
	*tmp++ = 0x80;  // SHIFT key?
	*tmp++ = 1;

	i = 0;
	do {
		j = 0;
		do *tmp++ = *(char *)(0xf800+i*0x10+j);
		while (++j < 12);
	} while (++i < 32);
	return;
}

// FUNCTION: GY455XE  Im 04E44
// FUNCTION: GY460XF  Im 03F60
void f_04E44_E(void) {
	char loc_m2[2];
	char loc_m12[10];
	char loc_m22[10];
	char loc_m46[24];
	char loc_m70[24];
	emu_kb kb;

	loc_m2[0] = '\x8b';
	loc_m2[1] = '\0';
	memcpy(loc_m12, var_ans, 10);
	if (mode == MODE_CMPLX) memcpy(loc_m22, &mode_ram[174], 10);
	else memset(loc_m22, 0, 10);
	emu_num_to_str(loc_m22, loc_m46);
	memset(loc_m70, 0, 23);
	memset(loc_m46, 0, 23);
	emu_num_to_str(loc_m12, loc_m70);
	emu_num_to_str(loc_m22, loc_m46);
	init_emu_kb(&kb);
	*kb.d_08E10 = 0x30;
	++kb.d_08E10;
	*kb.d_08E10 = 0;
	++kb.d_08E10;
	*kb.d_08E10 = 0x43;
	++kb.d_08E10;
	*kb.d_08E10 = 9;
	++kb.d_08E10;
	memcpy(kb.d_08E10, loc_m70, 23);
	kb.d_08E10 += 22;
	*kb.d_08E10 = 9;
	++kb.d_08E10;
	memcpy(kb.d_08E10, loc_m46, 23);
	return;
}

// FUNCTION: GY455XE  Im 04F6E
// FUNCTION: GY460XF  Im 0408A
static int emu_num_to_str(char *num, char *out) {
	int v0;
	char loc_m10[10];
	int loc_m12;
	signed char loc_m13;
	char loc_m14;
	unsigned int loc_m16;
	int loc_m18;

	// ERROR value
	if ((num[0] & 0xf0) == 0xf0) {
		out[0] = 'E';
		out[1] = 'R';
		out[2] = 'R';
		out[3] = 'O';
		out[4] = 'R';
		out[5] = '\0';
		return 1;
	}
	v0 = 1;
	memcpy(loc_m10, num, 10);
	if (!*(unsigned int *)(loc_m10+8)) {
		out[0] = '+';
		loc_m16 = 1;
		do out[loc_m16] = '0';
		while (++loc_m16 <= 16);
		out[2] = '.';
		out[17] = 'E';
		out[18] = '+';
		out[19] = '0';
		out[20] = '0';
		out[21] = '0';
		out[22] = '\0';
		return 0;
	}
	if (*(unsigned int *)(loc_m10+8) >= 0x500) {
		out[0] = '-';
		*(int *)(&loc_m10[8]) -= 0x500;
	} else out[0] = '+';
	loc_m12 = 1;
	loc_m18 = 2;
	loc_m14 = 0;
	if ((loc_m10[0] & 0xf0) == 0x20) {
		loc_m14 = 1;
		loc_m18 = 0;
	}
	if ((loc_m10[0] & 0xf0) == 0x80) {
		loc_m14 = 2;
		loc_m18 = 0;
	}
	loc_m16 = 0;
	do {
		loc_m13 = loc_m10[loc_m16];
		if (!v0) {
			loc_m13 = (loc_m13 >> 4) & 0xf;
			out[loc_m12] = '0' + loc_m13;
			if (loc_m13 == 10) out[loc_m12] = 'a';
			++loc_m12;
			++v0;
		}
		if (v0 == 1) {
			loc_m10[loc_m16] &= 0xf;
			out[loc_m12] = '0' + loc_m10[loc_m16];
			if (loc_m10[loc_m16] == 10) out[loc_m12] = 'a';
			++loc_m12;
			if (loc_m18 == loc_m12) out[loc_m12++] = '.';
			v0 = 0;
		}
	} while (++loc_m16 < 8);
	out[loc_m12] = 'E';
	v0 = *(int *)(&loc_m10[8]);
	{
		int tmp, tmp2, tmp3, tmp4;

		tmp = loc_m10[9] & 0xf;
		tmp2 = tmp << 6;
		tmp3 = (tmp << 5) + tmp2;
		tmp <<= 2;
		tmp4 = tmp + tmp3;
		tmp = (v0 >> 4) & 0xf;
		tmp2 = tmp << 3;
		tmp <<= 1;
		tmp += tmp2;
		tmp2 = tmp + tmp4;
		v0 &= 0xf;
		v0 += tmp2;
	}
	++loc_m12;
	if (v0 >= 100) {
		out[loc_m12] = '+';
		v0 -= 100;
	} else {
		v0 = 100 - v0;
		out[loc_m12] = '-';
	}
	if (loc_m14 == 1) {
		for (loc_m16 = v0 + 1; loc_m16 < 22; loc_m16++) out[loc_m16] = ' ';
		out[loc_m16] = '\0';
		out[17] = 'B';
		return 0;
	} else if (loc_m14 == 2) {
		out[21] = 'T';
		out[20] = ' ';
		out[19] = out[15];
		out[18] = out[14];
		out[17] = 's';
		out[16] = out[11];
		out[15] = out[10];
		out[14] = out[9];
		out[11] = out[12];
		out[12] = out[13];
		out[13] = 'r';
		out[10] = '+';
		if ((*(unsigned int *)(loc_m10+8) & 0xff) >= 5) out[10] = '-';
		out[9] = out[7];
		out[8] = out[6];
		out[7] = 's';
		out[6] = out[3];
		out[3] = out[5];
		out[5] = out[2];
		out[2] = out[4];
		out[4] = out[1];
		out[1] = out[2];
		out[2] = out[3];
		out[3] = 'r';
		return 0;
	} else {
		int tmp, tmp2;
		for (tmp = 0; v0 >= 100; tmp++) v0 -= 100;
		for (tmp2 = 0; v0 >= 10; tmp2++) v0 -= 10;
		v0 = (char)tmp + (tmp2 << 4) + tmp;
		loc_m12 += 3;
		out[loc_m12+1] = '\0';
		for (loc_m16 = 0; loc_m16 < 3; loc_m16++) {
			out[loc_m12] = '0' + ((char)v0 & 0xf);
			v0 >>= 4;
			--loc_m12;
		}
		return 0;
	}
}

// FUNCTION: GY455XE  Im 052E0
// FUNCTION: GY460XF  Im 043FC
static int f_052E0_E(void) {
	return 0;
}

// FUNCTION: GY455XE  Im 052E4
// FUNCTION: GY460XF  Im 04400
static int f_052E4_E(void) {
	return 0;
}

// FUNCTION: GY455XE  Im 052E8
// FUNCTION: GY460XF  Im 04404
static int f_052E8_E(void) {
	return 0;
}

// FUNCTION: GY455XE  Im 052EC
// FUNCTION: GY460XF  Im 04408
static void f_052EC_E(void) {
	return;
}

// FUNCTION: GY455XE  Im 052EE
// FUNCTION: GY460XF  Im 0440A
static void f_052EE_E(void) {
	return;
}

// FUNCTION: GY455XE  Im 052F0
// FUNCTION: GY460XF  Im 0440C
static void f_052F0_E(void) {
	return;
}

// FUNCTION: GY455XE  Im 052F2
// FUNCTION: GY460XF  Im 0440E
static int f_052F2_E(void) {
	return 0;
}

// FUNCTION: GY455XE  Im 052F6
// FUNCTION: GY460XF  Im 04412
void init_emu_kb(emu_kb *kb) {
	kb->stop_type = 0x8e00;
	kb->ki = 0x8e01;
	kb->ko = 0x8e02;
	kb->d_08E10 = 0x8e10;
	kb->screen_buf = 0x9000;
	kb->d_09800 = 0x9800;
	kb->d_09804 = 0x9804;
	kb->d_09808 = 0x9808;
	kb->d_09834 = 0x9834;
	kb->error_buf = 0x9838;
	return;
}

// FUNCTION: GY455XE  Im 05338
// FUNCTION: GY460XF  Im 04454
void f_05338_E(char *a) {
	memset(a, 0, 48);
	a[0] = 0x11;
	a[1] = 0xff;
	a[2] = 0;
	a[3] = 0x30;
	a[4] = 0;
	a[5] = cursor_x;
	a[6] = 0;
	a[7] = cursor_y;
	if (d_08117) a[8] = 1;
	// INS
	if (modifiers & (1 << 7)) a[10] = 1;
	if (modifiers & (1 << 3)) a[11] = 1;
	if (modifiers & (1 << 2)) a[12] = 1;
	if (modifiers & (1 << 1)) a[14] = 1;
	if (modifiers & (1 << 0)) a[15] = 1;
	switch (setup_angle_unit) {
		case 4:
			a[9] = 1;
			break;
		case 5:
			a[9] = 2;
			break;
		case 6:
			a[9] = 3;
			break;
		default:
			a[9] = 0;
			break;
	}
	if (f_0B3EC()) {
		a[8] = d_08117;
		// Input almost full
		if (cursor_char == 0xcc) a[8] |= 0x80;
	} else {
		a[8] = 0;
		a[4] = 0xff;
		a[5] = 0xff;
		a[6] = 0xff;
		a[7] = 0xff;
	}
	return;
}

// FUNCTION: GY455XE  Im 05428
// FUNCTION: GY460XF  Im 04544
void f_05428_E(emu_kb *kb) {
	return;
}

#endif
