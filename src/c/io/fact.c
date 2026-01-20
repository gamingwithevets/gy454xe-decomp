#if ENABLE_FACT == 1

#include "fact.h"
#include "../consts.h"
#include "../generals.h"
#include "../unk/unk2.h"
#include "../unk/unk5.h"

// FUNCTION: GY465XG  Im 048B6
static void f_048B6_465G(char *out, char b) {
	char v0;
	char v1;
	char loc_m4[4];

	v0 = 0;
	if (v1 = b / 10) {
		loc_m4[0] = b + 0x30;
		v0 = 1;
	}
	loc_m4[v0++] = b - v1 * 10 + 0x30;
	loc_m4[v0] = 0;
	smart_strcat(out, loc_m4);
	return;
}

// FUNCTION: GY465XG  Im 04912
char num_to_str_fact(char *out, char *num) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = 0;
	f_04796();
	v1 = f_131EE(num);
	v2 = mode_ram[462];
	for (v3 = 0; v3 < v2; v3++) {
		char tmp;

		if (v1 && v2 - 1 == v3) v0 = 1;
		// STRING: GY465XG  Im 01EB0
		if (v3 > 0) smart_strcat(out, "$");
		// STRING: GY465XG  Im 01EB2
		if (v0) smart_strcat(out, "(");
		concat_num_str(out, &mode_ram[476 + v3 * 10]);
		// STRING: GY465XG  Im 01EB4
		if (v0) smart_strcat(out, ")");
		tmp = mode_ram[463 + v3];
		if (mode_ram[463 + v3] > 1) {
			// STRING: GY465XG  Im 01EB6
			smart_strcat(out, "^\xb8");
			f_048B6_465G(out, tmp);
			concat_mathi_r(out);
		}
	}
	return RESULT_FACT;
}

#endif
