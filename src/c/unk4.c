#include <string.h>
#include "consts.h"
#include "generals.h"
#include "input.h"
#include "unk2.h"
#include "unk3.h"
#include "unk4.h"
#include "unk5.h"

// Weird struct used in f_09962 and the functions that it calls from a jump table.
typedef struct {
	char **input_area_ptr;
	char **result_ptr;
	char unk_0x04;
	char unk_0x05;
	char mode;
	char unk_0x07;
	char unk_0x08;
	char unk_0x09;
	char unk_0x0a;
	char unk_0x0b;
} f_09962_struct;

// Only getscancode and its neighboring functions seem to use this. There may be more functions found to be using
// this struct type as the decomp goes on.
typedef struct {
	char cursor_on[20];
	char cursor_off[20];
	char cursor_toggle;
	char cursor_enable_flash;
	char *screen_addr;
	char unk_0x2c;
	char unk_0x2d;
	int shutdown_timer;
} getscancode_struct;

// Struct used by the menu list.
typedef struct {
	const char *string;	// String printed when menu is displayed.
	char ret_mode;		// Return value in values if the corresponding bit (MSB->LSB = 0->7) is set; otherwise use it as index
	char char_mode;		// Set bits (MSB->LSB = 0->7) denote tokens. display_menu returns 2 
	char values[8];		// Values corresponding to the (up to) 8 choices.
	char prev_page;		// Index of menu to go to when pressing up. NULL = none.
	char next_page;		// Index of menu to go to when pressing down. NULL = none.
	char parent;		// Index of menu to go to when pressing left.
	char keycode;		// Keycode associated with this menu.
} menu;

// Static declarations (placed first so we can define jump table)
// 0995E
static char keyfunc_nop(f_09962_struct *a);
// 0B226
static void f_0B226(getscancode_struct *a);
// 0B370
static void copy_cursor_from_scr(char *cursor, char *scr);
// 0B3B2
static void copy_cursor_to_scr(char *scr, char *cursor);
// 0B67E
static void f_0B67E(void);
// 0B782
static char is_rcl_keycode(char keycode);
// 0B79C
static char is_sto_keycode(char keycode);
// 0B7E6
static char show_error(char idx);
// 0B984
static char conv_func_token(char keycode);
// 0B9C8
static char f_0B9C8(f_09962_struct *a);
// 0BA28
static void f_0BA28(f_09962_struct *a);
// 0BB42
static char f_0BB42(char *a);
// 0BEEE
static char f_0BEEE(f_09962_struct *a);
// 0BF8A
static char f_0BF8A(void);
// 0BFDC
static char f_0BFDC(f_09962_struct *a);
// 0C08C
static void f_0C08C(f_09962_struct *a);
// 0C148
static char f_0C148(f_09962_struct *a);
// 0C1A0
static char f_0C1A0(f_09962_struct *a);
// 0C1D4
static char f_0C1D4(f_09962_struct *a);
// 0C22E
static char keyfunc_ac(f_09962_struct *a);
// 0C25A
static char keyfunc_sto_rcl(f_09962_struct *a);
// 0C316
static char keyfunc_rcl(f_09962_struct *a);
// 0C31A
static char keyfunc_sto(f_09962_struct *a);
// 0C31E
static char keyfunc_exe(f_09962_struct *a);
// 0C64A
static char keyfunc_abc(f_09962_struct *a);
// 0C692
static char keyfunc_dc(f_09962_struct *a);
// 0C6DE
static char keyfunc_dms(f_09962_struct *a);
// 0C728
static char keyfunc_fact(f_09962_struct *a);
// 0C72C
static void f_0C72C(f_09962_struct *a);
// 0C74C
static char keyfunc_eng(f_09962_struct *a);
// 0C77A
static char keyfunc_eng_r(f_09962_struct *a);
// 0C7A8
static char keyfunc_del(f_09962_struct *a);
// 0C7C2
static char keyfunc_mov_x(f_09962_struct *a);
// 0C806
static char keyfunc_mov_y(f_09962_struct *a);
// 0C836
static char keyfunc_base(f_09962_struct *a);
// 0C86A
static char keyfunc_mode(f_09962_struct *a);
// 0C874
static char keyfunc_setup(f_09962_struct *a);

// 01FB6
static char (* const keyfuncs[])(f_09962_struct *) = {
	keyfunc_mov_y,	// K_UP
	keyfunc_mov_y,	// K_DOWN
	keyfunc_mov_x,	// K_RIGHT
	keyfunc_mov_x,	// K_LEFT
	keyfunc_mode,	// K_MODE
	keyfunc_setup,	// K_SETUP
	keyfunc_ac,		// K_AC
	keyfunc_nop,	// K_OFF
	keyfunc_nop,	// K_ALPHA
	keyfunc_nop,	// K_SHIFT
	keyfunc_rcl,	// K_RCL
	keyfunc_sto,	// K_STO
	keyfunc_nop,	// K_INS
	keyfunc_exe,	// K_APPROX
	keyfunc_sto,	// K_M_PLUS
	keyfunc_sto,	// K_M_MINUS
	keyfunc_exe,	// K_EXECUTE
	keyfunc_fact,	// K_FACT
	keyfunc_base,	// K_BASE_BIN
	keyfunc_base,	// K_BASE_OCT
	keyfunc_base,	// K_BASE_DEC
	keyfunc_base,	// K_BASE_HEX
	keyfunc_dms,	// K_DMS
	keyfunc_dms,	// K_DMS_R
	keyfunc_eng,	// K_ENG
	keyfunc_eng_r,	// K_ENG_R
	keyfunc_abc,	// K_FMT_DEC
	keyfunc_dc,		// K_FMT_FRAC
	keyfunc_nop,	// K_CALC
	keyfunc_nop,	// K_SOLVE
	keyfunc_del		// K_DEL
};

