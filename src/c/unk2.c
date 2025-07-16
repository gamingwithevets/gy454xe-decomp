#include "consts.h"
#include "generals.h"
#include "input.h"
#include "unk2.h"
#include "unk5.h"

// Static functions
static void num_to_str_norm(char *num, char *out, char n, char d);
static void num_to_str_fix(char *num, char *out, char n, char d);
static void num_to_str_sci(char *num, char *out, char n, char d);
static char f_08304(char *a, char *b);

// FUNCTION: GY454XE  Re 07F30
static void num_to_str_norm(char *num, char *out, char n, char d) {
	int v0;
	char v1;
	char num_tmp[10];
	num_struct nts;
	char exp_str[8];

	out[0] = '\0';
	if (!num) return;
	v0 = n == 2 ? -2 : -9;
	num_cpy(num_tmp, num);
	if (f_1AFD8(num_tmp, 11)) num_cpy(num_tmp, num);
	if (init_num_struct(num_tmp, &nts)) {
		// Force Sci9 for numbers with |exponent| > 9
		if (d && (nts.exponent > 9 || nts.exponent < -9) && nts.num[9]) num_to_str_sci(num, out, 9, 0);
		else {
			nts.unk_0x12 = 0;
			v1 = f_10CC2(&nts);
			nts.unk_0x15 = 1;
			if (nts.exponent < v0 || nts.exponent >= 10 || v1) {
				num_exp_to_str(&nts, exp_str);
				nts.unk_0x14 = 11;
				if (v1) f_10DA6(&nts);
				f_0285C(&nts, out);
				smart_strcat(out, exp_str);
			} else {
				nts.unk_0x12 = nts.exponent;
				nts.unk_0x14 = nts.exponent < 0 ? (char)(nts.exponent + 12) : 10;
				f_0285C(&nts, out);
			}
		}
	}
	return;
}

// STUB: GY454XE  Re 0802E
static void num_to_str_fix(char *num, char *out, char n, char d) {
	return;
}

// STUB: GY454XE  Re 081DE
static void num_to_str_sci(char *num, char *out, char n, char d) {
	return;
}

// FUNCTION: GY454XE  Re 082C6
char f_082C6(char *num) {
	char v0;

	v0 = f_02AB2();
	if (v0 == 1) return 1;
	// Degs Mins Secs format
	if (get_numtype(num) != 0x40) return 0;
	if (v0 < 2 || v0 > 12) {
		if (v0 == 13 || !f_02AAA()) return 1;
	}
	return 0;
}

// STUB: GY454XE  Re 08304
static char f_08304(char *a, char *b) {
	return 0;
}

// FUNCTION: GY454XE  Re 085C0
void f_085C0(void) {
	char v0;

	if (last_key_keycode == K_EXECUTE) v0 = 13;
	f_02ADE(v0);
}

// FUNCTION: GY454XE  Re 085D2
void f_085D2(void) {
	char v0;

	v0 = 0;
	if (f_0B588()) v0 = 13;
	f_02ADE(v0);
	return;
}

// FUNCTION: GY454XE  Re 085EC
char num_to_str(char *num, char *out, char c) {
	char v0;
	char v1;
	char v2;
	char v3;
	char loc_m10[10];

	v0 = d_080FF;
	// Send it to a different parser if it's an ERROR value
	if ((*num & 0xf0) == 0xf0) {
		f_10E14(num);
		return 0;
	}
	if (mode == MODE_BASE_N) {
		f_1444C(num, out);
		return 0;
	}
	v1 = f_02AB2();
	if (f_082C6(num)) {
		v1 = f_08304(num, out);
		if (v1 != 1) f_02ACA();
		else return v1;
	}
	num_cpy(loc_m10, num);
	f_1B208(loc_m10);
	if (v1 <= 10 && v1 > 1) goto j_08678;
	if (v0 & (1 << 4)) {
j_08678:
		f_14800(loc_m10);
		if (v1 >= 11) {
			char tmp = f_02AAA();
			if (tmp == 1) {
				char tmp2 = f_08304(loc_m10, out);
				if (tmp2 == 1) return tmp2;
				else f_02ACA();
			} else if (tmp > 1 && tmp < 10) f_02ACA();
		}
		if (v0 & (1 << 4)) {
			f_02986(num, out, 12);
			return 10;
		}
		if (c >= 4) c = 0;
		v2 = setup_num_fmt;
		if (setup_num_fmt == NUM_FMT_FIX) num_to_str_fix(loc_m10, out, setup_num_fmt_n, c);
		else if (setup_num_fmt == NUM_FMT_SCI) num_to_str_sci(loc_m10, out, setup_num_fmt_n, c);
		else if (v2 == NUM_FMT_NORM2) num_to_str_norm(loc_m10, out, 3, c);
		else num_to_str_norm(loc_m10, out, 2, c);
		v3 = f_02AB2();
		if (v3 && v3 <= 10) return v3;
		else return 10;
	} else {
		char tmp = f_077CC(out, loc_m10, c);
		if (tmp == 10) goto j_08678;
		return tmp;
	}
}

