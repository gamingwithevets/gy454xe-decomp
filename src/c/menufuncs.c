#include "consts.h"
#include "menufuncs.h"
#include "unk4.h"
#include "unk5.h"

// 09050
static char f_09050(void);
// 09058
static char f_09058(void);
// 09070
static char f_09070(void);
// 09078
static char f_09078(void);
// 09090
static char f_09090(void);
// 090BC
static void f_090BC(void);
// 09210
static char f_09210(void);
// 0923C
static char f_0923C(void);
// 09474
static void f_09474(void);
// 094AE
static char f_094AE(void);
// 09552
static char f_09552(void);
// 0958A
static char f_0958A(void);
// 095C2
static char clr_menu(void);
// 096E4
static char f_096E4(void);

// 01E8A
static char (* const menu_funcs[])(void) = {
	f_09090,	// STAT/DIST
	f_09070,	// CMPLX
	f_09050,	// BASE-N
	f_09210,	// MATRIX
	f_0923C,	// VECTOR
	f_09552,	// CONST
	f_0958A,	// CONV
	clr_menu,	// CLR
	f_09078,	// hyp
	f_09058,	// DRG>
	f_096E4 	// VERIF
};

// 09014
signed char f_09014(void) {
	char v0;

	if (d_080FE & (1 << 6)) return 0;
	if ((char)f_0B7B6() || last_key_keycode != K_CLR) return 0;
	v0 = menu_funcs[last_key_keycode-1]();
	return v0;
}

// 09050
static char f_09050(void) {
	return f_0A010(MODE_BASE_N, 0x14);
}

// 09058
static char f_09058(void) {
	if (f_036B8()) return 0;
	return f_0A010(NULL, 0x16);
}

// 09070
static char f_09070(void) {
	return f_0A010(MODE_CMPLX, 0x13);
}

// 09078
static char f_09078(void) {
	if (f_036B8()) return 0;
	return f_0A010(NULL, 0x17);
}

// 09090
static char f_09090(void) {
	char v0;

	if (mode != MODE_STAT) return 0;
	if (table_mode == TABLE_STAT_TABLE) v0 = 0x18;
	else if (submode == SMODE_STAT_1VAR) v0 = 0x19;
	else v0 = 0x1a;

	return f_09118(v0, 0);
}

// 090BC - STUB
static void f_090BC(void) {
	return;
}

// 09118 - STUB
char f_09118(char a, char b) {
	return 0;
}

// 09210
static char f_09210(void) {
	char v0;

	if (mode != MODE_MATRIX) return 0;
	if (table_mode == TABLE_MATRIX) {
		if (submode == SMODE_MATVCT_ANS) v0 = 0xd;
		else v0 = 8;
	} else v0 = 0xd;
	return f_09268(v0, 0);
}

// 0923C
static char f_0923C(void) {
	char v0;

	if (mode != MODE_VECTOR) return 0;
	if (table_mode == TABLE_VECTOR) {
		if (submode == SMODE_MATVCT_ANS) v0 = 0x12;
		else v0 = 0xe;
	} else v0 = 0x12;
	return f_09268(v0, 0);
}

// 09268 - STUB
char f_09268(char a, char b) {
	return 0;
}

// 09474 - STUB
static void f_09474(void) {
	return;
}

// 094AE
static char f_094AE(void) {
	return 0;
}

// 09552
static char f_09552(void) {
	char v0 = 0;

	if (f_036B8()) {
		print_4lines(const_screen);
		if (f_094AE() > 0) {
			last_key_keycode = f_14404();
			v0 = 1;
		} else last_key_keycode = 0;
	}
	return v0;
}

// 0958A
static char f_0958A(void) {
	char v0 = 0;

	if (f_036B8()) {
		print_4lines(conv_screen);
		if (f_094AE() > 0) {
			last_key_keycode = f_14404();
			v0 = 1;
		} else last_key_keycode = 0;
	}
	return v0;
}

// 095C2
static char clr_menu(void) {
	char v0;
	char v1;
	char *v2;
	char val;
	int scancode;

	v0 = 0xff;
	v1 = 1;
	d_08126 = 0;
	val = 1;
	if (display_menu(&val, 0) == 3) {
		if (val != 1) {
			if (val != 2) v2 = s_reset_all;
			else v2 = s_clr_mem;
		} else v2 = s_clr_setup;
		print_4lines_4str(v2, s_blank_line, s_prompt_yes, s_prompt_cancel);
		if (prompt_yes_no()) {
			if (val != 1) {
				if (val != 2) {
					if (val == 3) {
						scancode = last_key_scancode;
						reset_all();
						last_key_scancode = scancode;
					}
				} else {
					clear_mem();
					if (mode == MODE_EQN || mode == MODE_INEQ) {
						if (mode == MODE_EQN) table_mode = TABLE_EQN;
						else table_mode = TABLE_INEQ;
						f_0AF0A();
						d_080FD = 0;
						v1 = 0;
					} else if (mode == MODE_TABLE) {
						table_mode = TABLE_RANGE;
						d_080FD = 1;
					} else if (table_mode & (1 << 4)) {
						f_044B6();
						v1 = 0;
					}
				}
			} else clear_setup();
			if (val == 3) v2 = s_clr_done_all;
			else v2 = s_clr_done;
			print_4lines_4str(s_blank_line, v2, s_blank_line, s_clr_prompt_ac);
			prompt_yes_no();
			if (v1) f_044D6(K_AC);
		} else v0 = 0;
	} else v0 = 0;

	return v0;
}

// 096E4
static char f_096E4(void) {
	// This is supposedly the VERIF mode menu, but it appears to have been removed
	return 0;
}