// 02044
const char tokens_map[16] = {
	0xAE,	// K_FRAC
	0x7C,	// K_FRAC_ABC
	0x5E,	// K_POW
	0x98,	// K_SQRT
	0x77,	// K_POW_M1
	0x75,	// K_POW_2
	0x76,	// K_POW_3
	0xA8,	// K_CBRT
	0x9F,	// K_NTH_RT
	0x73,	// K_E_POW
	0x93,	// K_10_POW
	0x68,	// K_LOGAB
	0x63,	// K_ABS
	0x6A,	// K_INTEGRAL
	0x6B,	// K_DDX
	0x69	// K_SUM
};

// 02054
const char s_colon[] = ":";

// For keycode lists, every row is KI(1-8), every column is KO(1-8)

// 02056
const char keycodes[64] = {
	K_1,		K_2,		K_3,		K_PLUS,		K_MINUS,	NULL,		K_EXECUTE,	NULL,
	K_4,		K_5,		K_6,		K_MUL,		K_DIV,		NULL,		K_ANS,		NULL,
	K_7,		K_8,		K_9,		K_DEL,		K_AC,		NULL,		K_EXP,		NULL,
	K_RCL,		K_ENG,		K_PAREN_L,	K_PAREN_R,	K_FMT_DEC,	K_M_PLUS,	K_PERIOD,	NULL,
	K_NEGATIVE,	K_DMS,		K_HYP,		K_SIN,		K_COS,		K_TAN,		K_0,		NULL,
	K_FRAC,		K_SQRT,		K_POW_2,	K_POW,		K_LOG,		K_LN,		NULL,		NULL,
	K_CALC,		K_INTEGRAL,	K_LEFT,		K_DOWN,		K_POW_M1,	K_LOGAB,	NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		NULL,		NULL
};

// 02096
const char keycodes_shift[64] = {
	K_SD,		K_CMPLX,	K_BASE,		K_POL,		K_REC,		NULL,		K_APPROX,	NULL,
	K_MATRIX,	K_VECTOR,	NULL,		K_PERMU,	K_COMBI,	NULL,		K_DRG,		NULL,
	K_CONST,	K_CONV,		K_CLR,		K_INS,		K_OFF,		NULL,		K_PI,		NULL,
	K_STO,		K_ENG_R,	K_PERCENT,	K_COMMA,	K_FMT_FRAC,	K_M_MINUS,	K_RAN,		NULL,
	K_NEGATIVE,	K_DMS_R,	K_ABS,		K_ARCSIN,	K_ARCCOS,	K_ARCTAN,	K_RND,		NULL,
	K_FRAC_ABC,	K_CBRT,		K_POW_3,	K_NTH_RT,	K_10_POW,	K_E_POW,	NULL,		NULL,
	K_SOLVE,	K_DDX,		K_LEFT,		K_DOWN,		K_FCTRIAL,	K_SUM,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_SETUP,	NULL,		NULL,		NULL
};

// 020D6
const char keycodes_alpha[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		K_EULER,	NULL,
	K_RCL,		NULL,		NULL,		K_VAR_X,	K_VAR_Y,	K_VAR_M,	K_RANINT,	NULL,
	K_VAR_A,	K_VAR_B,	K_VAR_C,	K_VAR_D,	K_VAR_E,	K_VAR_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	K_EQUALS,	K_COLON,	K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		K_COLON,	K_EQUALS,	NULL
};

// 02116
const char keycodes_rcl[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		NULL,		K_RCL_X,	K_RCL_Y,	K_RCL_M,	NULL,		NULL,
	K_RCL_A,	K_RCL_B,	K_RCL_C,	K_RCL_D,	K_RCL_E,	K_RCL_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		K_CALC,		NULL
};

// 02156
const char keycodes_sto[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		NULL,		K_STO_X,	K_STO_Y,	K_STO_M,	NULL,		NULL,
	K_STO_A,	K_STO_B,	K_STO_C,	K_STO_D,	K_STO_E,	K_STO_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		K_CALC,		NULL
};

// 02196
const char keycodes_base_n[64] = {
	K_1,		K_2,		K_3,		K_PLUS,		K_MINUS,	NULL,		K_APPROX,	K_APPROX,
	K_4,		K_5,		K_6,		K_MUL,		K_DIV,		NULL,		K_ANS,		NULL,
	K_7,		K_8,		K_9,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		K_PAREN_L,	K_PAREN_R,	NULL,		K_M_PLUS,	NULL,		NULL,
	K_HEX_A,	K_HEX_B,	K_HEX_C,	K_HEX_D,	K_HEX_E,	K_HEX_F,	K_0,		NULL,
	NULL,		NULL,		K_BASE_DEC,	K_BASE_HEX,	K_BASE_BIN,	K_BASE_OCT,	NULL,		NULL,
	K_CALC,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		NULL,		NULL
};

// 021D6
const char keycodes_shift_base_n[64] = {
	NULL,		NULL,		K_BASE,		NULL,		NULL,		NULL,		K_APPROX,	NULL,
	NULL,		K_VECTOR,	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_CLR,		K_INS,		K_OFF,		NULL,		NULL,		NULL,
	K_STO,		NULL,		NULL,		NULL,		NULL,		K_M_MINUS,	NULL,		NULL,
	K_HEX_A,	K_HEX_B,	K_HEX_C,	K_HEX_D,	K_HEX_E,	K_HEX_F,	NULL,		NULL,
	NULL,		NULL,		K_BASE_DEC,	K_BASE_HEX,	K_BASE_BIN,	K_BASE_OCT,	NULL,		NULL,
	K_SOLVE,	NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_SETUP,	NULL,		NULL,		NULL
};

// 02216
const char menu_mode[] = {
	"1:COMP  2:CMPLX \0"
	"3:STAT  4:BASE-N\0"
	"5:EQN   6:MATRIX\0"
	"7:TABLE 8:VECTOR"
};

// 0225A
const char menu_setup_0[] = {
	"1:MthIO 2:LineIO\0"
	"3:Deg   4:Rad   \0"
	"5:Gra   6:Fix   \0"
	"7:Sci   8:Norm  \0"
};