// FUNCTION: GY454XE  Re 08764
void f_08764(char *a, char *b) {
	if (d_080FF & (1 << 4) && !use_output_charset) {
		smart_strcat(a, s_blank_line);
		a[16 - smart_strlen(b)] = '\0';
	}
	smart_strcat(a, b);
	return;
}

// FUNCTION: GY454XE  Re 087A2
char f_087A2(void) {
	if (table_mode & (1 << 4)) return 0;
	if (!f_02CB6()) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 087BA
char is_mathi(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 7)) return 1;
	} else return 0;
}

// FUNCTION: GY454XE  Re 087D8
char is_matho(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 6)) return 1;
	} else return 0;
}

// FUNCTION: GY454XE  Re 087F6
int f_087F6(int a, int b) {
	if (a > b) return a;
	return b;
}

// FUNCTION: GY454XE  Re 08800
char f_08800(unsigned int a, unsigned int b, char c, char d, unsigned int e) {
	if (a != b) c = (b * c + (d - c) * e - a * d) / (b - a);
	return c;
}

// FUNCTION: GY454XE  Re 0885C
char f_0885C(char a, char b, char c, char d, char e) {
	if (d != c) a = (d * a + (b - a) * e - c * b) / (d - b);
	return a;
}

// FUNCTION: GY454XE  Re 088AA
int f_088AA(void) {
	return formula_x + d_08000;
}

// STUB: GY454XE  Re 088B8
void f_088B8(char *a) {
	return;
}

// STUB: GY454XE  Re 088D6
void f_088D6(void) {
	return;
}

// FUNCTION: GY454XE  Re 088EC
char is_pow_char(char *a) {
	char v0 = 0;

	if (a[0] == 0x5e) v0 = 1;
	return v0;
}

// FUNCTION: GY454XE  Re 088FE
char f_088FE(char *a) {
	// Box, Hex B, Hex C
	if (*a++ == '!' && (*a == 0xb9 || *a == 0xba)) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0891A
char is_ins_mode(void) {
	char v0 = 0;
	if (modifiers & (1 << 7)) v0 = 1;
	return v0;
}

// FUNCTION: GY454XE  Re 08926
void concat_num_str(char *out, char *num) {
	char string[20];

	num_to_str_norm(num, string, 2, 0);
	smart_strcat(out, string);
	return;
}

// STUB: GY454XE  Re 0895A
void concat_sqrt(char *out, char *num) {
	// STRING: GY454XE  Re 01FB2
	smart_strcat(out, "\x98\xb8");
	concat_num_str(out, num);
	concat_mathi_r(out);
	return;
}

// FUNCTION: GY454XE  Re 0897C
char f_0897C(void) {
	char s = 10;
	if (font_size == 7) s = 7;
	return s;
}

// FUNCTION: GY454XE  Re 0898C
char f_0898C(void) {
	char s = 5;
	if (font_size == 7) s = 3;
	return s;
}

// FUNCTION: GY454XE  Re 0899C
char get_font_width(void) {
	char w = 5;
	if (font_size == 7) w = 4;
	return w;
}

// STUB: GY454XE  Re 089AC
void f_089AC(array_bw_4 *a, array_bw_4 b) {
	return;
}

// FUNCTION: GY454XE  Re 08A0A
char f_08A0A(char a) {
	if (a == 9 || a == 3 || a == 5) return 1;
	else if (a < 11) return 0;
	else if (a <= 12) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 08A2A
char f_08A2A(char a) {
	if (a == 8) return 1;
	if (a != 10 && a != 2 && a != 5 && a < 12) return 0;
	else return 1;
}

// FUNCTION: GY454XE  Re 08A48
char f_08A48(char *a) {
	if (a < input_area_ptr) return 1;
	else {
		if (!f_08A2A(f_05658(a))) return 0;
		else return 1;
	}
}

// FUNCTION: GY454XE  Re 08A66
char f_08A66(char *a, char *b) {
	if (f_08A48(a) && *b == '^') {
		// Hex A
		if (*a == 0xb8 || *a == 0xba) {
			// Hex B, Hex C
			if (*b != 0xb9 && *b != 0xba) return 0;
		}
	}
	return 1;
}

// FUNCTION: GY454XE  Re 08A9C
void f_08A9C(char a) {
	d_08009 = a;
	font_size = 10;
	d_0800A = 0;
	d_08008 = 0;
	draw_mode = 0;
	d_08007 = 0;
}

// FUNCTION: GY454XE  Re 08ABA
char f_08ABA(char a) {
#asm
	extrn data near : _d_08111
	mov	r2,	r0
	l	r0,	NEAR _d_08111
	add	r0,	r2
	mov	r1,	#5ah
	cmp	r1,	r0
	subc	r0,	r0
	neg	r0
#endasm 
}

// FUNCTION: GY454XE  Re 08ACC
char f_08ACC(void) {
	char v1 = font_size;
	font_size = 7;
	return v1;
}

// FUNCTION: GY454XE  Re 08ADC
char f_08ADC(void) {
	int v0 = 0;
	if (mode == MODE_TABLE && table_mode == 1) v0 = 1;
	return (char)v0;
}

