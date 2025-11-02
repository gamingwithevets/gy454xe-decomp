#include <string.h>
#include "consts.h"
#include "generals.h"
#include "diagnostic.h"
#include "modes/setup.h"
#include "modes/menufuncs.h"
#include "unk/unk4.h"

// FUNCTION: GY454XE  Re 09712
// FUNCTION: GY455XE  Im 0A036
// FUNCTION: GY460XF  Im 099BE
// FUNCTION: GY465XG  Im 095AA
void main(void) {
	char i, j;

	memset(&stack_start, 90, 800);
	f_046E0();
	delay(952);
	cursor_noflash = 0;
	if (need_reset()) reset_all();
	f_03558();
	set_contrast2_0();
	set_scr_normal();
	diag_factory_test();
	if (!diag_init_check()) diagnostic_mode();
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
			set_keycode(getkeycode(1));
			if (last_key_keycode == K_MODE) screen_state = 1;
			else if (last_key_keycode == K_SETUP) screen_state = 2;
			else if (K_SD <= last_key_keycode && last_key_keycode <= K_VERIFY) screen_state = 3;
			else if (last_key_keycode == K_CALC) {
				if (f_09D84()) continue;
				if (mode != MODE_COMP && mode != MODE_CMPLX) continue;
				f_09D54(TABLE_CALC);
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
						if (table_mode & (1 << 4)) f_044B6();
					}
					break;
				case 3:
					i = f_09014();
					if ((signed char)i > 0) force_nochar = 0;
					break;
			}
			if (!i) render();
			else j = 0;
			screen_state = 0;
			continue;
		}
		if (f_0B6B6()) j = f_09962(0);
		else {
			char tm = table_mode;
			if (tm != TABLE_EQN && tm != TABLE_INEQ && tm != TABLE_RATIO && last_key_keycode == K_AC) {
				table_mode = TABLE_NONE;
				f_0B8B8(1);
				j = 0;
			} else if (tm == TABLE_STAT_TABLE) {
				table_stat_handler();
				j = 1;
			}
			else if (tm == TABLE_MATRIX) j = table_mat_handler(i);
			else if (tm == TABLE_VECTOR) j = table_vct_handler(i);
			else if (tm == TABLE_EQN) j = table_eqn_handler(i);
			else if (tm == TABLE_RANGE) j = table_range_handler(i);
			else if (tm == TABLE_CALC) j = calc_solve_handler(i);
			else if (tm == TABLE_SOLVE) j = calc_solve_handler(i);
#if ENABLE_INEQ == 1
			else if (tm == TABLE_INEQ) j = table_ineq_handler(i);
#endif
#if ENABLE_RATIO == 1
			else if (tm == TABLE_RATIO) j = table_ratio_handler(i);
#endif
		}
		render();
	}
}
