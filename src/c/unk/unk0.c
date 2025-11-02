#include "unk5.h"

typedef union { char byte[4]; unsigned int word[2]; } num_unk_2_struct;

// FUNCTION: GY454XE  Re 04D64
// FUNCTION: GY455XE  Im 054A6
// FUNCTION: GY460XF  Im 0496A
long num_unk_1__(char *num) {
	char v0;
	long v1;
	char num_tmp[10];
	long loc_m14;

	loc_m14 = 0;
	v0 = 0;
	do {
		long tmp;
		char tmp2;
		if (v0 < 24) {
			num_cpy(num_tmp, num_256);
			num_unk_1_1__(num, num_tmp);
		}
		tmp = num_to_byte(num);
		if (tmp2 = v0) {
			do tmp <<= 1;
			while (--tmp2);
		}
		v1 = tmp;
		loc_m14 |= v1;
		v0 += 8;
		num_cpy(num, num_tmp);
	} while (v0 <= 24);
	return loc_m14;
}

// FUNCTION: GY454XE  Re 04DE0
// FUNCTION: GY455XE  Im 05522
// FUNCTION: GY460XF  Im 049E6
void num_unk_2__(char *num, num_unk_2_struct b) {
	char v0;
	char loc_m4[4];
	char num_tmp[10];

	v0 = 0;
	do {
		union { unsigned int word; char byte[2]; } tmp;
		loc_m4[v0] = b.byte[0];
		tmp.byte[1] = b.byte[2];
		tmp.byte[0] = b.byte[1];
		b.word[0] = tmp.word;
		b.word[1] = b.byte[3];
	} while (++v0 < 4);
	num_fromdigit(num, 0);
	v0 = 3;
	do {
		num_frombyte(num_tmp, loc_m4[v0]);
		f_1A460(num, num_tmp);
		if (v0) {
			f_1A438(num, num_256);
			--v0;
		} else break;
	} while (1);
	return;
}
