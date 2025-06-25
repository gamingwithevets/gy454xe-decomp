#include "consts.h"
#include "generals.h"
#include "unk1.h"
#include "unk2.h"
#include "unk5.h"

// Static declarations
// 04E6E
static void f_04E6E(char x, char y);
// 04E86
static char f_04E86(char a);
// 04E9C
static char f_04E9C(char a);
// 05162
static void f_05162(int a, int b, char c);
// 05260
static void f_05260(char *a, int b);
// 05366
static void f_05366(f_058DC_union *a, int b, char c);
// 05516
static void f_05516(int a, char b, char c, f_058DC_union d);
// 055BC
static void f_055BC(int a, char b, char *c);
// 05652
static char f_05652(char *a);
// 0568E
static void f_0568E(char *a);
// 05B2C
static char *f_05B2C(char *a, f_058DC_union *b, int c, char d);
// 05B86
static char *f_05B86(char *a, f_058DC_union *b, int c, char d);
// 05D0C
static char *f_05D0C(char *a, f_058DC_union *b, int c, char d);
// 05EBC
static char *f_05EBC(char *a, f_058DC_union *b, int c, char d);
// 06002
static char *f_06002(char *a, f_058DC_union *b, int c, char d);
// 062CE
static char *f_062CE(char *a, f_058DC_union *b, int c, char d);
// 06456
static char *f_06456(char *a, f_058DC_union *b, int c, char d);
// 0666C
static char *f_0666C(char *a, f_058DC_union *b, int c, char d);
// 06914
static char f_06914(char *a);
// 06944
static char *f_06944(char *a, f_058DC_union *b, int c, char d);

// 01DDC
const char unk_01ddc[] = {
	0, 0, 1, 3, 3, 1, 0, 4, 0, 0, 2, 2
};

// 01DF4
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

// 01E24
const char unk_01e24[] = {
	3, 5, 8, 7, 12
};

// 04E6E
static void f_04E6E(char x, char y) {
	cursor_x = x;
	cursor_y = y;
	f_10FD2();
	d_08117 = font_size;
}

// 04E86
static char f_04E86(char a) {
	if (d_080FE & (1 << 6) && a == 0x3a) return 1;
	else if (a) return 0;
	else return 1;
}

// 04E9C
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
	v2 = *input_area_ptr;
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

// 04F6E
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
	v0 = *input_area_ptr;
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
		if (cursor_pos_byte == v5) f_04E6E(v2, v1);
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

// 05162 - STUB
static void f_05162(int a, int b, char c) {
	int v0;

	if (reg0[9] && c >= reg0[5]) {
		c -= reg0[5];
		if (c <= 31) {
			v0 = f_088AA();
			if (b >= v0) {
				if (a < v0) a = formula_x;
				else {
					a -= *((int *)reg0);
					if (a > 95) return;
				}
				b -= *((int *)reg0);
				if (b > 95) b = 95;
				f_02DD8((char)a, c, (char)b, c);
			}
		}
	}

	return;
}

// 051CE - STUB
void f_051CE(int a, char b, char c) {
	return;
}

// 05222 - STUB
void f_05222(void) {
	return;
}

// 05260
static void f_05260(char *a, int b, char c) {
	int v0;

	if (reg0[9]) {
		c -= f_0899C();
		do {
			if (b < f_088AA()) {
				if (*++a) b += 6;
				else break;
			} else {
				b -= *((int *)reg0);
				if (b <= 95) {
					v0 = c - reg0[5];
					if (v0 >= -10 && v0 <= 31) line_print((char)b, (char)v0, a);
				}
				break;
			}
		} while (1);
	}

	return;
}

// 052D0 - STUB
char f_052D0(char a, int b, char c) {
	return 0;
}

// 05366 - STUB
static void f_05366(f_058DC_union *a, int b, char c) {
	return;
}

// 05516 - STUB
static void f_05516(int a, char b, char c, f_058DC_union d) {
	return;
}

// 055BC - STUB
static void f_055BC(int a, char b, char *c) {
	return;
}

// 05652
char f_05652(char *a) {
	return f_02676(a[0]);
}

// 05658
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

// 0568E - STUB
static void f_0568E(char *a) {
	return;
}

// 056AE
char f_056AE(char *a) {
	if (f_05652(a) != 1) return 5;
	return unk_01ddc[f_0270E(a[0])];
}

// 056D0 - STUB
void f_056D0(void) {
	return;
}

// 056F2 - STUB
void f_056F2(char *a, char *b) {
	return;
}

// 05766 - STUB
void f_05766(void) {
	return;
}

// 0579A - STUB
void f_0579A(void) {
	return;
}

// 05824 - STUB
void f_05824(void) {
	return;
}

// 0587E - STUB
void f_0587E(void) {
	return;
}

