#include <string.h>
#include "../init.h"
#include "../consts.h"
#include "../generals.h"
#include "../emu/emu_kb.h"
#include "../io/input.h"
#include "../modes/verif.h"
#include "unk2.h"
#include "unk4.h"
#include "unk4_1.h"
#include "unk5.h"

// Struct used by the menu list. NOTE: For any index, if the index has the MSB set, it is returned directly with a return code of 1.
typedef struct {
	const char *string;	// String printed when menu is displayed.
	char ret_mode;		// Return value in values if the corresponding bit (MSB->LSB = 0->7) is set; otherwise use it as index.
	char char_mode;		// Set bits (MSB->LSB = 0->7) denote tokens. display_menu returns 2 if tokens are returned, otherwide it will return 3.
	char values[8];		// Values corresponding to the (up to) 8 choices.
	char prev_page;		// Index of menu to go to when pressing up. NULL = none.
	char next_page;		// Index of menu to go to when pressing down. NULL = none.
	char parent;		// Index of menu to go to when pressing left. 
	char keycode;		// Keycode associated with this menu.
} menu;

// Static declarations
static void copy_input_prompt(char idx, char is_solve, char is_init_str);
static char table_stat_nav(char a, char keycode);
static void f_0A3B4(void);
static void table_set_cur_cell(char *num);
static void table_eqn_key_handler(void);
static void table_eqn_draw_text(void);
static void table_eqn_draw_lines(void);
static char table_eqn_nav(char keycode);
static char table_matvct_handler(char sm);
static char table_matvct_nav(char sm, char keycode);
static void table_matvct_draw_cols(char a, char b);

// DATA: GY454XE  Re 02216
// DATA: GY455XE  Im 023A8
const char menu_mode[] = {
	"1:COMP  2:CMPLX \0"
	"3:STAT  4:BASE-N\0"
	"5:EQN   6:MATRIX\0"
	"7:TABLE 8:VECTOR"
};

const char menu_mode_1[] = {
	"1:INEQ  2:VERIF \0"
	"3:RATIO         \0"
	"\0"
	"\0"
};

// DATA: GY454XE  Re 0225A
// DATA: GY455XE  Im 023EC
const char menu_setup_0[] = {
	"1:MthIO 2:LineIO\0"
	"3:Deg   4:Rad   \0"
	"5:Gra   6:Fix   \0"
	"7:Sci   8:Norm  \0"
};

// DATA: GY454XE  Re 0229F
// DATA: GY455XE  Im 02431
const char menu_setup_1[] = {
	"1:ab/c  2:d/c   \0"
	"3:CMPLX 4:STAT  \0"
	"5:Rdec  6:Disp  \0"
	"7:" "\x9f" "CONT" "\x9e" "\0"
	"\0"
};

// DATA: GY454XE  Re 022D4
// DATA: GY455XE  Im 02466
const char menu_stat_type[] = {
	"1:1-VAR 2:" "\x9a" "+" "\x9b" "X\0"
	"3:" "\x5f" "+" "\x9c" "X" "\xa2" " 4:ln X\0"
	"5:" "\x81" "^X   6:" "\x9a" "*" "\x9b" "^X\0"
	"7:" "\x9a" "*X^" "\x9b" " 8:1/X"
};

// DATA: GY454XE  Re 02310
// DATA: GY455XE  Im 024A2
const char menu_stat_table[] = {
	"1:Type  2:Data\0"
	"3:Edit\0"
	"\0"
	"\0"
};

// DATA: GY454XE  Re 02329
// DATA: GY455XE  Im 024BB
const char menu_stat_1var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Distr 6:MinMax\0"
	"\0"
};

// DATA: GY454XE  Re 02359
// DATA: GY455XE  Im 024EB
const char menu_stat_2var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Reg   6:MinMax\0"
	"\0"
};

// DATA: GY454XE  Re 02389
// DATA: GY455XE  Im 0251B
const char menu_eqn[] = {
	"1:a" "\x9d" "X+b" "\x9d" "Y=c" "\x9d" "\0"
	"2:a" "\x9d" "X+b" "\x9d" "Y+c" "\x9d" "Z=d" "\x9d" "\0"
	"3:aX" "\xa2" "+bX+c=0\0"
	"4:aX" "\xa3" "+bX" "\xa2" "+cX+d=0\0"
};

