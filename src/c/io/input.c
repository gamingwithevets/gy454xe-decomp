#include "../consts.h"
#include "../generals.h"
#include "../emu/emu_kb.h"
#include "rdec.h"
#include "input.h"
#include "../modes/ineq.h"
#include "../unk/unk2.h"
#include "../unk/unk4.h"
#include "../unk/unk5.h"

// Static declarations
static void update_cursor(char x, char y);
static char is_next_disp(char chr);
static char f_04E9C(char a);
static void mathi_draw_line(int a, int x, char y, char d);
static void mathi_draw_line_horz(int x1, int x2, char y);
static void mathi_draw_line_vert(unsigned int x, char y0, char y1);
static void mathi_plot_pixel(int x, char y);
static void mathi_line_print(char *string, int x, char y);
static char f_052D0(char tok, int x, char y, char d);
static void _mathi_draw_abs_sqrt(mathi_bbox *a, int x, char y, char d);
static void mathi_draw_integral_symb(int x, char y, mathi_bbox c);
static void mathi_draw_sigma(int x, char y);
static void mathi_draw_paren(int x, char y, char lr, mathi_bbox d);
static void f_055BC(int x, char y, char *input);
static char f_05652(char *input);
static char f_0568E(char *input);
static char f_056AE(char *input);
static char f_056D0(char *input);
static char *f_056F2(char *input, char *b);
static char f_05766(char *input);
static char f_0579A(char *input, char b);
static char f_05824(char *input);
static char f_0587E(char *input);
static char *mathi_draw_abs_sqrt(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_pow(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_logab(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_ddx(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_frac(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_nth_rt(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_integral(char *input, mathi_bbox *b, int x, char y);
static char *mathi_draw_sum(char *input, mathi_bbox *b, int x, char y);
static char f_06914(char *input);
static char *mathi_draw_expr(char *input, mathi_bbox *b, int x, char y);
static char math2line_frac(char *input);
static char *f_06D90(char *input, char *b);
static char f_06E40(char *input, char b, char c);
static char *f_06F1C(char *input);
static void mathi_insert_box(char *input, char pos, char count);
static void _insert_token(char token, char b, char *input);
static char is_mathi_char(char token);
static char *move_cursor(char a);
static char *f_07442(char a);
static char is_num_1(char *num);

// DATA: GY454XE  Re 01DDC
// DATA: GY455XE  Im 01DDC
// DATA: GY460XF  Im 01B2C
const char unk_01ddc[] = {
	0,
	0,
	1,
	3,
	3,
	1,
	0,
	4,
	0,
	0,
	2,
#if ENABLE_RDEC == 1
	0,
#endif
	2
};

// DATA: GY454XE  Re 01DE8
// DATA: GY455XE  Im 01DE8
// DATA: GY460XF  Im 01B39
const char unk_01de8[] = {
	1,
	2,
	4,
	2,
	2,
	4,
	2,
	1,
	2,
	1,
	1,
#if ENABLE_RDEC == 1
	1,
#endif
	1
};

// DATA: GY454XE  Re 01DF4
// DATA: GY455XE  Im 01DF4
// DATA: GY460XF  Im 01B46
static char *(*const mathi_draw[])(char *, mathi_bbox *, int, char) = {
	mathi_draw_pow,
	mathi_draw_abs_sqrt,
	mathi_draw_logab,
	mathi_draw_sum,
	mathi_draw_integral,
	mathi_draw_ddx,
	mathi_draw_pow,
	mathi_draw_frac,
	mathi_draw_pow,
	mathi_draw_abs_sqrt,
	mathi_draw_nth_rt,
#if ENABLE_RDEC == 1
	mathi_draw_rdec,
#endif
	mathi_draw_frac,
};

// DATA: GY454XE  Re 01E24
// DATA: GY455XE  Im 01E24
// DATA: GY460XF  Im 01B7A
const char unk_01e24[] = {
	3, 5, 8, 7, 12
};

// FUNCTION: GY454XE  Re 04E6E
// FUNCTION: GY455XE  Im 055B0
// FUNCTION: GY460XF  Im 04A74
static void update_cursor(char x, char y) {
	cursor_x = x;
	cursor_y = y;
	update_cursor_char();
	d_08117 = font_size;
	return;
}

// FUNCTION: GY454XE  Re 04E86
// FUNCTION: GY455XE  Im 055C8
// FUNCTION: GY460XF  Im 04A8C
static char is_next_disp(char chr) {
	if (d_080FE & (1 << 6) && chr == ':') return 1;
	else if (chr) return 0;
	else return 1;
}

// FUNCTION: GY454XE  Re 04E9C
// FUNCTION: GY455XE  Im 055DE
// FUNCTION: GY460XF  Im 04AA2
static char f_04E9C(char a) {
	char v0;
	char v1;
	char *v2;
	char v3;
	char v4;
	char *v5;
	char v6;
	char v7;
	char loc_m22[34];

	v0 = cursor_pos_byte;
	v1 = formula_x;
	v2 = input_area_ptr;
	if ((unsigned int)((v0+1) * 6) <= (char)(a - formula_x)) {
		v3 = 0;
		v4 = 0;
		v5 = v2;
		while ((char)(v0 + 1) >= (char)v4) {
			++v4;
			v6 = *v5;
			if (is_next_disp(v6)) {
				++v3;
				break;
			} else {
				v3 += get_token(v6, loc_m22);
				++v5;
			}
		}
		if ((unsigned int)(v3 * 6) <= (char)(a - v1)) {
			v0 = 0;
			goto j_04f60;
		}
	}
	v3 = a - v1;
	while (v3 > 0) {
		v6 = v2[v0];
		v4 = v6;
		v7 = (char)((is_next_disp(v6) ? 1 : get_token(v4, loc_m22)) * 6);
		if (v3 >= v7) {
			v3 -= v7;
			if (v0) --v0;
			else goto j_04f60;
		} else break;
	}
	++v0;
j_04f60:
	return v0;
}

// FUNCTION: GY454XE  Re 04F6E
// FUNCTION: GY455XE  Im 056B0
// FUNCTION: GY460XF  Im 04B74
void input_print_linei(void) {
	char *input;
	char y;
	char x;
	char v3;
	char x_tmp;
	char v5;
	char v6;
	char tok;
	char space[2];
	char inputstr[50];
	char tokstr[34];

	space[0] = ' ';
	space[1] = '\0';
	input = input_area_ptr;
	y = formula_y;
	if (is_table_func_input()) {
		table_prompt_print(y);
		formula_x += 30;
	}
	x = formula_x;
	v3 = f_04E9C(90);
	x_tmp = x;
	v5 = v3;
	inputstr[0] = '\0';
	input = &input[v3];
	while (x_tmp <= 96) {
		char len;
		if (cursor_pos_byte == v5) update_cursor(x_tmp, y);
		tok = *input;
		if (!is_next_disp(tok) && (len = get_token(tok, tokstr))) {
			x_tmp += (char)(len * 6);
			smart_strcat(inputstr, tokstr);
			++v5;
			++input;
		} else break;
	}
	v6 = 1;
	if (is_next_disp(tok)) v6 = 0;
	x_tmp = (char)(smart_strlen(inputstr) * 6) + x;
	while (x_tmp < 96) {
		smart_strcat(inputstr, space);
		x_tmp += 6;
		v6 = 0;
	}
	line_print(x, y, inputstr);
	if (v3 > 0) f_10F52(y);
	if (v6) f_10F76(y);
	formula_x = 0;

	return;
}

// FUNCTION: GY454XE  Re 05076
// FUNCTION: GY455XE  Im 057B8
// FUNCTION: GY460XF  Im 04C7C
static void mathi_draw_line(int x0, int x1, char y0, char y1) {
	int v0;
	char v1;
	int tmp;
	char v2;
	char v3;

	v0 = d_08000;
	v1 = d_08005;
	if (!mathi_enable_draw) return;
	if (x1 >= (tmp = f_088AA())) {
		if (x0 < tmp) {
			y0 = lerp16(x0, x1, y0, y1, tmp);
			v2 = formula_x;
		} else {
			x0 -= v0;
			if (x0 <= 95) v2 = (char)x0;
			else return;
		}
		x1 -= v0;
		if (x1 > 95) {
			y1 = lerp16(x0, x1, y0, y1, 95);
			v3 = 95;
		} else v3 = (char)x1;
		if (y1 >= v1) {
			if (y0 < v1) {
				v2 = lerp8(v2, v3, y0, y1, v1);
				y0 = 0;
j_05124:
				y1 -= v1;
				if (y1 > 31) {
					v3 = lerp8(v2, v3, y0, y1, 31);
					y1 = 31;
				}
				draw_line(v3, y1, v2, y0);
			} else {
				y0 -= v1;
				if (y0 <= 31) goto j_05124;
			}
		}
	}
}

// FUNCTION: GY454XE  Re 05162
// FUNCTION: GY455XE  Im 058A4
// FUNCTION: GY460XF  Im 04D68
void mathi_draw_line_horz(int x1, int x2, char y) {
	int v0;

	if (mathi_enable_draw && y >= d_08005) {
		y -= d_08005;
		if (y <= 31) {
			v0 = f_088AA();
			if (x2 >= v0) {
				if (x1 < v0) x1 = formula_x;
				else {
					x1 -= d_08000;
					if (x1 > 95) return;
				}
				x2 -= d_08000;
				if (x2 > 95) x2 = 95;
				draw_line((char)x1, y, (char)x2, y);
			}
		}
	}

	return;
}

// FUNCTION: GY454XE  Re 051CE
// FUNCTION: GY455XE  Im 05910
// FUNCTION: GY460XF  Im 04DD4
static void mathi_draw_line_vert(unsigned int x, char y0, char y1) {
	if (mathi_enable_draw && x >= f_088AA()) {
		x -= d_08000;
		if (x <= 95 && y1 >= d_08005) {
			if (y0 < d_08005) y0 = 0;
			else {
				y0 -= d_08005;
				if (y0 > 31) return;
			}
			y1 -= d_08005;
			if (y1 > 31) y1 = 31;
			draw_line_vert((char)x, y0, y1, 0);
		}
	}
}

// FUNCTION: GY454XE  Re 05222
// FUNCTION: GY455XE  Im 05964
// FUNCTION: GY460XF  Im 04E28
void mathi_plot_pixel(int x, char y) {
	if (mathi_enable_draw && x >= f_088AA()) {
		x -= d_08000;
		if (x <= 95 && y >= d_08005) {
			y -= d_08005;
			if (y <= 31) plot_pixel((char)x, y);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 05260
// FUNCTION: GY455XE  Im 059A2
// FUNCTION: GY460XF  Im 04E66
void mathi_line_print(char *string, int x, char y) {
	int v0;

	if (mathi_enable_draw) {
		y -= get_font_width();
		do {
			if (x < f_088AA()) {
				if (*++string) x += 6;
				else break;
			} else {
				x -= d_08000;
				if (x <= 95) {
					v0 = y - d_08005;
					if (v0 >= -10 && v0 <= 31) line_print((char)x, (char)v0, string);
				}
				break;
			}
		} while (1);
	}

	return;
}

// FUNCTION: GY454XE  Re 052D0
// FUNCTION: GY455XE  Im 05A12
// FUNCTION: GY460XF  Im 04ED6
static char f_052D0(char tok, int x, char y, char d) {
	char v0;
	char loc_m20[20];

	if (mathi_enable_draw) {
		if (use_output_charset && (tok < 0x5e || tok >= 0xc0)) {
			v0 = 1;
			if (tok >= 0xe0) tok -= 0x50;
			loc_m20[0] = tok;
			loc_m20[1] = '\0';
		} else v0 = get_token(tok, loc_m20);
		if (d) loc_m20[--v0] = '\0';
		mathi_line_print(loc_m20, x, y);
	} else {
		char tmp;
		if (use_output_charset && (tok < 0x5e || tok >= 0xc0)) tmp = 1;
		else tmp = get_token_length(tok);
		v0 = tmp;
		if (d) --v0;
	}
	return v0 * 6;
}

// FUNCTION: GY454XE  Re 05366
// FUNCTION: GY455XE  Im 05AA8
// FUNCTION: GY460XF  Im 04F6C
static void _mathi_draw_abs_sqrt(mathi_bbox *a, int x, char y, char tok) {
	int tmp;

	// Absolute value
	if (tok != 0x63) {
		a->byte[2] += 2;
		tmp = a->word[0] + 7;
	} else {
		++a->byte[2];
		tmp = a->word[0] + 12;
	}
	a->word[0] = tmp;
	if (mathi_enable_draw) {
		char v0;
		int tmp2;
		int v1;

		v0 = a->byte[3] + (char)y - 1;
		y = v0 - a->byte[2] + 2;
		tmp2 = a->word[0] + x - 1;
		x += 3;
		if (tok != 0x63) {
			mathi_draw_line_horz((v1 = x+1), tmp2, y);
			mathi_draw_line(v1, x, y, v0);
			mathi_plot_pixel(--x, --v0);
			v1 = x - 1;
			mathi_plot_pixel(v1, v0 - 1);
		} else {
			mathi_draw_line_vert(x, y, v0);
			mathi_draw_line_vert(a->word[0] + x - 6, y, v0);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0542A
// FUNCTION: GY455XE  Im 05B6C
// FUNCTION: GY460XF  Im 05030
static void mathi_draw_integral_symb(int x, char y, mathi_bbox c) {
	if (mathi_enable_draw) {
		char v0;
		char v1;
		int v2;
		int v3;

		x += 2;
		v0 = c.byte[3] + y - 2;
		v1 = v0 - c.byte[2] + 4;
		mathi_draw_line_vert(x, v1, v0);
		v2 = x;
		mathi_plot_pixel(++x, --v1);
		mathi_plot_pixel(--v2, ++v0);
		v3 = x + 1;
		mathi_plot_pixel(v3, v1 + 1);
		v3 = v2 - 1;
		mathi_plot_pixel(v3, v0 - 1);
	}
	return;
}

// FUNCTION: GY454XE  Re 054A4
// FUNCTION: GY455XE  Im 05BE6
// FUNCTION: GY460XF  Im 050AA
static void mathi_draw_sigma(int x, char y) {
	if (mathi_enable_draw) {
		char v0;
		char v1;
		int v2;

		v0 = y;
		mathi_plot_pixel(x, y);
		v1 = 0;
		do {
			mathi_plot_pixel(--x, --y);
			mathi_plot_pixel(x, ++v0);
		} while (++v1 < 4);
		++x;
		mathi_draw_line_horz(x, (v2 = x + 6), y);
		mathi_draw_line_horz(x, v2, v0);
		mathi_plot_pixel(v2, y + 1);
		mathi_plot_pixel(v2, v0 - 1);
	}
	return;
}

// FUNCTION: GY454XE  Re 05516
// FUNCTION: GY455XE  Im 05C58
// FUNCTION: GY460XF  Im 0511C
static void mathi_draw_paren(int x, char y, char lr, mathi_bbox d) {
	char v0;
	char v1;
	char paren[2];

	if (mathi_enable_draw) {
		v0 = f_0897C();
		if (d.byte[2] <= v0) {
			paren[0] = !lr ? '(' : ')';
			paren[1] = '\0';
			mathi_line_print(paren, x, y);
		} else {
			if (!lr) x += 2;
			else x += 3;
			v1 = d.byte[3] + y - 3;
			y = v1 - d.byte[2] + 6;
			mathi_draw_line_vert(x, y, v1);
			v0 = 0;
			do {
				if (!lr) mathi_plot_pixel(++x, --y);
				else mathi_plot_pixel(--x, --y);
				mathi_plot_pixel(x, ++v1);
			} while (++v0 < 2);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 055BC
// FUNCTION: GY455XE  Im 05CFE
// FUNCTION: GY460XF  Im 051C2
static void f_055BC(int x, char y, char *input) {
	char v0;
	char tokstr[20];

	if ((char)(input - input_area_ptr) == cursor_pos_byte) {
		v0 = get_token(*input, tokstr);
		if (f_05658(input) == 1) v0 = unk_01de8[get_mathi_draw_idx(*input)];
		if (!v0) v0 = 1;
		d_08002 = (char)(v0 * 6) + x;
		d_08004 = y;
		if (mathi_enable_draw && d_080FE == 1) {
			x -= d_08000;
			v0 = d_08005;
			y -= get_font_width() + v0;
			update_cursor(x, y);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 05652
// FUNCTION: GY455XE  Im 05D94
// FUNCTION: GY460XF  Im 05258
static char f_05652(char *input) {
	return f_02676(*input);
}

// FUNCTION: GY454XE  Re 05658
// FUNCTION: GY455XE  Im 05D9A
// FUNCTION: GY460XF  Im 0525E
char f_05658(char *input) {
	char v0;

	v0 = f_05652(input);
	if (v0 == 1) {
		// Hex A, Hex D, Hex F
		if (input[1] != 0xb8 && input[1] != 0xbb && input[1] != 0xbd) {
			// Fraction / Mixed Fraction
			if (input[0] == 0xae || input[0] == 0x7c) v0 = RESULT_STANDARD;
			else v0 = RESULT_DECIMAL;
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0568E
// FUNCTION: GY455XE  Im 05DD0
// FUNCTION: GY460XF  Im 05294
static char f_0568E(char *input) {
	char v0;

	v0 = f_05658(input);
	// Hex A
	if (v0 == 3 && input[1] == 0xb8) v0 = 4;
	return v0;
}

// FUNCTION: GY454XE  Re 056AE
// FUNCTION: GY455XE  Im 05DF0
// FUNCTION: GY460XF  Im 052B4
static char f_056AE(char *input) {
	if (f_05652(input) != 1) return 5;
	return unk_01ddc[get_mathi_draw_idx(input[0])];
}

// FUNCTION: GY454XE  Re 056D0
// FUNCTION: GY455XE  Im 05E12
// FUNCTION: GY460XF  Im 052D6
static char f_056D0(char *input) {
	if (f_05658(input) != 1) return 5;
	return unk_01ddc[get_mathi_draw_idx(input[0])];
}

// FUNCTION: GY454XE  Re 056F2
// FUNCTION: GY455XE  Im 05E34
// FUNCTION: GY460XF  Im 052F8
static char *f_056F2(char *input, char *b) {
	char v0;
	char v1;
	char *v2;
	char v3;

	v0 = 1;
	v1 = 1;
	*b = 0;
	v2 = input_area_ptr;
	if (v2 != input) do {
		--input;
		v3 = f_05658(input);
		if (v1 == 1 && v3 == 9) ++v0;
		else if (v1 == 1 && (v3 == 8 || v3 == 10)) {
			if (!--v0) {
				*b = 1;
				break;
			}
		} else if (v3 == 3) ++v1;
		else if (v3 == 2) {
			if (!--v1) break;
		} else if (v3 == 5) {
			if (v1 == 1) break;
		}
	} while (input != v2);
	return input;
}

// FUNCTION: GY454XE  Re 05766
// FUNCTION: GY455XE  Im 05EA8
// FUNCTION: GY460XF  Im 0536C
static char f_05766(char *input) {
	char v0;
	char v1;

	v0 = 0;
	v1 = 1;
	while (*input) {
		// Hex A
		if (*input == 0xb8) ++v1;
		// Hex B
		else if (*input == 0xb9) {
			if (!--v1) break;
		}
		// Hex C
		else if (*input == 0xba) {
			if (v1 == 1) break;
		}
		++v0;
		++input;
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0579A
// FUNCTION: GY455XE  Im 05EDC
// FUNCTION: GY460XF  Im 053A0
static char f_0579A(char *input, char b) {
	char v0;
	char v1;
	char v2;

	v0 = 0;
	v1 = f_056D0(input);
	if ((b < 1 || v1) && (b != 2 || v1 == 3 || v1 == 4)) {
		char tmp;
		if (v1 == 2) tmp = 3;
		else if (v1 == 4) tmp = 4;
		else tmp = 2;
		v0 = tmp;
		if (b) {
			input += v0;
			v1 = 0;
			while (v1 < b) {
				v2 = 1;
				while (*input) {
					// Hex A
					if (*input == 0xb8) ++v2;
					// Hex B
					else if (*input == 0xb9) {
						if (!--v2) break;
					}
					// Hex C
					else if (*input == 0xba && v2 == 1) break;
					++v0;
					++input;
				}
				++v0;
				// Hex A
				if (*++input == 0xb8) {
					++input;
					++v0;
				}
				++v1;
			}
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 05824
// FUNCTION: GY455XE  Im 05F66
// FUNCTION: GY460XF  Im 0542A
static char f_05824(char *input) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = f_056D0(input);
	v1 = 0;
	if (v2 = f_0579A(input, v1)) {
		input += v2;
		v1 = 1;
		if (v0 == 2 || v0 == 4) {
			// Hex D, Hex E
			v0 = 0xbb;
			v3 = 0xbc;
		} else {
			// Hex A, Hex B
			v0 = 0xb8;
			v3 = 0xb9;
		}
		while (*input) {
			if (*input == v0) ++v1;
			else if (*input == v3 && !--v1) break;
			++v2;
			++input;
		}
	}
	return v2;
}

// FUNCTION: GY454XE  Re 0587E
// FUNCTION: GY455XE  Im 05FC0
// FUNCTION: GY460XF  Im 05484
static char f_0587E(char *input) {
	char v0;
	char v1;
	char v2;

	v0 = 1;
	v1 = 1;
	do if (!is_next_disp(*++input)) {
		v2 = f_05658(input);
		if (v2 == 9) {
			if (!--v0) break;
		} else if (v2 == 8 || v2 == 10) {
			++v0;
			continue;
		} else if (v2 == 1) {
			char tmp = f_05824(input);
			input += tmp;
			v1 += tmp;
		} else if (v2 == 3 || v2 == 5) break;
		++v1;
	} else break;
	while (1);
	return v1;
}

// FUNCTION: GY454XE  Re 058DC
// FUNCTION: GY455XE  Im 0601E
// FUNCTION: GY460XF  Im 054E2
void input_print_mathi(void) {
	char v0;
	char v1;
	char v2;
	char *input;
	unsigned int v4;
	char v5;
	mathi_bbox arr;

	v0 = 0;
	v1 = 0;
	v2 = d_08122;
	if (d_080FE != 1 && v2) v1 = 1;
	input = input_area_ptr;
	v4 = 0;
	d_08000 = 0;
	d_08005 = 0;
	mathi_set_draw_mode(0);
	if (!mathi_draw_expr(input, &arr, 0, 62) || arr.byte[2] > 62) {
		smart_strcpy(input, get_result_str_ptr());
		cursor_pos_byte = d_08006;
		mathi_set_draw_mode(0);
		mathi_draw_expr(input, &arr, 0, 62);
	}
	if (is_table_func_input()) {
		formula_x += 30;
		v0 = 1;
	}
	v5 = arr.word[0] > 96 ? 96 : arr.byte[0];
	if (!v1) {
		d_08124 = arr.byte[2];
		d_08111 = v5;
		v4 = formula_x;
		v1 = 96 - formula_x - 6;
		while (d_08002 - d_08000 > v1 && arr.word[0] - d_08000 > v1) d_08000 += 8;
		v1 = arr.byte[2] - arr.byte[3] + 1;
		if (arr.byte[2] > 31) {
			char tmp = arr.byte[3] + 62 - d_08004;
			if (v2 && tmp > 12) {
				v5 = 93	- v1 - 4;
				while ((char)(d_08004 - d_08005) > v5) d_08005 += 8;
			} else {
				d_08005 = arr.byte[2] - 31;
				d_08124 = 31;
			}
		}
		buffer_clear();
	} else {
		if (f_08ABA(v5)) f_10E5C(arr.byte[2]);
		if (arr.word[0] <= 96) v4 = (char)(96 - arr.byte[0]);
		else {
			d_08000 = cursor_pos_byte << 3;
			if (arr.word[0] - (cursor_pos_byte << 3) <= 88) {
				d_08000 = --cursor_pos_byte << 3;
				f_046C4();
			}
			d_0812C = 1;
			buffer_clear_lastnline(32 - arr.byte[2]);
		}
		v1 = 32 - arr.byte[3];
	}
	mathi_set_draw_mode(1);
	mathi_draw_expr(input, &arr, v4, v1);
	v4 = d_08000;
	v5 = d_08005;
	v1 -= v5 + 5;
	if (v0) table_prompt_print(v1);
	if (v4 > 0) f_10F52(v1);
	if (arr.word[0] - v4 > (char)(96 - formula_x)) f_10F76(v1);
	formula_x = 0;
	if (d_080FE == 1) {
		font_size = 6;
		if (v5 > 0) f_10F92();
		if (arr.byte[2] > (char)(v5 + 31)) f_10FA2();
	}

	return;
}

// FUNCTION: GY454XE  Re 05B2C
// FUNCTION: GY455XE  Im 0626E
// FUNCTION: GY460XF  Im 05732
static char *mathi_draw_abs_sqrt(char *input, mathi_bbox *b, int x, char y) {
	char v0;
	char *v1;

	// Square root
	v0 = *input == 0x98 ? 5 : 6;
	if (v1 = mathi_draw_expr(input + 2, b, v0 + x, y)) _mathi_draw_abs_sqrt(b, x, y, *input);
	return v1;
}

// FUNCTION: GY454XE  Re 05B86
// FUNCTION: GY455XE  Im 062C8
// FUNCTION: GY460XF  Im 0578C
static char *mathi_draw_pow(char *input, mathi_bbox *b, int x, char y) {
	char v0;
	char *v1;
	char v2;
	char chr[2];
	char loc_m3;
	char loc_m4;
	char size;
	char loc_m6;
	char loc_m7;

	loc_m6 = 0;
	v0 = d_08008;
	if (++d_08008 > 50) {
j_05bac:
		v1 = NULL;
		goto j_05cc0;
	}
	if (!mathi_enable_draw) {
		loc_m7 = d_0800A;
		d_0800A = 0;
	}
	loc_m4 = b->byte[3];
	v2 = b->byte[2];
	// Not regular pow
	if (*input != '^') {
		// If e^( -> Euler, else 10 in x10
		chr[0] = *input == 0x73 ? 0x81 : 0x83;
		chr[1] = '\0';
		mathi_line_print(chr, x, y);
		loc_m6 = 6;
		x += 6;
		loc_m4 = f_0898C();
		v2 = f_0897C();
	}
	size = begin_small_font();
	loc_m3 = v2 - 5;
	if (!mathi_enable_draw) {
		if (v1 = mathi_draw_expr(input+2, b, x, y)) {
			d_08640[v0].dword = b->dword;
			if (d_0800A) d_08004 -= loc_m3 - loc_m4 + b->byte[3];
j_05c7a:
			if (!is_pow_char(v1+1)) {
				font_size = size;
				b->word[0] += (char)(loc_m6 + 1);
				b->byte[2] += loc_m3;
				b->byte[3] = loc_m4;
				if (!mathi_enable_draw && !d_0800A) d_0800A = loc_m7;
			} else goto j_05bac;
		}
j_05cc0:
		return v1;
	} else {
		b->dword = d_08640[v0].dword;
		y -= loc_m3 - loc_m4 + b->byte[3];
		if (!(v1 = mathi_draw_expr(input+2, b, x, y))) goto j_05cc0;
		else goto j_05c7a;
	}
}

// FUNCTION: GY454XE  Re 05D0C
// FUNCTION: GY455XE  Im 0644E
// FUNCTION: GY460XF  Im 05912
static char *mathi_draw_logab(char *input, mathi_bbox *b, int x, char y) {
	char *v0;
	char v1;
	mathi_bbox loc_m4;
	char loc_m5;
	char loc_m6;
	char loc_m7;

	v0 = 0;
	v1 = d_08008++;
	if (d_08008 <= 50) {
		if (!mathi_enable_draw) {
			loc_m7 = d_0800A;
			d_0800A = 0;
		}
		// STRING: GY454XE  Re 01E29
		// STRING: GY455XE  Im 01E29
		mathi_line_print("log", x, y);
		x += 18;
		if (!mathi_enable_draw) {
			loc_m6 = begin_small_font();
			if (v0 = mathi_draw_expr(input+2, &loc_m4, x, y)) d_08640[v1].dword = loc_m4.dword;
			else goto j_05e5c;
		} else {
			loc_m4.dword = d_08640[v1].dword;
			loc_m5 = f_0898C() + loc_m4.byte[2] - 5;
			loc_m6 = begin_small_font();
			if (!(v0 = mathi_draw_expr(input+2, &loc_m4, x, y + loc_m5 - loc_m4.byte[3]))) goto j_05e5c;
		}
		font_size = loc_m6;
		if (!mathi_enable_draw) {
			loc_m5 = f_0898C() + loc_m4.byte[2] - 5;
			if (d_0800A) d_08004 += loc_m5 - loc_m4.byte[3];
		}
		x += loc_m4.word[0];
		if (v0 = mathi_draw_expr(v0+1, b, x+6, y)) {
			mathi_draw_paren(x, y, 0, *b);
			mathi_draw_paren(b->word[0] + x + 6, y, 1, *b);
			b->word[0] += loc_m4.word[0] + 30;
			loc_m4.byte[3] = loc_m5;
			loc_m4.byte[2] += f_0897C() - 5;
			f_089AC(b, loc_m4);
			if (!mathi_enable_draw && !d_0800A) d_0800A = loc_m7;
		}
	}
j_05e5c:
	return v0;
}

// FUNCTION: GY454XE  Re 05EBC
// FUNCTION: GY455XE  Im 065FE
// FUNCTION: GY460XF  Im 05AC2
static char *mathi_draw_ddx(char *input, mathi_bbox *b, int x, char y) {
	char size;
	int v1;
	char *v2;
	mathi_bbox arr;

	size = begin_small_font();
	// STRING: GY454XE  Re 01E2D
	// STRING: GY455XE  Im 01E2D
	mathi_line_print("d", x+4, y-4);
	// STRING: GY454XE  Re 01E2F
	// STRING: GY455XE  Im 01E2F
	mathi_line_print("dx", (v1 = x+1), y+5);
	font_size = size;
	mathi_draw_line_horz(v1, x+13, y);
	x += 20;
	if (v2 = mathi_draw_expr(input+2, b, x, y)) {
		mathi_draw_paren(x-6, y, 0, *b);
		mathi_draw_paren(b->word[0] + x, y, 1, *b);
		size = begin_small_font();
		x = b->word[0] + x + 6;
		mathi_draw_line_vert(x, y-4, y+7);
		x += 3;
		y += 5;
		// STRING: GY454XE  Re 01E32
		// STRING: GY455XE  Im 01E32
		mathi_line_print("x=", x, y);
		x += 12;
		if (v2 = mathi_draw_expr(v2+1, &arr, x, y)) {
			arr.byte[3] += 5;
			font_size = size;
			b->word[0] += arr.word[0] + 42;
			f_089AC(b, arr);
			arr.byte[2] = 16;
			arr.byte[3] = 8;
			f_089AC(b, arr);
		}
	}
	return v2;
}

// FUNCTION: GY454XE  Re 06002
// FUNCTION: GY455XE  Im 06744
// FUNCTION: GY460XF  Im 05C08
static char *mathi_draw_frac(char *input, mathi_bbox *b, int x, char y) {
	char v0;
	char v1;
	mathi_bbox loc_m4;
	mathi_bbox *loc_m6;
	char loc_m7;
	char *ip;
	char loc_m11;
	char size;

	loc_m7 = 0;
	v0 = d_08008;
	d_08008 += 2;
	if (d_08008 > 50)
j_06028:
		ip = 0;
	else {
		char *tmp;
		int tmp2;

		loc_m6 = &d_08640[1 + v0];
		if (!mathi_enable_draw) {
			loc_m11 = d_0800A;
			d_0800A = 0;
		}
		size = begin_small_font();
		// Mixed fraction
		if (*input == 0x7c) {
			ip = input + 4;
			v1 = f_06E40(ip, 1, 1);
			if (!v1) v1 = 1;
			if (v1 == f_05766(ip) && (is_sign_char(*ip) || f_143EA(ip) || *ip == '(' || *ip == '!')) {
				++x;
				if (ip = mathi_draw_expr(ip, &loc_m4, x, y)) {
					if (d_0800A) loc_m7 = 1;
					x += loc_m4.word[0]++;
					tmp = ip + 2;
				} else goto j_062ca;
			} else goto j_06028;
		} else {
			memzero(&loc_m4, 4);
			tmp = input + 3;
		}
		ip = tmp;
		if (!mathi_enable_draw) {
			if (ip = mathi_draw_expr(ip, b, x, y)) {
				if (d_0800A && !loc_m7) loc_m7 = 2;
				d_08640[v0].dword = b->dword;
				if (ip = mathi_draw_expr(ip+2, loc_m6, x, y)) {
					if (d_0800A && !loc_m7) loc_m7 = 3;
					tmp2 = max(b->word[0], loc_m6->word[0]) + 2;
					if (loc_m7 == 2) {
						d_08002 += (tmp2 - b->word[0]) >> 1;
						d_08004 -= b->byte[3] + 1;
					} else if (loc_m7 == 3) {
						d_08002 += (tmp2 - loc_m6->word[0]) >> 1;
						d_08004 += loc_m6->byte[2] - loc_m6->byte[3] + 1;
					}
					if (!d_0800A) d_0800A = loc_m11;
				} else goto j_062ca;
			} else goto j_062ca;
		} else {
			b->dword = d_08640[v0].dword;
			tmp2 = max(b->word[0], loc_m6->word[0]) + 2;
			if (ip = mathi_draw_expr(ip, b, ((tmp2 - b->word[0]) >> 1) + x, y - (b->byte[3] + 1))) {
				if (ip = mathi_draw_expr(ip+2, loc_m6, ((tmp2 - loc_m6->word[0]) >> 1) + x, loc_m6->byte[2] + y - loc_m6->byte[3] + 1)) mathi_draw_line_horz(x+1, x + tmp2 - 1, y);
				else goto j_062ca;
			} else goto j_062ca;
		}
		font_size = size;
		b->word[0] = loc_m4.word[0] + tmp2;
		b->byte[2] += loc_m6->byte[2] + 2;
		b->byte[3] = loc_m6->byte[2] + 1;
		f_089AC(b, loc_m4);
		++ip;
	}
j_062ca:
	return ip;
}

// FUNCTION: GY454XE  Re 062CE
// FUNCTION: GY455XE  Im 06A10
// FUNCTION: GY460XF  Im 05ED4
static char *mathi_draw_nth_rt(char *input, mathi_bbox *b, int x, char y) {
	mathi_bbox *v0;
	char loc_m1;
	char *loc_m4;
	char size;
	char loc_m6;
	char loc_m7;

	loc_m6 = 0;
	loc_m4 = 0;
	loc_m1 = d_08008;
	d_08008 += 2;
	if (d_08008 <= 3) {
		v0 = &d_08640[loc_m1];
		if (!mathi_enable_draw) {
			loc_m7 = d_0800A;
			d_0800A = 0;
		}
		size = begin_small_font();
		if (!mathi_enable_draw) {
			if (loc_m4 = mathi_draw_expr(input+3, v0, x+1, y)) {
				if (d_0800A) loc_m6 = 1;
j_06356:
				font_size = size;
				if (loc_m4 = mathi_draw_expr(loc_m4+2, b, v0->word[0] + x + 3, y)) {
					// Draw regular root
					_mathi_draw_abs_sqrt(b, v0->word[0] + x - 2, y, 0x98);
					if (!mathi_enable_draw) {
						d_08640[1+loc_m1].dword = b->dword;
						if (loc_m6) d_08004 -= b->byte[2] - b->byte[3] + v0->byte[3] - 5;
						if (!d_0800A) d_0800A = loc_m7;
					}
					b->word[0] += v0->word[0] - 2;
					b->byte[2] += v0->byte[2] - 5;
					++loc_m4;
				}
			}
		} else {
			b->dword = d_08640[1+loc_m1].dword;
			if (loc_m4 = mathi_draw_expr(input+3, v0, x+1, y - (b->byte[2] - b->byte[3] + v0->byte[3] - 5))) goto j_06356;
		}
	}
	return loc_m4;
}

// FUNCTION: GY454XE  Re 06456
// FUNCTION: GY455XE  Im 06B98
// FUNCTION: GY460XF  Im 0605C
static char *mathi_draw_integral(char *input, mathi_bbox *b, int x, char y) {
	char v0;
	mathi_bbox *v1;
	char *v2;
	mathi_bbox loc_m4;
	char loc_m5;
	char loc_m6;
	int loc_m8;
	char size;
	char loc_m10;
	char *loc_m12;

	loc_m5 = 0;
	v0 = d_08008;
	d_08008 += 2;
	if (d_08008 > 50) {
j_0647c:
		loc_m12 = 0;
		goto j_065fc;
	}
	v1 = &d_08640[1 + v0];
	if (!mathi_enable_draw) {
		loc_m10 = d_0800A;
		d_0800A = 0;
	}
	loc_m6 = f_0579A(input, 1);
	size = begin_small_font();
	loc_m8 = x + 6;
	if (!mathi_enable_draw) {
		if (v2 = mathi_draw_expr(&input[loc_m6], &loc_m4, loc_m8, y)) {
			if (d_0800A) loc_m5 = 2;
			if (loc_m12 = mathi_draw_expr(v2 + 1, v1, loc_m8, y)) {
				if (d_0800A && !loc_m5) loc_m5 = 3;
				d_08640[v0].dword = loc_m4.dword;
				v0 = loc_m4.byte[2] + 2;
				if (loc_m5 == 2) d_08004 += v0 - loc_m4.byte[3];
				else if (loc_m5 == 3) d_08004 -= v1->byte[3] + 3;
j_0656a:
				loc_m8 = max(loc_m4.word[0], v1->word[0]) + 7;
				font_size = size;
				if (mathi_draw_expr(&input[2], b, x + loc_m8, y)) {
					b->word[0] += loc_m8 + 12;
					loc_m4.byte[3] = v0;
					loc_m4.byte[2] += v1->byte[2] + 6;
					f_089AC(b, loc_m4);
					// STRING: GY454XE  Re 01E2F
					// STRING: GY455XE  Im 01E2F
					mathi_line_print("dx", b->word[0] + x - 12, y);
					mathi_draw_integral_symb(x + 1, y, loc_m4);
					if (!mathi_enable_draw && !d_0800A) d_0800A = loc_m10;
j_065fc:
					return loc_m12;
				}
			}
		}
	} else {
		loc_m4.dword = d_08640[v0].dword;
		v0 = loc_m4.byte[2] + 2;
		if (v2 = mathi_draw_expr(&input[loc_m6], &loc_m4, loc_m8, v0 + y - loc_m4.byte[3])) {
			if (!(loc_m12 = mathi_draw_expr(v2 + 1, v1, loc_m8, y - (v1->byte[3] + 3)))) {}
			else goto j_0656a;
		}
	}
	goto j_0647c;
}

// FUNCTION: GY454XE  Re 0666C
// FUNCTION: GY455XE  Im 06DAE
// FUNCTION: GY460XF  Im 06272
static char *mathi_draw_sum(char *input, mathi_bbox *b, int x, char y) {
	char v0;
	mathi_bbox *v1;
	char *v2;
	char *v3;
	mathi_bbox loc_m4;
	unsigned int loc_m6;
	char loc_m7;
	char loc_m8;
	char loc_m9;
	int loc_m12;
	char size;
	char loc_m14;
	char *loc_m16;

	loc_m7 = 0;
	v0 = d_08008;
	d_08008 += 2;
	if (d_08008 > 50) {
j_06692:
		loc_m16 = NULL;
		goto j_06860;
	}
	v1 = &d_08640[v0];
	if (!mathi_enable_draw) {
		loc_m14 = d_0800A;
		d_0800A = 0;
	}
	loc_m9 = f_0579A(input, 1);
	size = begin_small_font();
	if (!mathi_enable_draw) {
		if (v3 = mathi_draw_expr(&input[loc_m9], v1, x + 12, y)) {
			int tmp;
			if (d_0800A) loc_m7 = 2;
			if (loc_m16 = mathi_draw_expr(&v3[1], &loc_m4, x, y)) {
				if (d_0800A && !loc_m7) loc_m7 = 3;
				v1->word[0] += 12;
				d_08640[1 + v0].dword = loc_m4.dword;
				loc_m6 = max(max(v1->word[0], loc_m4.word[0]), 9);
				loc_m8 = v1->byte[2] + 5;
				if (loc_m7 == 2) {
					d_08002 += (loc_m6 - v1->word[0]) >> 1;
					d_08004 += loc_m8 - v1->byte[3];
				} else if (loc_m7 == 3) {
					d_08002 += (loc_m6 - loc_m4.word[0]) >> 1;
					d_08004 -= loc_m4.byte[3] + 5;
				}
j_067bc:
				font_size = size;
				if (mathi_draw_expr(&input[2], b, (tmp = x + loc_m6) + 5, y)) {
					loc_m12 = tmp - 1;
					mathi_draw_paren(loc_m12, y, 0, *b);
					mathi_draw_paren(b->word[0] + loc_m12 + 6, y, 1, *b);
					b->word[0] += loc_m6 + 12;
					loc_m4.byte[2] += v1->byte[2] + 10;
					loc_m4.byte[3] = loc_m8;
					f_089AC(b, loc_m4);
					mathi_draw_sigma((loc_m6 >> 1) + x, y);
					if (!mathi_enable_draw && !d_0800A) d_0800A = loc_m14;
j_06860:
					return loc_m16;
				}
			}
		}
	} else {
		char tmp;
		loc_m4.dword = d_08640[1 + v0].dword;
		loc_m6 = max(max(v1->word[0], loc_m4.word[0]), 9);
		loc_m8 = v1->byte[2] + 5;
		loc_m12 = ((loc_m6 - v1->word[0]) >> 1) + x;
		tmp = y + loc_m8 - v1->byte[3];
		// STRING: GY454XE  Re 01E32
		// STRING: GY455XE  Im 01E32
		mathi_line_print("x=", loc_m12, tmp);
		if (v3 = mathi_draw_expr(&input[loc_m9], v1, loc_m12 + 12, tmp)) {
			if (!(loc_m16 = mathi_draw_expr(&v3[1], &loc_m4, ((loc_m6 - loc_m4.word[0]) >> 1) + x, y - (loc_m4.byte[3] + 5)))) {}
			else goto j_067bc;
		}
	}
	goto j_06692;
}

// FUNCTION: GY454XE  Re 06914
// FUNCTION: GY455XE  Im 07056
// FUNCTION: GY460XF  Im 0651A
static char f_06914(char *input) {
	if (input > input_area_ptr) {
		if (input[-1] == 0xb9 || input[-1] == 0xbc) return 1;
		else if (input[-1] == 0xb8 || input[-1] == 0xba) return 0;
		else if (f_14532(input[-1])) return 1;
		else return 0;
	} else return 0;
}

// FUNCTION: GY454XE  Re 06944
// FUNCTION: GY455XE  Im 07086
// FUNCTION: GY460XF  Im 0654A
char *mathi_draw_expr(char *input, mathi_bbox *b, int x, char y) {
	int v0;
	mathi_bbox loc_m4;
	char loc_m5;

	v0 = x;
	if (++d_08007 > 27) {
j_06966:
		input = NULL;
		goto j_069ea;
	}

	if (get_remaining_stack_space() >= 200) {
		b->word[0] = 0;
		b->byte[2] = f_0897C();
		b->byte[3] = f_0898C();
		loc_m4.dword = b->dword;
		do {
			x = b->word[0] + v0;
			if ((char)(input - input_area_ptr) == cursor_pos_byte) {
				f_055BC(x, y, input);
				if (!mathi_enable_draw) d_0800A = 1;
			}
			// SWITCH: GY454XE  Re 01DB2
			// SWITCH: GY455XE  Im 01DB2
			// SWITCH: GY460XF  Im 01B02
			switch (f_05658(input)) {
				// CASE: GY454XE  Re 069F4
				// CASE: GY455XE  Im 07136
				// CASE: GY460XF  Im 065FA
				case 1:
					++d_08007;
					if (input = mathi_draw[get_mathi_draw_idx(*input)](input, &loc_m4, x, y)) {
						--d_08007;
						if (loc_m4.byte[2] <= 62) {
							b->word[0] += loc_m4.word[0];
							f_089AC(b, loc_m4);
							break;
						}
						else goto j_06966;
					}
					goto j_069ea;
				// CASE: GY454XE  Re 06A58
				// CASE: GY455XE  Im 0719A
				// CASE: GY460XF  Im 0665E
				case 9:
					mathi_draw_paren(x, y, 1, *b);
					b->word[0] += 6;
					f_056F2(input, &loc_m5);
					if (!loc_m5) {
						loc_m4.dword = b->dword;
						break;
					}
					goto j_069ea;
				// CASE: GY454XE  Re 06A92
				// CASE: GY455XE  Im 071D4
				// CASE: GY460XF  Im 06698
				case 10:
					loc_m5 = f_052D0(*input, x, y, 1);
					x += loc_m5;
					b->word[0] += loc_m5;
				// CASE: GY454XE  Re 06AB4
				// CASE: GY455XE  Im 071F6
				// CASE: GY460XF  Im 066BA
				case 8:
					if (input = mathi_draw_expr(input+1, &loc_m4, x+6, y)) {
						mathi_draw_paren(x, y, 0, loc_m4);
						b->word[0] += loc_m4.word[0] + 6;
						f_089AC(b, loc_m4);
						if (*input != 41) goto j_069ea;
						else break;
					}
					goto j_069ea;
				// CASE: GY454XE  Re 06B0E
				// CASE: GY455XE  Im 07250
				// CASE: GY460XF  Im 06714
				case 3:
				case 5:
				case 11:
					goto j_069ea;
				// CASE: GY454XE  Re 06B12
				// CASE: GY455XE  Im 07254
				// CASE: GY460XF  Im 06718
				case 0:
				case 12:
				case 13:
					if (!is_next_disp(*input)) {
						loc_m4.byte[2] = f_0897C();
						loc_m4.byte[3] = f_0898C();
						b->word[0] += f_052D0(*input, x, y, 0);
						break;
					}
					goto j_069ea;
				// CASE: GY454XE  Re 06B4E
				// CASE: GY455XE  Im 07290
				// CASE: GY460XF  Im 06754
				default: goto j_06966;
			}
			++input;
		} while (1);
	} else goto j_06966;

j_069ea:
	--d_08007;
	return input;
}

// FUNCTION: GY454XE  Re 06B52
// FUNCTION: GY455XE  Im 07294
// FUNCTION: GY460XF  Im 06758
char f_06B52(char *input) {
	char v0;
	char *v1;
	char loc_m1;

	v0 = 0;
	while (*input) {
		loc_m1 = f_05658(input);
		if (loc_m1 == 8 || loc_m1 == 10) {
			v1 = &input[f_0587E(input)];
			// Hex B
			if (*v1 == 0xb9 || *v1 == 0xba) {
j_06b8e:
				cursor_pos_byte = v0;
				return 0;
			}
		} else if (loc_m1 == 9) {
			v1 = f_056F2(input, &loc_m1);
			if (*v1 == 0xb8 || *v1 == 0xba) goto j_06b8e;
		} else if (loc_m1 == 12) {
			if (*input == ',') {
				v1 = f_056F2(input, &loc_m1);
				// log, Pol, Rec, RanInt#
				if (*v1 == 0x68 || *v1 == 0x6c || *v1 == 0x6d || *v1 == 0xc2) goto j_06ba0;
			}
			if (f_06D90(input, &loc_m1)) goto j_06b8e;
		}
j_06ba0:
		++v0;
		++input;
	}
	return 1;
}

// FUNCTION: GY454XE  Re 06C06
// FUNCTION: GY455XE  Im 07348
// FUNCTION: GY460XF  Im 0680C
static char math2line_frac(char *input) {
	char v0;
	char v1;

	v0 = 0;
	v1 = 0;
	while (1) {
		char chr = *input--;
		// Hex B
		if (chr == 0xb9) {
			++v0;
			continue;
		// Hex A
		} else if (chr == 0xb8 && !--v0) break;
	}
	// Hex D 
	if (*input == 0xbb) {
		// Hex F
		if (*--input == 0xbd) --input;
		// Mixed Fraction, Fraction
		v1 = *input == 0x7c ? 0xae : *input;
	}
	// Hex B, Fraction
	else if (*input == 0xb9) v1 = 0xae;
	return v1;
}

// FUNCTION: GY454XE  Re 06C54
// FUNCTION: GY455XE  Im 07396
// FUNCTION: GY460XF  Im 0685A
void math2line(char *input, char *out, char c, char d) {
	char *v0;
	char *v1;
#if ENABLE_RDEC == 1
	char loc_m1_rdec;
	char *tmp;
#endif
	char loc_m1;

	loc_m1 = d_080FE & (1 << 6);
	v0 = input;
	v1 = out;
	do {
		// SWITCH: GY454XE  Re 01DCE
		// SWITCH: GY455XE  Im 01DCE
		// SWITCH: GY460XF  Im 01B1E
		switch (f_0568E(input)) {
			// CASE: GY454XE  Re 06D0A
			// CASE: GY455XE  Im 0744C
			// CASE: GY460XF  Im 06910
			case 1:
				switch (f_056D0(input)) {
					case 2:
						break;
					case 4:
						++input;
						break;
					default:
						*out++ = *input;
						break;
				}
				break;
			// CASE: GY454XE  Re 06D20
			// CASE: GY455XE  Im 07462
			// CASE: GY460XF  Im 06926
			case 2:
				// Fraction
				if (f_05652(&out[-1]) != 1 || out[-1] == 0xae) *out++ = '(';
				break;
			// CASE: GY454XE  Re 06D42
			// CASE: GY455XE  Im 07484
			// CASE: GY460XF  Im 06948
			case 4:
				*out++ = ')';
				if (d || c + v1 + 1 != out) *out++ = math2line_frac(input);
				else return;
				break;
			// CASE: GY454XE  Re 06D68
			// CASE: GY455XE  Im 074AA
			// CASE: GY460XF  Im 0696E
			case 6:
				*out++ = '(';
				break;
			// CASE: GY460XF  Im 06978
			case 3:
#if ENABLE_RDEC == 1
				tmp = f_06D90(input, &loc_m1_rdec);
				// Recurring decimal
				if (*tmp != 0xa4) *out++ = ')';
				break;
#endif
			// CASE: GY454XE  Re 06D72
			// CASE: GY455XE  Im 074B4
			// CASE: GY460XF  Im 06998
			case 7:
				*out++ = ')';
				break;
			// CASE: GY454XE  Re 06D7C
			// CASE: GY455XE  Im 074BE
			// CASE: GY460XF  Im 069A2
			case 5:
				*out++ = ',';
				break;
			// CASE: GY454XE  Re 06D86
			// CASE: GY455XE  Im 074C8
			// CASE: GY460XF  Im 069AC
			default:
				*out++ = *input;
				break;
		}
		if (d) {
			if (loc_m1 && d_0812A == out && input_area_ptr == v0) input_area_ptr = input + 1;
		} else if (c + v1 + 1 == out) {
			// Hex E, Hex A
			if (*input == 0xbc || *input == 0xb8) ++input;
			// Hex D
			else if (*input == 0xbb) --input;
			// Hex F
			if (*input == 0xbd) --input;
j_06cf6:
			cursor_pos_byte = input - v0;
			return;
		}
		++input;
	} while (*input);
	*out = '\0';
	if (!d) goto j_06cf6;
	return;
}

// FUNCTION: GY454XE  Re 06D90
// FUNCTION: GY455XE  Im 074D2
// FUNCTION: GY460XF  Im 069B6
static char *f_06D90(char *input, char *b) {
	char v0;
	char *v1;
	char v2;
	char v3;

	v0 = 1;
	*b = 0;
	v1 = input;
	// Hex E
	if (*input == 0xbc) --input;
	// Hex B
	if (*input == 0xb9) --input;
	while ((v2 = *input), input_area_ptr != input) {
		--input;
		// Hex B
		if (v2 == 0xb9) ++v0;
		// Hex A, Hex B
		else if (v2 == 0xb8 && !--v0 && *input != 0xb9) {
			// Hex D, Hex F
			if (*input == 0xbb && *--input == 0xbd) --input;
			v3 = (char)v1 - (char)input;
			if (v3 <= f_05824(input)) {
				if (v2 = f_056D0(input) && v3 >= f_0579A(input, 1)) {
					*b = 1;
					if (v2 == 1 || v2 == 2) return input;
					if (v3 >= f_0579A(input, 2)) *b = 2;
				}
			} else break;
			return input;
		}
	}
	return NULL;
}

// FUNCTION: GY454XE  Re 06E40
// FUNCTION: GY455XE  Im 07582
// FUNCTION: GY460XF  Im 06A66
static char f_06E40(char *input, char b, char c) {
	char v0;
	char v1;
	char v2;

	if (!f_088FE(input)) {
		if (is_sign_char(*input)) {
			do ++v0;
			while (!is_sign_char(*++input));
			if (f_143EA(input) || !c) {
				v0 = 0;
				goto j_06eae;
			}
		}
		while (1) {
			v1 = f_05658(input);
			if (!f_08A0A(v1) && v1 != 13) {
				if (v1 == 1) {
					v0 += f_05824(input);
					input += v0;
				} else if (v1 == 8 || v1 == 10) {
					v0 += f_0587E(input);
					input += v0;
					if (*input != ')') break;
				} else if (f_143EA(input)) {
					v1 = 0;
					v2 = 0;
					do {
						// x10
						if (*input == 0x74) {
							if (v1 > 0) break;
							v1 = 1;
							v2 = 1;
							do ++v0;
							while (is_sign_char(*++input));
						} else if (*input == '.') {
							if (v2 > 0) break;
							v2 = 1;
						}
						++v0;
					} while (f_143EA(++input));
					if (b) break;
					else continue;
				}
				++v0;
				if (!b) ++input;
				else break;
			} else break;
		}
	}
j_06eae:
	return v0;
}

// FUNCTION: GY454XE  Re 06F1C
// FUNCTION: GY455XE  Im 0765E
// FUNCTION: GY460XF  Im 06B42
static char *f_06F1C(char *input) {
	char *v0;
	char loc_m1;

	v0 = input_area_ptr;
	--input;
	while (1) {
		loc_m1 = f_05658(input);
		if (!f_08A2A(loc_m1)) {
			if (loc_m1 == 3 || loc_m1 == 7) input = f_06D90(input, &loc_m1);
			else if (loc_m1 == 9) {
				input = f_056F2(input, &loc_m1);
				if (!loc_m1 && input != v0) break;
			} else if (f_143EA(input)) {
				do {
					if (input != v0) {
						--input;
						if (is_sign_char(*input)) {
							char *tmp = input;
							do {
								if (tmp != v0) --tmp;
								else goto j_06f64;
							} while (is_sign_char(*tmp));
							// x10 character
							if (*tmp == 0x74) input = tmp;
							else goto j_06f64;
						}
					} else goto j_06f66;
				} while (f_143EA(input));
				continue;
			}
			if (input != v0) --input;
			else goto j_06f66;
		} else break;
	}
j_06f64:
	++input;
j_06f66:
	return input;
}

// FUNCTION: GY454XE  Re 06FCE
// FUNCTION: GY455XE  Im 07710
// FUNCTION: GY460XF  Im 06BF4
static void mathi_insert_box(char *input, char pos, char count) {
	char *ip_curr;
	char i;

	while (*input) ++input;
	if (!count) *input++ = '!';
	else {
		ip_curr = &input_area[pos];
		if (is_pow_char(ip_curr)) *input++ = '!';
		i = 0;
		// memcpy(input, ip_curr, count);
		do *input++ = *ip_curr++;
		while (++i < count);
	}
	*input = '\0';
	return;
}

// FUNCTION: GY454XE  Re 07022
// FUNCTION: GY455XE  Im 07764
// FUNCTION: GY460XF  Im 06C48
static void _insert_token(char token, char is_func, char *input) {
	char *v0;
	char count;
	char *v2;
	char v3;
	char tmp;
	char loc_m1;
	char loc_m2;
	char ins;
	char pos;
	char mathi;
	char loc_m6;
	char loc_m7;
	char *loc_m10;
	char loc_m11;

	v0 = input_area;
	pos = cursor_pos_byte;
	loc_m1 = cursor_pos_byte;
	mathi = is_mathi();
	ins = is_ins_mode();
	input[0] = token;
	input[1] = '\0';
	loc_m6 = 1;
	count = 0;
	v2 = &v0[pos];
	if (mathi) {
		smart_strcpy(get_result_str_ptr(), v0);
		d_08006 = pos;
		loc_m7 = is_pow_char(v2);
		if (!is_func) {
			if (loc_m7 || (*v2 == '!' && is_pow_char(&v2[1]))) {
				// STRING: GY454XE  Re 01E35
				// STRING: GY455XE  Im 01E35
				if (f_08A2A(f_05658(input))) smart_strcat(input, "!");
			}
		} else {
			// Check for x^n, nth root, fraction, mixed fraction
			if (ins == 1 || token == '^' || token == 0x9f || token == 0xae || token == 0x7c) count = f_06E40(v2, 0, ins);
			loc_m6 = 2;
			// Check for x^2, x^3, x^-1
			if (token == 0x75 || token == 0x76 || token == 0x77) {
				// Convert to x^n
				input[0] = '^';
				count = 0;
				loc_m6 = 4;
				// Check for x^-1
				if (token == 0x77) loc_m6 = 5;
			// Check for cube root, replace with nth root
			} else if (token == 0xa8) input[0] = 0x9f;
			v3 = 0;
			loc_m2 = f_056AE(input);
			if (input[0] == '^' && f_08A48(&v2[-1])) {
				// Add a box then the pow
				// STRING: GY454XE  Re 01E37
				// STRING: GY455XE  Im 01E37
				smart_strcpy(input, "!^");
				loc_m6 = 0;
				v3 = 1;
			}
			loc_m11 = unk_01e24[loc_m2];
			if ((char)(unk_01e24[loc_m2] + count + loc_m7 + v3) < 100) {
				// x^2, x^3, x^-1, x^n, recurring decimal
				if (loc_m2 == 0) {
					concat_mathi_l(input);
					// STRING: GY454XE  Re 01E3A
					// STRING: GY455XE  Im 01E3A
					if (token == 0x75) smart_strcat(input, "2");
					// STRING: GY454XE  Re 01E3C
					// STRING: GY455XE  Im 01E3C
					else if (token == 0x76) smart_strcat(input, "3");
					// STRING: GY454XE  Re 01E3E
					// STRING: GY455XE  Im 01E3E
					else if (token == 0x77) smart_strcat(input, "`1");
					else mathi_insert_box(input, pos, count);
					concat_mathi_r(input);
				// logab
				} else if (loc_m2 == 1) {
					concat_mathi_l(input);
					if (token == 0x68) loc_m2 = 0;
					else loc_m2 = count;
					mathi_insert_box(input, pos, loc_m2);
					// STRING: GY454XE  Re 01E41
					// STRING: GY455XE  Im 01E41
					smart_strcat(input, "\xba");
					if (token == 0x68) loc_m2 = count;
					else loc_m2 = 0;
					mathi_insert_box(input, pos, loc_m2);
					concat_mathi_r(input);
				} else if (loc_m2 == 2) {
					loc_m6 = 3;
					// Cube root
					if (token == 0xa8) {
j_07200:
						// STRING: GY454XE  Re 01E43
						// STRING: GY455XE  Im 01E43
						smart_strcat(input, "\xbb\xb8");
						if (token == 0xa8) {
							// STRING: GY454XE  Re 01E3C
							// STRING: GY455XE  Im 01E3C
							smart_strcat(input, "3");
							loc_m6 = 6;
						} else mathi_insert_box(input, loc_m1, v3);
						concat_mathi_rl(input);
						mathi_insert_box(input, pos, count);
						concat_mathi_rend(input);
						count += v3;
					} else goto j_07272;
				} else if (loc_m2 == 3) {
					concat_mathi_l(input);
					mathi_insert_box(input, pos, count);
					// STRING: GY454XE  Re 01E46
					// STRING: GY455XE  Im 01E46
					smart_strcat(input, "\xba!\xba!\xb9");
				} else if (loc_m2 == 4) {
					loc_m6 = 4;
j_07272:
					if (f_06914(v2)) {
						loc_m10 = f_06F1C(v2);
						v3 = v2 - loc_m10;
						if (v3) {
							tmp = is_pow_char(loc_m10);
							if ((char)(loc_m7 + count + v3 + tmp + loc_m11) < 100) {
								loc_m1 -= v3;
								loc_m6 += tmp + v3 + 2;
							} else return;
						} else return;
					}
					if (loc_m2 == 4) {
						// STRING: GY454XE  Re 01E4C
						// STRING: GY455XE  Im 01E4C
						smart_strcat(input, "\xbd\xbb\xb8");
						mathi_insert_box(input, loc_m1, v3);
						concat_mathi_rl(input);
						loc_m2 = 0;
						if (pos != loc_m1) loc_m2 = count;
						mathi_insert_box(input, pos, loc_m2);
						concat_mathi_rl(input);
						loc_m2 = 0;
						if (pos == loc_m1) loc_m2 = count;
						mathi_insert_box(input, pos, loc_m2);
						concat_mathi_rend(input);
						count += v3;
					} else goto j_07200;
				}
			} else return;
		}
	}
	v3 = smart_strlen(input) - count;
	if (!(mathi || ins != 1 || !*v2) || (*v2 == '!' && !count)) --v3;
	loc_m7 = smart_strlen(v0);
	if ((char)(v3 + loc_m7) <= 99) {
		pos = loc_m1;
		loc_m10 = &v0[v3];
		loc_m2 = loc_m7;
		while (loc_m2 > pos) {
			loc_m10[loc_m2] = v0[loc_m2];
			--loc_m2;
		}
		loc_m10[loc_m2] = v0[loc_m2];
		v2 = &v0[pos];
		// strcpy(v2, input);
		do {
			*v2 = *input;
			++input;
			++v2;
		} while (*input);
		cursor_pos_byte = loc_m6 + pos;
	}
	return;
}

// FUNCTION: GY454XE  Re 073BC
// FUNCTION: GY455XE  Im 07AFE
// FUNCTION: GY460XF  Im 06FE2
static char is_mathi_char(char token) {
	// Hex A - Hex F
	if (is_mathi() && token >= 0xb8 && token <= 0xbd) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 073DC
// FUNCTION: GY455XE  Im 07B1E
// FUNCTION: GY460XF  Im 07002
static char *move_cursor(char direction) {
	char pos;
	char *input;
	char *cur_char;

	pos = cursor_pos_byte;
	input = input_area;
	if (!direction) {
		if (pos) --pos;
	} else ++pos;
	do {
		cur_char = &input[pos];
		if (is_mathi_char(*cur_char)) {
			// Hex B, Hex C, Box
			if ((*cur_char != 0xb9 && *cur_char != 0xba) || cur_char[-1] == '!') {
				if (!direction) {
					if (pos) --pos;
				} else ++pos;
				continue;
			}
		}
		break;
	} while (1);
	cursor_pos_byte = pos;
	return cur_char;
}

// FUNCTION: GY454XE  Re 07442
// FUNCTION: GY455XE  Im 07B84
// FUNCTION: GY460XF  Im 07068
static char *f_07442(char direction) {
	char *cur_char;

	cur_char = &input_area[cursor_pos_byte];
	// Check if character behind cursor is a box
	if (cursor_pos_byte && cur_char[-1] == '!') cur_char = move_cursor(direction);
	return cur_char;
}

// FUNCTION: GY454XE  Re 07470
// FUNCTION: GY455XE  Im 07BB2
// FUNCTION: GY460XF  Im 07096
void insert_token(char token, char is_func) {
	char input[100];

	_insert_token(token, is_func, input);
	return;
}

// FUNCTION: GY454XE  Re 07488
// FUNCTION: GY455XE  Im 07BCA
// FUNCTION: GY460XF  Im 070AE
void process_cursor_action(char keycode) {
	char *v0;
	char v1;
	char v2;
	char v3;
	char *v4;
	char v4_0;
	char *v5;
	char loc_m1;
	char loc_m6[5];
	char *loc_m8;
	char *loc_m10;
	char loc_m11;

	v0 = input_area;
	v1 = cursor_pos_byte;
	loc_m10 = &input_area[v1];
	v2 = is_ins_mode();
	if (v3 = is_mathi()) {
		smart_strcpy(get_result_str_ptr(), v0);
		d_08006 = v1;
	}
	switch (keycode) {
		case K_DEL:
			if (!*loc_m10) {
				if (v1 && v1 != 1) {
					if (!is_mathi_char(loc_m10[-1])) loc_m10[-1] = 0;
					move_cursor(0);
					return;
				} else {
					cursor_pos_byte = 0;
					input_area[0] = 0;
					return;
				}
			} else {
				if (v3 || !v2) loc_m10 = move_cursor(0);
				v4 = &loc_m10[1];
				if (v3) {
					// Box
					if (*loc_m10 == '!') {
						if (loc_m10 == v0) ++loc_m10;
						else return;
					}
					// Hex B, C
					if (*loc_m10 == 0xb9 || *loc_m10 == 0xba) return;
					if (f_05658(loc_m10) == 1) {
						char tmp;
						v4 = loc_m10;
						v4_0 = f_05824(loc_m10) + 1;
						loc_m1 = f_056D0(loc_m10);
						// Logarithm function
						if (*loc_m10 != 0x68) tmp = f_0579A(loc_m10, 0);
						else tmp = 0;
						loc_m6[0] = tmp;
						// Derivative function
						if (loc_m1 != 3 && *loc_m10 != 0x6b) tmp = f_0579A(loc_m10, 1);
						else tmp = 0;
						loc_m6[1] = tmp;
						if (loc_m1 == 4) tmp = f_0579A(loc_m10, 2);
						else tmp = 0;
						loc_m6[2] = tmp;
						// Exponentiation
						if (*v4 == '^') v4 = f_07442(0);
						loc_m8 = v4;
						loc_m1 = 0;
						do {
							loc_m11 = loc_m6[loc_m1];
							if (loc_m6[loc_m1]) {
								v5 = &loc_m10[loc_m11];
								if (f_088FE(v5)) tmp = 0;
								else tmp = f_05766(v5);
								loc_m11 = tmp;
								// memcpy(v5, v4, loc_m11);
								v2 = 0;
								while (v2 < loc_m11) {
									*v4++ = *v5++;
									++v2;
								}
							}
						} while (++loc_m1 < 3);
						v5 = &loc_m10[v4_0];
						if (v4 == loc_m8 && f_08A66(v4-1, v5)) *v4++ = '!';
						loc_m10 = v4;
						v4 = v5;
					} else if (f_08A66(loc_m10-1, v4)) {
						*loc_m10 = '!';
						return;
					}
				}
				do *loc_m10++ = *v4++;
				while (loc_m10[-1]);
			}
			break;
		case K_RIGHT:
			f_046AE();
			if (*loc_m10) {
				move_cursor(1);
				loc_m10 = f_07442(1);
				if (!*loc_m10) goto j_076a6;
			} else if (!v3) goto j_076a6;
			else
j_07696:
				cursor_pos_byte = 0;
j_0769c:
			if (!input_area[0])
j_076a6:
				f_046C4();
			break;
		case K_UP:
		case K_DOWN:
			if (!v3) {
				if (keycode != K_UP) goto j_076b6;
				else goto j_07696;
			} else {
				f_046AE();
				do {
					loc_m10 = f_06D90(loc_m10, &loc_m1);
					if (loc_m10) {
						loc_m11 = 0;
						switch (*loc_m10) {
							default: continue;
							case 0xae:
								if (keycode == K_UP) {
									v2 = 0;
									loc_m11 = 1;
								} else v2 = 1;
								break;
							case '|':
								if (loc_m1) {
									if (keycode == K_UP) {
										v2 = 1;
										loc_m11 = 1;
									} else v2 = 2;
								} else continue;
								break;
							case 0x69:
							case 0x6a:
								if (keycode == K_UP) v2 = 2;
								else {
									v2 = 1;
									loc_m11 = 1;
								}
								break;
						}
						if (v2 != loc_m1) {
							v2 = f_0579A(loc_m10, v2);
							loc_m1 = 0;
							if (loc_m11) loc_m1 = f_05766(&loc_m10[v2]);
							cursor_pos_byte = loc_m1 + (char)(loc_m10 - v0 + v2);
							f_07442(0);
							return;
						}
					} else goto j_076a6;
				} while (1);
			}
			break;
		case K_LEFT:
			f_046AE();
			if (v1) {
				move_cursor(0);
				loc_m10 = f_07442(0);
				if (loc_m10 != v0) goto j_0769c;
				else goto j_076a6;

			} else if (!v3) goto j_076a6;
			else {
j_076b6:
				cursor_pos_byte = smart_strlen(v0);
				goto j_0769c;
			}
	}
	return;
}

// FUNCTION: GY454XE  Re 077C4
// FUNCTION: GY455XE  Im 07F06
// FUNCTION: GY460XF  Im 073EA
static char is_num_1(char *num) {
	return num_cmp(num, num_1);
}

// FUNCTION: GY454XE  Re 077CC
// FUNCTION: GY455XE  Im 07F0E
// FUNCTION: GY460XF  Im 073F2
char num_to_str_std(char *out, char *num, char c) {
	char v0;
	char v1;
	char v2;
	char v3;
	char v4;
	char loc_m50[50];

	v0 = 0;
	v1 = 0;
	v2 = get_numtype(num);
	// Not ERROR format
	if (v2 != 0xf0) {
		num_cpy(loc_m50, num);
		out[0] = '\0';
		v0 = get_result_store_fmt();
#if ENABLE_FACT == 1
		if (v0 == RESULT_FACT) return num_to_str_fact(out, num);
#endif
		f_1B378(loc_m50);
		if (num_invalid__(loc_m50) == 1)
j_0781e:
			v0 = RESULT_DECIMAL;
		else {
			num_cpy(loc_m50, num);
			v3 = is_matho();
			f_04796();
			// Fraction format
			if (v2 == 0x20) {
				f_1AC92(loc_m50);
				num_cpy(&loc_m50[10], num);
				f_1AC7E(&loc_m50[10]);
				num_cpy(&loc_m50[20], num);
				f_1AC6A(&loc_m50[20]);
				v1 = 1;
				goto j_07b16;
			// Radical format
			} else if (v2 == 0x80) {
				if (!v3 || v0 != RESULT_STANDARD) {
					num_to_str_std_lineo(num);
					goto j_0781e;
				} else {
					f_16C54(loc_m50);
					v2 = num_invalid__(&loc_m50[20]);
					v0 = num_invalid__(loc_m50);
					v1 = is_num_1(&loc_m50[40]);
					v4 = is_eqn_result();
					if (v0 == 2) {
						if (v2 == 1) {
							num_abs(loc_m50);
							concat_negative(out);
						} else if (!v4 && v2 == 2 && (c || v1 != 1)) {
							num_abs(loc_m50);
							num_abs(&loc_m50[20]);
							v0 = 4;
							v2 = 4;
							concat_negative(out);
						}
					}
					if (v1 != 1) concat_mathi_frac_startl(out);
					// STRING: GY454XE  Re 01E50
					else if (c && v2 != 1) smart_strcat(out, "(");
					if (v0 == 2) {
						concat_negative(out);
						num_abs(loc_m50);
					}
					v0 = is_num_1(&loc_m50[10]);
					if (is_num_1(loc_m50) != 1 || v0 == 1) concat_num_str(out, loc_m50);
					if (v0 != 1) concat_sqrt(out, &loc_m50[10]);
					if (v2 != 1) {
						// STRING: GY454XE  Re 01E52
						if (v2 == 2) smart_strcat(out, "-");
						else concat_plus(out);
						num_abs(&loc_m50[20]);
						if (is_num_1(&loc_m50[20]) != 1) concat_num_str(out, &loc_m50[20]);
						concat_sqrt(out, &loc_m50[30]);
					}
					if (v1 != 1) {
						concat_mathi_rl(out);
						concat_num_str(out, &loc_m50[40]);
						concat_mathi_rend(out);
					// STRING: GY454XE  Re 01E54
					} else if (c && v2 != 1) smart_strcat(out, ")");
					v0 = RESULT_STANDARD;
				}
			} else if (num_get_exp(num) <= 9) {
				if (v0 > RESULT_DECIMAL) v1 = f_10C1E(loc_m50);
j_07b16:
				if (v1 == 1) {
#if ENABLE_RDEC == 1
					if (v0 == RESULT_RDEC) return num_to_str_rdec(out, loc_m50);
#endif
				} else {
					if (!get_disp_setting_noradical() && v0 >= RESULT_STANDARD && num_get_exp(num) >= -7 && v3) {
						v1 = f_10C2C(loc_m50);
						if (!v1) goto j_0781e;
					} else goto j_0781e;
				}
				v4 = num_invalid__(&loc_m50[10]);
				if (v4 == 1) {
					if (v1 == -1) concat_num_str(out, loc_m50);
					else goto j_0781e;
				} else {
					char tmp;
					char tmp2;
					
					num_abs(loc_m50);
					num_abs(&loc_m50[10]);
					v2 = 0;
					if (v0 == RESULT_FRAC_MIX) v2 = 1;
					else if ((v0 == 0 || v0 == 13) && setup_frac_result != FRAC_RESULT_IMP) v2 = 1;
					tmp = num_invalid__(loc_m50);
					tmp2 = 0;
					if (tmp != 1) tmp2 = 1;
					v0 = tmp2;
					if (v4 == 2) concat_negative(out);
					if (!v2 && v0 == 1) {
						f_1A438(loc_m50, &loc_m50[20]);
						f_1A460(&loc_m50[10], loc_m50);
					}
					if (v2 == 1 && v0 == 1) {
						// STRING: GY454XE  Re 01E56
						if (v3) smart_strcat(out, "|\xbd\xbb\xb8");
						concat_num_str(out, loc_m50);
						if (v3) concat_mathi_rl(out);
						else concat_10pow(out);
						tmp = RESULT_FRAC_MIX;
					} else {
						if (v3) concat_mathi_frac_startl(out);
						tmp = RESULT_FRAC;
					}
					v0 = tmp;
					if (v1 == -1) v0 = RESULT_STANDARD;
					concat_num_str(out, &loc_m50[10]);
					if (v3) concat_mathi_rl(out);
					else concat_10pow(out);
					concat_num_str(out, &loc_m50[20]);
					if (v3) concat_mathi_rend(out);
				}
				if (v1 == -1) f_02B3E(out, 0x82);
			} else goto j_0781e;
		}
	}
	return v0;
}

#if REAL == 0
// FUNCTION: GY455XE  Im 082A2
// FUNCTION: GY460XF  Im 077B0
int f_082A2_E(emu_kb *kb) {
	int v0;
	int v1;
	int v2;
	char v3;

	f_05338_E(kb->d_09804);
	v0 = smart_strlen(kb->error_buf) + 1;
	if (v0 > 1) {
		v0 += 4;
		kb->d_09834[0] = 0x22;
		kb->d_09834[1] = 0xff;
		kb->d_09834[2] = (char)(v0 >> 8);
		kb->d_09834[3] = (char)v0;
	} else kb->d_09834[0] = '\0';
	v1 = v0 + 0x30;
	v2 = 3;
	do {
		v3 = (char)v1 & 0xf;
		v1 >>= 4;
		if (v3 <= 9) v3 += 0x30;
		else if (0xa <= v3 && v3 <= 0xf) v3 += 0x37;
		kb->d_09800[v2] = v3;
	} while (--v2 >= 0);
	return v2;
}

// FUNCTION: GY455XE  Im 0832A
// FUNCTION: GY460XF  Im 07838
static void f_0832A_E(char *result_str, char *error_buf) {
	do {
		switch (f_0568E(result_str)) {
			case 1:
				*error_buf++ = '\x7f';
				switch (*result_str) {
					case 0x5e:
						*error_buf++ = 0x5a;
						break;
					case 0xa4:
						*error_buf++ = 0x73;
						break;
				}
				++result_str;
				break;
			case 3: {
				char tmp;
				*error_buf++ = '\x7f';
				switch (*f_06D90(result_str, &tmp)) {
					case 0x5e:
						*error_buf++ = 0x5b;
						break;
					case 0xa4:
						*error_buf++ = 0x74;
						break;
				}
				break;
			}
			default: {
				char tmp = *result_str;
				if (*result_str >= 0xe0) tmp -= 0x50;
				*error_buf++ = tmp;
				break;
			}
		}
	} while (*++result_str);
	return;
}
#endif

// FUNCTION: GY454XE  Re 07B60
// FUNCTION: GY455XE  Im 083B8
// FUNCTION: GY460XF  Im 078C6
void print_result(char *result) {
	char v0;
	char v1;
	char v2;
	char v3;
	char v4;
	char v5;
	char *v6;
	char v7;
#if REAL == 0
	emu_kb kb;
#endif
	char loc_m36[36];
	char loc_m56[20];
	char loc_m92[36];
	char loc_m128[36];
	char loc_m129;
	char loc_m130;
	char loc_m131;
	char *loc_m134;

#if REAL == 0
	init_emu_kb(&kb);
#endif
	if (f_0A57A()) num_output_print(result);
#if ENABLE_INEQ == 1
	else if (is_ineq_result()) print_result_ineq();
#endif
	else {
		v0 = 0;
		loc_m129 = 0;
		v1 = result_template;
		v2 = f_02C76();
		v3 = is_matho();
		v4 = 0;
		v5 = 0;
		if (v3 || is_mathi()) v4 = 1;
		v5 = v4;
		v6 = get_result_str_ptr();
		if (!v6 || !v4) v6 = loc_m36;
		v7 = get_result_store_fmt();
		v4 = 0;
		if (v1 & (1 << 4)) {
			if (v1 == 0x13 || !v5) set_result_fmt(RESULT_DECIMAL);
			v7 = get_result_store_fmt();
			v4 = 1;
		}
		if (v5) {
			if (v7 != RESULT_DECIMAL) {
				v0 = 1;
				use_output_charset = 1;
			}
			if (!d_0812C) {
j_07bfe:
				*v6 = 0;
				cursor_pos_byte = 0;
				num_cpy_im(loc_m56, result);
				loc_m130 = num_invalid__(loc_m56);
				v1 = num_invalid__(&loc_m56[10]);
				loc_m131 = 0;
				if (!v2) {
					v2 = 1;
					if (loc_m130 != 1 || v1 != 1) {
						if (!is_dms_num(loc_m56) || v1 != 1 || loc_m130 != 4) {
							f_16E44(loc_m56, &loc_m56[10]);
							loc_m130 = num_invalid__(loc_m56);
							v1 = num_invalid__(&loc_m56[10]);
							if (v1 != 1) {
								v2 = 0;
								loc_m131 = 6;
							}
						}
					}
				}
				if (!v5) buffer_clear_lastnline(12);
				loc_m134 = v6;
				if (v4) loc_m134 = loc_m92;
				if (loc_m130 != 1 || v1 == 1 || v4) {
					loc_m129 = 1;
					loc_m130 = num_to_str(loc_m56, loc_m134, loc_m131);
				} else {
					loc_m130 = 0xf0;
					loc_m92[0] = '\0';
				}
				if (v4) {
					concat_result_template(0, v6);
					concat_result(v6, loc_m92);
				}
				if (v1 == 1 && !v4) {
					set_result_store2disp(loc_m130);
					if (!v0) {
						if (mode == MODE_BASE_N) basen_base_print();
						smart_strcpy(loc_m92, v6);
						v6[0] = '\0';
					} else goto j_07e64;
				} else {
					if (loc_m129) {
						loc_m131 = 1;
						if (!v2) {
							char tmp;
							if (v1 == 2) tmp = 2;
							else tmp = 5;
							loc_m131 = tmp;
						}
					} else if (v1 == 2) loc_m131 = 3;
					else loc_m131 = 4;
					v1 = num_to_str(&loc_m56[10], loc_m128, loc_m131);
					if (v2 && !v4) f_02B3E(loc_m128, 0x80);
					concat_result_template(1, loc_m92);
					if (!v4) {
						if (v2) {
							if (loc_m129) {
								if (loc_m128[0] == '`') loc_m128[0] = '-';
								else concat_plus(loc_m92);
							}
						} else concat_angle(loc_m92);
					}
					if (v7) {
						loc_m130 = select_result_format(loc_m130, v1);
						set_result_store2disp(loc_m130);
					}
					concat_result(loc_m92, loc_m128);
					if (v0) goto j_07e4e;
				}
				if (v5) {
					char tmp;
					char tmp2;
					v4 = smart_strlen(v6);
					tmp = max(v4, (char)smart_strlen(loc_m92)) * 6;
					if (!v4) tmp2 = 10;
					else tmp2 = 20;
					loc_m131 = tmp2;
					if (f_08ABA(tmp)) f_10E5C(loc_m131);
				}
				result_str_print(v6, 0);
				result_str_print(loc_m92, 1);
#if REAL == 0
				v4 = smart_strlen(v6);
				kb.error_buf[0] = '\0';
				if (v4) {
					smart_strcat(kb.error_buf, v6);
					// STRING: GY455XE  Im 01E5B
					smart_strcat(kb.error_buf, "\r");
				}
				smart_strcat(kb.error_buf, loc_m92);
				f_082A2_E(&kb);
#endif
				goto j_07e88;
			}
		} else goto j_07bfe;
		goto j_07e64;
j_07e4e:
		if (v4) concat_argsep(v6);
		smart_strcat(v6, loc_m92);
j_07e64:
#if REAL == 0
		kb.error_buf[0] = '\0';
		if (v7 > 13) f_0832A_E(v6, kb.error_buf);
		f_082A2_E(&kb);
#endif
		if (!is_mathi()) buffer_clear_lastnline(22);  // This line doesn't appear in fx-570ES+ VerE real for some reason...???
		d_08122 = 1;
		loc_m134 = input_area_ptr;
		input_area_ptr = v6;
#if REAL == 0
		if (mode == MODE_VERIF) {
			v4 = smart_strlen(v6);
			kb.error_buf[0] = '\0';
			if (v4) smart_strcat(kb.error_buf, v6);
			f_082A2_E(&kb);
		}
#endif
		input_print_mathi();
		input_area_ptr = loc_m134;
		use_output_charset = 0;
	}
j_07e88:
	return;
}
