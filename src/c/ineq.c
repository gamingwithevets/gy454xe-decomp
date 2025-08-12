#if ENABLE_INEQ == 1

#include "generals.h"
#include "init.h"
#include "input.h"
#include "unk2.h"
#include "unk5.h"

// FUNCTION: GY460XF  Im 07D8A
static char *concat_x_eq(char *str) {
	// STRING: GY460XF  Im 007CC
	return smart_strcat(str, "X=");
}

// FUNCTION: GY460XF  Im 07D92
static char *concat_x_ne(char *str) {
	// STRING: GY460XF  Im 007CF
	return smart_strcat(str, "X\x95");  // X≠
}

// FUNCTION: GY460XF  Im 07D9A
static char *concat_x_lt(char *str) {
	// STRING: GY460XF  Im 007D2
	return smart_strcat(str, "X<");
}

// FUNCTION: GY460XF  Im 07DA2
static char *concat_x_le(char *str) {
	// STRING: GY460XF  Im 007D5
	return smart_strcat(str, "X\x94");  // X≤
}

// FUNCTION: GY460XF  Im 07DAA
static char *concat_lt_x(char *str) {
	// STRING: GY460XF  Im 007D8
	return smart_strcat(str, "<X");
}

// FUNCTION: GY460XF  Im 07DB2
static char *concat_le_x(char *str) {
	// STRING: GY460XF  Im 007DB
	return smart_strcat(str, "\x94X");  // ≤X
}

// FUNCTION: GY460XF  Im 07DBA
static char *concat_lt_x_lt(char *str) {
	// STRING: GY460XF  Im 007DE
	return smart_strcat(str, "<X<");
}

// FUNCTION: GY460XF  Im 07DC2
static char *concat_le_x_le(char *str) {
	// STRING: GY460XF  Im 007E2
	return smart_strcat(str, "\x94X<\x94");  // ≤X≤
}

// FUNCTION: GY460XF  Im 07DCA
static char *concat_ineq_result_template(char *out, char b, char c) {
	switch (b) {
		case 0:
			switch (c) {
				case 3:
				case 13:
					concat_x_eq(out);
					break;
				case 4:
				case 14:
					concat_x_ne(out);
					break;
				case 5:
				case 11:
				case 19:
					concat_x_lt(out);
					break;
				case 6:
				case 12:
				case 15:
				case 20:
					concat_x_le(out);
					break;
			break;
			}
		case 1:
			switch (c) {
				case 7:
					concat_lt_x(out);
					break;
				case 8:
					concat_le_x(out);
					break;
				case 9:
				case 17:
					concat_lt_x_lt(out);
					break;
				case 10:
				case 18:
					concat_le_x_le(out);
					break;
				case 11:
				case 12:
				case 13:
				case 19:
				case 20:
					concat_argsep(out);
					break;
				case 14:
					concat_argsep(out);
					concat_x_lt(out);
					break;
				case 16:
					concat_lt_x(out);
					concat_argsep(out);
					concat_x_ne(out);
					break;
				case 15:
					concat_argsep(out);
					concat_x_eq(out);
			break;
			}
		case 2:
			switch (c) {
				case 11:
					concat_lt_x(out);
					break;
				case 12:
				case 13:
					concat_le_x(out);
					break;
				case 17:
				case 18:
					concat_argsep(out);
					break;
				case 19:
					concat_lt_x_lt(out);
					break;
				case 20:
					concat_le_x_le(out);
					break;
			}
			break;
		case 17:
			concat_lt_x(out);
			break;
		case 18:
			concat_le_x(out);
			break;
	}
	return out;
}

// FUNCTION: GY460XF  Im 07F10
void f_07F10_460F(void) {
	char v0;
	char v1;
	char v2;
	char v3;
	char *v4;
	char v5;
	char v6;
	char v7;
	char loc_m36[36];
	char loc_m72[36];
	char **loc_m74;
	char loc_m90[16];

	v0 = mode_ram[360];
	if (v0 <= 2) {
		buffer_clear();
		font_size = 10;
		smart_strcpy(loc_m36, ineq_result[v0 - 1]);
		loc_m72[0] = '\0';
		concat_result(loc_m72, loc_m36);
		line_print(0, 22, loc_m72);
		set_result_store2disp(RESULT_STANDARD);
		return;
	} else {
		char i;

		v1 = get_result_store_fmt();
		v2 = 0;
		v3 = is_matho();
		v4 = get_result_str_ptr();
		if (!v4 || !v3) v4 = loc_m72;
		font_size = 7;
		// STRING: GY460XF  Im 007E6
		smart_strcpy(loc_m74, "@");
		if (v3) {
			if (v1 != RESULT_DECIMAL) {
				v2 = 1;
				use_output_charset = 1;
				font_size = 10;
			}
			if (d_0812C) {
				char *tmp;
j_07fcc_460f:
				d_08122 = 1;
				tmp = input_area_ptr;
				input_area_ptr = v4;
				input_print_mathi();
				input_area_ptr = tmp;
				use_output_charset = 0;
				return;
			}
		}
		v4[0] = '\0';
		cursor_pos_byte = 0;
		buffer_clear();
		smart_strcpy(loc_m90, concat_ineq_result_template(v4, 0, v0));
		v5 = 1;
		v6 = RESULT_DECIMAL;
		v7 = 1;
		if (v0 >= 9) v7 = 2;
		if (v0 >= 17) v7 = 3;
		for (i = 0; i <= v7; i++) {
			char tmp;
			v6 = select_result_format(v6, num_to_str(&mode_ram[370 + i * 10], loc_m36, 0));
			++*loc_m74;
			smart_strcat(loc_m90, loc_m74);
			loc_m72[0] = '\0';
			concat_ineq_result_template(loc_m72, i, v0);
			smart_strcat(loc_m90, loc_m72);
			if (!v2) {
				smart_strcpy(v4, loc_m74);
				// STRING: GY460XF  Im 007E8
				smart_strcat(v4, "=");
				concat_result(v4, loc_m36);
				v5 += 7;
				line_print(0, v5, v4);
			} else {
				smart_strcat(v4, loc_m36);
				smart_strcat(v4, loc_m72);
			}
		}
		line_print(0, 1, loc_m90);
		set_result_store2disp(v6);
		if (!v2) return;
		else goto j_07fcc_460f;
	}
}

#endif
