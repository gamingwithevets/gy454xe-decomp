#include "consts.h"
#include "generals.h"
#include "emu_kb.h"
#include "input.h"
#include "unk2.h"
#include "unk4.h"
#include "unk5.h"

// Static declarations
static void f_04E6E(char x, char y);
static char f_04E86(char a);
static char f_04E9C(char a);
static void f_05162(int a, int b, char c);
static void f_051CE(int a, char b, char c);
static void f_05222(int a, char b);
static void f_05260(char *a, int b);
static char f_052D0(char a, int b, char c, char d);
static void f_05366(f_058DC_union *a, int b, char c);
static void f_05516(int a, char b, char c, f_058DC_union d);
static void f_055BC(int a, char b, char *c);
static char f_05652(char *a);
static char f_0568E(char *a);
static char f_056AE(char *a);
static char f_056D0(char *a);
static void f_056F2(char *a, char *b);
static char f_05766(char *a);
static char f_0579A(char *a, char b);
static char f_05824(char *a);
static char f_0587E(char *a);
static char *f_05B2C(char *a, f_058DC_union *b, int c, char d);
static char *f_05B86(char *a, f_058DC_union *b, int c, char d);
static char *f_05D0C(char *a, f_058DC_union *b, int c, char d);
static char *f_05EBC(char *a, f_058DC_union *b, int c, char d);
static char *f_06002(char *a, f_058DC_union *b, int c, char d);
static char *f_062CE(char *a, f_058DC_union *b, int c, char d);
static char *f_06456(char *a, f_058DC_union *b, int c, char d);
static char *f_0666C(char *a, f_058DC_union *b, int c, char d);
static char f_06914(char *a);
static char *f_06944(char *a, f_058DC_union *b, int c, char d);
static char f_06C06(char *a);
static char *f_06D90(char *a, char *b);
static char f_06E40(char *a, char b, char c);
static char *f_06F1C(char *a);
static void f_06FCE(char *a, char b, char c);
static void f_07022(char token, char b, char *input);
static char is_mathi_char(char token);
static char *move_cursor(char a);
static char *f_07442(char a);

extern const char num_1[];

// DATA: GY454XE  Re 01DDC
const char unk_01ddc[] = {
	0, 0, 1, 3, 3, 1, 0, 4, 0, 0, 2, 2
};

// DATA: GY454XE  Re 01DF4
static char *(*const jmp_01df4[])(char *, f_058DC_union *, int, char) = {
	f_05B86,
	f_05B2C,
	f_05D0C,
	f_0666C,
	f_06456,
	f_05EBC,
	f_05B86,
	f_06002,
	f_05B86,
	f_05B2C,
	f_062CE,
	f_06002,
};

// DATA: GY454XE  Re 01E24
const char unk_01e24[] = {
	3, 5, 8, 7, 12
};

// FUNCTION: GY454XE  Re 04E6E
static void f_04E6E(char x, char y) {
	cursor_x = x;
	cursor_y = y;
	f_10FD2();
	d_08117 = font_size;
}

// FUNCTION: GY454XE  Re 04E86
static char f_04E86(char a) {
	if (d_080FE & (1 << 6) && a == 0x3a) return 1;
	else if (a) return 0;
	else return 1;
}

// FUNCTION: GY454XE  Re 04E9C
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
			if (f_04E86(v6)) {
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
		v6 = *(v0 + v2);
		v4 = v6;
		v7 = (char)((f_04E86(v6) ? 1 : get_token(v4, loc_m22)) * 6);
		if (v3 >= v7) {
			v3 -= v7;
			if (v0) --v0;
			else break;
		} else {
			++v0;
			break;
		}
	}
	
j_04f60:
	return v0;
}

// FUNCTION: GY454XE  Re 04F6E
void f_04F6E(void) {
	char *v0;
	char v1;
	char v2;
	char v3;
	char v4;
	char v5;
	char len;
	char v6;
	char v7;
	char loc_m2[2];
	char loc_m34[50];
	char loc_m56[34];

	loc_m2[0] = ' ';
	loc_m2[1] = '\0';
	v0 = input_area_ptr;
	v1 = formula_y;
	if (f_08ADC()) {
		table_prompt_print(v1);
		formula_x += 30;
	}
	v2 = formula_x;
	v3 = f_04E9C(90);
	v4 = v2;
	v5 = v3;
	loc_m34[0] = '\0';
	v0 = v3 + v0;
	while (v4 <= 96) {
		if (cursor_pos_byte == v5) f_04E6E(v4, v1);
		v7 = *v0;
		if (!f_04E86(v7) && (len = get_token(v7, loc_m56))) {
			v4 += (char)(len * 6);
			smart_strcat(loc_m34, loc_m56);
			++v5;
			++v0;
		} else break;
	}
	v6 = 1;
	if (f_04E86(v7)) v6 = 0;
	v4 = (char)(smart_strlen(loc_m34) * 6) + v2;
	while (v4 < 60) {
		smart_strcat(loc_m34, loc_m2);
		v4 += 6;
		v6 = 0;
	}
	line_print(v2, v1, loc_m34);
	if (v3 > 0) f_10F52(v1);
	if (v6) f_10F76(v1);
	formula_x = 0;

	return;
}