// 0229F
const char menu_setup_1[] = {
	"1:ab/c  2:d/c   \0"
	"3:CMPLX 4:STAT  \0"
	"5:Disp  6:" "\x9f" "CONT" "\x9e" "\0"
	"\0"
};

// 022D4
const char menu_stat_type[] = {
	"1:1-VAR 2:" "\x9a" "+" "\x9b" "X\0"
	"3:" "\x5f" "+" "\x9c" "X" "\xa2" " 4:ln X\0"
	"5:" "\x81" "^X   6:" "\x9a" "*" "\x9b" "^X\0"
	"7:" "\x9a" "*X^" "\x9b" " 8:1/X"
};

// 02310
const char menu_stat_table[] = {
	"1:Type  2:Data\0"
	"3:Edit\0"
	"\0"
	"\0"
};

// 02329
const char menu_stat_1var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Distr 6:MinMax\0"
	"\0"
};

// 02359
const char menu_stat_2var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Reg   6:MinMax\0"
	"\0"
};

// 02389
const char menu_eqn[] = {
	"1:a" "\x9d" "X+b" "\x9d" "Y=c" "\x9d" "\0"
	"2:a" "\x9d" "X+b" "\x9d" "Y+c" "\x9d" "Z=d" "\x9d" "\0"
	"3:aX" "\xa2" "+bX+c=0\0"
	"4:aX" "\xa3" "+bX" "\xa2" "+cX+d=0\0"
};

