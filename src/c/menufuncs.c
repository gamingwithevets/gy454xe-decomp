#include "consts.h"
#include "setup.h"
#include "menufuncs.h"
#include "unk4.h"
#include "unk5.h"

static char menufunc_base_n(void);
static char menufunc_drg(void);
static char menufunc_cmplx(void);
static char menufunc_hyp(void);
static char menufunc_stat_dist(void);
static char stat_prompt_clr_mem(char a);
static char menufunc_matrix(void);
static char menufunc_vector(void);
static void set_matvct_dim(char a, dim *b);
static char const_conv_inputter(char menucode);
static char menufunc_const(void);
static char menufunc_conv(void);
static char menufunc_clr(void);
static char menufunc_verif(void);

// DATA: GY454XE  Re 01E8A
// DATA: GY455XE  Im 01E8A
// DATA: GY460XF  Im 01BE2
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
// FUNCTION: GY455XE  Im 09938
// FUNCTION: GY460XF  Im 0931E
char f_09014(void) {
	char v0;

	if (d_080FE & (1 << 6)) return 0;
	if ((
		is_eqn_result()
#if ENABLE_RATIO == 1
		|| is_ratio_result()
#endif
#if ENABLE_INEQ == 1
		|| is_ineq_result()
#endif
		) && last_key_keycode != K_CLR) return 0;
	v0 = menu_funcs[last_key_keycode-1]();
	return v0;
}

// FUNCTION: GY454XE  Re 09050
// FUNCTION: GY455XE  Im 09974
// FUNCTION: GY460XF  Im 0936A
static char menufunc_base_n(void) {
	return display_token_menu(MODE_BASE_N, 20);
}

// FUNCTION: GY454XE  Re 09058
// FUNCTION: GY455XE  Im 0997C
// FUNCTION: GY460XF  Im 09372
static char menufunc_drg(void) {
	if (is_func_table()) return 0;
	return display_token_menu(NULL, 22);
}

// FUNCTION: GY454XE  Re 09070
// FUNCTION: GY455XE  Im 09994
// FUNCTION: GY460XF  Im 0938A
static char menufunc_cmplx(void) {
#if ENABLE_CMPLX == 1
	return display_token_menu(MODE_CMPLX, 19);
#else
	return 0;
#endif
}

// FUNCTION: GY454XE  Re 09078
// FUNCTION: GY455XE  Im 0999C
static char menufunc_hyp(void) {
	if (is_func_table()) return 0;
	return display_token_menu(NULL, 23);
}

// FUNCTION: GY454XE  Re 09090
// FUNCTION: GY455XE  Im 099B4
static char menufunc_stat_dist(void) {
	char v0;

	if (mode != MODE_STAT) return 0;
	if (table_mode == TABLE_STAT_TABLE) v0 = 24;
	else if (submode == SMODE_STAT_1VAR) v0 = 25;
	else v0 = 26;

	return stat_display_menu(v0, 0);
}

// FUNCTION: GY454XE  Re 090BC
// FUNCTION: GY455XE  Im 099E0
static char stat_prompt_clr_mem(char sm) {
	char v0;
	char v1;

	v0 = 1;
	v1 = 0;
	if (submode == SMODE_STAT_1VAR) {
		if (sm != SMODE_STAT_1VAR) v1 = 1;
	} else if (sm == SMODE_STAT_1VAR) v1 = 1;
	if (v1) {
		print_4lines_4str(s_clr_mem, s_blank_line, s_prompt_yes, s_prompt_cancel);
		if (prompt_yes_no()) {
			setup_stat();
			f_044B6();
		} else v0 = 0;
	}

	return v0;
}

