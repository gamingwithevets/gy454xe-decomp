#include "consts.h"
#include "generals.h"
#include "unk1.h"
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

// 085C0 - STUB
void f_085C0(void) {
	return;
}

// 085D2 - STUB
void f_085D2(void) {
	return;
}

// 085EC - STUB
void num_to_str(void) {
	return;
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
char f_087BA(void) {
	if (f_087A2()) {
		if (!setup_mathi) return 0;
		if (mode & (1 << 7)) return 1;
	} else return 0;
}

// 087D8
char f_087D8(void) {
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

// 088AA - STUB
int f_088AA(void) {
	return 0;
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

// 088FE - STUB
void f_088FE(void) {
	return;
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

// 08A66 - STUB
void f_08A66(void) {
	return;
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

