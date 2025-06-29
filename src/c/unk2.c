#include "consts.h"
#include "generals.h"
#include "input.h"
#include "unk2.h"

// 07F30 - STUB
void f_07F30(void) {
	return;
}

// 0802E - STUB
void f_0802E(void) {
	return;
}

// 081DE - STUB
void f_081DE(void) {
	return;
}

// 082C6 - STUB
void f_082C6(void) {
	return;
}

// 08304 - STUB
void f_08304(void) {
	return;
}

// 085C0
char f_085C0(void) {
	char v0;

	if (last_key_keycode == K_EXECUTE) v0 = 13;
	return f_02ADE(v0);
}

// 085D2
char f_085D2(void) {
	char v0;

	v0 = 0;
	if (f_0B588()) v0 = 13;
	f_02ADE(v0);
	return v0;
}

// 085EC - STUB
char num_to_str(char *num, char *out, char c) {
	out[0] = '\0';
	return 0;
}

// 08764 - STUB
void f_08764(void) {
	return;
}

// 087A2
char f_087A2(void) {
	if (table_mode & (1 << 4)) return 0;
	if (!f_02CB6()) return 1;
	else return 0;
}

// 087BA
char is_mathi(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 7)) return 1;
	} else return 0;
}

// 087D8
char is_matho(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 6)) return 1;
	} else return 0;
}

// 087F6
void f_087F6(int a, int b) {
	if (a > b) a = b;
}

// 08800
char f_08800(unsigned int a, unsigned int b, char c, char d, unsigned int e) {
	if (a != b) c = (b * c + (d - c) * e - a * d) / (b - a);
	return c;
}

// 0885C
char f_0885C(char a, char b, char c, char d, char e) {
	if (d != c) a = (d * a + (b - a) * e - c * b) / (d - b);
	return a;
}

// 088AA
int f_088AA(void) {
	return formula_x + *(int *)&reg0[0];
}

// 088B8 - STUB
void f_088B8(void) {
	return;
}

// 088D6 - STUB
void f_088D6(void) {
	return;
}

// 088EC
char is_pow_char(char *a) {
	char v0 = 0;

	if (a[0] == 0x5e) v0 = 1;
	return v0;
}

// 088FE
char f_088FE(char *a) {
	// Box, Hex B, Hex C
	if (*a++ == '!' && (*a == 0xb9 || *a == 0xba)) return 1;
	return 0;
}

// 0891A
char is_ins_mode(void) {
	char v0 = 0;
	if (modifiers & (1 << 7)) v0 = 1;
	return v0;
}

// 08926 - STUB
void f_08926(void) {
	return;
}

// 0895A - STUB
void f_0895A(void) {
	return;
}

// 0897C
char f_0897C(void) {
	char s = 10;
	if (font_size == 7) s = 7;
	return s;
}

// 0898C
char f_0898C(void) {
	char s = 5;
	if (font_size == 7) s = 3;
	return s;
}

// 0899C
char f_0899C(void) {
	return 0;
}

// 089AC - STUB
void f_089AC(f_058DC_union *a, f_058DC_union b) {
	return;
}

// 08A0A - STUB
char f_08A0A(void) {
	return 0;
}

// 08A2A
char f_08A2A(char a) {
	if (a == 8) return 1;
	if (a != 10 && a != 2 && a != 5 && a < 12) return 0;
	else return 1;
}

// 08A48
char f_08A48(char *a) {
	if (a < *input_area_ptr) return 1;
	else {
		if (!f_08A2A(f_05658(a))) return 0;
		else return 1;
	}
}

// 08A66
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

// 08A9C
void f_08A9C(char a) {
	reg0[9] = 0;
	font_size = 10;
	d_0800A = 0;
	reg0[8] = 0;
	draw_mode = 0;
	reg0[7] = 0;
}

// 08ABA
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

// 08ACC
char f_08ACC(void) {
	char v1 = font_size;
	font_size = 7;
	return v1;
}

// 08ADC
char f_08ADC(void) {
	int v0 = 0;
	if (mode == MODE_TABLE && table_mode == 1) v0 = 1;
	return (char)v0;
}