// DATA: GY454XE  Re 023C6
// DATA: GY455XE  Im 02558
// DATA: GY460XF  Im 022AC
// DATA: GY465XG  Im 0222A
// DATA: GY468XB  Im 022CC
const menu menus[] = {
// n		 String						Return	Char	Option values/Menu indexes/Tokens (up to 8)			Up		Down	Left	Keycode
// Placeholder
/* 0  */	{NULL,						0xff,	0,		{0},												NULL,	NULL,	NULL,	NULL},

// CLR menu (SHIFT 9)
/* 1  */	{menu_clr,					0xff,	0,		{1, 2, 3},											NULL,	NULL,	NULL,	K_CLR},

// SETUP menu (SHIFT MODE)
/* 2  */	{menu_setup_0,				0x7f,	0,		{4, 2, 3, 4, 5, 6, 7, 8},							NULL,	3,		NULL,	K_SETUP},
/* 3  */	{menu_setup_1,				0xc3,	0,		{9, 10, 5, 6, MENU_IDX_SETUP_RDEC,
																		MENU_IDX_SETUP_DEC_MARK, 11},		2,		NULL,	NULL,	K_SETUP},
/* 4  */	{menu_setup_decimalo,		0xff,	0,		{22, 23},											NULL,	NULL,	2,		K_SETUP},
/* 5  */	{menu_setup_cmplx_result,	0xff,	0,		{12, 13},											NULL,	NULL,	3,		K_SETUP},
/* 6  */	{menu_setup_stat_freq,		0xff,	0,		{14, 15},											NULL,	NULL,	3,		K_SETUP},
#if ENABLE_RDEC == 1
			{menu_setup_rdec,			0xff,	0,		{16, 17},											NULL,	NULL,	3,		K_SETUP},
#endif
/* 7  */	{menu_setup_decimal_mark,	0xff,	0,		{18, 19},											NULL,	NULL,	3,		K_SETUP},

#if ENABLE_MATRIX == 1
// MATRIX mode (SHIFT 4)
/* 8  */	{menu_matrix_table,			0xff,	0,		{1, 2},												NULL,	NULL,	NULL,	K_MATRIX},
/* 9  */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 10 */	{menu_matrix_data,			0x1f,	0,		{6, 7, 8},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 11 */	{menu_matrix_dim0,			0xff,	0,		{9, 10, 11, 12, 13, 14},							NULL,	MATRIX_MENU_START+4,
																															0xff,	K_MATRIX},
/* 12 */	{menu_matrix_dim1,			0xff,	0,		{15, 16, 17},										MATRIX_MENU_START+3,
																													NULL,	0xff,	K_MATRIX},
/* 13 */	{menu_matrix,				0xff,	0x3f,	{1, 2, 0xc8, 0xc9, 0xca, 0xcb, 0xc0, 0xc1},			NULL,	NULL,	NULL,	K_MATRIX},
#endif

#if ENABLE_VECTOR == 1
// VECTOR mode (SHIFT 5)
/* 14 */	{menu_vector_table,			0xff,	0,		{18, 19},											NULL,	NULL,	NULL,	K_VECTOR},
/* 15 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 16 */	{menu_vector_data,			0x1f,	0,		{23, 24, 25},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 17 */	{menu_vector_dim,			0xff,	0,		{26, 27},											NULL,	NULL,	0xfc,	K_VECTOR},
/* 18 */	{menu_vector,				0xff,	0x3f,	{18, 19, 0xcc, 0xcd, 0xce, 0xcf, 0x9e},				NULL,	NULL,	NULL,	K_VECTOR},
#endif

#if ENABLE_CMPLX == 1
// CMPLX mode (SHIFT 2)
/* 19 */	{menu_cmplx,				0xff,	0xf0,	{0xc3, 0x88, 0x56, 0x55},							NULL,	NULL,	NULL,	K_CMPLX},
#endif

#if ENABLE_BASE_N == 1
// BASE-N mode (SHIFT 3)
/* 20 */	{menu_base_n_0,				0xff,	0xfc,	{0x6e, 0x6f, 0x7e, 0x7f, 0xc1, 0x62},				NULL,	BASE_N_MENU_START+1,
																															NULL,	K_BASE},
/* 21 */	{menu_base_n_1,				0xff,	0xfc,	{0x51, 0x50, 0x53, 0x52},							BASE_N_MENU_START,
																													NULL,	NULL,	K_BASE},
#endif

// Angle unit (DRG>) menu (SHIFT Ans)
/* 22 */	{menu_drg,					0xff,	0xe0,	{0x85, 0x86, 0x87},									NULL,	NULL,	NULL,	K_DRG},

// Hyperbolic functions (hyp) menu
/* 23 */	{menu_hyp,					0xff,	0xfc,	{0x70, 0x71, 0x72, 0x90, 0x91, 0x92},				NULL,	NULL,	NULL,	K_HYP},

// STAT mode (SHIFT 1)
/* 24 */	{menu_stat_table,			0x5f,	0,		{27, 20, 28},										NULL,	NULL,	NULL,	K_SD},
/* 25 */	{menu_stat_1var,			0x43,	0,		{27, 20, 29, 31, 35, 33},							NULL,	NULL,	NULL,	K_SD},
/* 26 */	{menu_stat_2var,			0x4b,	0,		{27, 20, 30, 32, 23, 34},							NULL,	NULL,	NULL,	K_SD},
/* 27 */	{menu_stat_type,			0xff,	0,		{1, 2, 3, 4, 5, 6, 7, 8},							NULL,	NULL,	0xff,	K_SD},
/* 28 */	{menu_stat_data_edit,		0xff,	0,		{21, 22},											NULL,	NULL,	24,		K_SD},
/* 29 */	{menu_stat_1var_sum,		0xff,	0,		{0x10, 0x11},										NULL,	NULL,	25,		K_SD},
/* 30 */	{menu_stat_2var_sum,		0xff,	0,		{0x10, 0x11, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18},	NULL,	NULL,	26,		K_SD},
/* 31 */	{menu_stat_1var_var,		0xff,	0,		{0x12, 0x89, 0xaa, 0xab},							NULL,	NULL,	25,		K_SD},
/* 32 */	{menu_stat_2var_var,		0xff,	0,		{0x12, 0x89, 0xaa, 0xab, 0x8a, 0xac, 0xad},			NULL,	NULL,	26,		K_SD},
/* 33 */	{menu_stat_1var_minmax,		0xff,	0xc0,	{0x19, 0x1a},										NULL,	NULL,	25,		K_SD},
/* 34 */	{menu_stat_2var_minmax,		0xff,	0xf0,	{0x19, 0x1a, 0x1b, 0x1c},							NULL,	NULL,	26,		K_SD},
/* 35 */	{menu_stat_distr,			0xff,	0xf0,	{0xd0, 0xd1, 0xd2, 0xd3},							NULL,	NULL,	25,		K_SD},
/* 36 */	{menu_stat_reg,				0xff,	0xf8,	{0x9a, 0x9b, 0x9d, 0x65, 0x66},						NULL,	NULL,	26,		K_SD},
/* 37 */	{menu_stat_reg_quad,		0xff,	0xfc,	{0x9a, 0x9b, 0x9c, 0x64, 0x67, 0x66},				NULL,	NULL,	26,		K_SD},

// EQN mode
/* 38 */	{menu_eqn,					0xff,	0,		{1, 2, 3, 4},										NULL,	NULL,	NULL,	NULL},

#if ENABLE_MATRIX == 1
// MATRIX mode (submode select)
/* 39 */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	NULL,	NULL},
#endif

#if ENABLE_VECTOR == 1
// VECTOR mode (submode select)
/* 40 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	NULL,	NULL},
#endif

// STAT mode (submode select)
/* 41 */	{menu_stat_type,			0xff,	0,		{1, 2, 3, 4, 5, 6, 7, 8},							NULL,	NULL,	NULL,	NULL},

// MODE menu
/* 42 */	{menu_mode,					0xff,	0,		{MODE_COMP, MODE_CMPLX, MODE_STAT, MODE_BASE_N,
														MODE_EQN, MODE_MATRIX, MODE_TABLE, MODE_VECTOR},	NULL,	MODE_MENU_START+1,
																															NULL,	K_MODE},
			{menu_mode_1,				0xff,	0,		{MODE_INEQ, MODE_VERIF, MODE_RATIO},				MODE_MENU_START,
																													NULL,	NULL,	K_MODE},

#if ENABLE_RATIO == 1
// RATIO mode (submode select)
			{menu_ratio,				0xff,	0,		{1, 2},												NULL,	NULL,	NULL,	NULL},
#endif

#if ENABLE_INEQ == 1
// INEQ mode (submode select)
			{menu_ineq,					0xff,	0,		{9, 10},											NULL,	NULL,	NULL,	K_MODE},
			{menu_ineq_poly2,			0xff,	0,		{1, 2, 3, 4},										NULL,	NULL,	0xff,	K_MODE},
			{menu_ineq_poly3,			0xff,	0,		{5, 6, 7, 8},										NULL,	NULL,	0xff,	K_MODE},
#endif

#if ENABLE_VERIF == 1
// VERIF mode (SHIFT 6)
			{menu_verify,				0xff,	0xfc,	{0x3d, 0x95, 0x3e, 0x3c, 0x96, 0x94},				NULL,	NULL,	NULL,	K_VERIFY},
#endif

};

// FUNCTION: GY454XE  Re 0995E
// FUNCTION: GY455XE  Im 0A282
// FUNCTION: GY460XF  Im 09C28
char keyfunc_nop(keyfunc_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 09962
// FUNCTION: GY455XE  Im 0A286
// FUNCTION: GY460XF  Im 09C2C
char f_09962(char no_keyfunc) {
	keyfunc_struct v0;
	char v1;
	char v2;

	f_0B67E();
	font_size = 10;
	if (table_mode & (1 << 4)) font_size = 7;

	memzero(&v0, 12);
	v0.unk_0x04 = 1;
	v0.input_area = input_area;
	v0.result = result;
	v0.mode = mode;
	v0.unk_0x07 = f_0A564();
	v0.is_mathi = is_mathi();
	v0.is_matho = is_matho();
	v0.unk_0x0a = f_02CB6();
	if (!(d_080FE & 0x60)) arrow_state = 0;
	if (!no_keyfunc) {
		if (!is_char_keycode(last_key_keycode)) {
			v1 = last_key_keycode - 0xe0;
			if (is_rcl_keycode(last_key_keycode)) v1 = K_RCL - 0xe0;
			else if (is_sto_keycode(last_key_keycode)) v1 = K_STO - 0xe0; 
			else if (last_key_keycode < 0xe0) goto j_09b9c;
			switch (keyfuncs[v1](&v0)) {
				case 0: goto j_09b9c;
				case 1: goto j_09b6c;
				case 2: goto j_09b72;
				case 3: goto j_09b76;
			}
		} else if (force_nochar && 0xcf <= last_key_keycode) {
			set_char_keycode(get_func_tok(last_key_keycode));
			if (v0.is_mathi) v0.is_func = 1;
		} else if (last_key_keycode == 0x7c) last_key_keycode = 0;
		if (!(d_080FE & (1 << 6))) {
			if (table_mode & (1 << 7) && d_080FD == 2 && (last_key_keycode == K_COLON || last_key_keycode == 0xa4)) last_key_keycode = 0;
			else if (table_mode & (1 << 7) && d_080FD == 2 && last_key_keycode == K_EQUALS) last_key_keycode = 0;
			else if (table_mode & (1 << 4)) last_key_keycode = filter_chars_tables(last_key_keycode);
			else if (v0.mode == MODE_STAT || v0.mode == MODE_MATRIX || v0.mode == MODE_VECTOR) last_key_keycode = filter_chars_stat_mat_vct(last_key_keycode);
			else if (v0.mode == MODE_TABLE) last_key_keycode = filter_chars_table(last_key_keycode);
			else if (v0.mode == MODE_CMPLX) last_key_keycode = filter_chars_cmplx(last_key_keycode);
			else if (v0.mode == MODE_VERIF) last_key_keycode = filter_chars_verif(last_key_keycode);
			if (last_key_keycode == K_EQUALS && v0.mode != MODE_COMP && v0.mode != MODE_CMPLX && v0.mode != MODE_VERIF) last_key_keycode = 0;
			if (last_key_keycode == 0xa4 && !setup_mathi) last_key_keycode = 0;
			if (last_key_keycode) {
				if (is_char_keycode(last_key_keycode)) f_0B736();
				if (d_080FE == 1) {
					insert_token(last_key_keycode, v0.is_func);
					goto j_09b6c;
				}
			}
		}
		goto j_09b9c;
	} else goto j_09b72;
j_09b6c:
	d_080FE = 1;
j_09b72:
	f_0C0D0();
j_09b76:
	if (d_080FE != 1) {
		if (f_0C1A0(&v0) != 1 && f_0C1D4(&v0) != 1) print_result(v0.result);
		else goto j_09b6c;
	}
j_09b9c:
	if (!(d_080FE & (1 << 5)) && (char)get_num_replay_entries() > 0) arrow_state = 1;
	return v0.unk_0x04;
}

// FUNCTION: GY454XE  Re 09BDC
// FUNCTION: GY455XE  Im 0A500
// FUNCTION: GY460XF  Im 09EA6
char calc_solve_handler(char a) {
	char v0;
	char v1;
	char *v2;
	char tmp;
	char *loc_m2;

	v0 = 1;
	if (last_key_keycode) {
		if (is_meta_keycode()) f_09962(0);
		else {
			if ((last_key_keycode == K_APPROX || last_key_keycode == K_EXECUTE) && d_080FE == 4) {
				++mode_ram[351];
				f_044CE();
			}
			switch (d_080FD) {
				case 1:
					input_area_ptr = input_area;
					if (!f_0BB42(&loc_m2)) {
						v1 = ERROR_SYNTAX;
						goto j_09c7c;
					} else if (v1 = f_15DE8()) {
j_09c7c:
						d_080FE = 0x80;
						set_keycode(show_error(v1));
						buffer_clear();
						table_mode = TABLE_NONE;
						v0 = 0;
						break;
					} else if (mode_ram[352] == 0xff) {
						f_0AF16();
						v0 = K_APPROX;
						if (f_0B588()) v0 = K_EXECUTE;
						set_keycode(v0);
						v0 = 0;
						break;
					}
				case 0x10:
				case 0x20:
					d_080FD = 2;
					mode_ram[351] = 0;
					f_0B8B8(1);
					d_080FE = 4;
				case 2:
					v2 = &mode_ram[mode_ram[351] + 352];
					if (*v2 != 0xff) {
						if (table_mode == TABLE_SOLVE && *v2 == mode_ram[350]) copy_input_prompt(mode_ram[350], 1, 0);
						else copy_input_prompt(*v2, 0, 0);
					} else {
						if (table_mode == TABLE_SOLVE) tmp = 4;
						else tmp = 3;
						d_080FD = tmp;
						v0 = 0;
					}
					break;
				case 4:
					set_keycode(K_APPROX);
				case 3:
					smart_strcpy(input_area, cache_area);
					d_080FE = 1;
					v0 = 0;
					break;
				case 0x40:
					d_080FD = 4;
					v0 = 0;
					break;
			}
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 09D54
// FUNCTION: GY455XE  Im 0A678
// FUNCTION: GY460XF  Im 0A01E
char f_09D54(char tmode) {
	if (table_mode & (1 << 7)) smart_strcpy(input_area, cache_area);
	table_mode = tmode;
	d_080FD = 1;
	d_080FE = 4;
	f_044CE();
}

// FUNCTION: GY454XE  Re 09D84
// FUNCTION: GY455XE  Im 0A6A8
// FUNCTION: GY460XF  Im 0A04E
char f_09D84(void) {
	char v0 = 0;

	if (d_080FE & (1 << 6)) v0 = 1;
	if (table_mode & (1 << 7) && d_080FD == 2) v0 = 1;
	if (!smart_strlen(input_area)) v0 = 1;
	return v0;
}

// FUNCTION: GY454XE  Re 09DB6
// FUNCTION: GY455XE  Im 0A6DA
// FUNCTION: GY460XF  Im 0A080
static void copy_input_prompt(char idx, char is_solve, char is_table_prompt) {
	char v0;
	char out[16];
	char varstr[4];

	v0 = 0;
	if (!is_format_keycode(last_key_keycode)) v0 = 1;
	if (is_table_prompt) {
		strcpy(out, table_prompts[idx-1]);
		set_result(&mode_ram[(idx-1)*10]);
	} else {
		varstr[0] = vars_list[idx];
		if (is_solve) {
			strcpy(out, s_prompt_solve);
			varstr[1] = '\0';
		} else {
			out[0] = '\0';
			varstr[1] = '?';
			varstr[2] = '\0';
		}
		l_var(idx, result);
		strcat(out, varstr);
	}
	buffer_clear();
	smart_strcpy(input_area, out);
	font_size = 10;
	d_080FE = 4;
	f_09962(v0);
	return;
}

// FUNCTION: GY454XE  Re 09E70
// FUNCTION: GY455XE  Im 0A794
// FUNCTION: GY460XF  Im 0A13A
char display_menu(char *val, char *head) {
	char v0;
	char v00;
	char v000;
	char v0000;
	int idx;
	int idx2;
	char v3;
	char v4;

	v4 = 0;
	if (head == 0xffff) {
		v4 = 1;
		head = NULL;
	}
	idx2 = *val;
	idx = *val;
	while (1) {
		if (idx) {
			idx2 = idx;
			if (head) print_4lines_head(head, menus[idx].string);
			else print_4lines(menus[idx].string);
			if (menus[idx].prev_page) set_up_arrow();
			if (menus[idx].next_page) set_down_arrow();
		}
		idx = 0;
		v0 = getkeycode(1);
		if (v0) {
			// Check for keys 1-8
			if (K_1 <= v0 && v0 <= K_8) {
				v3 = menus[idx2].values[v0 - 0x31];
				v00 = 7 - (v0 - 0x31);
				v0 = menus[idx2].ret_mode;
				if (v000 = v00) {
					do v0 >>= 1;
					while (--v000);
				}
				v000 = v0 & 1;
				v0 = menus[idx2].char_mode;
				if (v00) do v0 >>= 1; while (--v00);
				v0000 = v0 & 1;
				if (!v3) { /* jump to below */ }
				else if (v000 == 0) idx = v3;
				else {
					*val = v3;
					if (v0000 == 0) return 3;
					else return 2;
				}
			} else if (v0 == K_AC) {
				last_key_keycode = 0;
				// Handles the Disp indicator (AC pressed)
				set_disp_indicator(1);
				delay(1904);
				set_disp_indicator(0);
				return 0;
			} else if (menus[idx2].keycode == v0) {
				if (!v4 && screen_state == 1) { /* jump to below */ }
				else {
					if (menus[idx2].keycode) {
						*val = v0;
						last_key_keycode = 0;
						return 1;
					}
				}
			} else if (v0 == K_UP) idx = menus[idx2].prev_page;
			else if (v0 == K_DOWN) idx = menus[idx2].next_page;
			else if (v0 == K_LEFT) {
				idx = menus[idx2].parent;
				if (idx) head = NULL;
			}
			if (idx & (1 << 7)) {
				*val = (char)idx;
				return 1;
			}
		}
		if (!idx) {
			// Handles the Disp indicator (invalid key pressed)
			set_disp_indicator(1);
			delay(1904);
			set_disp_indicator(0);
		} else continue;
	}
}

// FUNCTION: GY454XE  Re 0A010
// FUNCTION: GY455XE  Im 0A934
// FUNCTION: GY460XF  Im 0A2DA
char display_token_menu(char m, char menu_idx) {
	char loc_m1;

	if (m != NULL && mode != m) return 0;
	else {
		loc_m1 = menu_idx;
		if (display_menu(&loc_m1, NULL) == 2) {
			last_key_keycode = loc_m1;
			return 1;
		} else return 0;
	}
}

// FUNCTION: GY454XE  Re 0A050
// FUNCTION: GY455XE  Im 0A974
// FUNCTION: GY460XF  Im 0A31A
char table_range_handler(char a) {
	char v0;

	v0 = 1;
	if (is_format_keycode(last_key_keycode)) f_09962(0);
	else {
		if ((last_key_keycode == K_APPROX || last_key_keycode == K_EXECUTE) && d_080FE == 4) {
			++d_080FD;
			f_044CE();
		}
		switch (d_080FD) {
			case 1: 
			case 2: 
			case 3:
				copy_input_prompt(d_080FD, 0, 1);
				break;
			case 4:
				set_keycode(K_APPROX);
				d_080FE = 1;
				v0 = 0;
				break;
		}
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0A0BC
// FUNCTION: GY455XE  Im 0A9E0
// FUNCTION: GY460XF  Im 0A386
void table_stat_handler(void) {
	char v0;
	char v1;
	char v2;
	char v3;
	char *loc_m18[3][3];
	char *loc_m20;

	memzero(loc_m18, 18);
	v0 = d_080DE;
	v1 = table_stat_get_num_cols();
	if (is_mov_keycode(last_key_keycode)) {
		f_046AE();
		if (table_stat_nav(0, last_key_keycode)) {
			f_046C4();
			goto j_0a242;
		}
	} else if (last_key_keycode == K_DEL) {
		if (mode != MODE_TABLE) {
			f_0AE14(table_viewport + table_y - 1);
			f_044CE();
			d_08126 = 0;
		} else goto j_0a242;
	}
	v2 = 0;
	for (v3 = 0; v3 <= v1 - 1; v3++) {
		char tmp;
		char tmp2;

		tmp = 0;
		for (tmp2 = table_viewport; tmp2 < table_viewport + 3; tmp2++) {
			table_stat_get_cell_addr(v2, tmp2, &loc_m20);
			loc_m18[tmp++][v3] = loc_m20;
		}
		if (submode == SMODE_STAT_1VAR) ++v2;
		++v2;
	}
	loc_m20 = loc_m18[table_y - 1][table_x - 1];
	set_result(loc_m20);
	if (is_format_keycode(last_key_keycode)) {
		if (loc_m20) f_09962(0);
		else if (mode != MODE_TABLE && last_key_keycode == K_DMS) {
			f_0B8B8(0);
			f_09962(0);
		}
		goto j_0a242;
	}
	buffer_clear();
	f_0336A(table_viewport);
	f_0A3B4();
	f_032A4(3, v1, table_y, table_x, loc_m18);
	table_stat_draw_cols(v1);
	font_size = 7;
	if (table_x <= v0) num_output_print(loc_m20);
	else num_output_print(NULL);
j_0a242:
	return;
}

// FUNCTION: GY454XE  Re 0A24E
// FUNCTION: GY455XE  Im 0AB72
// FUNCTION: GY460XF  Im 0A518
static char table_stat_nav(char a, char keycode) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = 1;
	v1 = table_stat_get_num_cols();
	v2 = d_080DE;
	if ((char)(80 - (v2 * v1 + d_080DF)) < v1) --v2;
	switch (keycode) {
		case K_UP:
			if (table_y == 1) {
				if (table_viewport >= 2) --table_viewport;
				else if (v2 < 3) table_y = v2 + 1;
				else {
					table_y = 3;
					table_viewport = v2 - 1;
				}
			} else if (table_y > 1) --table_y;
			else break;
			v0 = 0;
			break;
		case K_DOWN:
		case K_APPROX:
			if (table_viewport + table_y <= v2 + 1) {
				if (table_y == 3) {
					if (table_viewport <= v2 - 2) {
						++table_viewport;
						v0 = 0;
					}
				} else {
					++table_y;
					v0 = 0;
				}
			} else if (keycode != K_APPROX) {
				table_viewport = 1;
				table_y = 1;
				v0 = 0;
			}
			break;
		case K_RIGHT:
			if (table_x < v1) {
				++table_x;
				v0 = 0;
			}
			break;
		case K_LEFT:
			if (table_x > 1) {
				--table_x;
				v0 = 0;
			}
			break;
	}
	if (!v0) f_044CE();
	return v0;
}

// FUNCTION: GY454XE  Re 0A372
// FUNCTION: GY455XE  Im 0AC96
// FUNCTION: GY460XF  Im 0A63C
static void table_stat_draw_cols(char cols) {
	draw_line_vert(10, 1, 24, 0);
	draw_line_vert(38, 1, 24, 0);
	if (cols >= 2) draw_line_vert(66, 1, 24, 0);
	if (cols >= 3) draw_line_vert(94, 1, 24, 0);
	return;
}

// FUNCTION: GY454XE  Re 0A3B4
// FUNCTION: GY455XE  Im 0ACD8
// FUNCTION: GY460XF  Im 0A67E
static void f_0A3B4(void) {
	char v0;

	v0 = 12;
	font_size = 6;
	// STRING: GY454XE  Re 00928
	// STRING: GY455XE  Im 00928
	line_print(12, 1, s_table_x);
	if (mode == MODE_TABLE) line_print(40, 1, s_table_fx);
	else {
		if (submode != SMODE_STAT_1VAR) {
			char tmp = 40;
			v0 = 40;
			line_print(tmp, 1, s_table_y);
		}
		if (setup_stat_freq) {
			char tmp2;
			v0 += 28;
			tmp2 = v0;
			line_print(tmp2, 1, s_table_freq);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0A410
// FUNCTION: GY455XE  Im 0AD34
// FUNCTION: GY460XF  Im 0A6DA
void table_set_cur_cell(char *num) {
	char v0;
	char v1;

	f_044CE();
	switch (table_mode) {
		case TABLE_STAT_TABLE:
			v0 = table_viewport + table_y - 1;
			if (v0 > d_080DE) f_0AD08(v0);
			v1 = table_x - 1;
			if (submode == SMODE_STAT_1VAR && v1 == 1) v1 = 2;
			table_stat_set_cell(v1, v0, num);
			table_stat_nav(table_x - 1, K_APPROX);
			d_08126 = 0;
			break;
		case TABLE_MATRIX:
		case TABLE_VECTOR:
			table_matvct_set_cell(num, submode, table_y, table_x);
			table_matvct_nav(submode, K_APPROX);
			break;
		case TABLE_EQN:
		case TABLE_RATIO:
		case TABLE_INEQ:
			smart_num_cpy(unk_007e6[table_viewport * 9 + table_y * 3 + table_x - 13], num);
			table_eqn_nav(K_APPROX);
			d_080FD = 1;
			break;
		case TABLE_CALC:
		case TABLE_SOLVE:
			st_var(mode_ram[352 + mode_ram[351]], num);
			++mode_ram[351];
			return;
		case TABLE_RANGE:
			num_to_str_std_lineo(num);
			smart_num_cpy(table_param_ptrs[d_080FD - 1], num);
			++d_080FD;
			break;
	}
	d_080FE = 3;
	return;
}

// FUNCTION: GY454XE  Re 0A564
// FUNCTION: GY455XE  Im 0AE88
// FUNCTION: GY460XF  Im 0A82E
char f_0A564(void) {
	if (table_mode & (1 << 4) && d_080FE == 1) return 1;
	else return 0;
}

// FUNCTION: GY454XE  Re 0A57A
// FUNCTION: GY455XE  Im 0AE9E
// FUNCTION: GY460XF  Im 0A844
char f_0A57A(void) {
	if (table_mode & (1 << 4) && (d_080FE == 3 || d_080FE == 0)) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0A594
// FUNCTION: GY455XE  Im 0AEB8
// FUNCTION: GY460XF  Im 0A85E
char table_eqn_handler(char a) {
	char v0 = 1;

	arrow_state = 0;
	if ((last_key_keycode == K_APPROX || last_key_keycode == K_EXECUTE) && !d_080FD) {
		char tmp;
		if (tmp = f_13BEA()) set_keycode(show_error(tmp));
		else {
			table_mode = TABLE_NONE;
			d_080FD = 2;
			v0 = 0;
		}
	} else {
		d_080FD = 0;
		table_eqn_key_handler();
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0A5E4
// FUNCTION: GY455XE  Im 0AF08
// FUNCTION: GY460XF  Im 0A8AE
static void table_eqn_key_handler(void) {
	char v0;
	int v1;
	char *v2;

	v0 = submode - 1;
	if (is_mov_keycode(last_key_keycode)) {
		f_046AE();
		if (table_eqn_nav(last_key_keycode)) {
			f_046C4();
			goto j_0a6d4;
		}
	} else if (last_key_keycode == K_AC) {
		f_044B6();
		if (mode == MODE_INEQ) table_ineq_setup();
		else if (mode == MODE_RATIO) table_ratio_setup();
		else table_eqn_setup();
	}
	v1 = table_viewport * 18;
	v1 += table_y * 6;
	v1 += table_x * 2;
	v2 = unk_007e6[v1 - 26];
	set_result(v2);
	if (is_format_keycode(last_key_keycode)) {
		if (v2) f_09962(0);
	} else {
		buffer_clear();
		table_eqn_draw_text();
		table_eqn_draw_lines();
		f_032A4(unk_0080a[v0].b.m, unk_0080a[v0].b.n, table_y, table_x, &unk_007e6[(table_viewport - 1) * 18]);
		font_size = 7;
#if ENABLE_RATIO == 1
		if (mode == MODE_RATIO) line_print(54, 13, ratio_optn[ratio_mode - 1]);
#endif
#if ENABLE_INEQ == 1
		if (mode == MODE_INEQ) {
			int y;
			char *string;
			int i;
			if (submode == SMODE_EQN_POLY2) {
				y = 36;
				string = menu_ineq_poly2;
			} else {
				y = 13;
				string = menu_ineq_poly3;
			}
			for (i = 0; i < ineq_mode; i++) string += strlen(string) + 1;
			line_print(y, 13, string + 2);
		}
#endif
		num_output_print(v2);
	}
j_0a6d4:
	return;
}

// FUNCTION: GY454XE  Re 0A6DA
// FUNCTION: GY455XE  Im 0AFFE
// FUNCTION: GY460XF  Im 0AA0E
static void table_eqn_draw_text(void) {
	char loc_m6[6];	

	strcpy(loc_m6, s_table_a);
	loc_m6[2] += table_viewport - 1;
	font_size = 6;
	line_print(12, 1, loc_m6);
	++loc_m6[2];
	line_print(40, 1, loc_m6);
	++loc_m6[2];
#if ENABLE_RATIO == 1
	if (mode == MODE_RATIO && ratio_mode == 1) ++loc_m6[2];
#endif
	line_print(68, 1, loc_m6);
	if (submode <= SMODE_EQN_SIMUL3) {
		strcpy(loc_m6, s_table_1);
		line_print(5, 7, loc_m6);
		++loc_m6[0];
		line_print(5, 13, loc_m6);
		if (submode == SMODE_EQN_SIMUL3) {
			++loc_m6[0];
			line_print(5, 19, loc_m6);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0A782
// FUNCTION: GY455XE  Im 0B0A6
// FUNCTION: GY460XF  Im 0AACC
static void table_eqn_draw_lines(void) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = table_eqn_linel_params[submode - 1][0];
	v1 = table_eqn_linel_params[submode - 1][1];
	v2 = table_eqn_linel_params[submode - 1][2];
	v3 = table_eqn_linel_params[submode - 1][3];
	if (table_viewport != 1) v3 = 0;
	draw_line_vert(v0, v1, v2, v3);
	v0 = table_eqn_liner_params[submode - 1][0];
	v1 = table_eqn_liner_params[submode - 1][1];
	v2 = table_eqn_liner_params[submode - 1][2];
	v3 = table_eqn_liner_params[submode - 1][3];
	if (table_viewport != 1 || (submode != SMODE_EQN_SIMUL3 && submode != SMODE_EQN_POLY3)) draw_line_vert(v0, v1, v2, v3);
	return;
}

// FUNCTION: GY454XE  Re 0A7FA
// FUNCTION: GY455XE  Im 0B11E
// FUNCTION: GY460XF  Im 0AB44
static char table_eqn_nav(char keycode) {
	char v0;
	dim loc_m2;
	dim loc_m4;

	v0 = 1;
	loc_m2.mn = unk_0080a[submode - 1].mn;
	loc_m4.mn = unk_00812[submode - 1].mn;
	switch (keycode) {
		case K_UP:
			if (table_y > 1) {
				--table_y;
				v0 = 0;
			}
			break;
		case K_DOWN:
			if (table_y < loc_m4.b.m) {
				++table_y;
				v0 = 0;
			}
			break;
		case K_RIGHT:
		case K_APPROX:
			if (table_x == 3) {
				if (table_viewport == 1) {
					if (table_x + table_viewport < loc_m4.b.n + 1) ++table_viewport;
					else if (table_y < loc_m4.b.m) {
						++table_y;
						table_x = 1;
					}
					v0 = 0;
				} else if (table_y < loc_m4.b.m) {
					++table_y;
					table_viewport = 1;
					table_x = 1;
				}
			} else if (table_x < loc_m4.b.n) {
				++table_x;
				v0 = 0;
			}
			break;
		case K_LEFT:
			if (table_x == 1) {
				if (table_viewport == 2) {
					--table_viewport;
					v0 = 0;
				} else if (table_y != 1) {
					--table_y;
					v0 = 0;
					table_x = loc_m2.b.n;
					if (loc_m4.b.n > 3) table_viewport = 2;
				}
			} else {
				--table_x;
				v0 = 0;
			}
			break;
	}
	if (!v0) f_044CE();
	return v0;
}

// FUNCTION: GY454XE  Re 0A936
// FUNCTION: GY455XE  Im 0B25A
// FUNCTION: GY460XF  Im 0AC80
char table_vct_handler(char a) {
	return table_mat_handler(a);
}

// FUNCTION: GY454XE  Re 0A93A
// FUNCTION: GY455XE  Im 0B25E
// FUNCTION: GY460XF  Im 0AC84
char table_mat_handler(char a) {
	char v0;

	if (!matvct_dims[submode].b.m || !matvct_dims[submode].b.n) {
		screen_state = 0;
		table_mode = TABLE_NONE;
		f_0B8B8(2);
		v0 = 0;
	} else v0 = table_matvct_handler(submode);
	return v0;
}

// FUNCTION: GY454XE  Re 0A978
// FUNCTION: GY455XE  Im 0B29C
// FUNCTION: GY460XF  Im 0ACC2
static char table_matvct_handler(char sm) {
	char v0;
	char *v1;

	if (is_sto_abc_keycode(last_key_keycode)) {
		v0 = last_key_keycode - K_STO_A;
		if (v0 != sm) f_1547E(v0, sm);
		sm = v0;
		submode = sm;
		f_0AF0A();
	} else if (is_mov_keycode(last_key_keycode)) {
		f_046AE();
		if (table_matvct_nav(sm, last_key_keycode)) {
			f_046C4();
			goto j_0aa46;
		}
	}
	v1 = unk_0078a[sm * 9 + table_y * 3 + table_x - 4];
	set_result(v1);
	if (is_format_keycode(last_key_keycode)) {
		if (v1) f_09962(0);
	} else {
		dim *tmp = &matvct_dims[sm];
		buffer_clear();
		matvct_string_print(sm);
		table_matvct_draw_cols(tmp->b.m, tmp->b.n);
		f_032A4(tmp->b.m, tmp->b.n, table_y, table_x, &unk_0078a[sm * 9]);
		font_size = 7;
		num_output_print(v1);
	}
j_0aa46:
	return 1;
}

// FUNCTION: GY454XE  Re 0AA7C
// FUNCTION: GY455XE  Im 0B3A0
// FUNCTION: GY460XF  Im 0ADC6
static char table_matvct_nav(char sm, char keycode) {
	char v0;
	char tmp;

	v0 = 1;
	switch (keycode) {
		case K_UP:
			if (table_y > 1) {
				--table_y;
				v0 = 0;
			}
			break;
		case K_DOWN:
			if (table_y < matvct_dims[sm].b.m) {
				++table_y;
				v0 = 0;
			}
			break;
		case K_RIGHT:
		case K_APPROX:
			if (table_x < matvct_dims[sm].b.n) tmp = table_x + 1;
			else if (table_y < matvct_dims[sm].b.m) {
				++table_y;
				tmp = 1;
			} else break;
			table_x = tmp;
			v0 = 0;
			break;
		case K_LEFT:
			if (table_x > 1) tmp = table_x - 1;
			else if (table_y > 1) {
				--table_y;
				tmp = matvct_dims[sm].b.n;
			} else break;
			table_x = tmp;
			v0 = 0;
			break;
	}
	if (!v0) f_044CE();
	return v0;
}

// FUNCTION: GY454XE  Re 0AB44
// FUNCTION: GY455XE  Im 0B468
// FUNCTION: GY460XF  Im 0AE8E
static void matvct_string_print(char sm) {
	font_size = 6;
	line_print(1, 1, matvct_strings[sm]);
	return;
}

// FUNCTION: GY454XE  Re 0AB6A
// FUNCTION: GY455XE  Im 0B48E
// FUNCTION: GY460XF  Im 0AEB4
static void table_matvct_draw_cols(char m, char n) {
	char v0;
	char v1;

	v0 = n * 28 + 10;
	v1 = m * 6 + 6;
	draw_line_vert(10, 7, v1, 0xc0);
	draw_line_vert(v0, 7, v1, 0x30);
	return;
}

#if ENABLE_INEQ == 1
// FUNCTION: GY460XF  Im 0AEF2
char table_ineq_handler(char a) {
	char v0 = 1;

	arrow_state = 0;
	if ((last_key_keycode == K_APPROX || last_key_keycode == K_EXECUTE) && !d_080FD) {
		char tmp = f_131EC();
		if (tmp) set_keycode(show_error(tmp));
		else {
			table_mode = TABLE_NONE;
			d_080FD = 2;
			v0 = 0;
		}
	} else {
		d_080FD = 0;
		table_eqn_key_handler();
	}
	return v0;
}
#endif

#if ENABLE_RATIO == 1
// FUNCTION: GY460XF  Im 0AF42
char table_ratio_handler(char a) {
	char v0 = 1;

	arrow_state = 0;
	if ((last_key_keycode == K_APPROX || last_key_keycode == K_EXECUTE) && !d_080FD) {
		char tmp = f_131E8();
		if (tmp) set_keycode(show_error(tmp));
		else {
			table_mode = TABLE_NONE;
			d_080FD = 2;
			v0 = 0;
		}
	} else {
		d_080FD = 0;
		table_eqn_key_handler();
	}
	return v0;
}
#endif