// FUNCTION: GY454XE  Re 09118
// FUNCTION: GY455XE  Im 09A3C
char stat_display_menu(char val, char noclr) {
	char v0;
	char loc_m1;

	v0 = val;
	while (1) {
		char tmp;
		char tmp2;
		loc_m1 = v0;
		switch (display_menu(&loc_m1, NULL)) {
			case 3:
				tmp2 = SMODE_STAT_1VAR;
				if (SMODE_STAT_1VAR <= loc_m1 && (tmp2 = loc_m1) && loc_m1 <= SMODE_STAT_REG_INV) {
					if (!noclr) {
						if (stat_prompt_clr_mem(tmp2)) {
							submode = loc_m1;
							d_08126 = 0;
							clear_cache_area();
							goto j_0916e;
						} else return -1;
					} else return loc_m1;
				} else if (loc_m1 == 20) {
j_0916e:
					table_mode = TABLE_STAT_TABLE;
					f_0AF0A();
					return -1;
				} else if (loc_m1 == 21) {
					if (table_mode == TABLE_STAT_TABLE) {
						f_044B6();
						f_0AD08(table_viewport + table_y - 1);
						d_08126 = 0;
						return -1;
					}
				} else if (loc_m1 == 22) {
					if (table_mode == TABLE_STAT_TABLE) {
						f_044B6();
						setup_stat();
						return -1;
					}
				} else if (loc_m1 == 23) {
					if (submode != SMODE_STAT_REG_QUAD) tmp = 36;
					else tmp = 37;
					v0 = tmp;
					continue;
				} else continue;
			case 2:
				last_key_keycode = loc_m1;
				return 1;
			case 1:
				if (loc_m1 == -1) {
					v0 = val;
					continue;
				}
				break;
		}
		return 0;
	}
}

// FUNCTION: GY454XE  Re 09210
// FUNCTION: GY455XE  Im 09B34
static char menufunc_matrix(void) {
#if ENABLE_MATRIX == 1
	char v0;

	if (mode != MODE_MATRIX) return 0;
	if (table_mode == TABLE_MATRIX) {
		if (submode == SMODE_MATVCT_ANS) v0 = 13;
		else v0 = 8;
	} else v0 = 13;
	return show_menu_matvct(v0, 0);
#else
	return 0;
#endif
}

// FUNCTION: GY454XE  Re 0923C
// FUNCTION: GY455XE  Im 09B60
static char menufunc_vector(void) {
#if ENABLE_VECTOR == 1
	char v0;

	if (mode != MODE_VECTOR) return 0;
	if (table_mode == TABLE_VECTOR) {
		if (submode == SMODE_MATVCT_ANS) v0 = 18;
		else v0 = 14;
	} else v0 = 18;
	return show_menu_matvct(v0, 0);
#else
	return 0;
#endif
}

#if ENABLE_MATRIX == 1 || ENABLE_VECTOR == 1
// FUNCTION: GY454XE  Re 09268
// FUNCTION: GY455XE  Im 09B8C
char show_menu_matvct(char idx, char mode_enter) {
	char v0;
	char v1;
	char v2;
	char v3;
	char v4;
	char v5;
	char *prompt;
	char *prompt_sel[6];
	char loc_m13;

	prompt_sel[0] = menu_mata_dim_sel;
	prompt_sel[1] = menu_matb_dim_sel;
	prompt_sel[2] = menu_matc_dim_sel;
	prompt_sel[3] = menu_vcta_dim_sel;
	prompt_sel[4] = menu_vctb_dim_sel;
	prompt_sel[5] = menu_vctc_dim_sel;
	v0 = idx;
	v1 = idx;
	v2 = idx;
	v3 = 0;
	v4 = 0;
	if (mode_enter) {
		char tmp;
		if (idx == 39) {
			prompt = menu_matrix_sel;
			tmp = TABLE_MATRIX;
		} else {
			prompt = menu_vector_sel;
			tmp = TABLE_VECTOR;
		}
		v5 = tmp;
	} else {
		prompt = NULL;
		if (mode == MODE_MATRIX) v5 = TABLE_MATRIX;
		else v5 = TABLE_VECTOR;
	}
	while (1) {
		loc_m13 = v0;
		switch (display_menu(&loc_m13, prompt)) {
			case 3:
				if (loc_m13 == 1 || loc_m13 == 2) {
					v1 = loc_m13;
					v2 = v0;
					v0 = 9;
					prompt = menu_matrix_sel;
					continue;
				} else if (loc_m13 == 18 || loc_m13 == 19) {
					v1 = loc_m13;
					v2 = v0;
					v0 = 15;
					prompt = menu_vector_sel;
				} else if (loc_m13 == 3 || loc_m13 == 4 || loc_m13 == 5) {
					v3 = v0;
					v4 = loc_m13 - 3;
					v0 = 11;
					prompt = prompt_sel[loc_m13 - 3];
					if (mode_enter) mode_init(MODE_MATRIX, v4);
					if (v1 == 2 && matvct_dims[v4].b.m && matvct_dims[v4].b.n) {}
					else continue;
				} else if (loc_m13 == 20 || loc_m13 == 21 || loc_m13 == 22) {
					v3 = v0;
					v4 = loc_m13 - 20;
					v0 = 17;
					prompt = prompt_sel[loc_m13 - 20];
					if (mode_enter) mode_init(MODE_VECTOR, v4);
					if (v1 == 19 && matvct_dims[v4].b.m && matvct_dims[v4].b.n) {}
					else continue;
				} else if (9 <= loc_m13 && loc_m13 <= 17) set_matvct_dim(v4, &matrix_dims[loc_m13 - 9]);
				else if (loc_m13 == 26 || loc_m13 == 27) set_matvct_dim(v4, &vector_dims[loc_m13 - 26]);
				table_mode = v5;
				submode = v4;
				f_0AF0A();
				return 0xff;
			case 2:
				last_key_keycode = loc_m13;
				return 1;
			case 1:
				if (loc_m13 == 0xff) {
					prompt = menu_matrix_sel;
					v0 = v3;
				} else if (loc_m13 == 0xfc) {
					prompt = menu_vector_sel;
					v0 = v3;
				} else if (loc_m13 == 0xfd || loc_m13 == 0xfa) v0 = v2;
				else break;
				continue;
		}
		if (mode_enter) {
			if (v5 == 19) mode_init(MODE_MATRIX, SMODE_MATVCT_A);
			else mode_init(MODE_VECTOR, SMODE_MATVCT_A);
		}
		last_key_keycode = NULL;
		break;
	}
	return;
}

