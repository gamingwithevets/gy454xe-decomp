#include "consts.h"
#include "menufuncs.h"
#include "unk4.h"
#include "unk5.h"

static char menufunc_base_n(void);
static char menufunc_drg(void);
static char menufunc_cmplx(void);
static char menufunc_hyp(void);
static char menufunc_stat_dist(void);
static char f_090BC(char a);
static char menufunc_matrix(void);
static char menufunc_vector(void);
static void f_09474(char a, char *b);
static char const_conv_inputter(char menucode);
static char menufunc_const(void);
static char menufunc_conv(void);
static char menufunc_clr(void);
static char menufunc_verif(void);

// DATA: GY454XE  Re 01E8A
static char (* const menu_funcs[])(void) = {
	menufunc_stat_dist,	// STAT/DIST
	menufunc_cmplx,		// CMPLX
	menufunc_base_n,	// BASE-N
	menufunc_matrix,	// MATRIX
	menufunc_vector,	// VECTOR
	menufunc_const,		// CONST
	menufunc_conv,		// CONV
	menufunc_clr,		// CLR
	menufunc_hyp,		// hyp
	menufunc_drg,		// DRG>
	menufunc_verif 		// VERIF
};

// FUNCTION: GY454XE  Re 09014
signed char f_09014(void) {
	char v0;

	if (d_080FE & (1 << 6)) return 0;
	if ((char)f_0B7B6() && last_key_keycode != K_CLR) return 0;
	v0 = menu_funcs[last_key_keycode-1]();
	return v0;
}

// FUNCTION: GY454XE  Re 09050
static char menufunc_base_n(void) {
	return f_0A010(MODE_BASE_N, 0x14);
}

// FUNCTION: GY454XE  Re 09058
static char menufunc_drg(void) {
	if (f_036B8()) return 0;
	return f_0A010(NULL, 0x16);
}

// FUNCTION: GY454XE  Re 09070
static char menufunc_cmplx(void) {
	return f_0A010(MODE_CMPLX, 0x13);
}

// FUNCTION: GY454XE  Re 09078
static char menufunc_hyp(void) {
	if (f_036B8()) return 0;
	return f_0A010(NULL, 0x17);
}

// FUNCTION: GY454XE  Re 09090
static char menufunc_stat_dist(void) {
	char v0;

	if (mode != MODE_STAT) return 0;
	if (table_mode == TABLE_STAT_TABLE) v0 = 0x18;
	else if (submode == SMODE_STAT_1VAR) v0 = 0x19;
	else v0 = 0x1a;

	return f_09118(v0, 0);
}

// FUNCTION: GY454XE  Re 090BC
static char f_090BC(char a) {
	char v0;
	char v1;

	v0 = 1;
	v1 = 0;
	if (submode == SMODE_STAT_1VAR) if (a != 1) v1 = 1;
	else if (a == 1) v1 = 1;
	if (v1) {
		print_4lines_4str(s_clr_mem, s_blank_line, s_prompt_yes, s_prompt_cancel);
		if (prompt_yes_no()) {
			setup_stat();
			f_044B6();
		} else v0 = 0;
	}

	return v0;
}

// STUB: GY454XE  Re 09118
char f_09118(char a, char b) {
	return 0;
}

// FUNCTION: GY454XE  Re 09210
static char menufunc_matrix(void) {
	char v0;

	if (mode != MODE_MATRIX) return 0;
	if (table_mode == TABLE_MATRIX) {
		if (submode == SMODE_MATVCT_ANS) v0 = 0xd;
		else v0 = 8;
	} else v0 = 0xd;
	return f_09268(v0, 0);
}

// FUNCTION: GY454XE  Re 0923C
static char menufunc_vector(void) {
	char v0;

	if (mode != MODE_VECTOR) return 0;
	if (table_mode == TABLE_VECTOR) {
		if (submode == SMODE_MATVCT_ANS) v0 = 0x12;
		else v0 = 0xe;
	} else v0 = 0x12;
	return f_09268(v0, 0);
}

// STUB: GY454XE  Re 09268
char f_09268(char a, char b) {
	return 0;
}

// FUNCTION: GY454XE  Re 09474
static void f_09474(char a, char *b) {
	char *v0;

	v0 = d_080E0[a];
	if (v0[0] != b[0] && b[1] != v0[1]) f_0ABA8(a, b[0], b[1]);
	return;
}

// FUNCTION: GY454XE  Re 094AE
static char const_conv_inputter(char menucode) {
	char out;
	char min;
	char max;
	char numpos;
	char digitpos;
	char keycode;
	char num_str[10];

	out = 0;
	min = K_0;
	max = K_4;
	numpos = 1;
	digitpos = 10;
	strcpy(num_str, const_input_template);
	do {
		font_size = 10;
		use_rambuf = 0;
		line_print(72, 21, num_str);
		if (numpos == 3) {
			delay(4762);
			break;
		} else {
			keycode = getkeycode(1);
			if (min <= keycode && keycode <= max) {
				num_str[numpos++] = keycode;
				out += (char)((keycode & 0xf) * digitpos);
				digitpos = 1;
				max = K_9;
				if (keycode == K_4) max = K_0;
			} else if (keycode == K_AC || menucode == keycode) {
				out = 0;
				break;
			}
		}
	} while (1);
	use_rambuf = 1;
	return out;
}

// FUNCTION: GY454XE  Re 09552
static char menufunc_const(void) {
	char v0;
	char v1;

	v0 = 0;
	if (!f_036B8()) {
		print_4lines(const_screen);
		v1 = const_conv_inputter(K_CONST);
		if (v1 > 0) {
			last_key_keycode = get_const_token(v1);
			v0 = 1;
		} else last_key_keycode = 0;
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0958A
static char menufunc_conv(void) {
	char v0;
	char v1;

	v0 = 0;
	if (!f_036B8()) {
		print_4lines(conv_screen);
		v1 = const_conv_inputter(K_CONV);
		if (v1 > 0) {
			last_key_keycode = get_conv_token(v1);
			v0 = 1;
		} else last_key_keycode = 0;
	}
	return v0;
}

// FUNCTION: GY454XE  Re 095C2
static char menufunc_clr(void) {
	char v0;
	char v1;
	char *v2;
	char val;
	scancode sc;

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
						sc.kio = last_key_scancode.kio;
						reset_all();
						last_key_scancode.kio = sc.kio;
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

// FUNCTION: GY454XE  Re 096E4
static char menufunc_verif(void) {
	// VERIF mode does not exist on fx-570ES PLUS
	return 0;
}