// FUNCTION: GY454XE  Re 05162
static void f_05162(int a, int b, char c) {
	int v0;

	if (d_08009 && c >= d_08005) {
		c -= d_08005;
		if (c <= 31) {
			v0 = f_088AA();
			if (b >= v0) {
				if (a < v0) a = formula_x;
				else {
					a -= d_08000;
					if (a > 95) return;
				}
				b -= d_08000;
				if (b > 95) b = 95;
				f_02DD8((char)a, c, (char)b, c);
			}
		}
	}

	return;
}

// STUB: GY454XE  Re 051CE
static void f_051CE(int a, char b, char c) {
	return;
}

// STUB: GY454XE  Re 05222
static void f_05222(int a, char b) {
	return;
}

// FUNCTION: GY454XE  Re 05260
static void f_05260(char *a, int b, char c) {
	int v0;

	if (d_08009) {
		c -= get_font_width();
		do {
			if (b < f_088AA()) {
				if (*++a) b += 6;
				else break;
			} else {
				b -= d_08000;
				if (b <= 95) {
					v0 = c - d_08005;
					if (v0 >= -10 && v0 <= 31) line_print((char)b, (char)v0, a);
				}
				break;
			}
		} while (1);
	}

	return;
}

// FUNCTION: GY454XE  Re 052D0
static char f_052D0(char a, int b, char c, char d) {
	char v0;
	char loc_m20[20];

	if (d_08009) {
		if (use_output_charset && (a < 0x5e || a >= 0xc0)) {
			v0 = 1;
			if (a >= 0xe0) a -= 0x50;
			loc_m20[0] = a;
			loc_m20[1] = '\0';
		} else v0 = get_token(a, loc_m20);
		if (d) loc_m20[--v0] = '\0';
		f_05260(loc_m20, b, c);
	} else {
		v0 = use_output_charset && (a < 0x5e || a >= 0xc0) ? (char)1 : (char)get_token_length(a);
		if (d) --v0;
	}
	return v0 * 6;
}

// STUB: GY454XE  Re 05366
static void f_05366(f_058DC_union *a, int b, char c) {
	return;
}

// STUB: GY454XE  Re 05516
static void f_05516(int a, char b, char c, f_058DC_union d) {
	return;
}

// STUB: GY454XE  Re 055BC
static void f_055BC(int a, char b, char *c) {
	return;
}

// FUNCTION: GY454XE  Re 05652
static char f_05652(char *a) {
	return f_02676(a[0]);
}