// FUNCTION: GY454XE  Re 09474
// FUNCTION: GY455XE  Im 09D98
static void set_matvct_dim(char a, dim *b) {
	dim *v0;

	v0 = &matvct_dims[a];
	if (v0->b.m != b->b.m && b->b.n != v0->b.n) set_dim(a, b->b.m, b->b.n);
	return;
}
#endif

// FUNCTION: GY454XE  Re 094AE
// FUNCTION: GY455XE  Im 09DD2
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
// FUNCTION: GY455XE  Im 09E76
static char menufunc_const(void) {
	char v0;
	char v1;

	v0 = 0;
	if (!is_func_table()) {
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
// FUNCTION: GY455XE  Im 09EAE
static char menufunc_conv(void) {
	char v0;
	char v1;

	v0 = 0;
	if (!is_func_table()) {
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
// FUNCTION: GY455XE  Im 09EE6
static char menufunc_clr(void) {
	char v0;
	char v1;
	char *prompt;
	char val;
	scancode sc;

	v0 = -1;
	v1 = 1;
	d_08126 = 0;
	val = 1;
	if (display_menu(&val, 0) == 3) {
		switch (val) {
			case 1:
				prompt = s_clr_setup;
				break;
			case 2:
				prompt = s_clr_mem;
				break;
			default:
				prompt = s_reset_all;
				break;
		}
		print_4lines_4str(prompt, s_blank_line, s_prompt_yes, s_prompt_cancel);
		if (prompt_yes_no()) {
			switch (val) {
				case 1:
					clear_setup();
					break;
				case 2:
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
					break;
				case 3:
					sc.kio = last_key_scancode.kio;
					reset_all();
					last_key_scancode.kio = sc.kio;
					break;
			}
			if (val == 3) prompt = s_clr_done_all;
			else prompt = s_clr_done;
			print_4lines_4str(s_blank_line, prompt, s_blank_line, s_clr_prompt_ac);
			prompt_yes_no();
			if (v1) set_keycode(K_AC);
			else goto j_0965c;
		} else
j_0965c:
		v0 = 0;
	} else goto j_0965c;

	return v0;
}

// FUNCTION: GY454XE  Re 096E4
// FUNCTION: GY455XE  Im 0A008
static char menufunc_verif(void) {
	// TODO: Add code from a ROM with VERIF mode
	return 0;
}

// TODO: add f_0988E_460F and f_098C8_460F
