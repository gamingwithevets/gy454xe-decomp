#include "consts.h"

#ifndef EMU_KB_H
#define EMU_KB_H

// Only used in emulator ROMs.
typedef struct {
	char *stop_type;		// 8E00H
	char *ki;				// 8E01H
	char *ko;				// 8E02H
	char *d_08E10;			// 8E10H
	char *d_09000;			// 9000H
	char *d_09800;			// 9800H
	char *d_09804;			// 9804H
	char *d_09808;			// 9808H
	char *d_09834;			// 9834H
	char *error_buf;		// 9838H
} emu_kb;

enum es_stop_type {
	ES_STOP_GETKEY = 1,
	ES_STOP_ACBREAK = 2,
	ES_STOP_DOOFF = 3,
	ES_STOP_DDOUT = 4,
	ES_STOP_QRCODE_IN = 5,
	ES_STOP_QRCODE_OUT = 6,
	ES_STOP_QRCODE_IN3 = 7,
	ES_STOP_ACBREAK2 = 8
};

#endif

void f_04DF6_E(void);
void f_04E44_E(void);
void init_emu_kb(emu_kb *kb);
void f_05338_E(char *a);
void f_05428_E(emu_kb *kb);
