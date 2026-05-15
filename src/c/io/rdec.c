#if ENABLE_RDEC == 1

#include "../consts.h"
#include "../unk/unk0.h"
#include "input.h"
#include "../unk/unk2.h"
#include "../unk/unk5.h"
#include "../unk/unk6.h"

// FUNCTION: GY460XF  Im 045C2
char f_045C2_460F(void) {
	if (!f_08D3A_460F())
j_045cc:
		return 0;
	else if (setup_rdec) return 1;
	else goto j_045cc;
}

// FUNCTION: GY460XF  Im 045DA
static void draw_rdec_notation(mathi_bbox *a, int x, char y) {
	char v0;
	int tmp;

	v0 = get_disp_setting_rdec();
	if (v0 != 3) {
		a->byte[2] += 2;
		a->word[0] += 2;
	} else a->word[0] += 12;
	if (v0 == 2) a->byte[3] += 2;
	if (mathi_enable_draw) {
		if (v0 <= 1) y -= get_font_width() + 1;
		if (v0 == 2) y += f_0898C() + 1;
		if (v0 == 0) {
			mathi_plot_pixel(x + 3, y);
			mathi_plot_pixel(a->word[0] + x - 5, y);
		} else if (v0 == 1 || v0 == 2) mathi_draw_line_horz(x, a->word[0] + x - 2, y);
		else {
			// STRING: GY460XF  Im 01F1E
			mathi_line_print("\xad", x - 6, y);  // ₍
			// STRING: GY460XF  Im 01F20
			mathi_line_print("\xae", a->word[0] + x - 12, y);  // ₎
		}
	}
	return;
}

// FUNCTION: GY460XF  Im 046A6
char *mathi_draw_rdec(char *input, mathi_bbox *b, int x, char y) {
	char *v0;

	v0 = input + 2;
	if (*v0 == '!') ++v0;
j_046da:
	if (*v0 == 0xb9) {
		if (get_disp_setting_rdec() == 3) x += 6;
		else ++x;
		if (v0 = mathi_draw_expr(input + 2, b, x, y)) {
			if (v0[1] < '0' || v0[1] > '9') draw_rdec_notation(b, x, y);
			else goto j_046ca;
		}
	} else if (*v0 < '0' || *v0 > '9')
j_046ca:
		return 0;
	else {
		++v0;
		goto j_046da;
	}
	return v0;
}

// FUNCTION: GY460XF  Im 0472C
char num_to_str_rdec(char *out, char *num) {
	int v0;
	char loc_m100[100];
	long loc_m500[100];
	long loc_m504;
	char loc_m505;
	char loc_m506;
	unsigned long loc_m510;
	unsigned long loc_m514;
	char loc_m515;
	char loc_m516;

	if (num_sign(&num[10]) == NUM_SIGN_NEGATIVE) concat_negative(out);
	num_abs(num);
	concat_num_str(out, num);
	// STRING: GY460XF  Im 01F22
	if (setup_decimal_mark) smart_strcat(out, ".");
	// STRING: GY460XF  Im 01F24
	else smart_strcat(out, ",");

	v0 = smart_strlen(out);
	loc_m515 = v0;
	loc_m516 = 100 - v0 - 3;
	num_abs(&num[10]);
	loc_m510 = num_unk_1__(&num[10]);
	loc_m514 = num_unk_1__(&num[20]);
	loc_m505 = 0;
	do {
		loc_m510 *= 10;
		loc_m100[loc_m505] = loc_m510 / loc_m514;
		loc_m510 -= loc_m514 * (char)(loc_m510 / loc_m514);
		loc_m500[loc_m505] = loc_m510;
		for (loc_m506 = 0; loc_m506 < loc_m505; loc_m506++) {
			if (loc_m100[loc_m506] == loc_m100[loc_m505]) {
				loc_m504 = loc_m500[loc_m506];
				if (loc_m504 == loc_m500[loc_m505]) {
					if (loc_m500[loc_m506]) {
						char *tmp;
						char *tmp2;
						char i;

						tmp = &out[loc_m515];
						tmp2 = loc_m100;
						for (i = 0; i < loc_m506; i++) {
							*tmp = *tmp2 + '0';
							++tmp2;
							++tmp;
						}
						*tmp++ = 0xa4;  // Recurring decimal
						*tmp = 0xb8;
						for (++tmp; i < loc_m505; tmp++) {
							*tmp = *tmp2 + '0';
							++tmp2;
							++i;
						}
						*tmp++ = 0xb9;
						*tmp = '\0';
						return RESULT_RDEC;
					} else goto j_048da;
				}
			}
		}
	} while (++loc_m505 < loc_m516);
j_048da:
	set_result_store_fmt(RESULT_DECIMAL);
	return RESULT_DECIMAL;
}

#endif
