#include "unk0.h"
#include "unk5.h"

extern const char num_256[];

// FUNCTION: GY454XE  Re 04D64
long num_unk_1__(char *a) {
	long v0;
	char v1[10];
	long v2;
	unsigned char i, j, k;
	
	v0 = 0;
	i = 0;
	
	do {
		if (i < 24) {
			num_cpy(v1, num_256);
			num_unk_1_1__(a, v1);
		}
		k = num_to_byte(a);
		v2 = k;
		j = i;
		if (j) {
			do {
				v2 <<= 1;
			} while (--j);
		}
		
		v0 |= v2;
		i += 8;
		num_cpy(a, v1);
	} while (i <= 24);
	
	return v2;
}

// FUNCTION: GY454XE  Re 04DE0
void num_unk_2__(char *a, long b) {
	unsigned char i;
	char v1[4];
	int v2;
	
	i = 0;
	do {
		v1[i] = b & 0xff;
		b = ((b & 0x00ffff00) >> 8) | ((b & 0xff000000) >> 24);
	} while (++i < 4);
	num_fromdigit(a, 0);
	
	i = 3;
	do {
		num_frombyte(&v2, v1[i]);
		f_1A460(a, &v2);
		if (!i) return;
		f_1A438(a, num_256);
		--i;
	} while (1);
}
