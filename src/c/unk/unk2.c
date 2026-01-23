#include "../consts.h"
#include "../generals.h"
#include "../io/input.h"
#include "unk2.h"
#include "unk4.h"
#include "unk5.h"
#include "unk6.h"

// Static functions
static void num_to_str_norm(char *num, char *out, char n, char d);
static void num_to_str_fix(char *num, char *out, char n, char d);
static void num_to_str_sci(char *num, char *out, char n, char d);
static char num_to_str_dms(char *a, char *b);

// FUNCTION: GY454XE  Re 07F30
// FUNCTION: GY455XE  Im 08854
// FUNCTION: GY460XF  Im 08158
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

// FUNCTION: GY454XE  Re 0802E
// FUNCTION: GY455XE  Im 08952
// FUNCTION: GY460XF  Im 08256
static void num_to_str_fix(char *num, char *out, char n, char d) {
	char num_tmp[10];
	num_struct nts;
	char exp_str[8];

	out[0] = '\0';
	if (!num) return;
	num_cpy(num_tmp, num);
	if (num_invalid__(num) == 1) {
		memzero(&nts, 24);
		nts.unk_0x14 = n + 1;
		nts.unk_0x15 = 2;
		f_0285C(&nts, out);
		return;
	} else {
		char v0;
		int v1;
		char v2;
		int v3;

		v0 = num[9] & 0xf;
		v2 = num[8] & 0xf;
		v3 = (char)((*(int *)(num+8) >> 4) & 0xf) * 10 + v2;
		if (v0 == 0 || v0 == 5) v3 -= 99;
		else ++v3;
		v1 = n + v3 + 1;
		v2 = v1;
		if (v1 > 10) v2 = 11;
		else if (v1 < 0) v2 = 0;
		f_1AFD8(num_tmp, v2);
		if (num_tmp[0] & 0xf0) num_cpy(num_tmp, num);
		if (init_num_struct(num_tmp, &nts)) {
			// Force Sci9 for numbers with exponent > 9 and 8 decimal digits or more
			if (d && nts.exponent > 9 && nts.num[9]) num_to_str_sci(num, out, 9, 0);
			else {
				nts.unk_0x12 = 0;
				v0 = f_10CC2(&nts);
				if (nts.exponent >= 10 || v0) {
					num_exp_to_str(&nts, exp_str);
					nts.unk_0x14 = 10;
					nts.unk_0x15 = 1;
					if (v0) f_10DCA(&nts);
					f_0285C(&nts, out);
					smart_strcat(out, exp_str);
				} else {
					int tmp;
					char tmp2;
					tmp = nts.exponent;
					if (nts.exponent < 0) {
						if (n + nts.exponent < 0) {
							nts.negative = 0;
							tmp = 0 - (n + 1);
						} else tmp = nts.exponent;
					}
					nts.unk_0x12 = tmp;
					if ((char)(n + 1) < 0 - nts.exponent) tmp2 = 0;
					else tmp2 = nts.exponent + n + 1;
					nts.unk_0x14 = tmp2;
					if (tmp2 >= 10) nts.unk_0x14 = 10;
					nts.unk_0x15 = 2;
					f_0285C(&nts, out);
				}
			}
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 081DE
// FUNCTION: GY455XE  Im 08B02
// FUNCTION: GY460XF  Im 08406
static void num_to_str_sci(char *num, char *out, char n, char d) {
	char v0;
	char num_tmp[10];
	num_struct nts;
	char exp_str[8];

	out[0] = '\0';
	if (!num) return;
	num_cpy(num_tmp, num);
	if (!n) n = 10;
	if (f_1AFD8(num_tmp, n+1)) num_cpy(num_tmp, num);
	if (init_num_struct(num_tmp, &nts)) {
		if (d && n == 10 && (nts.exponent > 9 || nts.exponent < -9)) {
			n = 9;
			if (f_1AFD8(num_tmp, 10)) num_cpy(num_tmp, num);
			if (!init_num_struct(num_tmp, &nts)) return;
		}
		nts.unk_0x12 = 0;
		v0 = f_10CC2(&nts);
		num_exp_to_str(&nts, exp_str);
		nts.unk_0x14 = n;
		nts.unk_0x15 = 0;
		if (v0) f_10DCA(&nts);
		f_0285C(&nts, out);
		smart_strcat(out, exp_str);
	}
	return;
}

// FUNCTION: GY454XE  Re 082C6
// FUNCTION: GY455XE  Im 08BEA
// FUNCTION: GY460XF  Im 084EE
char is_dms_num(char *num) {
	char v0;
	char v1;

	v0 = get_result_store_fmt();
	if (v0 == RESULT_DMS) {
j_082d8:
		v1 = 1;
		goto j_082e8;
	}
	// Degs Mins Secs format
	if (get_numtype(num) != 0x40) {
j_082e6:
		v1 = 0;
j_082e8:
		return v1;
	}
	if ((v0 < RESULT_ENGM4 || v0 > RESULT_FRAC_MIX) && (v0 != RESULT_STANDARD || !get_result_disp_fmt())) goto j_082d8;
	goto j_082e6;
}

// FUNCTION: GY454XE  Re 08304
// FUNCTION: GY455XE  Im 08C28
// FUNCTION: GY460XF  Im 08530
static char num_to_str_dms(char *num, char *out) {
	char v0;
	int v1;
	int v2;
	char num_sec[10];
	char num_60[10];
	char num_deg[10];
	char num_min[10];
	num_struct nts;
	char outstr[20];

	v0 = RESULT_DECIMAL;
	num_cpy(num_sec, num);
	num_abs(num_sec);
	num_to_str_std_lineo(num_sec);
	v1 = num_get_exp(num_sec);
	if (v1 < 7) {
		num_frombyte(num_60, 60);
		out[0] = '\0';
		num_cpy(num_deg, num_sec);
		num_trunc__(num_deg);
		f_1B378(num_sec);
		v1 -= num_get_exp(num_sec);
		f_1A438(num_sec, num_60);
		f_1B238(num_sec);
		v2 = num_get_exp(num_sec);
		num_cpy(num_min, num_sec);
		num_trunc__(num_min);
		f_1B378(num_sec);
		v2 -= num_get_exp(num_sec);
		f_1A438(num_sec, num_60);
		f_1B238(num_sec);
		f_1AFD8(num_sec, 14 - (v1 + v2));
		num_mulxp__(num_sec, 2);
		f_1AFB8(num_sec);
		num_mulxp__(num_sec, -2);
		if (num_cmp(num_sec, num_60) != 2) {
			f_1A424(num_sec, num_60);
			f_1A460(num_min, num_1);
			if (num_cmp(num_min, num_60) != 2) {
				f_1A424(num_min, num_60);
				f_1A460(num_deg, num_1);
			}
			if (num_get_exp(num_deg) >= 7) goto j_085b2;
		}
		if (num_invalid__(num) == 2 && (num_invalid__(num_deg) != 1 || num_invalid__(num_min) != 1 || num_invalid__(num_sec) != 1)) concat_negative(out);;
		if (init_num_struct(num_deg, &nts)) {
			char tmp = 0;
			if (nts.exponent == 4) tmp = 1;
			else if (nts.exponent <= 3) tmp = 2;
			nts.unk_0x12 = nts.exponent;
			nts.unk_0x14 = 7;
			nts.unk_0x15 = 1;
			f_0285C(&nts, outstr);
			smart_strcat(out, outstr);
			// STRING: GY454XE  Re 01FAC
			// STRING: GY455XE  Im 01FAE
			smart_strcat(out, "\x85");  // °
			if (init_num_struct(num_min, &nts)) {
				nts.unk_0x12 = nts.exponent;
				nts.unk_0x14 = nts.exponent + 1;
				nts.unk_0x15 = 1;
				f_0285C(&nts, outstr);
				smart_strcat(out, outstr);
				// STRING: GY454XE  Re 01FAE
				// STRING: GY455XE  Im 01FB0
				smart_strcat(out, "'");
				if (init_num_struct(num_sec, &nts)) {
					char tmp2;
					nts.unk_0x12 = tmp + nts.exponent + 1;
					if (nts.unk_0x12 <= 0) tmp2 = 1;
					else tmp2 = nts.unk_0x12;
					nts.unk_0x14 = tmp2;
					nts.unk_0x12 = nts.exponent;
					nts.unk_0x15 = 1;
					f_0285C(&nts, outstr);
					smart_strcat(out, outstr);
					// STRING: GY454XE  Re 01FB0
					// STRING: GY455XE  Im 01FB2
					smart_strcat(out, "\"");
					v0 = RESULT_DMS;
				}
			}
		}
	}
j_085b2:
	return v0;
}

// FUNCTION: GY454XE  Re 085C0
// FUNCTION: GY455XE  Im 08EE4
// FUNCTION: GY460XF  Im 087EC
void set_default_result_fmt(void) {
	char v0 = 0;

	if (last_key_keycode == K_EXECUTE) v0 = RESULT_STANDARD;
	set_result_fmt(v0);
}

// FUNCTION: GY454XE  Re 085D2
// FUNCTION: GY455XE  Im 08EF6
// FUNCTION: GY460XF  Im 087FE
void f_085D2(void) {
	char v0;

	v0 = 0;
	if (f_0B588()) v0 = RESULT_STANDARD;
	set_result_fmt(v0);
	return;
}

// FUNCTION: GY454XE  Re 085EC
// FUNCTION: GY455XE  Im 08F10
// FUNCTION: GY460XF  Im 08818
char num_to_str(char *num, char *out, char c) {
	char v0;
	char v1;
	char v2;
	char v3;
	char loc_m10[10];

	v0 = result_template;
	// ERROR value
	if ((*num & 0xf0) == 0xf0) {
		num_to_str_error(num);
		return 0;
	}
	if (mode == MODE_BASE_N) {
		num_to_str_base_n(num, out);
		return 0;
	}
#if ENABLE_VERIF == 1
	if (mode == MODE_VERIF) {
		num_to_str_verif(num, out);
		return 0;
	}
#endif
	v1 = get_result_store_fmt();
	if (is_dms_num(num)) {
		v1 = num_to_str_dms(num, out);
		if (v1 != RESULT_DMS) set_result_store_fmt(v1);
		else return v1;
	}
	num_cpy(loc_m10, num);
	f_1B208(loc_m10);
	if ((v1 <= RESULT_DECIMAL && v1 > RESULT_DMS) || v0 & (1 << 4)) {
j_08678:
		num_to_str_std_lineo(loc_m10);
		if (v1 >= RESULT_FRAC) {
			char tmp = get_result_disp_fmt();
			if (tmp == RESULT_DMS) {
				char tmp2 = num_to_str_dms(loc_m10, out);
				if (tmp2 == RESULT_DMS) return tmp2;
				else set_result_store_fmt(tmp2);
			// Check ENG notation format
			} else if (tmp > RESULT_DMS && tmp < RESULT_DECIMAL) set_result_store_fmt(tmp);
		}
		if (v0 & (1 << 4)) {
			f_02986(num, out, 12);
			return RESULT_DECIMAL;
		}
		if (c >= 4) c = 0;
		v2 = setup_num_fmt;
		if (setup_num_fmt == NUM_FMT_FIX) num_to_str_fix(loc_m10, out, setup_num_fmt_n, c);
		else if (setup_num_fmt == NUM_FMT_SCI) num_to_str_sci(loc_m10, out, setup_num_fmt_n, c);
		else if (v2 == NUM_FMT_NORM2) num_to_str_norm(loc_m10, out, 3, c);
		else num_to_str_norm(loc_m10, out, 2, c);
		v3 = get_result_store_fmt();
		if (v3 && v3 <= RESULT_DECIMAL) return v3;
		else return RESULT_DECIMAL;
	} else {
		char tmp = num_to_str_std(out, loc_m10, c);
		if (tmp == RESULT_DECIMAL) goto j_08678;
		return tmp;
	}
}

// FUNCTION: GY454XE  Re 08764
// FUNCTION: GY455XE  Im 09088
// FUNCTION: GY460XF  Im 08998
void concat_result(char *out, char *res) {
	if (
		(result_template & (1 << 4) && !use_output_charset)
#if ENABLE_INEQ == 1
		|| is_ineq_result()
#endif
		) {
		smart_strcat(out, s_blank_line);
		out[(char)(16 - smart_strlen(res))] = '\0';
	}
	smart_strcat(out, res);
	return;
}

// FUNCTION: GY454XE  Re 087A2
// FUNCTION: GY455XE  Im 090C6
// FUNCTION: GY460XF  Im 089DE
char f_087A2(void) {
	if (table_mode & (1 << 4)) return 0;
	if (!f_02CB6()) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 087BA
// FUNCTION: GY455XE  Im 090DE
// FUNCTION: GY460XF  Im 089F6
char is_mathi(void) {
	if (f_087A2()) {
		if (!setup_mathi)
j_087ca:
			return 0;
		if (mode & (1 << 7)) return 1;
	}
	goto j_087ca;
}

// FUNCTION: GY454XE  Re 087D8
// FUNCTION: GY455XE  Im 090FC
// FUNCTION: GY460XF  Im 08A14
char is_matho(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 6)) return 1;
	} else return 0;
}

// FUNCTION: GY454XE  Re 087F6
// FUNCTION: GY455XE  Im 0911A
// FUNCTION: GY460XF  Im 08A3C
int max(int a, int b) {
	if (a > b) return a;
	return b;
}

// FUNCTION: GY454XE  Re 08800
// FUNCTION: GY455XE  Im 09124
// FUNCTION: GY460XF  Im 08A46
char lerp16(unsigned int x0, unsigned int x1, char y0, char y1, unsigned int x) {
	if (x0 != x1) y0 = (x1 * y0 + (y1 - y0) * x - x0 * y1) / (x1 - x0);
	return y0;
}

// FUNCTION: GY454XE  Re 0885C
// FUNCTION: GY455XE  Im 09180
// FUNCTION: GY460XF  Im 08AA2
char lerp8(char y0, char y1, char x0, char x1, char x) {
	if (x1 != x0) y0 = (x1 * y0 + (y1 - y0) * x - x0 * y1) / (x1 - y1);
	return y0;
}

// FUNCTION: GY454XE  Re 088AA
// FUNCTION: GY455XE  Im 091CE
// FUNCTION: GY460XF  Im 08AF0
int f_088AA(void) {
	return formula_x + d_08000;
}

// FUNCTION: GY454XE  Re 088B8
// FUNCTION: GY455XE  Im 091DC
// FUNCTION: GY460XF  Im 08AFE
char *concat_angle(char *input) {
	input[0] = 0xaf;  // Angle token
	if (use_output_charset) input[0] = 0x88;  // Angle character
	input[1] = '\0';
	return input;
}

// FUNCTION: GY454XE  Re 088D6
// FUNCTION: GY455XE  Im 091FA
// FUNCTION: GY460XF  Im 08B1C
char is_sign_char(char tok) {
	if (tok == '`' || tok == '-' || tok == '+') return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 088EC
// FUNCTION: GY455XE  Im 09210
// FUNCTION: GY460XF  Im 08B32
char is_pow_char(char *a) {
	char v0 = 0;

	if (a[0] == 0x5e) v0 = 1;
	return v0;
}

// FUNCTION: GY454XE  Re 088FE
// FUNCTION: GY455XE  Im 09222
// FUNCTION: GY460XF  Im 08B44
char f_088FE(char *a) {
	// Box, Hex B, Hex C
	if (*a++ != '!')
j_08908:
		return 0;
	if (*a == 0xb9 || *a == 0xba) return 1;
	goto j_08908;
}

// FUNCTION: GY454XE  Re 0891A
// FUNCTION: GY455XE  Im 0923E
// FUNCTION: GY460XF  Im 08B60
char is_ins_mode(void) {
	char v0 = 0;
	if (modifiers & (1 << 7)) v0 = 1;
	return v0;
}

// FUNCTION: GY454XE  Re 08926
// FUNCTION: GY455XE  Im 0924A
// FUNCTION: GY460XF  Im 08B6C
void concat_num_str(char *out, char *num) {
	char string[20];

	num_to_str_norm(num, string, 2, 0);
	smart_strcat(out, string);
	return;
}

// FUNCTION: GY454XE  Re 0895A
// FUNCTION: GY455XE  Im 0927E
// FUNCTION: GY460XF  Im 08BA0
void concat_sqrt(char *out, char *num) {
	// STRING: GY454XE  Re 01FB2
	// STRING: GY455XE  Im 01FB4
	// STRING: GY460XF  Im 01D0E
	smart_strcat(out, "\x98\xb8");
	concat_num_str(out, num);
	concat_mathi_r(out);
	return;
}

// FUNCTION: GY454XE  Re 0897C
// FUNCTION: GY455XE  Im 092A0
// FUNCTION: GY460XF  Im 08BC2
char f_0897C(void) {
	char s = 10;
	if (font_size == 7) s = 7;
	return s;
}

// FUNCTION: GY454XE  Re 0898C
// FUNCTION: GY455XE  Im 092B0
// FUNCTION: GY460XF  Im 08BD2
char f_0898C(void) {
	char s = 5;
	if (font_size == 7) s = 3;
	return s;
}

// FUNCTION: GY454XE  Re 0899C
// FUNCTION: GY455XE  Im 092C0
// FUNCTION: GY460XF  Im 08BE2
char get_font_width(void) {
	char w = 5;
	if (font_size == 7) w = 4;
	return w;
}

// FUNCTION: GY454XE  Re 089AC
// FUNCTION: GY455XE  Im 092D0
// FUNCTION: GY460XF  Im 08BF2
void f_089AC(mathi_bbox *a, mathi_bbox b) {
	char v0;
	char v1;

	v0 = a->byte[2] >= a->byte[3] ? a->byte[2] - a->byte[3] : 0;
	v1 = b.byte[2] >= b.byte[3] ? b.byte[2] - b.byte[3] : 0;
	v0 = max(v0, v1);
	a->byte[3] = max(a->byte[3], b.byte[3]);
	a->byte[2] = a->byte[3] + v0;
	return;
}

// FUNCTION: GY454XE  Re 08A0A
// FUNCTION: GY455XE  Im 0932E
// FUNCTION: GY460XF  Im 08C50
char f_08A0A(char a) {
	if (a == 9 || a == 3 || a == 5) return 1;
	else if (a < 11) return 0;
	else if (a <= 12) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 08A2A
// FUNCTION: GY455XE  Im 0934E
// FUNCTION: GY460XF  Im 08C70
char f_08A2A(char a) {
	if (a == 8) return 1;
	if (a != 10 && a != 2 && a != 5 && a < 12) return 0;
	else return 1;
}

// FUNCTION: GY454XE  Re 08A48
// FUNCTION: GY455XE  Im 0936C
// FUNCTION: GY460XF  Im 08C8E
char f_08A48(char *input) {
	if (input < input_area_ptr)
j_08a52:
		return 1;
	if (!f_08A2A(f_05658(input))) return 0;
	goto j_08a52;
}

// FUNCTION: GY454XE  Re 08A66
// FUNCTION: GY455XE  Im 0938A
// FUNCTION: GY460XF  Im 08CAC
char f_08A66(char *input, char *b) {
	if (f_08A48(input) && *b == '^')
j_08a7c:
		return 1;
	// Hex A
	if (*input == 0xb8 || *input == 0xba) {
		// Hex B, Hex C
		if (*b == 0xb9 || *b == 0xba) goto j_08a7c;
	}
	return 0;
}

// FUNCTION: GY454XE  Re 08A9C
// FUNCTION: GY455XE  Im 093C0
// FUNCTION: GY460XF  Im 08CE2
void mathi_set_draw_mode(char dmode) {
	mathi_enable_draw = dmode;
	font_size = 10;
	d_0800A = 0;
	d_08008 = 0;
	draw_mode = 0;
	d_08007 = 0;
}

// FUNCTION: GY454XE  Re 08ABA
// FUNCTION: GY455XE  Im 093DE
// FUNCTION: GY460XF  Im 08D00
char f_08ABA(char a) {
	return -((char)(0x5a-(d_08111 + a)) >> 7);
/*
	Original assembly below. Currently the conditions required to generate a CMP-SUBC sequence is unknown.
#asm
	extrn data near : _d_08111
	mov	r2,	r0
	l	r0,	NEAR _d_08111
	add	r0,	r2
	mov	r1,	#5ah
	cmp	r1,	r0
	subc	r0,	r0
	neg	r0
	rt
#endasm
*/
}

// FUNCTION: GY454XE  Re 08ACC
// FUNCTION: GY455XE  Im 093F0
// FUNCTION: GY460XF  Im 08D12
char begin_small_font(void) {
	char size = font_size;
	font_size = 7;
	return size;
}

// FUNCTION: GY454XE  Re 08ADC
// FUNCTION: GY455XE  Im 09400
// FUNCTION: GY460XF  Im 08D22
// FUNCTION: GY465XG  Im 08AE0
char is_table_func_input(void) {
	int v0 = 0;
	if (mode == MODE_TABLE && table_mode == TABLE_NONE) v0 = 1;
	return v0;
}

#if ENABLE_RDEC == 1
// FUNCTION: GY460XF  Im 08D3A
// FUNCTION: GY465XG  Im 08AF8
char f_08D3A_460F(void) {
	if (mode != MODE_COMP)
j_08d44:
		return 0;
	else if (!f_02CB6() && !(result_template & (1 << 4))) return 1;
	else goto j_08d44;
}
#endif