// 058DC
void f_058DC(void) {
	char v0;
	char v1;
	char v2;
	char *v3;
	int v4;
	char v5;
	f_058DC_union loc_m4;

	v0 = 0;
	v1 = 0;
	v2 = d_08122;
	if (d_080FE != 1 && v2) v1 = 1;
	v3 = *input_area_ptr;
	v4 = 0;
	*((int *)reg0) = 0;
	reg0[5] = 0;
	f_08A9C(0);
	if (!f_06944(v3, &loc_m4, 0, 62) || loc_m4.byte[2] > 62) {
		smart_strcpy(v3, f_11030());
		cursor_pos_byte = reg0[6];
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
		do {
			*((int *)reg0) += 8;
		} while (*((int *)(reg0+2)) - *((int *)reg0) <= v1 && loc_m4.word[0] - *((int *)reg0) <= v1);
		v1 = loc_m4.byte[2] - loc_m4.byte[3] + 1;
		if (loc_m4.byte[2] > 31) {
			if (v2 && loc_m4.byte[3] + 62 - reg0[4] > 12) {
				v5 = 93	- v1 - 4;
				do reg0[5] += 8; 
				while (reg0[4] - reg0[5] > v5);
			} else {
				reg0[5] = loc_m4.byte[2] - 31;
				d_08124 = 31;
			}
		}
		buffer_clear();
	} else {
		if (f_08ABA(v5)) f_10E5C(loc_m4.byte[2]);
		if (loc_m4.word[0] <= 96) v4 = (char)(96 - loc_m4.byte[0]);
		else {
			*((int *)reg0) = cursor_pos_byte << 3;
			if (loc_m4.word[0] - *((int *)reg0) <= 88) {
				*((int *)reg0) = --cursor_pos_byte << 3;
				f_046C4();
			}
			d_0812C = 1;
			buffer_clear_lastnline(32 - loc_m4.byte[2]);
		}
		v1 = 32 - loc_m4.byte[3];
	}
	f_08A9C(1);
	f_06944(v3, &loc_m4, v4, v1);
	v4 = *((int *)reg0);
	v5 = reg0[5];
	v1 -= v5 + 5;
	if (v0) table_prompt_print(v1);
	if (v4 > 0) f_10F52(v1);
	if (loc_m4.word[0] - v4 > 96 - formula_x) f_10F76(v1);
	formula_x = 0;
	if (d_080FE == 1) {
		font_size = 6;
		if (v5 > 0) f_10F92();
		if (loc_m4.byte[2] > v5 + 31) f_10FA2();
	}

	return;
}

// 05B2C
static char *f_05B2C(char *a, f_058DC_union *b, int c, char d) {
	char v0;
	char *v1;

	v0 = *a == 0x98 ? 5 : 6;
	if (v1 = f_06944(a+2, b, v0+c, d)) f_05366(b, c, d);
	return v1;
}