// 023C6
const menu menus[] = {
// n		 String						Return	Char	Option values/Menu indexes/Tokens (up to 8)			Up		Down	Left	Keycode
// Placeholder
/* 0  */	{NULL,						0xff,	0,		{0},												NULL,	NULL,	NULL,	NULL},

// CLR menu (SHIFT 9)
/* 1  */	{menu_clr,					0xff,	0,		{1, 2, 3},											NULL,	NULL,	NULL,	K_CLR},

// SETUP menu (SHIFT MODE)
/* 2  */	{menu_setup_0,				0x7f,	0,		{4, 2, 3, 4, 5, 6, 7, 8},							NULL,	3,		NULL,	K_SETUP},
/* 3  */	{menu_setup_1,				0xc7,	0,		{9, 10, 5, 6, 7, 11},								2,		NULL,	NULL,	K_SETUP},
/* 4  */	{menu_setup_decimalo,		0xff,	0,		{22, 23},											NULL,	NULL,	2,		K_SETUP},
/* 5  */	{menu_setup_cmplx_result,	0xff,	0,		{12, 13},											NULL,	NULL,	3,		K_SETUP},
/* 6  */	{menu_setup_stat_freq,		0xff,	0,		{14, 15},											NULL,	NULL,	3,		K_SETUP},
/* 7  */	{menu_setup_decimal_mark,	0xff,	0,		{18, 19},											NULL,	NULL,	3,		K_SETUP},

// MATRIX mode (SHIFT 4)
/* 8  */	{menu_matrix_table,			0xff,	0,		{1, 2},												NULL,	NULL,	NULL,	K_MATRIX},
/* 9  */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 10 */	{menu_matrix_data,			0x1f,	0,		{6, 7, 8},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 11 */	{menu_matrix_dim0,			0xff,	0,		{9, 10, 11, 12, 13, 14},							NULL,	12,		0xff,	K_MATRIX},
/* 12 */	{menu_matrix_dim1,			0xff,	0,		{15, 16, 17},										11,		NULL,	0xff,	K_MATRIX},
/* 13 */	{menu_matrix,				0xff,	0x3f,	{1, 2, 0xc8, 0xc9, 0xca, 0xcb, 0xc0, 0xc1},			NULL,	NULL,	NULL,	K_MATRIX},

// VECTOR mode (SHIFT 5)
/* 14 */	{menu_vector_table,			0xff,	0,		{18, 19},											NULL,	NULL,	NULL,	K_VECTOR},
/* 15 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 16 */	{menu_vector_data,			0x1f,	0,		{23, 24, 25},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 17 */	{menu_vector_dim,			0xff,	0,		{26, 27},											NULL,	NULL,	0xfc,	K_VECTOR},
/* 18 */	{menu_vector,				0xff,	0x3f,	{18, 19, 0xcc, 0xcd, 0xce, 0xcf, 0x9e},				NULL,	NULL,	NULL,	K_VECTOR},

// CMPLX mode (SHIFT 2)
/* 19 */	{menu_cmplx,				0xff,	0xf0,	{0xc3, 0x88, 0x56, 0x55},							NULL,	NULL,	NULL,	K_CMPLX},

// BASE-N mode (SHIFT 3)
/* 20 */	{menu_base_n_0,				0xff,	0xfc,	{0x6e, 0x6f, 0x7e, 0x7f, 0xc1, 0x62},				NULL,	21,		NULL,	K_BASE},
/* 21 */	{menu_base_n_1,				0xff,	0xfc,	{0x51, 0x50, 0x53, 0x52},							20,		NULL,	NULL,	K_BASE},

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

// MATRIX mode (submode select)
/* 39 */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	NULL,	NULL},

// VECTOR mode (submode select)
/* 40 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	NULL,	NULL},


// STAT mode (submode select)
/* 41 */	{menu_stat_type,			0xff,	0,		{1, 2, 3, 4, 5, 6, 7, 8},							NULL,	NULL,	NULL,	NULL},

// MENU mode
/* 42 */	{menu_mode,					0xff,	0,		{MODE_COMP, MODE_CMPLX, MODE_STAT, MODE_BASE_N,
														MODE_EQN, MODE_MATRIX, MODE_TABLE, MODE_VECTOR},	NULL,	NULL,	NULL,	K_MODE}, 
};

// 0995E
static char keyfunc_nop(f_09962_struct *a) {
	return 0;
}

// 09962
char f_09962(char a) {
	f_09962_struct v0;
	char v1;
	char v2;

	f_0B67E();
	font_size = 10;
	if (table_mode & (1 << 4)) font_size = 7;

	memzero(&v0, 12);
	v0.unk_0x04 = 1;
	v0.input_area_ptr = &input_area;
	v0.result_ptr = &result;
	v0.mode = mode;
	v0.unk_0x07 = f_0A564();
	v0.unk_0x08 = is_mathi();
	v0.unk_0x09 = is_matho();
	v0.unk_0x0a = f_02CB6();
	if (!(d_080FE & 0x60)) d_08125 = 0;
	if (!a) {
		if (!is_char_keycode(last_key_keycode)) {
			v1 = last_key_keycode + 0x20;
			if (is_rcl_keycode(last_key_keycode)) v1 = K_RCL + 0x20;
			else if (is_sto_keycode(last_key_keycode)) v1 = K_STO + 0x20; 
			else if (last_key_keycode < 0xe0) goto j_09b9c;
			switch (keyfuncs[v1](&v0)) {
				case 0: goto j_09b9c;
				case 1: goto j_09b6c;
				case 2: goto j_09b72;
				case 3: goto j_09b76;
			}
		} else if (d_080F7 && 0xcf <= last_key_keycode) {
			f_044E2(conv_func_token(last_key_keycode));
			if (v0.unk_0x08) v0.unk_0x05 = 1;
		} else if (last_key_keycode == 0x7c) last_key_keycode = 0;
		if (!(d_080FE & (1 << 6))) {
			if (table_mode & (1 << 7) && d_080FD == 2 && (last_key_keycode == K_COLON || last_key_keycode == 0xa4)) last_key_keycode = 0;
			else if (table_mode & (1 << 7) && d_080FD == 2 && last_key_keycode == K_EQUALS) last_key_keycode = 0;
			else if (table_mode & (1 << 4)) last_key_keycode = filter_chars(last_key_keycode);
			else if (v0.mode == MODE_STAT || v0.mode == MODE_MATRIX || v0.mode == MODE_VECTOR) last_key_keycode = filter_chars_stat_mat_vct(last_key_keycode);
			else if (v0.mode == MODE_TABLE) last_key_keycode = filter_chars_table(last_key_keycode);
			else if (v0.mode == MODE_CMPLX) last_key_keycode = filter_chars_cmplx(last_key_keycode);
			else if (v0.mode == MODE_VERIF) last_key_keycode = f_03714(last_key_keycode);
			if (last_key_keycode == K_EQUALS && v0.mode != MODE_COMP && v0.mode != MODE_CMPLX && v0.mode != MODE_VERIF) last_key_keycode = 0;
			if (last_key_keycode == 0xa4 && !setup_mathi) last_key_keycode = 0;
			if (last_key_keycode) {
				if (is_char_keycode(last_key_keycode)) f_0B736();
				if (d_080FE == 1) {
					f_07470(last_key_keycode, v0.unk_0x05);
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
		if (f_0C1A0(&v0) != 1 && f_0C1D4(&v0) != 1) f_07B60(*v0.result_ptr);
		else goto j_09b6c;
	}
j_09b9c:
	if (!(d_080FE & (1 << 5)) && f_0BC34() > 0) d_08125 = 1;
	return v0.unk_0x04;
}

// 09BDC - STUB
char f_09BDC(char a) {
	return 0;
}

// 09D54
char f_09D54(char tmode) {
	if (table_mode & (1 << 7)) {
		smart_strcpy(input_area, cache_area);
	}
	table_mode = tmode;
	d_080FD = 1;
	d_080FE = 4;
	f_044CE();
}

// 09D84
char f_09D84(void) {
	char v0 = 0;

	if (d_080FE & (1 << 6)) v0 = 1;
	if (table_mode & (1 << 7) && d_080FD == 2) v0 = 1;
	if (!smart_strlen(input_area)) v0 = 1;
	return v0;
}

// 09DB6 - STUB
void f_09DB6(void) {
	return;
}

// 09E70
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
			if (0x31 <= v0 && v0 <= 0x38) {
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
				delay(0x770);
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
				idx = menus[idx2].next_page;
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
			delay(0x770);
			set_disp_indicator(0);
		} else continue;
	}
}

// 0A010
char f_0A010(char m, char b) {
	char loc_m1;

	if (m != NULL && mode != m) return 0;
	else {
		loc_m1 = b;
		if (display_menu(&loc_m1, NULL) == 2) {
			last_key_keycode = loc_m1;
			return 1;
		} else return 0;
	}
}

// 0A050 - STUB
void f_0A050(char a) {
	return;
}

// 0A0BC - STUB
void f_0A0BC(void) {
	return;
}

// 0A24E - STUB
void f_0A24E(void) {
	return;
}

// 0A372 - STUB
void f_0A372(void) {
	return;
}

// 0A3B4 - STUB
void f_0A3B4(void) {
	return;
}

// 0A410 - STUB
void f_0A410(void) {
	return;
}

// 0A564
char f_0A564(void) {
	if (table_mode & (1 << 4) && d_080FE == 1) return 1;
	else return 0;
}

// 0A57A - STUB
void f_0A57A(void) {
	return;
}

// 0A594 - STUB
void f_0A594(char a) {
	return;
}

// 0A5E4 - STUB
void f_0A5E4(void) {
	return;
}

// 0A6DA - STUB
void f_0A6DA(void) {
	return;
}

// 0A782 - STUB
void f_0A782(void) {
	return;
}

// 0A7FA - STUB
void f_0A7FA(void) {
	return;
}

// 0A936 - STUB
void f_0A936(char a) {
	return;
}

// 0A93A - STUB
void f_0A93A(char a) {
	return;
}

// 0A978 - STUB
void f_0A978(void) {
	return;
}

// 0AA7C - STUB
void f_0AA7C(void) {
	return;
}

// 0AB44 - STUB
void f_0AB44(void) {
	return;
}

// 0AB6A - STUB
void f_0AB6A(void) {
	return;
}

// 0ABA8 - STUB
char *f_0ABA8(char a, char b, char c) {
	char *v0;

	d_080E0[a][0] = b;
	d_080E0[a][1] = c;
	v0 = f_043AC(a, 1, 1);
	memzero(v0, 0x5a);
	return v0;
}

// 0ABDE - STUB
void f_0ABDE(void) {
	return;
}

// 0ABFC - STUB
void f_0ABFC(void) {
	return;
}

// 0AC28 - STUB
void f_0AC28(void) {
	return;
}

// 0AC36 - STUB
void f_0AC36(void) {
	return;
}

// 0AC44 - STUB
void f_0AC44(void) {
	return;
}

// 0AD08 - STUB
void f_0AD08(void) {
	return;
}

// 0AE14 - STUB
void f_0AE14(void) {
	return;
}

// 0AECE
void setup_stat(void) {
	if (mode == MODE_STAT) {
		d_080DE = 0;
		d_080DF = 0;
		memzero(&mode_ram[8], 800);
		d_08126 = 0;
	}
	return;
}

// 0AEFA
void table_home(void) {
	table_viewport = 1;
	table_x = 1;
	table_y = 1;
	return;
}

// 0AF0A
void f_0AF0A(void) {
	table_home();
	f_044B6();
	return;
}

// 0AF16 - STUB
void f_0AF16(void) {
	table_mode = 1;
	d_080FD = 0;
	d_080FE = 1;
	d_080FF = 0;
}

// 0AF30 - STUB
void f_0AF30(void) {
	return;
}

// 0AFB0
void reset_all(void) {
	memzero(&ram_start, 0xa17);
	setup_contrast = 0x11;
	set_contrast_sfr(0x11);
	use_rambuf = 1;
	d_080DC = pd_value();
	clear_setup();
	reset_magic_string();
	return;
}


// 0AFE0
void clear_setup(void) {
	clear_input_area();
	clear_cache_area();
	set_default_settings();
	mode = MODE_COMP;
	submode = 0;
	screen_state = 3;
	table_mode = 1;
	f_0B8B8(2);
}

// 0B00E
void clear_mem(void) {
	memzero(&vars_start, 100);
	if (mode == MODE_CMPLX) memzero(&mode_ram[362], 100);
	if (f_1B288(result) != 13) clear_result();
	return;
}

// 0B042
void clear_input_area(void) {
	memzero(input_area, 100);
}

// 0B04E
void clear_cache_area(void) {
	memzero(cache_area, 100);
}

// 0B05A
void f_0B05A(void) {
	char m = mode;
	
	d_08125 = 0;
	if (m == MODE_COMP || m == MODE_BASE_N || m == MODE_CMPLX || m == MODE_VERIF) memzero(mode_ram, 250);
	clear_cache_area();
	return;
}

// 0B08A - STUB
void f_0B08A(void) {
	return;
}

// 0B0C6
void getscancode(int *scancode) {
	char v0;				// R4
	int v1;					// -52
	int v2;					// -50
	getscancode_struct v3;	// -48

	v3.shutdown_timer = 0x4ab;
	v0 = 48;
	if (f_046D4()) v0 = 0xee;
	v2 = last_key_scancode;
	v3.cursor_enable_flash = f_0B3EC();
	font_size = d_08117;
	draw_mode = 1;
	use_rambuf = 0;
	get_screen_addr(cursor_x, cursor_y, &v3.screen_addr);
	copy_cursor_from_scr(v3.cursor_off, v3.screen_addr);
	if (v3.cursor_enable_flash) char_print(cursor_x, cursor_y, cursor_char);
	copy_cursor_from_scr(v3.cursor_on, v3.screen_addr);
	use_rambuf = 1;
	draw_mode = 0;
	v3.cursor_toggle = 1;
	v3.unk_0x2c = 0xac;
	v3.unk_0x2d = 0xac;
j_0b156:
	f_0B226(&v3);
	if (!(v0--) && f_046CC()) {
		f_046C0();
		v1 = v2;
		*scancode = v2;
		goto j_0b19c;
	}
	delay(19);
	if (check_key_kio((char *)&v2)) goto j_0b156;
	v3.unk_0x2c = 1;
	v3.unk_0x2d = 1;
	v3.cursor_toggle = 0;
	goto j_0b1ee;
j_0b19c:
	clr_all_ko();
	last_key_scancode = v1;
	copy_cursor_to_scr(v3.screen_addr, v3.cursor_off);
	return;
j_0b1bc:
	f_0B226(&v3);
	delay(4762);
j_0b1cc:
	if (get_IRQ0() & (1 << 1)) {
		if (!is_key_pressed()) {
			waitkey();
			goto j_0b1cc;
		}
		goto j_0b1f8;
	}
	++timer;
	goto j_0b1bc;
j_0b1ee:
	set_all_kimask();
	set_all_ko();
	goto j_0b1bc;
j_0b1f8:
	clr_all_ko();
	clr_all_kimask();
	delay(1);
	if (get_key_kio((char *)&v1) && check_key_kio2((char *)&v1)) {
		*scancode = v1;
		goto j_0b19c;
	}
	goto j_0b1ee;
}

// 0B226
static void f_0B226(getscancode_struct *a) {
	--a->unk_0x2d;
	if (!a->unk_0x2d) {
		a->cursor_toggle ^= 1;
		a->unk_0x2d = a->unk_0x2c;
		if (!a->shutdown_timer--) shutdown();
	}
	if (a->cursor_enable_flash) {
		if (a->cursor_toggle) copy_cursor_to_scr(a->screen_addr, a->cursor_on);
		else copy_cursor_to_scr(a->screen_addr, a->cursor_off);
	} else delay(5);
	return;
}

// 0B292
char check_key_kio(char *kio) {
	char i;
	char v1;
	volatile char *_KOD;
	volatile char *_KID;

	v1 = 0;
	_KOD = 0xf046;
	_KID = 0xf040;

	*_KOD = kio[1];
	i = 1;
	do {
		if ((char)((*_KID ^ 0xff) & kio[0])) {
			v1 = 1;
			break;
		} else delay(13);
	} while (++i <= 10);

	*_KOD = 0;
	if (v1) f_046C4();
	return v1;
}

// 0B2E0
char get_key_kio(char *kio) {
	char v0;
	volatile char *_KOD;
	volatile char *_KID;
	char v1;
	char v2;

	v0 = 1;
	_KOD = 0xf046;
	_KID = 0xf040;
	v1 = 1;
	v2 = 1;
	do {
		*_KOD = v1;
		if ((char)*_KID != (char)0xff) {
			kio[1] = v1;
			kio[0] = *_KID ^ 0xff;
			goto j_0b312;
		}
		v1 <<= 1;
	} while (++v2 <= 7);
	v0 = 0;
j_0b312:
	clr_all_ko();
	return v0;
}

// 0B31E
char check_key_kio2(char *kio) {
	char v0;
	char v1;
	char v2;
	volatile char *_KOD;
	volatile char *_KID;
	char v3;

	v0 = 1;
	v2 = 0;
	_KOD = 0xf046;
	_KID = 0xf040;
	v3 = 1;
	do {
		delay(13);
		*_KOD = kio[1];
		v1 = *_KID ^ 0xff;
		clr_all_ko();
		if ((char)(v1 & kio[0])) ++v2;
	} while (++v3 <= 5);
	if (v2 != 5) v0 = 0;
	return v0;
}

// 0B370
static void copy_cursor_from_scr(char *cursor, char *scr) {
	char v0;

	v0 = 0;
	while (v0 < d_08117) {
		cursor[v0*2] = scr[0];
		cursor[v0*2+1] = scr[1];
		scr += 0x10;
		++v0;
	}
}

// 0B3B2
static void copy_cursor_to_scr(char *scr, char *cursor) {
	char v0;

	v0 = 0;
	while (v0 < d_08117) {
		scr[0] = cursor[v0*2];
		scr[1] = cursor[v0*2+1];
		scr += 0x10;
		++v0;
	}
	return;
}

// 0B3EC
char f_0B3EC(void) {
	if (cursor_noflash) return 0;
	if (screen_state) return 0;
	if (d_080FE != 1) return 0;
	return 1;
}

// 0B410
char scancode_to_int(char *a, char *keycodes) {
	char v0;
	char v1;
	char v2;

	v0 = a[0];
	v1 = a[1];
	if (!v1 || !v0) return 0;
	v2 = 0;
	while (v0 > 0) {
		++v2;
		v0 >>= 1;
	}
	v0 = v2;
	v2 = 0;
	while (v1 > 0) {
		++v2;
		v1 >>= 1;
	}
	v1 = v2 - 1;
	--v0;
	return keycodes[(char)((v0 << 3) + v1)];
}

// 0B45E
char getkeycode(char a) {
	int *v0;
	char v1;
	char *v2;

	while (1) {
		if (!screen_state) setup_status_bar();
		getscancode(&v0);
		if (modifiers & (1 << 2)) {
			v1 = scancode_to_int((char *)&v0, keycodes_alpha);
			if (mode == MODE_BASE_N && (v1 == K_EULER || v1 == K_CONV_N || v1 == K_RANINT)) v1 = 0;
		}
		else if (modifiers & (1 << 1)) v1 = scancode_to_int((char *)&v0, keycodes_rcl);
		else if (modifiers & (1 << 0)) v1 = scancode_to_int((char *)&v0, keycodes_sto);
		else if (!(modifiers & (1 << 3))) v1 = scancode_to_int((char *)&v0, mode != MODE_BASE_N ? keycodes : keycodes_base_n);
		else v1 = scancode_to_int((char *)&v0, mode != MODE_BASE_N ? keycodes_shift : keycodes_shift_base_n);

		if (mode == MODE_CMPLX) {
			if (v1 != K_NEGATIVE) if (v1 == K_ENG || v1 == K_ENG_R) v1 = 0x80;
			else if (modifiers & (1 << 3)) v1 = 0xaf;
		}
		if (v1 == K_OFF) shutdown();
		if (is_modifier_keycode(v1)) set_modifiers(v1);
		else {
			set_modifiers(0);
			if (v1 == K_EXECUTE && !f_0B588()) v1 = K_APPROX;
			return v1;
		}
	}
}

// 0B588
char f_0B588(void) {
	if (!setup_mathi) return 0;
	if (setup_decimalo) return 0;
	if (!(mode & (1 << 6))) return 0;
	if (mode == MODE_EQN && d_080FE == 1 && submode <= 2) return 0;
	return 1;
}

// 0B5C6
void set_modifiers(char keycode) {
	char mods;

	mods = modifiers;
	switch (keycode) {
		case K_ALPHA:
			mods = (mods ^ 4) & 0xf4;
			break;
		case K_SHIFT:
			mods = (mods ^ 8) & 0xf8;
			break;
		case K_RCL:
			mods = (mods ^ 2) & 0xf2;
			break;
		case K_STO:
			mods = (mods ^ 1) & 0xf1;
			break;
		case K_INS:
			if (!screen_state) mods += 0x80;
			mods &= 0xf7;
			modifiers = mods;
			f_10FD2();
			break;
		default:
			mods &= 0xf0;
			break;
	}
	modifiers = mods;
	return;
}

// 0B620
char is_modifier_keycode(char keycode) {
	if (0xe8 <= keycode && keycode <= 0xec) return 1;
	return 0;
}

// 0 = AC, 1 = EXE
// 0B634
char prompt_yes_no(void) {
	char v0;

	do {
		v0 = getkeycode(1);
		if (v0 == K_APPROX || v0 == K_EXECUTE) return 1;
		else if (v0 == K_AC) return 0;
	} while (1);
}

// 0B654
void wait_shift(void) {
	int scancode;

	do getscancode(&scancode);
	while (scancode_to_int((char *)&scancode, keycodes) != K_SHIFT);
}

// 0B67E
static void f_0B67E(void) {
	if (d_080FE == 1 && d_080F7 == 1) {
		if (last_key_keycode == K_DMS) {
			// Original version jumps to `f_044E2` with B. Currently there is no known way to replicate this in CCU8
			f_044E2(0x5c); // Degs-Mins-Secs
			return;
		}
		if ((K_DMS_R <= last_key_keycode && last_key_keycode <= K_FMT_FRAC) || last_key_keycode == K_FACT) last_key_keycode = 0;
	}
	return;
}

// 0B6B6
char f_0B6B6(void) {
	char v0 = 1;

	if (table_mode != 1 && d_080FE != 1 && !(d_080FE & 0xf0) && !is_char_keycode(last_key_keycode) && !is_rcl_keycode(last_key_keycode)) v0 = 0;
	return v0;
}

// 0B6F0 - STUB
void f_0B6F0(void) {
	return;
}

// 0B736 - STUB
void f_0B736(void) {
	return;
}

// 0B782
static char is_rcl_keycode(char keycode) {
	if (d_080F7 && K_RCL_A <= keycode && keycode <= K_RCL_M) return 1;
	return 0;
}

// 0B79C
static char is_sto_keycode(char keycode) {
	if (d_080F7 && K_STO_A <= keycode && keycode <= K_STO_M) return 1;
	return 0;
}

// 0B7B6
char f_0B7B6(void) {
	if (mode == MODE_EQN && table_mode == 1) return 1;
	return 0;
}

// 0B7E6
static char show_error(char idx) {
	char keycode;

	print_error(idx);
	do keycode = getkeycode(1);
	while (is_ac_key(keycode));
	return keycode;
}

// 0B804
char diag_initloop(void) {
	char v0;
	int v1;
	char v2[2];
	char v3[2];

	v0 = 0;
	v1 = 2381;
	v2[0] = 0x80;
	v2[1] = 1;
j_0b81c:
	if (v1--) {
		delay(20);
		if (!check_key_kio(v2)) {
j_0b850:
			clr_all_ko();
			clr_all_kimask();
			delay(1);
			if (get_key_kio(v3) && check_key_kio2(v3)) {
				if (v3[0] == 4 && v3[1] == 4) {
					v0 = 1;
					goto j_0b838;
				} else if (v3[0] != 4 || v3[1] != 0x10) {
					// Original ROM copies 2 bytes at once
					*(int*)v2 = *(int*)v3;
					goto j_0b81c;
				} else goto j_0b838;
			}
		}
		set_all_kimask();
		set_all_ko();
j_0b898:
		if (!--v1) {
			clr_all_ko();
			clr_all_kimask();
			goto j_0b838;
		}
		delay(20);
		if (is_key_pressed()) goto j_0b850;
		else goto j_0b898;
	}

j_0b838:
	clr_all_ko();
	return v0;
}

// 0B8B8
void f_0B8B8(char a) {
	char v0;

	v0 = 1;
	if (a & (1 << 7)) v0 = 0;

	a &= 0xf;
	d_08120 = 0;
	d_080FE = 1;
	d_080FF = 0;
	if (v0) {
		memzero(input_area, 100);
		cursor_pos_byte = 0;
	}
	formula_x = 0;
	formula_y = 1;
	font_size = 10;
	if (!f_0A564()) {
		if (a) buffer_clear();
	} else {
		font_size = 7;
		formula_y = 25;
		line_print(formula_x, formula_y, s_blank_line);
	}
	cursor_x = 0;
	cursor_y = 1;
	f_10FD2();
	d_08117 = font_size;
	f_044CE();
	d_08124 = 0;
	use_output_charset = 0;
	input_area_ptr = &input_area;
	if (a == 2) f_0B998();
	return;
}

// 0B968 - STUB
void f_0B968(void) {
	return;
}

// 0B984
static char conv_func_token(char keycode) {
	if (keycode < K_FRAC || keycode > K_SUM) return 0;
	else return tokens_map[keycode - 0xd0];
}

// 0B998
void f_0B998(void) {
	char num[10];

	if (!f_08ADC() && !is_mathi()) {
		num_fromdigit(&num, 0);
		num_output_print(&num);
	}
	return;
}

// 0B9C8
static char f_0B9C8(f_09962_struct *a) {
	f_0B8B8(2);
	if (a->unk_0x07) {
		f_044B6();
		num_output_print(*a->result_ptr);
		a->unk_0x04 = 1;
		return 0;
	} else {
		clear_result();
		if (a->mode == MODE_TABLE) {
			if (d_080FD) smart_strcpy(*a->input_area_ptr, cache_area);
			else memzero(cache_area, 100);
			buffer_clear();
		}
		f_0AF16();
		return 1;
	}
}

// 0BA28
static void f_0BA28(f_09962_struct *a) {
	f_0B8B8(1);
	f_0AF0A();
	table_mode = TABLE_EQN;
	last_key_keycode = NULL;
	d_08125 = 0;
	a->unk_0x04 = 0;
	return;
}

// 0BA50 - STUB
void f_0BA50(void) {
	return;
}

// 0BAA8 - STUB
void f_0BAA8(void) {
	return;
}

// 0BADA - STUB
void f_0BADA(void) {
	return;
}

// 0BAF2 - STUB
void f_0BAF2(void) {
	return;
}

// 0BB42 - STUB
static char f_0BB42(char *a) {
	return 0;
}

// 0BBDA - STUB
void f_0BBDA(void) {
	return;
}

// 0BC34 - STUB
char f_0BC34(void) {
	return 0;
}

// 0BC6C - STUB
void f_0BC6C(void) {
	return;
}

// 0BC90 - STUB
void f_0BC90(void) {
	return;
}

// 0BDFA - STUB
void f_0BDFA(void) {
	return;
}

// 0BEEE
static char f_0BEEE(f_09962_struct *a) {
	char v0;

	if (!f_03698(last_key_keycode))
j_0bf04:
		return 4;
	v0 = table_mode;
	if (d_080FE & (1 << 7)) {
		if (!f_02CB6()) {
			if (!(v0 & (1 << 7)) && v0 == 6) f_0AF16();
		}
		f_0B8B8(0x82);
	} else if (a->mode != MODE_TABLE && a->mode != MODE_EQN) {
		if (f_03660()) {
			f_0AF16();
			f_0B8B8(0x80);
		} else if (!(char)*a->input_area_ptr && v0 == 1) smart_strcpy(a->input_area_ptr, cache_area);
		else goto j_0bf04;
		cursor_pos_byte = last_key_keycode == K_RIGHT ? 0 : smart_strlen(a->input_area_ptr);
	} else goto j_0bf04;
	f_046AE();
	return 1;
}

// 0BF8A
static char f_0BF8A(void) {
	if (f_03698(last_key_keycode) && is_matho() && d_0812C) {
		f_046AE();
		if (last_key_keycode == K_RIGHT) ++cursor_pos_byte;
		else if (cursor_pos_byte) --cursor_pos_byte;
		else f_046C4();
		return 3;
	}
	return 4;
}

// 0BFDC - STUB
static char f_0BFDC(f_09962_struct *a) {
	return 0;
}

// 0C084
void print_result(void) {
	num_output_print(result);
}

// 0C08C
static void f_0C08C(f_09962_struct *a) {
	char v0;

	if (f_08ADC() && !a->unk_0x07) {
		if (a->mode != MODE_CMPLX) num_fromdigit(&(*a->result_ptr)[10], 0);
		v0 = cursor_pos_byte;
		f_07B60(*a->result_ptr);
		cursor_pos_byte = v0;
	}

	return;
}

// 0C0D0
void f_0C0D0(void) {
	char v0;

	if (f_0B7B6()) f_10EF8();
	else if (table_mode == TABLE_SOLVE && d_080FD == 0x40) print_continue_prompt();
	else {
		v0 = f_02CB6();
		if (d_080FE == 1) {
			d_08122 = 1;
			if (!is_mathi()) f_04F6E();
			else {
				f_10FC0();
				f_058DC();
			}
		} else {
			d_08122 = 0;
			cursor_pos_byte = 0;
			if (table_mode == 1 || table_mode == TABLE_RANGE || (table_mode & (1 << 7))) if (v0) f_02BE8();
		}
	}
	return;
}

// 0C148
static char f_0C148(f_09962_struct *a) {
	char v0;

	v0 = d_080FE;
	d_080FE = 0x80;
	f_044D6(show_error(ERROR_MATH));
	clear_result();
	if (f_0BEEE(a) == 1) {
		if (v0 == 4) {
			f_0AF16();
			smart_strcpy(a->input_area_ptr, cache_area);
		} else cursor_pos_byte = smart_strlen(a->input_area_ptr);
	} else f_0B9C8(a);
}

// 0C1A0
static char f_0C1A0(f_09962_struct *a) {
	if (a->mode == MODE_BASE_N && submode == SMODE_BASE_N_BIN) {
		if (f_149D8(a->result_ptr)) return 4;
		else return f_0C148(a);
	} else return 4;
}

// 0C1D4
static char f_0C1D4(f_09962_struct *a) {
	char loc_m20[20];

	if (f_02C76() || f_02AAA() || d_0812C) return 4;
	num_cpy_im(loc_m20, *a->result_ptr);
	if (num_invalid__(&loc_m20[10]) == 1 || !cmplx_abs(loc_m20)) return 4;
	return f_0C148(a);
}

// 0C22E
static char keyfunc_ac(f_09962_struct *a) {
	if (f_0B7B6()) {
		f_0BA28(a);
		return 0;
	} else if (!f_0B9C8(a)) return 0;
	else return 1;
}

// 0C25A - STUB
static char keyfunc_sto_rcl(f_09962_struct *a) {
	return 0;
}

// 0C316
static char keyfunc_rcl(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// 0C31A
static char keyfunc_sto(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// 0C31E - STUB
static char keyfunc_exe(f_09962_struct *a) {
	return 0;
}

// 0C64A - STUB
static char keyfunc_abc(f_09962_struct *a) {
	return 0;
}

// 0C692 - STUB
static char keyfunc_dc(f_09962_struct *a) {
	return 0;
}

// 0C6DE - STUB
static char keyfunc_dms(f_09962_struct *a) {
	return 0;
}

// 0C728
static char keyfunc_fact(f_09962_struct *a) {
	// fx-570/991ES PLUS does not have prime factor
	return 0;
}

// 0C72C - STUB
static void f_0C72C(f_09962_struct *a) {
	return;
}

// 0C74C - STUB
static char keyfunc_eng(f_09962_struct *a) {
	return 0;
}

// 0C77A - STUB
static char keyfunc_eng_r(f_09962_struct *a) {
	return 0;
}

// 0C7A8
static char keyfunc_del(f_09962_struct *a) {
	if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// 0C7C2
static char keyfunc_mov_x(f_09962_struct *a) {
	if (f_0BF8A() == 3) return 3;
	else if (d_080FE & (1 << 6)) return 0;
	else if (f_0BEEE(a) == 1) return 1;
	else if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// 0C806
static char keyfunc_mov_y(f_09962_struct *a) {
	if (d_080FE & (1 << 6)) return 0;
	else if (f_0BFDC(a) == 2) return 2;
	else if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// 0C836 - STUB
static char keyfunc_base(f_09962_struct *a) {
	submode = base_n_submodes[last_key_keycode - K_BASE_BIN];
	if (f_03664()) {
		if (f_0C1A0(a) == 1) return 1;
		else print_result();
	}
	return 2;
}

// 0C86A - STUB
static char keyfunc_mode(f_09962_struct *a) {
	f_0B998();
	return 2;
}

// 0C874
static char keyfunc_setup(f_09962_struct *a) {
	if (is_matho()) f_085D2();
	if (f_03664()) {
		if (a->unk_0x08 && f_0C1D4(a) != 1) f_0C08C(a);
		else return 1;
	}
	return 2;
}
