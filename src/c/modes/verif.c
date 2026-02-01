#if ENABLE_VERIF == 1

#include "verif.h"
#include "../init.h"
#include "../consts.h"
#include "../io/input.h"
#include "../generals.h"
#include "../unk/unk5.h"
#include "../unk/unk6.h"

// FUNCTION: GY465XG  Im 049C4
static char is_compare_operator(char tok) {
	if (tok >= '<' && tok <= '>')
j_049ce_465g:
		return 1;
	if (tok < 0x94 || tok > 0x96) return 0;
	else goto j_049ce_465g;
}

// FUNCTION: GY465XG  Im 049DE
static char find_compare_operator(char *input) {
	char tmp;
	do {
		tmp = *input;
		if (!tmp) return 0;
		++input;
	} while (!is_compare_operator(tmp));
	return 1;
}

// FUNCTION: GY465XG  Im 049FC
char verif_try_insert_eq0(char *input) {
	if (mode == MODE_VERIF && !find_compare_operator(input)) {
		cursor_pos_byte = smart_strlen(input);
		insert_token('=', 0);
		insert_token('0', 0);
	}
	return;
}

// FUNCTION: GY465XG  Im 04A32
void num_to_str_verif(char *num, char *out) {
	char v0;

	v0 = 0;
	if (num_sign(num) != NUM_SIGN_ZERO) v0 = 1;
	smart_strcpy(out, verif_result[v0]);
	return;
}

// FUNCTION: GY465XG  Im 04A5C
void verif_draw_truefalse() {
	char loc_m16[16];

	smart_strcpy(loc_m16, verif_result[1]);
	// STRING: GY465XG  Im 01EB9
	smart_strcat(loc_m16, "/");
	smart_strcat(loc_m16, verif_result[0]);
	line_print(96 - smart_strlen(loc_m16) * 6, 22, loc_m16);
	return;
}

#endif