// 05B86 - STUB
static char *f_05B86(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 05D0C - STUB
static char *f_05D0C(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 05EBC
static char *f_05EBC(char *a, f_058DC_union *b, int c, char d) {
	char v0;
	int v1;
	char *v2;
	f_058DC_union loc_m4;

	v0 = f_08ACC();
	// cstring @ 01E2D
	f_05260("d", c+4, d-4);
	// cstring @ 01E2F
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

// 05EBC - STUB
static char *f_06002(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 05EBC - STUB
static char *f_062CE(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 06456 - STUB
static char *f_06456(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 0666C - STUB
static char *f_0666C(char *a, f_058DC_union *b, int c, char d) {
	return 0;
}

// 06914
static char f_06914(char *a) {
	if (a > *input_area_ptr) {
		if (a[-1] == 0xb9 || a[-1] == 0xbc) return 1;
		else if (a[-1] == 0xb8 || a[-1] == 0xba) return 0;
		else if (f_14532(a[-1])) return 1;
		else return 0;
	} else return 0;
}

// 06944
static char *f_06944(char *a, f_058DC_union *b, int c, char d) {
	int v0;
	f_058DC_union loc_m4;
	char loc_m5;

	v0 = c;
	if (++reg0[7] > 27 || get_remaining_stack_space() < 200)
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
				if (!reg0[9]) d_0800A = 1;
			}
			// 01DB2: jump table
			switch (f_05658(a)) {
				// 069F4
				case 1:
					++reg0[7];
					if (a = jmp_01df4[f_0270E(*a)](a, &loc_m4, c, d)) {
						--reg0[7];
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
				// 06A58
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
				// 06A92
				case 10:
					loc_m5 = f_052D0(*a, c, d);
					c += loc_m5;
					b->word[0] += loc_m5;
				// 06AB4
				case 8:
					if (a = f_06944(a+1, &loc_m4, c+6, d)) {
						f_05516(c, d, 0, loc_m4);
						b->word[0] += loc_m4.word[0] + 6;
						f_089AC(b, loc_m4);
						if (*a == 41) goto j_069e6;
					}
					break;
				// 06B0E
				case 3:
				case 5:
				case 11:
					break;
				// 06B12
				case 0:
				case 12:
				case 13:
					if (!f_04E86(*a)) {
						loc_m4.byte[2] = f_0897C();
						loc_m4.byte[3] = f_0898C();
						b->word[0] += f_052D0(*a, c, d);
						goto j_069e6;
					}
					break;
				// 06B4E
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
	--reg0[7];
	return a;
}

// 069F4 - STUB
void f_069F4(void) {
	return;
}

// 06A58 - STUB
void f_06A58(void) {
	return;
}

// 06A92 - STUB
void f_06A92(void) {
	return;
}

// 06AB4 - STUB
void f_06AB4(void) {
	return;
}

// 06B0E - STUB
void f_06B0E(void) {
	return;
}

// 06B12 - STUB
void f_06B12(void) {
	return;
}

// 06B4E - STUB
void f_06B4E(void) {
	return;
}

// 06B52 - STUB
void f_06B52(void) {
	return;
}

// 06C06 - STUB
void f_06C06(void) {
	return;
}

// 06C54 - STUB
void f_06C54(void) {
	return;
}

// 06D0A - STUB
void f_06D0A(void) {
	return;
}

// 06D20 - STUB
void f_06D20(void) {
	return;
}

// 06D42 - STUB
void f_06D42(void) {
	return;
}

// 06D68 - STUB
void f_06D68(void) {
	return;
}

// 06D72 - STUB
void f_06D72(void) {
	return;
}

// 06D7C - STUB
void f_06D7C(void) {
	return;
}

// 06D90 - STUB
void f_06D90(void) {
	return;
}

// 06E40 - STUB
void f_06E40(char *a, char b, char c) {
	return;
}

// 06F1C - STUB
char *f_06F1C(char *a) {
	return;
}

// 06FCE - STUB
void f_06FCE(char *a, char b, char c) {
	return;
}

// 07022
void f_07022(char token, char b, char *input) {
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
	loc_m5 = f_087BA();
	loc_m3 = is_ins_mode();
	input[0] = token;
	input[1] = 0;
	loc_m6 = 1;
	v1 = 0;
	v2 = &v0[loc_m4];
	if (loc_m5) {
		smart_strcpy(f_11030(), v0);
		reg0[6] = loc_m4;
		loc_m7 = is_pow_char(v2);
		if (b == 0) {
			if (loc_m7 || (*v2 == 0x21 && is_pow_char(&v2[1]))) {
				// cstring @ 01E35
				if (f_08A2A(f_05658(input))) smart_strcat(input, "!");
			}
		} else {
			// Check for x^n, nth root, char 0x7c (unused)
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
				// cstring @ 01E37
				smart_strcpy(input, "!^");
				loc_m6 = 0;
				v3 = 1;
			}
			loc_m11 = unk_01e24[loc_m2];
			if (unk_01e24[loc_m2] + v1 + loc_m7 + v3 < 100) {
				if (!loc_m2) {
					concat_mathi_l(input);
					// cstring @ 01E3A
					if (token == 0x75) smart_strcat(input, "2");
					// cstring @ 01E3C
					else if (token == 0x76) smart_strcat(input, "3");
					// cstring @ 01E3E
					else if (token == 0x77) smart_strcat(input, "`1");
					else f_06FCE(input, loc_m4, v1);
					concat_mathi_r(input);
				} else if (loc_m2 == 1) {
					concat_mathi_l(input);
					if (token == 0x68) loc_m2 = 0;
					else loc_m2 = v1;
					f_06FCE(input, loc_m4, loc_m2);
					// cstring @ 01E41
					smart_strcat(input, "\xba");
					if (token == 0x68) loc_m2 = v1;
					else loc_m2 = 0;
					f_06FCE(input, loc_m4, loc_m2);
					concat_mathi_r(input);
				} else if (loc_m2 == 2) {
					loc_m6 = 3;
					if (token == 0xa8) {
j_07200:
						// cstring @ 01E43
						smart_strcat(input, "\xbb\xb8");
						if (token == 0xa8) {
							// cstring @ 01E3C
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
					// cstring @ 01E46
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
						// cstring @ 01E4C
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

// 073BC - STUB
void f_073BC(void) {
	return;
}

// 073DC - STUB
void f_073DC(void) {
	return;
}

// 07442 - STUB
void f_07442(void) {
	return;
}

// 07470
void f_07470(char token, char b) {
	char input[100];

	f_07022(token, b, input);
	return;
}

// 07488 - STUB
void f_07488(void) {
	return;
}

// 077C4 - STUB
void f_077C4(void) {
	return;
}

// 077CC - STUB
void f_077CC(void) {
	return;
}

// 07B60 - STUB
void f_07B60(char **a) {
	return;
}

