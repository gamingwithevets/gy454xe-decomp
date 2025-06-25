#include <string.h>
#include "consts.h"
#include "generals.h"
#include "diagnostic.h"
#include "menufuncs.h"
#include "unk4.h"

const char *init_unk_0[] = {
	"Start?",
	"End?",
	"Step?",
	"a:b=X:d",
	"a:b=c:X",
	"FALSE",
	"TRUE",
	"All",
	"No-Solution",
};

const char **init_unk_1 = &d_080DC;

// 09712
void main(void) {
	char i, j;

	memset(&stack_start, 90, 800);
	f_046E0();
	delay(952);
	cursor_noflash = 0;
	if (need_reset()) {
		reset_all();
	}
	f_03558();
	set_contrast2_0();
	set_scr_normal();
	diag_factory_test();
	if (!diag_init_check()) {
		diagnostic_mode();
	}
	if (mode == MODE_EQN || mode == MODE_INEQ || mode == MODE_RATIO) {
		d_080FE = 0;
		last_key_keycode = 0;
		fill_screen(0x200);
	}

	if (mode == MODE_EQN) table_mode = TABLE_EQN;
	else if (mode == MODE_INEQ) table_mode = TABLE_INEQ;
	else if (mode == MODE_RATIO) table_mode = TABLE_RATIO;
	f_112EA();

	j = 0;
	while (1) {
		if (j) {
			set_scr_normal();
			f_044D6(getkeycode(1));
			if (last_key_keycode == K_MODE) screen_state = 1;
			else if (last_key_keycode == K_SETUP) screen_state = 2;
			else if (1 <= last_key_keycode && last_key_keycode <= 11) screen_state = 3;
			else if (last_key_keycode == K_CALC) {
				if (f_09D84()) continue;
				if (mode == MODE_COMP || mode == MODE_CMPLX) f_09D54(TABLE_CALC);
				else continue;
			}
			else if (last_key_keycode == K_SOLVE) {
				if (f_09D84()) continue;
				if (mode != MODE_COMP) continue;
				f_09D54(TABLE_SOLVE);
			}
			else screen_state = 0;
		}
		j = 1;
		if (screen_state) {
			switch (screen_state) {
				case 1:
					d_08126 = 0;
					i = mode_menu();
					break;
				case 2:
					d_08126 = 0;
					if (i = setup_menu()) {
						f_044D2();
						if (!(table_mode & (1 << 4))) f_044B6();
					}
					break;
				case 3:
					i = f_09014();
					if (i > 0) d_080F7 = 0;
					break;
			}
			if (!i) render();
			else j = 0;
			screen_state = 0;
			continue;
		}
		if (f_0B6B6()) j = f_09962(0);
		else {
			if (table_mode != TABLE_EQN && table_mode != TABLE_INEQ && TABLE_RATIO && last_key_keycode == K_AC) {
				table_mode = 1;
				f_0B8B8(1);
				j = 0;
			} else if (table_mode == TABLE_STAT_TABLE) {
				f_0A0BC();
				j = 1;
			}
			else if (table_mode == TABLE_MATRIX) j = f_0A93A(i);
			else if (table_mode == TABLE_VECTOR) j = f_0A936(i);
			else if (table_mode == TABLE_EQN) j = f_0A594(i);
			else if (table_mode == TABLE_RANGE) j = f_0A050(i);
			else if (table_mode == TABLE_CALC) j = f_09BDC(i);
			else if (table_mode == TABLE_SOLVE) j = f_09BDC(i);
		}
	}
}