// FUNCTION: GY454XE  Re 05658
char f_05658(char *a) {
	char v0;

	v0 = f_05652(a);
	if (v0 == 1) {
		if (a[1] != 0xd0 && a[1] != 0xd3 && a[1] != 0xd5) {
			if (a[0] == 0xcc || a[0] == 0x7c) v0 = 13;
			else v0 = 10;
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0568E
static char f_0568E(char *a) {
	char v0;

	v0 = f_05658(a);
	// Hex A
	if (v0 == 3 && a[1] == 0xb8) v0 = 4;
	return v0;
}

// FUNCTION: GY454XE  Re 056AE
static char f_056AE(char *a) {
	if (f_05652(a) != 1) return 5;
	return unk_01ddc[f_0270E(a[0])];
}

// FUNCTION: GY454XE  Re 056D0
static char f_056D0(char *a) {
	if (f_05658(a) != 1) return 5;
	return unk_01ddc[f_0270E(a[0])];
}

// FUNCTION: GY454XE  Re 056F2
static void f_056F2(char *a, char *b) {
	char v0;
	char v1;
	char v2;

	v0 = 1;
	v1 = 1;
	*b = '\0';
	while (input_area_ptr != a) {
		--a;
		v2 = f_05658(a);
		if (v1 == 1 || v1 == 9) ++v0;
		else if (v1 == 1 && (v2 == 8 || v2 == 10)) {
			if (!--v0) {
				*b = 1;
				break;
			}
		} else if (v2 == 3) ++v1;
		else if (v2 == 2) {
			if (!--v1) break;
		} else if (v2 == 5) {
			if (v1 != 1) continue;
			else break;
		}
	}
}

// FUNCTION: GY454XE  Re 05766
static char f_05766(char *a) {
	char v0;
	char v1;
	char v2;

	v0 = 0;
	v1 = 1;
	while (v2 = *a) {
		// Hex A
		if (v2 == 0xb8) ++v1;
		// Hex B
		else if (v2 == 0xb9 && !--v1) break;
		// Hex C
		else if (v2 == 0xba && v1 == 1) break;
		++v0;
		++a;
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0579A
static char f_0579A(char *a, char b) {
	char v0;
	char v1;
	char v2;

	v0 = 0;
	v1 = f_056D0(a);
	if ((b < 1 || v1) && (b != 2 || v1 == 3 || v1 == 4)) {
		v0 = v1 == 2 ? 3 : (v1 == 4 ? 4 : 2);
		if (b) {
			a += v0;
			v1 = 0;
			while (v1 < b) {
				v2 = 1;
				while (*a) {
					// Hex A
					if (*a == 0xb8) ++v2;
					// Hex B
					else if (*a == 0xb9 && !--v2) break;
					// Hex C
					else if (*a == 0xba && v2 == 1) break;
					++v0;
					++a;
				}
				++v0;
				// Hex A
				if (*++a == 0xb8) {
					++a;
					++v0;
				}
				++v1;
			}
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 05824
static char f_05824(char *a) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = f_056D0(a);
	v1 = 0;
	if (v2 = f_0579A(a, v1)) {
		a += v2;
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
		while (*a) {
			if (*a == v0) ++v1;
			else if (*a == v3 && !--v1) break;
			++v2;
			++a;
		}
	}
	return v2;
}

// STUB: GY454XE  Re 0587E
static char f_0587E(char *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 058DC
void f_058DC(void) {
	char v0;
	char v1;
	char v2;
	char *v3;
	unsigned int v4;
	char v5;
	f_058DC_union loc_m4;

	v0 = 0;
	v1 = 0;
	v2 = d_08122;
	if (d_080FE != 1 && v2) v1 = 1;
	v3 = input_area_ptr;
	v4 = 0;
	d_08000 = 0;
	d_08005 = 0;
	f_08A9C(0);
	if (!f_06944(v3, &loc_m4, 0, 62) || loc_m4.byte[2] > 62) {
		smart_strcpy(v3, f_11030());
		cursor_pos_byte = d_08006;
		f_08A9C(0);
		f_06944(v3, &loc_m4, 0, 62);
	}
	if (f_08ADC()) {
		formula_x += 30;
		v0 = 1;
	}
	v5 = loc_m4.word[0] > 96 ? 96 : loc_m4.byte[0];
	if (!v1) {
		d_08124 = loc_m4.byte[2];
		d_08111 = v5;
		v4 = formula_x;
		v1 = 96 - formula_x - 6;
		while (d_08002 - d_08000 <= v1 && loc_m4.word[0] - d_08000 <= v1) d_08000 += 8;
		v1 = loc_m4.byte[2] - loc_m4.byte[3] + 1;
		if (loc_m4.byte[2] > 31) {
			char tmp = loc_m4.byte[3] + 62 - d_08004;
			if (v2 && tmp > 12) {
				v5 = 93	- v1 - 4;
				while ((char)(d_08004 - d_08005) > v5) d_08005 += 8;
			} else {
				d_08005 = loc_m4.byte[2] - 31;
				d_08124 = 31;
			}
		}
		buffer_clear();
	} else {
		if (f_08ABA(v5)) f_10E5C(loc_m4.byte[2]);
		if (loc_m4.word[0] <= 96) v4 = (char)(96 - loc_m4.byte[0]);
		else {
			d_08000 = cursor_pos_byte << 3;
			if (loc_m4.word[0] - d_08000 <= 88) {
				d_08000 = --cursor_pos_byte << 3;
				f_046C4();
			}
			d_0812C = 1;
			buffer_clear_lastnline(32 - loc_m4.byte[2]);
		}
		v1 = 32 - loc_m4.byte[3];
	}
	f_08A9C(1);
	f_06944(v3, &loc_m4, v4, v1);
	v4 = d_08000;
	v5 = d_08005;
	v1 -= v5 + 5;
	if (v0) table_prompt_print(v1);
	if (v4 > 0) f_10F52(v1);
	if (loc_m4.word[0] - v4 > (char)(96 - formula_x)) f_10F76(v1);
	formula_x = 0;
	if (d_080FE == 1) {
		font_size = 6;
		if (v5 > 0) f_10F92();
		if (loc_m4.byte[2] > (char)(v5 + 31)) f_10FA2();
	}

	return;
}

// FUNCTION: GY454XE  Re 05B2C
static char *f_05B2C(char *a, f_058DC_union *b, int c, char d) {
	char v0;
	char *v1;

	v0 = *a == 0x98 ? 5 : 6;
	if (v1 = f_06944(a+2, b, v0+c, d)) f_05366(b, c, d);
	return v1;
}

// FUNCTION: GY454XE  Re 05B86
static char *f_05B86(char *a, f_058DC_union *b, int c, char d) {
	char v0;
	char *v1;
	char v2;
	char loc_m2[2];
	char loc_m3;
	char loc_m4;
	char loc_m5;
	char loc_m6;
	char loc_m7;

	loc_m6 = 0;
	v0 = d_08008;
	if (++d_08008 > 50) return 0;
	if (!d_08009) {
		loc_m7 = d_0800A;
		d_0800A = 0;
	}
	loc_m4 = b->byte[3];
	v2 = b->byte[2];
	if (*a != 0x5e) {
		loc_m2[0] = *a == 0x73 ? 0x81 : 0x83;
		loc_m2[1] = '\0';
		f_05260(loc_m2, c, d);
		loc_m6 = 6;
		c += 6;
		loc_m4 = f_0898C();
		v2 = f_0897C();
	}
	loc_m5 = f_08ACC();
	loc_m3 = v2 - 5;
	if (!d_08009) {
		if (v1 = f_06944(a+2, b, c, d)) {
			d_08640[v0].word[0] = b->word[0];
			d_08640[v0].word[1] = b->word[1];
			if (d_0800A) d_08004 -= loc_m3 - loc_m4 + b->byte[3];
j_05c7a:
			if (is_pow_char(v1+1)) return 0;
			font_size = loc_m5;
			b->word[0] += (char)(loc_m6 + 1);
			b->byte[2] += loc_m3;
			b->byte[3] += loc_m4;
			if (!d_08009 && !d_0800A) d_0800A = loc_m7;
		}
		return v1;
	} else {
		b->word[0] = d_08640[v0].word[0];
		b->word[1] = d_08640[v0].word[1];
		d -= loc_m3 - loc_m4 + b->byte[3];
		if (v1 = f_06944(a+2, b, c, d)) goto j_05c7a;
		else return v1;
	}
}

// STUB: GY454XE  Re 05D0C
static char *f_05D0C(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// FUNCTION: GY454XE  Re 05EBC
static char *f_05EBC(char *a, f_058DC_union *b, int c, char d) {
	char v0;
	int v1;
	char *v2;
	f_058DC_union loc_m4;

	v0 = f_08ACC();
	// STRING: GY454XE  Re 01E2D
	f_05260("d", c+4, d-4);
	// STRING: GY454XE  Re 01E2F
	v1 = c+1;
	f_05260("dx", v1, d+5);
	font_size = v0;
	f_05162(v1, c+13, d);
	c += 20;
	if (v2 = f_06944(a+2, b, c, d)) {
		f_05516(c-6, d, 0, *b);
		f_05516(b->word[0] + c, d, 1, *b);
		v0 = f_08ACC();
		c = b->word[0] + c + 6;
		f_051CE(c, d-4, d+7);
		c += 3;
		d += 5;
		f_05260("x=", c, d);
		c += 12;
		if (v2 = f_06944(v2+1, &loc_m4, c, d)) {
			loc_m4.byte[3] += 5;
			font_size = v0;
			b->word[0] += loc_m4.word[0] + 42;
			f_089AC(b, loc_m4);
			loc_m4.byte[2] = 16;
			loc_m4.byte[3] = 8;
			f_089AC(b, loc_m4);
		}
	}
	return v2;
}

// STUB: GY454XE  Re 06002
static char *f_06002(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// STUB: GY454XE  Re 062CE
static char *f_062CE(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// STUB: GY454XE  Re 06456
static char *f_06456(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// STUB: GY454XE  Re 0666C
static char *f_0666C(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// FUNCTION: GY454XE  Re 06914
static char f_06914(char *a) {
	if (a > input_area_ptr) {
		if (a[-1] == 0xb9 || a[-1] == 0xbc) return 1;
		else if (a[-1] == 0xb8 || a[-1] == 0xba) return 0;
		else if (f_14532(a[-1])) return 1;
		else return 0;
	} else return 0;
}

// FUNCTION: GY454XE  Re 06944
static char *f_06944(char *a, f_058DC_union *b, int c, char d) {
	int v0;
	f_058DC_union loc_m4;
	char loc_m5;

	v0 = c;
	if (++d_08007 > 27 || get_remaining_stack_space() < 200)
j_06966:
		a = 0;
	else {
		b->word[0] = 0;
		b->byte[2] = f_0897C();
		b->byte[3] = f_0898C();
		loc_m4.word[0] = b->word[0];
		loc_m4.word[1] = b->word[1];
		do {
			c = b->word[0] + v0;
			if ((char)a - (char)input_area_ptr == cursor_pos_byte) {
				f_055BC(c, d, a);
				if (!d_08009) d_0800A = 1;
			}
			// SWITCH: GY454XE  Re 01DB2
			switch (f_05658(a)) {
				default: goto j_06966;
				// CASE: GY454XE  Re 069F4
				case 1:
					++d_08007;
					if (a = jmp_01df4[f_0270E(*a)](a, &loc_m4, c, d)) {
						--d_08007;
						if (loc_m4.byte[2] <= 62) {
							loc_m4.word[0] += loc_m4.word[1];
							f_089AC(b, loc_m4);
j_069e6:
							++a;
							continue;
						}
						else goto j_06966;
					}
					break;
				// CASE: GY454XE  Re 06A58
				case 9:
					f_05516(c, d, 1, *b);
					b->word[0] += 6;
					f_056F2(a, &loc_m5);
					if (!loc_m5) {
						loc_m4.word[0] = b->word[0];
						loc_m4.word[1] = b->word[1];
						goto j_069e6;
					}
					break;
				// CASE: GY454XE  Re 06A92
				case 10:
					loc_m5 = f_052D0(*a, c, d, 1);
					c += loc_m5;
					b->word[0] += loc_m5;
				// CASE: GY454XE  Re 06AB4
				case 8:
					if (a = f_06944(a+1, &loc_m4, c+6, d)) {
						f_05516(c, d, 0, loc_m4);
						b->word[0] += loc_m4.word[0] + 6;
						f_089AC(b, loc_m4);
						if (*a == 41) goto j_069e6;
					}
					break;
				// CASE: GY454XE  Re 06B0E
				case 3:
				case 5:
				case 11:
					break;
				// CASE: GY454XE  Re 06B12
				case 0:
				case 12:
				case 13:
					if (!f_04E86(*a)) {
						loc_m4.byte[2] = f_0897C();
						loc_m4.byte[3] = f_0898C();
						b->word[0] += f_052D0(*a, c, d, 0);
						goto j_069e6;
					}
					break;
				// CASE: GY454XE  Re 06B4E
				case 2:
				case 4:
				case 6:
				case 7:
					break;
			}
			break;
		} while (1);
	}
j_069ea:
	--d_08007;
	return a;
}

// STUB: GY454XE  Re 06B52
char f_06B52(char *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 06C06
static char f_06C06(char *a) {
	char v0;
	char v1;

	v0 = 0;
	v1 = 0;
	while (1) {
		// Hex B
		if (*a-- == 0xb9) {
			++v0;
			continue;
		// Hex A
		} else if (*a == 0xb8 && !--v0) break;
	}
	// Hex D 
	if (*a == 0xbb) {
		// Hex F
		if (*--a == 0xbd) --a;
		// Cursor (unused), Fraction
		v1 = *a == 0x7c ? 0xae : *a;
	}
	// Hex B, Fraction
	else if (*a == 0xb9) v1 = 0xae;
	return v1;
}

// FUNCTION: GY454XE  Re 06C54
void f_06C54(char *a, char *b, char c, char d) {
	char *v0;
	char *v1;
	char v2;
	char *v3;
	char loc_m1;

	loc_m1 = d_080FE & (1 << 6);
	v0 = a;
	v1 = b;
	do {
		// SWITCH: GY454XE  Re 01DCE
		switch (f_0568E(a)) {
			// CASE: GY454XE  Re 06D0A
			case 1:
				v2 = f_056D0(a);
				if (v2 != 2) {
					if (v2 != 4) *b++ = *a;
					else ++a;
				}
				break;
			// CASE: GY454XE  Re 06D20
			case 2:
				// Fraction
				if (f_05652(&b[-1]) == 1 || b[-1] == 0xae) *b++ = '(';
				break;
			// CASE: GY454XE  Re 06D42
			case 4:
				*b++ = ')';
				if (d || c + v1 + 1 != b) *b++ = f_06C06(a);
				else return;
				break;
			// CASE: GY454XE  Re 06D68
			case 6:
				*b++ = '(';
				break;
			// CASE: GY454XE  Re 06D72
			case 3:
			case 7:
				*b++ = ')';
				break;
			// CASE: GY454XE  Re 06D7C
			case 5:
				*b++ = ',';
				break;
			// CASE: GY454XE  Re 06D86
			default:
				*b++ = *a;
				break;
		}
		if (d) {
			if (loc_m1 && d_0812A == b && input_area_ptr == v0) input_area_ptr = a + 1;
		} else if (c + v1 + 1 == b) {
			// Hex E, Hex A
			if (*a == 0xbc || *a == 0xb8) ++a;
			// Hex D
			else if (*a == 0xbb) --a;
			// Hex F
			if (*a == 0xbd) --a;
			goto j_06cf6;
		}
	} while (*++a);
	*b = '\0';
	if (!d)
j_06cf6:
		cursor_pos_byte = a - v0;
	return;
}

// FUNCTION: GY454XE  Re 06D90
static char *f_06D90(char *a, char *b) {
	char v0;
	char *v1;
	char v2;
	char v3;

	v0 = 1;
	*b = 0;
	v1 = a;
	// Hex E
	if (*a == 0xbc) --a;
	// Hex B
	if (*a == 0xb9) --a;
	while ((v2 = *a), input_area_ptr != a) {
		--a;
		// Hex B
		if (v2 == 0xb9) ++v0;
		// Hex A, Hex B
		else if (v2 == 0xb8 && !--v0 && *a != 0xb9) {
			// Hex D, Hex F
			if (*a == 0xbb && *--a == 0xbd) --a;
			v3 = (char)v1 - (char)a;
			if (v3 <= f_05824(a)) {
				if (v2 = f_056D0(a) && v3 >= f_0579A(a, 1)) {
					*b = 1;
					if (v2 != 1 && v2 != 2 && v3 >= f_0579A(a, 2)) *b = 2;
				}
			} else a = 0;
			break;
		}
	}
	return a;
}

// STUB: GY454XE  Re 06E40
static char f_06E40(char *a, char b, char c) {
	return 0;
}

// STUB: GY454XE  Re 06F1C
static char *f_06F1C(char *a) {
	return 0;
}

// STUB: GY454XE  Re 06FCE
static void f_06FCE(char *a, char b, char c) {
	return;
}

// FUNCTION: GY454XE  Re 07022
static void f_07022(char token, char b, char *input) {
	char *v0;
	char v1;
	char *v2;
	char v3;
	char tmp;
	char loc_m1;
	char loc_m2;
	char loc_m3;
	char loc_m4;
	char loc_m5;
	char loc_m6;
	char loc_m7;
	char *loc_m10;
	char loc_m11;

	v0 = input_area;
	loc_m4 = cursor_pos_byte;
	loc_m1 = cursor_pos_byte;
	loc_m5 = is_mathi();
	loc_m3 = is_ins_mode();
	input[0] = token;
	input[1] = 0;
	loc_m6 = 1;
	v1 = 0;
	v2 = &v0[loc_m4];
	if (loc_m5) {
		smart_strcpy(f_11030(), v0);
		d_08006 = loc_m4;
		loc_m7 = is_pow_char(v2);
		if (b == 0) {
			if (loc_m7 || (*v2 == 0x21 && is_pow_char(&v2[1]))) {
				// STRING: GY454XE  Re 01E35
				if (f_08A2A(f_05658(input))) smart_strcat(input, "!");
			}
		} else {
			// Check for x^n, nth root, cursor (unused)
			if (loc_m3 == 1 || token == 0x5e || token == 0x9f || token == 0xae || token == 0x7c) v1 = f_06E40(v2, 0, loc_m3);
			loc_m6 = 2;
			// Check for x^2, x^3, x^-1
			if (token == 0x75 || token == 0x76 || token == 0x77) {
				// Convert to x^n
				input[0] = 0x5e;
				v1 = 0;
				loc_m6 = 4;
				// Check for x^-1
				if (token == 0x77) loc_m6 = 5;
			// Check for cube root, replace with nth root
			} else if (token == 0xa8) input[0] = 0x9f;
			v3 = 0;
			loc_m2 = f_056AE(input);
			if (input[0] == 0x5e && f_08A48(&input[-1])) {
				// Add a box then the pow
				// STRING: GY454XE  Re 01E37
				smart_strcpy(input, "!^");
				loc_m6 = 0;
				v3 = 1;
			}
			loc_m11 = unk_01e24[loc_m2];
			if (unk_01e24[loc_m2] + v1 + loc_m7 + v3 < 100) {
				if (!loc_m2) {
					concat_mathi_l(input);
					// STRING: GY454XE  Re 01E3A
					if (token == 0x75) smart_strcat(input, "2");
					// STRING: GY454XE  Re 01E3C
					else if (token == 0x76) smart_strcat(input, "3");
					// STRING: GY454XE  Re 01E3E
					else if (token == 0x77) smart_strcat(input, "`1");
					else f_06FCE(input, loc_m4, v1);
					concat_mathi_r(input);
				} else if (loc_m2 == 1) {
					concat_mathi_l(input);
					if (token == 0x68) loc_m2 = 0;
					else loc_m2 = v1;
					f_06FCE(input, loc_m4, loc_m2);
					// STRING: GY454XE  Re 01E41
					smart_strcat(input, "\xba");
					if (token == 0x68) loc_m2 = v1;
					else loc_m2 = 0;
					f_06FCE(input, loc_m4, loc_m2);
					concat_mathi_r(input);
				} else if (loc_m2 == 2) {
					loc_m6 = 3;
					if (token == 0xa8) {
j_07200:
						// STRING: GY454XE  Re 01E43
						smart_strcat(input, "\xbb\xb8");
						if (token == 0xa8) {
							// STRING: GY454XE  Re 01E3C
							smart_strcat(input, "3");
							loc_m6 = 6;
						} else f_06FCE(input, loc_m1, v3);
						concat_mathi_root_l(input);
						f_06FCE(input, loc_m4, v1);
						concat_mathi_root_r(input);
						v1 += v3;
					} else goto j_07272;
				} else if (loc_m2 == 3) {
					concat_mathi_l(input);
					f_06FCE(input, loc_m4, v1);
					// STRING: GY454XE  Re 01E46
					smart_strcat(input, "\xba!\xba!\xb9");
				} else if (loc_m2 == 4) {
					loc_m6 = 4;
j_07272:
					if (f_06914(v2)) {
						loc_m10 = f_06F1C(v2);
						v3 = (char)v2 - (char)loc_m10;
						if (v3) {
							tmp = is_pow_char(loc_m10);
							if (!v3 && loc_m7 + v1 + v3 + tmp + loc_m11 < 100) {
								loc_m1 -= v3;
								loc_m6 += tmp + v3 + 2;
							} else return;
						} else return;
					}
					if (loc_m2 == 4) {
						// STRING: GY454XE  Re 01E4C
						smart_strcat(input, "\xbd\xbb\xb8");
						f_06FCE(input, loc_m1, v3);
						concat_mathi_root_l(input);
						loc_m2 = 0;
						if (loc_m4 != loc_m1) loc_m2 = v1;
						f_06FCE(input, loc_m4, loc_m2);
						concat_mathi_root_l(input);
						loc_m2 = 0;
						if (loc_m4 == loc_m1) loc_m2 = v1;
						f_06FCE(input, loc_m4, loc_m2);
						concat_mathi_root_r(input);
						v1 += v3;
					} else goto j_07200;
				}
			} else return;
		}
	}
	v3 = smart_strlen(input) - v1;
	if (!(loc_m5 || loc_m3 == 1 || !*v2) || (*v2 == 0x21 || !v1)) --v3;
	loc_m7 = smart_strlen(v0);
	if (v3 + loc_m7 <= 99) {
		loc_m4 = loc_m1;
		loc_m10 = &v0[v3];
		loc_m2 = loc_m7;
		while (loc_m2 > loc_m4) {
			loc_m10[loc_m2] = v0[loc_m2];
			--loc_m2;
		}
		loc_m10[loc_m2] = v0[loc_m2];
		v2 = &v0[loc_m4];
		do {
			*v2 = input[0];
			++input;
			++v2;
		} while (*input);
		cursor_pos_byte = loc_m6 + loc_m4;
	}
	return;
}

// FUNCTION: GY454XE  Re 073BC
static char is_mathi_char(char token) {
	// Hex A - Hex F
	if (is_mathi() && token >= 0xb8 && token <= 0xbd) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 073DC
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
static char *f_07442(char direction) {
	char *cur_char;

	cur_char = &input_area[cursor_pos_byte];
	// Box
	if (cursor_pos_byte && cur_char[-1] == '!') cur_char = move_cursor(direction);
	return cur_char;
}

// FUNCTION: GY454XE  Re 07470
void f_07470(char token, char b) {
	char input[100];

	f_07022(token, b, input);
	return;
}

// FUNCTION: GY454XE  Re 07488
void f_07488(char keycode) {
	char *v0;
	char v1;
	char v2;
	char v3;
	char *v4;
	char v4_0;
	char *v5;
	char *v6;
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
		smart_strcpy(f_11030(), v0);
		d_08006 = v1;
	}
	switch (keycode) {
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
						// Sum and integral functions
						if (*loc_m10 == 0x69 || *loc_m10 == 0x6a) {
							if (keycode == K_UP) v2 = 2;
							else {
								v2 = 1;
								loc_m11 = 1;
							}
						}
						// Cursor (unused)
						else if (*loc_m10 == '|') {
							if (loc_m1) {
								if (keycode == K_UP) {
									v2 = 1;
									loc_m11 = 1;
								} else v2 = 2;
							} else continue;
						}
						// Fraction
						else if (*loc_m10 == 0xae) {
							if (keycode == K_UP) {
								v2 = 0;
								loc_m11 = 1;
							} else v2 = 1;
						} else continue;
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
		case K_LEFT:
			f_046AE();
			if (v1) {
				move_cursor(0);
				loc_m10 = f_07442(0);
				if (loc_m10 != v0) goto j_0769c;
				else goto j_076a6;

			} else if (!v3) goto j_076a6;
			else
j_076b6:
				cursor_pos_byte = smart_strlen(v0);
			break;
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
				if (!v3 || !v2) loc_m10 = move_cursor(0);
				v4 = &loc_m10[1];
				if (v3) {
					// Box
					if (*loc_m10 == '!') {
						if (loc_m10 == input_area) ++loc_m10;
						else return;
					}
					// Hex B (MathI control character)
					if (*loc_m10 == 0xb9) return;
					// Hex C (MathI control character)
					if (*loc_m10 == 0xba) return;
					if (f_05658(loc_m10) == 1) {
						v4_0 = f_05824(loc_m10) + 1;
						loc_m1 = f_056D0(loc_m10);
						// Logarithm function
						loc_m6[0] = *loc_m10 != 0x68 ? f_0579A(loc_m10, 0) : 0;
						// Derivative function
						loc_m6[1] = (loc_m1 != 3 && *loc_m10 != 0x6b) ? f_0579A(loc_m10, 1) : 0;
						loc_m6[2] = loc_m1 == 4 ? f_0579A(loc_m10, 2) : 0;
						// Exponentiation
						if (*v4 == '^') v4 = f_07442(0);
						loc_m8 = v4;
						loc_m1 = 0;
						do {
							loc_m11 = loc_m6[loc_m1];
							if (loc_m6[loc_m1]) {
								v5 = &loc_m10[loc_m11];
								loc_m11 = f_088FE(v5) ? 0 : f_05766(v5);
								v2 = 0;
								while (v2 < loc_m11) {
									*v4++ = *v6++;
									++v2;
								}
							}
						} while (++loc_m1 < 3);
						v6 = &loc_m10[v4_0];
						if (v4 == loc_m8 && f_08A66(v4-1, v6)) *v4++ = '!';
						loc_m10 = v4;
						v4 = v6;
					} else if (f_08A66(loc_m10-1, v4)) {
						*loc_m10 = '!';
						return;
					}
				}
				do *loc_m10++ = *v4++;
				while (loc_m10[-1]);
			}
	}
	return;
}

// FUNCTION: GY454XE  Re 077C4
char f_077C4(char *a) {
	return num_cmp(a, num_1);
}

// STUB: GY454XE  Re 077CC
char f_077CC(char *a, char *b, char c) {
	return 0;
}

#if REAL == 0
// FUNCTION: GY455XE  Im 082A2
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
static void f_0832A_E(char *a, char *b) {
	return;
}
#endif

// FUNCTION: GY454XE  Re 07B60
// STUB: GY455XE  Im 083B8
void f_07B60(char *result) {
	if (f_0A57A()) num_output_print(result);
	else {
		char v0;
		char v1;
		char v2;
		char v3;
		char v4;
		char v5;
		char *v6;
		char v7;
		char loc_m36[36];
		char loc_m56[20];
		char loc_m92[36];
		char loc_m128[36];
		char loc_m129;
		char loc_m130;
		char loc_m131;
		char *loc_m134;

		v0 = 0;
		loc_m129 = 0;
		v1 = d_080FF;
		v2 = f_02C76();
		v3 = is_matho();
		v4 = 0;
		v5 = 0;
		if (v3 && is_mathi()) v4 = 1;
		v5 = v4;
		if (v6 = f_11030() || v4) v6 = loc_m36;
		v7 = f_02AB2();
		v4 = 0;
		if (v1 && (1 << 4)) {
			if (v1 == 0x13 || !v5) f_02ADE(0xa);
			v7 = f_02AB2();
			v4 = 1;
		}
		if (v5) {
			if (v7 != 0xa) {
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
					if (loc_m130 == 1 || v1 != 1) {
						if (!f_082C6(loc_m56) || v1 != 1 || loc_m130 != 4) {
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
				if (loc_m130 == 1 || v1 == 1 || v4) {
					loc_m129 = 1;
					loc_m130 = num_to_str(loc_m56, loc_m134, loc_m131);
				} else {
					loc_m130 = 0xf0;
					loc_m92[0] = '\0';
				}
				if (v4) {
					f_10E7A(0, v6);
					f_08764(v6, loc_m92);
				}
				if (v1 == 1 && !v4) {
					f_02ABA(loc_m130);
					if (!v0) {
						if (mode == MODE_BASE_N) basen_base_print();
						smart_strcpy(loc_m92, v6);
					} else goto j_07e64;
				} else {
					if (loc_m129) {
						loc_m131 = 1;
						if (!v2) loc_m131 = v1 == 2 ? 2 : 5;
					} else if (v1 == 2) loc_m131 = 3;
					else loc_m131 = 4;
					v1 = num_to_str(&loc_m56[10], loc_m128, loc_m131);
					if (v2 && !v4) f_02B3E(loc_m128, 0x80);
					f_10E7A(1, loc_m92);
					if (!v4) {
						if (v2) {
							if (loc_m129) {
								if (loc_m128[0] == '`') loc_m128[0] = '-';
								else f_110A6(loc_m92);
							}
						} else f_088B8(loc_m92);
					}
					if (v7) {
						loc_m130 = f_10E34(loc_m130, v1);
						f_02ABA(loc_m130);
					}
					f_08764(loc_m92, loc_m128);
					if (v4) goto j_07e4e;
				}
				if (v5) {
					char tmp;
					v4 = smart_strlen(v6);
					tmp = f_087F6(v4, smart_strlen(loc_m92) * 6);
					loc_m131 = !v4 ? 10 : 20;
					if (f_08ABA(tmp)) f_10E5C(loc_m131);
				}
				f_02AEA(v6, 0);
				f_02AEA(loc_m92, 1);
			}
		} else goto j_07bfe;
		goto j_07e64;
j_07e4e:
		if (v4) f_110B2(v6);
		smart_strcat(v6, loc_m92);
j_07e64:
		d_08122 = 1;
		loc_m134 = input_area_ptr;
		input_area_ptr = v6;
		f_058DC();
		input_area_ptr = loc_m134;
		use_output_charset = 0;
	}
	return;
}
