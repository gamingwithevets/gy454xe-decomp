#include <string.h>
#include "../init.h"
#include "../consts.h"
#include "../generals.h"
#include "../emu/emu_kb.h"
#include "../io/input.h"
#include "../modes/verif.h"
#include "unk2.h"
#include "unk4.h"
#include "unk5.h"

// Weird struct used in f_09962 and the functions that it calls from a jump table.
typedef struct {
	char *input_area;
	char *result;
	char unk_0x04;
	char is_func;
	char mode;
	char unk_0x07;
	char is_mathi;
	char is_matho;
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
	char cursor_toggle_timer;
	char cursor_toggle_timer_curr;
	int shutdown_timer;
} getscancode_struct;

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

// Struct used in emulator ROM's log_error.
typedef struct {
	int index;
	int add_template;
	char *string;
} error_emu;

// Static declarations (placed first so we can define jump table)
static char keyfunc_nop(f_09962_struct *a);
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
static char table_matvct_set_cell(char *num, char sm, char ty, char tx);
static char f_0AE14(char a);
static void set_result(char *a);
static void f_0B226(getscancode_struct *a);
static void copy_cursor_from_scr(char *cursor, char *scr);
static void copy_cursor_to_scr(char *scr, char *cursor);
static void f_0B67E(void);
static char is_meta_keycode(void);
static void f_0B736(void);
static char is_rcl_keycode(char keycode);
static char is_sto_keycode(char keycode);
static char show_error(char idx);
static char get_func_tok(char keycode);
static char get_storcl_tok(char keycode);
static void print_result_0(void);
static char f_0B9C8(f_09962_struct *a);
static void f_0BA28(f_09962_struct *a);
static void f_0BA50(char m);
static void f_0BAA8(char *input);
static char is_mathi_mode(void);
static char f_0BAF2(char **a);
static char f_0BB42(char **a);
static char *find_replay_entry(char *a);
static char f_0BC6C(char *a);
static void read_replay_entry(void);
static char _keyfunc_mov_x(f_09962_struct *a);
static char f_0BF8A(void);
static char _keyfunc_mov_y(f_09962_struct *a);
static void print_result_basic(void);
static void f_0C08C(f_09962_struct *a);
static char f_0C148(f_09962_struct *a);
static char f_0C1A0(f_09962_struct *a);
static char f_0C1D4(f_09962_struct *a);
static char keyfunc_ac(f_09962_struct *a);
static char keyfunc_sto_rcl(f_09962_struct *a);
static char keyfunc_rcl(f_09962_struct *a);
static char keyfunc_sto(f_09962_struct *a);
static char keyfunc_exe(f_09962_struct *a);
static char keyfunc_fmt_dec(f_09962_struct *a);
static char keyfunc_fmt_frac(f_09962_struct *a);
static char keyfunc_dms(f_09962_struct *a);
static char keyfunc_fact(f_09962_struct *a);
static char f_0C72C(f_09962_struct *a);
static char keyfunc_eng(f_09962_struct *a);
static char keyfunc_eng_r(f_09962_struct *a);
static char keyfunc_del(f_09962_struct *a);
static char keyfunc_mov_x(f_09962_struct *a);
static char keyfunc_mov_y(f_09962_struct *a);
static char keyfunc_base(f_09962_struct *a);
static char keyfunc_mode(f_09962_struct *a);
static char keyfunc_setup(f_09962_struct *a);

// DATA: GY454XE  Re 01FB6
// DATA: GY455XE  Im 01FB8
// DATA: GY460XF  Im 01D14
static char (* const keyfuncs[])(f_09962_struct *) = {
	keyfunc_mov_y,		// K_UP
	keyfunc_mov_y,		// K_DOWN
	keyfunc_mov_x,		// K_RIGHT
	keyfunc_mov_x,		// K_LEFT
	keyfunc_mode,		// K_MODE
	keyfunc_setup,		// K_SETUP
	keyfunc_ac,			// K_AC
	keyfunc_nop,		// K_OFF
	keyfunc_nop,		// K_ALPHA
	keyfunc_nop,		// K_SHIFT
	keyfunc_rcl,		// K_RCL
	keyfunc_sto,		// K_STO
	keyfunc_nop,		// K_INS
	keyfunc_exe,		// K_APPROX
	keyfunc_sto,		// K_M_PLUS
	keyfunc_sto,		// K_M_MINUS
	keyfunc_exe,		// K_EXECUTE
	keyfunc_fact,		// K_FACT
	keyfunc_base,		// K_BASE_BIN
	keyfunc_base,		// K_BASE_OCT
	keyfunc_base,		// K_BASE_DEC
	keyfunc_base,		// K_BASE_HEX
	keyfunc_dms,		// K_DMS
	keyfunc_dms,		// K_DMS_R
	keyfunc_eng,		// K_ENG
	keyfunc_eng_r,		// K_ENG_R
	keyfunc_fmt_dec,	// K_FMT_DEC
	keyfunc_fmt_frac,	// K_FMT_FRAC
	keyfunc_nop,		// K_CALC
	keyfunc_nop,		// K_SOLVE
	keyfunc_del			// K_DEL
};

#if REAL == 0
// DATA: GY455XE  Im 0204C
const char s_err_emu_acbreak[] = "ACBREAK";

// DATA: GY455XE  Im 02054
const char s_err_emu_syntax[] = "Syntax";

// DATA: GY455XE  Im 0205B
const char s_err_emu_math[] = "Ma";

// DATA: GY455XE  Im 0205E
const char s_err_emu_mem[] = "Memory";

// DATA: GY455XE  Im 02065
const char s_err_emu_go[] = "Go";

// DATA: GY455XE  Im 02068
const char s_err_emu_nesting[] = "Nesting";

// DATA: GY455XE  Im 02070
const char s_err_emu_stack[] = "Stack";

// DATA: GY455XE  Im 02076
const char s_err_emu_argument[] = "Argument";

// DATA: GY455XE  Im 0207F
const char s_err_emu_dimension[] = "Dimension";

// DATA: GY455XE  Im 02089
const char s_err_emu_com[] = "Com";

// DATA: GY455XE  Im 0208D
const char s_err_emu_transmit[] = "Transmit";

// DATA: GY455XE  Im 02096
const char s_err_emu_receive[] = "Receive";

// DATA: GY455XE  Im 0209E
const char s_err_emu_outofmem[] = "Memory Full";

// DATA: GY455XE  Im 020AA
const char s_err_emu_undefined[] = "Undefined";

// DATA: GY455XE  Im 020B4
const char s_err_emu_overflow[] = "Overflow";

// DATA: GY455XE  Im 020BD
const char s_err_emu_domain[] = "Domain";

// DATA: GY455XE  Im 020C4
const char s_err_emu_nonreal[] = "Non-Real";

// DATA: GY455XE  Im 020CD
const char s_err_emu_nosolution[] = "No Solution";

// DATA: GY455XE  Im 020D9
const char s_err_emu_mismatch[] = "Mismatch";

// DATA: GY455XE  Im 020E2
const char s_err_emu_novar[] = "No Variable";

// DATA: GY455XE  Im 020EE
const char s_err_emu_notfound[] = "Not Found";

// DATA: GY455XE  Im 020F8
const char s_err_emu_app[] = "Application";

// DATA: GY455XE  Im 02104
const char s_err_emu_sys[] = "System";

// DATA: GY455XE  Im 0210B
const char s_err_emu_exists[] = "Already Exists";

// DATA: GY455XE  Im 0211A
const char s_err_emu_cmplx[] = "Complex Number In List";

// DATA: GY455XE  Im 02131
const char s_err_emu_solve[] = "Can't Solve!";

// DATA: GY455XE  Im 0213E
const char s_err_emu_range[] = "Range";

// DATA: GY455XE  Im 02144
const char s_err_emu_iter[] = "Iteration";

// DATA: GY455XE  Im 0214E
const char s_err_emu_cond[] = "Condition";

// DATA: GY455XE  Im 02158
const char s_err_emu_blank[] = "";

// DATA: GY455XE  Im 02159
const char s_err_emu_circular[] = "Circular";

// DATA: GY455XE  Im 02162
const char s_err_emu_imroot[] = "No Real Roots";

// DATA: GY455XE  Im 02170
const char s_err_emu_ver[] = "Version";

// DATA: GY455XE  Im 02178
const char s_err_emu_sd[] = "SD Card";

// DATA: GY455XE  Im 02180
const char s_err_emu_sdro[] = "SD Card is protected";

// DATA: GY455XE  Im 02195
const char s_err_emu_sdinvalid[] = "invarid Card";

// DATA: GY455XE  Im 021A2
const char s_err_emu_nosd[] = "No Card";

// DATA: GY455XE  Im 021AA
const char s_err_emu_timeout[] = "Time out";

// DATA: GY455XE  Im 021B3
const char s_err_emu_template[] = "ERROR";

// DATA: GY455XE  Im 021B9
const char s_err_emu_unknown[] = "??? ERROR";
#endif

// DATA: GY454XE  Re 02032
// DATA: GY455XE  Im 021C4
const char vars_map[] = {
	0x41,	// A
	0x42,	// B
	0x43,	// C
	0x44,	// D
	0x45,	// E
	0x46,	// F
	0x58,	// X
	0x59,	// Y
	0x54,	// M
	0x47,	// ->A
	0x48,	// ->B
	0x49,	// ->C
	0x4a,	// ->D
	0x83,	// ->E
	0x84,	// ->F
	0x4c,	// ->X
	0x4d,	// ->Y
	0x4b	// ->M
};


// DATA: GY454XE  Re 02044
// DATA: GY455XE  Im 021D6
const char tokens_map[] = {
#if ENABLE_RDEC == 1
#define TOKENS_MAP_START K_RDEC
	0xA4,	// K_RDEC
#else
#define TOKENS_MAP_START K_FRAC
#endif
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

// DATA: GY454XE  Re 02054
// DATA: GY455XE  Im 021E6
const char s_colon[] = ":";

// For keycode lists, every row is KI(1-8), every column is KO(1-8)

// DATA: GY454XE  Re 02056
// DATA: GY455XE  Im 021E8
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

// DATA: GY454XE  Re 02096
// DATA: GY455XE  Im 02228
const char keycodes_shift[64] = {
	K_SD,		K_CMPLX,	K_BASE,		K_POL,		K_REC,		NULL,		K_APPROX,	NULL,
	K_MATRIX,	K_VECTOR,	K_VERIFY,	K_PERMU,	K_COMBI,	NULL,		K_DRG,		NULL,
	K_CONST,	K_CONV,		K_CLR,		K_INS,		K_OFF,		NULL,		K_PI,		NULL,
	K_STO,		K_ENG_R,	K_PERCENT,	K_COMMA,	K_FMT_FRAC,	K_M_MINUS,	K_RAN,		NULL,
	K_NEGATIVE,	K_FACT,		K_ABS,		K_ARCSIN,	K_ARCCOS,	K_ARCTAN,	K_RND,		NULL,
	K_FRAC_ABC,	K_CBRT,		K_POW_3,	K_NTH_RT,	K_10_POW,	K_E_POW,	NULL,		NULL,
	K_SOLVE,	K_DDX,		K_LEFT,		K_DOWN,		K_FCTRIAL,	K_SUM,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_SETUP,	NULL,		NULL,		NULL
};

// DATA: GY454XE  Re 020D6
// DATA: GY455XE  Im 02268
const char keycodes_alpha[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		K_EULER,	NULL,
	K_RCL,		NULL,		NULL,		K_VAR_X,	K_VAR_Y,	K_VAR_M,	K_RANINT,	NULL,
	K_VAR_A,	K_VAR_B,	K_VAR_C,	K_VAR_D,	K_VAR_E,	K_VAR_F,	NULL,		NULL,
	NULL,		K_RDEC,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	K_EQUALS,	K_COLON,	K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		K_COLON,	K_EQUALS,	NULL
};

// DATA: GY454XE  Re 02116
// DATA: GY455XE  Im 022A8
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

// DATA: GY454XE  Re 02156
// DATA: GY455XE  Im 022E8
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

// DATA: GY454XE  Re 02196
// DATA: GY455XE  Im 02328
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

// DATA: GY454XE  Re 021D6
// DATA: GY455XE  Im 02368
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
static char keyfunc_nop(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 09962
// FUNCTION: GY455XE  Im 0A286
// FUNCTION: GY460XF  Im 09C2C
char f_09962(char no_keyfunc) {
	f_09962_struct v0;
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
static void table_set_cur_cell(char *num) {
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

// FUNCTION: GY454XE  Re 0ABA8
// FUNCTION: GY455XE  Im 0B4CC
// FUNCTION: GY460XF  Im 0AF92
char *set_dim(char idx, char m, char n) {
	char *v0;

	matvct_dims[idx].b.m = m;
	matvct_dims[idx].b.n = n;
	v0 = f_043AC(idx, 1, 1);
	memzero(v0, 90);
	return v0;
}

// FUNCTION: GY454XE  Re 0ABDE
// FUNCTION: GY455XE  Im 0B502
// FUNCTION: GY460XF  Im 0AFC8
void table_eqn_init(void) {
	table_home();
	memzero(matvct_dims, 18);
	memzero(mode_ram, 810);
	return;
}

// FUNCTION: GY454XE  Re 0ABFC
// FUNCTION: GY455XE  Im 0B520
// FUNCTION: GY460XF  Im 0AFE6
void table_eqn_setup(void) {
	table_eqn_init();
	set_dim(0, 3, 3);
	set_dim(1, 3, 3);
	set_dim(2, 3, 3);
	table_mode = TABLE_EQN;
	return;
}

// FUNCTION: GY454XE  Re 0AC28
// FUNCTION: GY455XE  Im 0B54C
// FUNCTION: GY460XF  Im 0B012
void table_ratio_setup(void) {
	table_eqn_setup();
	table_mode = TABLE_RATIO;
	return;
}

// FUNCTION: GY454XE  Re 0AC36
// FUNCTION: GY455XE  Im 0B55A
// FUNCTION: GY460XF  Im 0B020
void table_ineq_setup(void) {
	table_eqn_setup();
	table_mode = TABLE_INEQ;
	return;
}

// FUNCTION: GY454XE  Re 0AC44
// FUNCTION: GY455XE  Im 0B568
// FUNCTION: GY460XF  Im 0B02E
static char table_matvct_set_cell(char *num, char sm, char ty, char tx) {
	char v0;
	int *v1;
	int *tmp;
	int tmp2;

	v0 = 0;
	if (sm > 5 || ty > matvct_dims[sm].b.m || tx > matvct_dims[sm].b.n) {
		v0 = 2;
		goto j_0ac98;
	}
	v1 = (int *)f_043AC(sm, ty, tx);
	tmp = (int *)num;
	tmp2 = 8;
	do {
		v1[4] = tmp[4];
		--tmp;
		--v1;
	} while ((tmp2 -= 2) >= 0);
j_0ac98:
	return v0;
}

// FUNCTION: GY454XE  Re 0AD08
// FUNCTION: GY455XE  Im 0B62C
// FUNCTION: GY460XF  Im 0B0F2
char f_0AD08(char a) {
	char v0;
	char v1;
	char *v2;
	char *v3;
	char *v4;
	int tmp;
	char *loc_m2;
	char *loc_m4;
	char loc_m14[10];

	v0 = 0;
	v1 = table_stat_get_num_cols();
	if (50 - (d_080DE * v1 + d_080DF) < v1) {
		v0 = 1;
		goto j_0ae06;
	}
	if (a > d_080DE + 1 || !a) {
		v0 = 2;
		goto j_0ae06;
	}
	if (a > table_stat_get_col_addr(0, &loc_m2) + 1 || !a) {
		v0 = 2;
		goto j_0ae06;
	}
	v2 = table_stat_get_col_addr(3, &loc_m4) * 10 + loc_m4;
	v3 = (a - 1) * v1 * 10 + loc_m2;
	v4 = v1 * 10 + v3;
	tmp = v2 - v3;
	++d_080DE;
	memmove(v4, v3, tmp);
	memzero(v3, v1 * 10);
	if (setup_stat_freq) {
		num_fromdigit(&loc_m14, 1);
		table_stat_set_cell(2, a, &loc_m14);
	}
j_0ae06:
	return v0;
}

// FUNCTION: GY454XE  Re 0AE14
// FUNCTION: GY455XE  Im 0B738
// FUNCTION: GY460XF  Im 0B1FE
static char f_0AE14(char a) {
	char v0;
	char *v1;
	char v2;
	char *v3;
	char *loc_m2;
	char *loc_m4;

	v0 = 0;
	if (a > d_080DE || !a) {
		v0 = 2;
		goto j_0aec0;
	}
	v1 = loc_m2[table_stat_get_col_addr(3, &loc_m2) * 10];
	v2 = table_stat_get_num_cols();
	table_stat_get_col_addr(0, &loc_m4);
	v3 = loc_m4[(a - 1) * v2 * 10];
	memcpy(v3, &v3[v2 * 10], v1 - &v3[v2 * 10]);
	memzero(v1 - v2 * 10, v2 * 10);
	--d_080DE;
j_0aec0:
	return v0;
}

// FUNCTION: GY454XE  Re 0AECE
// FUNCTION: GY455XE  Im 0B7F2
// FUNCTION: GY460XF  Im 0B2B8
void setup_stat(void) {
	if (mode == MODE_STAT) {
		d_080DE = 0;
		d_080DF = 0;
		table_home();
		memzero(&mode_ram[8], 800);
		d_08126 = 0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0AEFA
// FUNCTION: GY455XE  Im 0B81E
// FUNCTION: GY460XF  Im 0B2E4
void table_home(void) {
	table_viewport = 1;
	table_x = 1;
	table_y = 1;
	return;
}

// FUNCTION: GY454XE  Re 0AF0A
// FUNCTION: GY455XE  Im 0B82E
// FUNCTION: GY460XF  Im 0B2F4
void f_0AF0A(void) {
	table_home();
	f_044B6();
	return;
}

// FUNCTION: GY454XE  Re 0AF16
// FUNCTION: GY455XE  Im 0B83A
// FUNCTION: GY460XF  Im 0B300
void f_0AF16(void) {
	table_mode = TABLE_NONE;
	d_080FD = 0;
	d_080FE = 1;
	result_template = 0;
}

// FUNCTION: GY454XE  Re 0AF30
// FUNCTION: GY455XE  Im 0B854
// FUNCTION: GY460XF  Im 0B31A
char table_stat_set_cell(char x, char y, char *num) {
	char v0;
	char *loc_m2;

	v0 = table_stat_get_cell_addr(x, y, &loc_m2);
	if (!v0) {
		int *tmp;
		int *tmp2;
		int tmp3;

		tmp = (int *)num;
		tmp2 = (int *)loc_m2;
		tmp3 = 8;
		do {
			tmp2[4] = tmp[4];
			tmp -= 1;
			tmp2 -= 1;
		} while ((tmp3 -= 2) >= 0);
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0AFB0
// FUNCTION: GY455XE  Im 0B8D4
// FUNCTION: GY460XF  Im 0B39A
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


// FUNCTION: GY454XE  Re 0AFE0
// FUNCTION: GY455XE  Im 0B904
// FUNCTION: GY460XF  Im 0B3CA
void clear_setup(void) {
	clear_input_area();
	clear_cache_area();
	set_default_settings();
	mode = MODE_COMP;
	submode = 0;
	screen_state = 3;
	table_mode = TABLE_NONE;
	f_0B8B8(2);
}

// FUNCTION: GY454XE  Re 0B00E
// FUNCTION: GY455XE  Im 0B932
// FUNCTION: GY460XF  Im 0B3F8
void clear_mem(void) {
	memzero(&vars_start, 100);
	if (mode == MODE_CMPLX) memzero(&mode_ram[362], 100);
	if (get_num_err_type(result) != ERROR_NULL) clear_result();
	return;
}

// FUNCTION: GY454XE  Re 0B042
// FUNCTION: GY455XE  Im 0B966
// FUNCTION: GY460XF  Im 0B42C
void clear_input_area(void) {
	memzero(input_area, 100);
}

// FUNCTION: GY454XE  Re 0B04E
// FUNCTION: GY455XE  Im 0B972
// FUNCTION: GY460XF  Im 0B438
void clear_cache_area(void) {
	memzero(cache_area, 100);
}

// FUNCTION: GY454XE  Re 0B05A
// FUNCTION: GY455XE  Im 0B97E
// FUNCTION: GY460XF  Im 0B444
void f_0B05A(void) {
	char m = mode;
	
	arrow_state = 0;
	if (m == MODE_COMP || m == MODE_BASE_N || m == MODE_CMPLX || m == MODE_VERIF) memzero(mode_ram, 250);
	clear_cache_area();
	return;
}

// FUNCTION: GY454XE  Re 0B08A
// FUNCTION: GY455XE  Im 0B9AE
// FUNCTION: GY460XF  Im 0B474
static void set_result(char *num) {
	char num_tmp[10];

	num_fromdigit(&result[10], 0);
	if (!num) {
		num = num_tmp;
		num_errorval(num_tmp, ERROR_NULL);
	}
	num_cpy(result, num);
	return;
}

// FUNCTION: GY454XE  Re 0B0C6
// FUNCTION: GY455XE  Im 0B9EA
// FUNCTION: GY460XF  Im 0B4B0
void getscancode(scancode *sc) {
#if REAL == 1
	char v0;
	getscancode_struct gsc;
	scancode loc_m50;
	scancode loc_m52;
#else
	int t;
	getscancode_struct gsc;
	char v0;
	emu_kb kb;
#endif

	gsc.shutdown_timer = 0x4ab;
	v0 = 48;
	if (f_046D4()) v0 = 0xee;
#if REAL == 1
	loc_m50.kio = last_key_scancode.kio;
#endif
	gsc.cursor_enable_flash = f_0B3EC();
	font_size = d_08117;
	draw_mode = 1;
	use_rambuf = 0;
	get_screen_addr(cursor_x, cursor_y, &gsc.screen_addr);
	copy_cursor_from_scr(gsc.cursor_off, gsc.screen_addr);
	if (gsc.cursor_enable_flash) char_print(cursor_x, cursor_y, cursor_char);
	copy_cursor_from_scr(gsc.cursor_on, gsc.screen_addr);
	use_rambuf = 1;
	draw_mode = 0;
#if REAL == 1
	gsc.cursor_toggle = 1;
	gsc.cursor_toggle_timer = 0xac;
	gsc.cursor_toggle_timer_curr = 0xac;
	do {
		f_0B226(&gsc);
		if (!(v0--) && f_046CC()) {
			f_046C0();
			loc_m52.kio = loc_m50.kio;
			sc->kio = loc_m50.kio;
			goto j_0b19c;
		} else delay(19);
	} while (check_key_kio(&loc_m50));
#endif
	gsc.cursor_toggle_timer = 1;
	gsc.cursor_toggle_timer_curr = 1;
	gsc.cursor_toggle = 0;
#if REAL == 1
	goto j_0b1ee;
j_0b19c:
	clr_all_ko();
	last_key_scancode = loc_m52;
	copy_cursor_to_scr(gsc.screen_addr, gsc.cursor_off);
	return;
j_0b1ee:
	while (set_all_kimask(), set_all_ko(), 1) {
#else
	init_emu_kb(&kb);
	*kb.stop_type = 1;
	while (1) {
#endif
		f_0B226(&gsc);
		delay(4762);
#if REAL == 1
j_0b1cc:
		if (get_IRQ0() & (1 << 1)) {
			if (!is_key_pressed()) {
				waitkey();
				goto j_0b1cc;
			}
			break;
		}
		++timer;
	}
	clr_all_ko();
	clr_all_kimask();
	delay(1);
	if (get_key_kio(&loc_m52) && check_key_kio2(&loc_m52)) {
		*sc = loc_m52;
		goto j_0b19c;
	}
	goto j_0b1ee;
#else
		if (*kb.ki | *kb.ko) break;
		t = ++timer;
	}
	*kb.stop_type = 0;
	sc->b.ki = *kb.ki;
	sc->b.ko = *kb.ko;
	copy_cursor_to_scr(gsc.screen_addr, gsc.cursor_off);
	return;
#endif
}

// FUNCTION: GY454XE  Re 0B226
// FUNCTION: GY455XE  Im 0BAEA
// FUNCTION: GY460XF  Im 0B5B0
static void f_0B226(getscancode_struct *a) {
	--a->cursor_toggle_timer_curr;
	if (!a->cursor_toggle_timer_curr) {
		a->cursor_toggle ^= 1;
		a->cursor_toggle_timer_curr = a->cursor_toggle_timer;
		
#if REAL == 1
		if (!a->shutdown_timer--) shutdown();
#else
		--a->shutdown_timer;
#endif
	}
	if (a->cursor_enable_flash) {
		if (a->cursor_toggle) copy_cursor_to_scr(a->screen_addr, a->cursor_on);
		else copy_cursor_to_scr(a->screen_addr, a->cursor_off);
	} else delay(5);
	return;
}

// FUNCTION: GY454XE  Re 0B292
// FUNCTION: GY455XE  Im 0BB40
// FUNCTION: GY460XF  Im 0B606
char check_key_kio(scancode *kio) {
	char i;
	char v1;
	volatile char *_KOD;
	volatile char *_KID;

	v1 = 0;
	_KOD = KOD_ADDR;
	_KID = KID_ADDR;

	*_KOD = kio->b.ko;
	i = 1;
	do {
		if ((char)((*_KID ^ 0xff) & kio->b.ki)) {
			v1 = 1;
			break;
		} else delay(13);
	} while (++i <= 10);

	*_KOD = 0;
	if (v1) f_046C4();
	return v1;
}

// FUNCTION: GY454XE  Re 0B2E0
// FUNCTION: GY455XE  Im 0BB8E
// FUNCTION: GY460XF  Im 0B654
char get_key_kio(scancode *kio) {
	char v0;
	volatile char *_KOD;
	volatile char *_KID;
	char v1;
	char v2;

	v0 = 1;
	_KOD = KOD_ADDR;
	_KID = KID_ADDR;
	v1 = 1;
	v2 = 1;
	do {
		*_KOD = v1;
		if ((char)*_KID != (char)0xff) {
			kio->b.ko = v1;
			kio->b.ki = *_KID ^ 0xff;
			goto j_0b312;
		}
		v1 <<= 1;
	} while (++v2 <= 7);
	v0 = 0;
j_0b312:
	clr_all_ko();
	return v0;
}

// FUNCTION: GY454XE  Re 0B31E
// FUNCTION: GY455XE  Im 0BBCC
// FUNCTION: GY460XF  Im 0B692
char check_key_kio2(scancode *kio) {
	char v0;
	char v1;
	char v2;
	volatile char *_KOD;
	volatile char *_KID;
	char v3;

	v0 = 1;
	v2 = 0;
	_KOD = KOD_ADDR;
	_KID = KID_ADDR;
	v3 = 1;
	do {
		delay(13);
		*_KOD = kio->b.ko;
		v1 = *_KID ^ 0xff;
		clr_all_ko();
		if ((char)(v1 & kio->b.ki)) ++v2;
	} while (++v3 <= 5);
	if (v2 != 5) v0 = 0;
	return v0;
}

// FUNCTION: GY454XE  Re 0B370
// FUNCTION: GY455XE  Im 0BC1E
// FUNCTION: GY460XF  Im 0B6E4
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

// FUNCTION: GY454XE  Re 0B3B2
// FUNCTION: GY455XE  Im 0BC60
// FUNCTION: GY460XF  Im 0B726
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

// FUNCTION: GY454XE  Re 0B3EC
// FUNCTION: GY455XE  Im 0BC9A
// FUNCTION: GY460XF  Im 0B760
char f_0B3EC(void) {
	if (cursor_noflash) return 0;
	if (screen_state) return 0;
	if (d_080FE != 1) return 0;
	return 1;
}

// FUNCTION: GY454XE  Re 0B410
// FUNCTION: GY455XE  Im 0BCBE
// FUNCTION: GY460XF  Im 0B784
char scancode_to_int(scancode *a, char *keycodes) {
	char v0;
	char v1;
	char v2;

	v0 = a->b.ki;
	v1 = a->b.ko;
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

// FUNCTION: GY454XE  Re 0B45E
// FUNCTION: GY455XE  Im 0BD0C
// FUNCTION: GY460XF  Im 0B7D2
char getkeycode(char a) {
	scancode v0;
	char v1;
#if REAL == 0
	emu_kb kb;
#endif
	char *v2;

#if REAL == 0
	init_emu_kb(&kb);
#endif
	while (1) {
		if (!screen_state) setup_status_bar();
j_0bd2e:
		getscancode(&v0);
#if REAL == 0
		if (v0.b.ko == 0x80) {
			if (v0.b.ki == 0x80 || v0.b.ki == 0x40) {
				f_04DF6_E();
				f_04E44_E();
				f_082A2_E(&kb);
				f_05428_E(&kb);
				goto j_0bd2e;
			} else if (v0.b.ki == 0x20) {
				f_082A2_E(&kb);
				f_05428_E(&kb);
				goto j_0bd2e;
			} else if (v0.b.ki == 0x10) {
				*kb.error_buf = '\0';
				f_082A2_E(&kb);
				reset();
			}
		}
		*kb.error_buf = '\0';
		f_082A2_E(&kb);
#endif
		if (modifiers & (1 << 2)) {
			v1 = scancode_to_int(&v0, keycodes_alpha);
			if (mode == MODE_BASE_N && (v1 == K_EULER || v1 == K_CONV_N || v1 == K_RANINT)) v1 = 0;
		}
		else if (modifiers & (1 << 1)) v1 = scancode_to_int(&v0, keycodes_rcl);
		else if (modifiers & (1 << 0)) v1 = scancode_to_int(&v0, keycodes_sto);
		else if (!(modifiers & (1 << 3))) v1 = scancode_to_int(&v0, mode != MODE_BASE_N ? keycodes : keycodes_base_n);
		else v1 = scancode_to_int(&v0, mode != MODE_BASE_N ? keycodes_shift : keycodes_shift_base_n);

		if (mode == MODE_CMPLX) {
			if (v1 != K_NEGATIVE) {
				if (v1 == K_ENG || v1 == K_ENG_R) v1 = 0x80;
			} else if (modifiers & (1 << 3)) v1 = 0xaf;
		}
		if (v1 == K_OFF) {
#if REAL == 1
			shutdown();
#else
			*kb.stop_type = ES_STOP_DOOFF;
			delay(1);
#endif
		}
		if (is_modifier_keycode(v1)) set_modifiers(v1);
		else {
			set_modifiers(0);
			if (v1 == K_EXECUTE && !f_0B588()) v1 = K_APPROX;
			return v1;
		}
	}
}

// FUNCTION: GY454XE  Re 0B588
// FUNCTION: GY455XE  Im 0BEAE
// FUNCTION: GY460XF  Im 0B974
char f_0B588(void) {
	if (!setup_mathi) return 0;
	if (setup_decimalo) return 0;
	if (!(mode & (1 << 6))) return 0;
	if (mode == MODE_EQN && d_080FE == 1 && submode <= SMODE_EQN_SIMUL3) return 0;
	return 1;
}

// FUNCTION: GY454XE  Re 0B5C6
// FUNCTION: GY455XE  Im 0BEEC
// FUNCTION: GY460XF  Im 0B9B2
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
			update_cursor_char();
			break;
		default:
			mods &= 0xf0;
			break;
	}
	modifiers = mods;
	return;
}

// FUNCTION: GY454XE  Re 0B620
// FUNCTION: GY455XE  Im 0BF46
// FUNCTION: GY460XF  Im 0BA0C
char is_modifier_keycode(char keycode) {
	if (0xe8 <= keycode && keycode <= 0xec) return 1;
	return 0;
}

// 0 = AC, 1 = EXE
// FUNCTION: GY454XE  Re 0B634
// FUNCTION: GY455XE  Im 0BF5A
// FUNCTION: GY460XF  Im 0BA20
char prompt_yes_no(void) {
	char v0;

	do {
		v0 = getkeycode(1);
		if (v0 == K_APPROX || v0 == K_EXECUTE) return 1;
		else if (v0 == K_AC) return 0;
	} while (1);
}

// FUNCTION: GY454XE  Re 0B654
// FUNCTION: GY455XE  Im 0BF7A
// FUNCTION: GY460XF  Im 0BA40
void wait_shift(void) {
	scancode sc;

	do getscancode(&sc);
	while (scancode_to_int(&sc, keycodes) != K_SHIFT);
}

// FUNCTION: GY454XE  Re 0B67E
// FUNCTION: GY455XE  Im 0BFA4
// FUNCTION: GY460XF  Im 0BA6A
static void f_0B67E(void) {
	if (d_080FE == 1 && force_nochar == 1) {
		if (last_key_keycode == K_DMS) {
			// Original version jumps to set_char_keycode with B instruction. Currently there is no known way to replicate this in CCU8
			set_char_keycode(0x5c); // Degs-Mins-Secs
			return;
		}
		if ((K_DMS_R <= last_key_keycode && last_key_keycode <= K_FMT_FRAC) || last_key_keycode == K_FACT) last_key_keycode = 0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0B6B6
// FUNCTION: GY455XE  Im 0BFDC
// FUNCTION: GY460XF  Im 0BAA2
char f_0B6B6(void) {
	char v0 = 1;

	if (table_mode != 1 && d_080FE != 1 && !(d_080FE & 0xf0) && !is_char_keycode(last_key_keycode) && !is_rcl_keycode(last_key_keycode)) v0 = 0;
	return v0;
}

// FUNCTION: GY454XE  Re 0B6F0
// FUNCTION: GY455XE  Im 0C016
// FUNCTION: GY460XF  Im 0BADC
static char is_meta_keycode(void) {
	char v0;

	v0 = 1;
	if (!is_format_keycode(last_key_keycode) && !is_mov_keycode(last_key_keycode) && !is_sto_keycode(last_key_keycode) && last_key_keycode != K_SETUP && last_key_keycode != K_M_PLUS && last_key_keycode != K_M_MINUS && last_key_keycode != K_DEL) v0 = 0;
	return v0;
}

// FUNCTION: GY454XE  Re 0B736
// FUNCTION: GY455XE  Im 0C05C
// FUNCTION: GY460XF  Im 0BB22
static void f_0B736(void) {
	char v0;
	char tmp;
	char loc_m1;

	v0 = 0;
	if (d_080FE != 1 && (tmp = f_035EC(&loc_m1))) {
		if (tmp == 2) v0 = 2;
		if (table_mode & (1 << 7) && d_080FD & 0xf0) f_0AF16();
		f_0B8B8(v0);
		f_0BA50(loc_m1);
	}
	return;
}

// FUNCTION: GY454XE  Re 0B782
// FUNCTION: GY455XE  Im 0C0A8
// FUNCTION: GY460XF  Im 0BB6E
static char is_rcl_keycode(char keycode) {
	if (force_nochar && K_RCL_A <= keycode && keycode <= K_RCL_M) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0B79C
// FUNCTION: GY455XE  Im 0C0C2
// FUNCTION: GY460XF  Im 0BB88
static char is_sto_keycode(char keycode) {
	if (force_nochar && K_STO_A <= keycode && keycode <= K_STO_M) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0B7B6
// FUNCTION: GY455XE  Im 0C0DC
// FUNCTION: GY460XF  Im 0BBA2
char is_eqn_result(void) {
	if (mode == MODE_EQN && table_mode == TABLE_NONE) return 1;
	return 0;
}

// Unused.
// FUNCTION: GY454XE  Re 0B7CE
// FUNCTION: GY460XF  Im 0BBBA
char is_table_table(void) {
	if (mode == MODE_TABLE && table_mode == TABLE_STAT_TABLE) return 1;
	return 0;
}

#if ENABLE_RATIO == 1
// FUNCTION: GY460XF  Im 0BBD2
char is_ratio_result(void) {
	if (mode == MODE_RATIO && table_mode == TABLE_NONE) return 1;
	return 0;
}
#endif

#if ENABLE_INEQ == 1
// FUNCTION: GY460XF  Im 0BBEA
char is_ineq_result(void) {
	if (mode == MODE_INEQ && table_mode == TABLE_NONE) return 1;
	return 0;
}
#endif

#if REAL == 0
// FUNCTION: GY455XE  Im 0C10C
// FUNCTION: GY460XF  Im 0BC02
static char log_error(unsigned int idx) {
	char *error_buf;
	int i;
	error_emu errors[47];
	emu_kb kb;
	
	errors[0].index = 1;
	errors[0].add_template = 0;
	errors[0].string = s_err_emu_acbreak;
	errors[1].index = 2;
	errors[1].add_template = 1;
	errors[1].string = s_err_emu_syntax;
	errors[2].index = 3;
	errors[2].add_template = 1;
	errors[2].string = s_err_emu_math;
	errors[3].index = 4;
	errors[3].add_template = 1;
	errors[3].string = s_err_emu_mem;
	errors[4].index = 5;
	errors[4].add_template = 1;
	errors[4].string = s_err_emu_go;
	errors[5].index = 6;
	errors[5].add_template = 1;
	errors[5].string = s_err_emu_nesting;
	errors[6].index = 7;
	errors[6].add_template = 1;
	errors[6].string = s_err_emu_stack;
	errors[7].index = 8;
	errors[7].add_template = 1;
	errors[7].string = s_err_emu_argument;
	errors[8].index = 9;
	errors[8].add_template = 1;
	errors[8].string = s_err_emu_dimension;
	errors[9].index = 10;
	errors[9].add_template = 1;
	errors[9].string = s_err_emu_com;
	errors[10].index = 11;
	errors[10].add_template = 1;
	errors[10].string = s_err_emu_transmit;
	errors[11].index = 12;
	errors[11].add_template = 1;
	errors[11].string = s_err_emu_receive;
	errors[12].index = 13;
	errors[12].add_template = 0;
	errors[12].string = s_err_emu_outofmem;
	errors[13].index = 14;
	errors[13].add_template = 0;
	errors[13].string = s_err_emu_undefined;
	errors[14].index = 15;
	errors[14].add_template = 1;
	errors[14].string = s_err_emu_overflow;
	errors[15].index = 16;
	errors[15].add_template = 1;
	errors[15].string = s_err_emu_domain;
	errors[16].index = 17;
	errors[16].add_template = 1;
	errors[16].string = s_err_emu_nonreal;
	errors[17].index = 18;
	errors[17].add_template = 0;
	errors[17].string = s_err_emu_nosolution;
	errors[18].index = 19;
	errors[18].add_template = 0;
	errors[18].string = s_err_emu_mismatch;
	errors[19].index = 20;
	errors[19].add_template = 0;
	errors[19].string = s_err_emu_novar;
	errors[20].index = 21;
	errors[20].add_template = 0;
	errors[20].string = s_err_emu_notfound;
	errors[21].index = 22;
	errors[21].add_template = 1;
	errors[21].string = s_err_emu_app;
	errors[22].index = 23;
	errors[22].add_template = 1;
	errors[22].string = s_err_emu_sys;
	errors[23].index = 24;
	errors[23].add_template = 0;
	errors[23].string = s_err_emu_exists;
	errors[24].index = 25;
	errors[24].add_template = 0;
	errors[24].string = s_err_emu_cmplx;
	errors[25].index = 26;
	errors[25].add_template = 0;
	errors[25].string = s_err_emu_cmplx;
	errors[26].index = 27;
	errors[26].add_template = 0;
	errors[26].string = s_err_emu_solve;
	errors[27].index = 28;
	errors[27].add_template = 1;
	errors[27].string = s_err_emu_range;
	errors[28].index = 29;
	errors[28].add_template = 1;
	errors[28].string = s_err_emu_iter;
	errors[29].index = 30;
	errors[29].add_template = 1;
	errors[29].string = s_err_emu_cond;
	errors[30].index = 31;
	errors[30].add_template = 0;
	errors[30].string = s_err_emu_blank;
	errors[31].index = 32;
	errors[31].add_template = 0;
	errors[31].string = s_err_emu_blank;
	errors[32].index = 33;
	errors[32].add_template = 1;
	errors[32].string = s_err_emu_range;
	errors[33].index = 34;
	errors[33].add_template = 1;
	errors[33].string = s_err_emu_circular;
	errors[34].index = 35;
	errors[34].add_template = 0;
	errors[34].string = s_err_emu_imroot;
	errors[35].index = 36;
	errors[35].add_template = 1;
	errors[35].string = s_err_emu_ver;
	errors[36].index = 37;
	errors[36].add_template = 1;
	errors[36].string = s_err_emu_sd;
	errors[37].index = 38;
	errors[37].add_template = 0;
	errors[37].string = s_err_emu_sdro;
	errors[38].index = 39;
	errors[38].add_template = 0;
	errors[38].string = s_err_emu_sdinvalid;
	errors[39].index = 40;
	errors[39].add_template = 0;
	errors[39].string = s_err_emu_nosd;
	errors[40].index = 41;
	errors[40].add_template = 0;
	errors[40].string = s_err_emu_blank;
	errors[41].index = 42;
	errors[41].add_template = 0;
	errors[41].string = s_err_emu_blank;
	errors[42].index = 43;
	errors[42].add_template = 0;
	errors[42].string = s_err_emu_blank;
	errors[43].index = 44;
	errors[43].add_template = 0;
	errors[43].string = s_err_emu_blank;
	errors[44].index = 45;
	errors[44].add_template = 0;
	errors[44].string = s_err_emu_blank;
	errors[45].index = 46;
	errors[45].add_template = 0;
	errors[45].string = s_err_emu_blank;
	errors[46].index = 47;
	errors[46].add_template = 0;
	errors[46].string = s_err_emu_timeout;

	init_emu_kb(&kb);
	switch (idx) {
		case ERROR_ACBREAK:
			idx = 1;  // ACBREAK
			break;
		case ERROR_SYNTAX:
			idx = 2;  // SyntaxERROR
			break;
		case ERROR_MATH:
			idx = 3;  // MaERROR
			break;
		case ERROR_OUTOFMEM:
			idx = 4;  // MemoryERROR
			break;
		case ERROR_STACK:
			idx = 7;  // StackERROR
			break;
		case ERROR_ARGUMENT:
			idx = 8;  // ArgumentERROR
			break;
		case ERROR_DIMENSION:
			idx = 9;  // DimensionERROR
			break;
		case ERROR_SOLVE:
			idx = 27;  // Can't Solve!
			break;
		case ERROR_TIMEOUT:
			idx = 47;  // Time out
			break;
		case ERROR_VARIABLE:
			idx = 20;  // No Variable
			break;
		default:
			idx = 23;  // SystemERROR
			break;
	}
	error_buf = kb.error_buf;
	if (!idx) smart_strcpy(error_buf, s_err_emu_blank);
	else {
		for (i = 0; i < 47; i++) {
			if (errors[i].index == idx) {
				smart_strcpy(error_buf, errors[i].string);
				if (errors[i].add_template) smart_strcat(error_buf, s_err_emu_template);
				f_082A2_E(&kb);
				return;
			}
		}
		smart_strcpy(error_buf, s_err_emu_unknown);  // ??? ERROR (unused failsafe)
		f_082A2_E(&kb);
	}
	return;
}
#endif

// FUNCTION: GY454XE  Re 0B7E6
// FUNCTION: GY455XE  Im 0C59A
// FUNCTION: GY460XF  Im 0C090
static char show_error(char idx) {
	char keycode;

#if REAL == 0
	log_error(idx);
#endif
	print_error(idx);
	do keycode = getkeycode(1);
	while (!is_ac_keycode(keycode));
	return keycode;
}

// FUNCTION: GY454XE  Re 0B804
// FUNCTION: GY455XE  Im 0C5C6
// FUNCTION: GY460XF  Im 0C0BC
char diag_initloop(void) {
	char v0;
	int v1;
	scancode v2;
	scancode v3;

	v0 = 0;
	v1 = 2381;
	v2.b.ki = 0x80;
	v2.b.ko = 1;
j_0b81c:
	if (v1--) {
		delay(20);
		if (!check_key_kio(&v2)) {
j_0b850:
			clr_all_ko();
			clr_all_kimask();
			delay(1);
			if (get_key_kio(&v3) && check_key_kio2(&v3)) {
				if (v3.b.ki == 4 && v3.b.ko == 4) {
					v0 = 1;
					goto j_0b838;
				} else if (v3.b.ki != 4 || v3.b.ko != 0x10) {
					v2.kio = v3.kio;
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

// FUNCTION: GY454XE  Re 0B8B8
// FUNCTION: GY455XE  Im 0C67A
// FUNCTION: GY460XF  Im 0C170
void f_0B8B8(char a) {
	char v0;

	v0 = 1;
	if (a & (1 << 7)) v0 = 0;

	a &= 0xf;
	d_08120 = 0;
	d_080FE = 1;
	result_template = 0;
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
	update_cursor_char();
	d_08117 = font_size;
	f_044CE();
	d_08124 = 0;
	use_output_charset = 0;
	input_area_ptr = input_area;
	if (a == 2) print_result_0();
	return;
}

// FUNCTION: GY454XE  Re 0B968
// FUNCTION: GY455XE  Im 0C72A
// FUNCTION: GY460XF  Im 0C220
static char get_storcl_tok(char keycode) {
	if (keycode == K_M_PLUS) return 0x99;  // M+
	else if (keycode == K_M_MINUS) return 0xa9;  // M-
	else return vars_map[keycode - K_RCL_A];
}

// FUNCTION: GY454XE  Re 0B984
// FUNCTION: GY455XE  Im 0C746
// FUNCTION: GY460XF  Im 0C23C
static char get_func_tok(char keycode) {
	if (keycode < TOKENS_MAP_START || keycode > K_SUM) return 0;
	else return tokens_map[keycode - TOKENS_MAP_START];
}

// FUNCTION: GY454XE  Re 0B998
// FUNCTION: GY455XE  Im 0C75A
// FUNCTION: GY460XF  Im 0C250
static void print_result_0(void) {
	char num[10];

	if (!is_table_func_input()) {
#if ENABLE_VERIF == 1
		if (mode == MODE_VERIF) verif_draw_truefalse();
		else
#endif
		if (!is_mathi()) {
			num_fromdigit(&num, 0);
			num_output_print(&num);

		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0B9C8
// FUNCTION: GY455XE  Im 0C78A
// FUNCTION: GY460XF  Im 0C280
static char f_0B9C8(f_09962_struct *a) {
	f_0B8B8(2);
	if (a->unk_0x07) {
		f_044B6();
		num_output_print(a->result);
		a->unk_0x04 = 1;
		return 0;
	} else {
		clear_result();
		if (a->mode == MODE_TABLE) {
			if (d_080FD) smart_strcpy(a->input_area, cache_area);
			else memzero(cache_area, 100);
			buffer_clear();
		}
		f_0AF16();
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0BA28
// FUNCTION: GY455XE  Im 0C7EA
// FUNCTION: GY460XF  Im 0C2E0
static void f_0BA28(f_09962_struct *a) {
	f_0B8B8(1);
	f_0AF0A();
	table_mode = TABLE_EQN;
#if ENABLE_INEQ == 1
	if (a->mode == MODE_INEQ) table_mode = TABLE_INEQ;
#endif
#if ENABLE_RATIO == 1
	if (a->mode == MODE_RATIO) table_mode = TABLE_RATIO;
#endif
	last_key_keycode = NULL;
	arrow_state = 0;
	a->unk_0x04 = 0;
	return;
}

// FUNCTION: GY454XE  Re 0BA50
// FUNCTION: GY455XE  Im 0C812
// FUNCTION: GY460XF  Im 0C324
static void f_0BA50(char m) {
	char tok;

	if (!f_02CB6() && table_mode == TABLE_NONE) {
		tok = last_key_keycode;
		// Fraction/nth root
		if (is_mathi() && (tok == 0xae || tok == 0x9f)) tok = 0;
		if (f_144F4(tok)) {
			tok = K_ANS;
			if (m == 6) tok = 0xcb;  // MatAns
			else if (m == 7) tok = 0xcf;  // VctAns
			insert_token(tok, 0);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0BAA8
// FUNCTION: GY455XE  Im 0C86A
// FUNCTION: GY460XF  Im 0C37C
static void f_0BAA8(char *input) {
	if (table_mode == TABLE_NONE && smart_strlen(input) == 1 && f_14516(input[0])) {
		input[1] = 0x8b;  // Ans
		input[2] = '\0';
	}
	return;
}

// FUNCTION: GY454XE  Re 0BADA
// FUNCTION: GY455XE  Im 0C89C
// FUNCTION: GY460XF  Im 0C3AE
static char is_mathi_mode(void) {
	if (f_02CB6())
j_0bae4:
		return 0;
	else if (mode & (1 << 7)) return 1;
	else goto j_0bae4;
}

// FUNCTION: GY454XE  Re 0BAF2
// FUNCTION: GY455XE  Im 0C8B4
// FUNCTION: GY460XF  Im 0C3C6
static char f_0BAF2(char **a) {
	*a = input_area;
	if (!f_02CB6()) {
		if (d_080FE & (1 << 6)) input_area_ptr = d_0812A;
		else {
			f_0BAA8(*a);
			if (!(mode & (1 << 7)) && (table_mode == TABLE_NONE || table_mode & (1 << 7))) smart_strcpy(cache_area, input_area);
		}
	}
	return 1;
}

// FUNCTION: GY454XE  Re 0BB42
// FUNCTION: GY455XE  Im 0C904
// FUNCTION: GY460XF  Im 0C416
static char f_0BB42(char **a) {
	char v0;
	char *v1;

	v0 = 1;
	v1 = input_area;
	*a = get_result_str_ptr();
	if (table_mode == TABLE_RANGE && d_080FD == 4) smart_strcpy(v1, cache_area);
	else if (!(d_080FE & (1 << 6))) {
		f_0BAA8(v1);
#if ENABLE_VERIF == 1
		verif_try_insert_eq0(v1);
#endif
		smart_strcpy(cache_area, v1);
	}
	if (!is_mathi()) {
		smart_strcpy(*a, v1);
		if (d_080FE & (1 << 6)) input_area_ptr += (char)(d_0812A - *a);
	} else if (v0 = f_06B52(v1)) {
		math2line(v1, *a, 0, 1);
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0BBDA
// FUNCTION: GY455XE  Im 0C99C
// FUNCTION: GY460XF  Im 0C4AE
static char *find_replay_entry(char *addr) {
	char v0;
	char *v1;

	v0 = 10;
	if (!(v1 = get_replay_addr())) return NULL;
	if (*addr++ & 0x80) v0 = 20;
	addr += 2;
	addr += v0;
	addr = strchr(addr, ':');
	if (addr > v1 + 250) return NULL;
	if (!addr) return NULL;
	++addr;
	return addr;
}

// FUNCTION: GY454XE  Re 0BC34
// FUNCTION: GY455XE  Im 0C9F6
// FUNCTION: GY460XF  Im 0C508
char get_num_replay_entries(void) {
	char count;
	char *addr;

	count = 0;
	if ((addr = get_replay_addr()) && (
		(mode != MODE_EQN || is_eqn_result())
#if ENABLE_RATIO == 1
		&& (mode != MODE_RATIO || is_ratio_result())
#endif
		)) {
		count = 0;
		do {
			if (!(addr = find_replay_entry(addr))) break;
			++count;
		} while (1);
	}
	return count;
}

// FUNCTION: GY454XE  Re 0BC6C
// FUNCTION: GY455XE  Im 0CA2E
// FUNCTION: GY460XF  Im 0C550
static char f_0BC6C(char *a) {
	char *v0;
	char *v1;

	v0 = a;
	do {
		v1 = a;
		if (!(a = find_replay_entry(a))) break;
	} while (1);
	return v1 - v0;
}

// FUNCTION: GY454XE  Re 0BC90
// FUNCTION: GY455XE  Im 0CA52
// FUNCTION: GY460XF  Im 0C574
void write_replay_entry(void) {
	char *v0;
	char v1;
	char v2;
	char *v3;
	char v4;
	char v5;
	char *v6;
	char loc_m1;
	char loc_m2;
	char *loc_m4;

	if (loc_m4 = get_replay_addr()) {
		v0 = input_area_ptr;
		v1 = 10;
		loc_m1 = result_template;
		if (num_invalid__(&result[10]) != 1) {
			loc_m1 |= 0x80;
			v1 = 20;
		}
		v2 = d_080FE & (1 << 6) ? strchr(v0, ':') - v0 : smart_strlen(v0);
		loc_m2 = v1 + v2 + 4;
		if (loc_m2 <= 250) {
			v3 = loc_m4;
			v4 = f_0BC6C(loc_m4);
			v5 = 0;
			while ((unsigned int)(v4 + loc_m2) > (unsigned int)(v5 + 250)) {
				v6 = v3;
				v3 = find_replay_entry(v3);
				v5 += v3 - v6;
			}
			v4 -= v5;
			memmove(loc_m4, v3, v4);
			loc_m4 += v4;
			memzero(loc_m4, v5);
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			loc_m1 = result_format;
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			loc_m1 = d_08101;
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			memcpy(loc_m4, result, v1);
			loc_m4 += v1; 
			memcpy(loc_m4, v0, v2);
			loc_m4 += v2;
			memcpy(loc_m4, s_colon, 1);
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0BDFA
// FUNCTION: GY455XE  Im 0CBBC
// FUNCTION: GY460XF  Im 0C6DE
static void read_replay_entry(void) {
	char v0;
	char *v1;
	char *v2;
	char v3;
	char v4;
	char v5;

	v0 = mode;
	v1 = result;
	v2 = get_replay_addr();
	if (v2 && (v3 = replay_idx)) {
		v4 = get_num_replay_entries();
		v5 = 0;
		if (v3 != 1 || (v4 <= 1 && v0 != MODE_EQN)) v5 = 1;
		if (v3 < v4) v5 |= 2;
		arrow_state = v5;
		f_0AF16();
		f_0B8B8(1);
		--v3;
		for (v4 = 1; v4 <= v3; v4++) v2 = find_replay_entry(v2);
		v4 = *v2++;
		v3 = 10;
		if (v4 & (1 << 7)) v3 = 20;
		v4 &= 0x7f;
		set_result_fmt(*v2++);
		d_08101 = *v2++;
		memcpy(v1, v2, v3);
		v2 += v3;
		if (v3 == 10) num_fromdigit(&v1[10], 0);
		d_080FE = 3;
		result_template = v4;
		d_080FE |= 1 << 5;
		v1 = input_area;
		v3 = strchr(v2, ':') - v2;
		memcpy(v1, v2, v3);
		v1[v3] = '\0';
		if (v0 == MODE_EQN) f_085D2();
	}
	return;
}

// FUNCTION: GY454XE  Re 0BEEE
// FUNCTION: GY455XE  Im 0CCB0
// FUNCTION: GY460XF  Im 0C7DA
static char _keyfunc_mov_x(f_09962_struct *a) {
	char v0;

	if (!is_mov_x_keycode(last_key_keycode))
j_0bf04:
		return 4;
	v0 = table_mode;
	if (d_080FE & (1 << 7)) {
		if (!f_02CB6() && (v0 & (1 << 7) || v0 == 6)) f_0AF16();
		f_0B8B8(0x82);
	} else if (
		a->mode != MODE_TABLE
		&& a->mode != MODE_EQN
#if ENABLE_INEQ == 1
		&& a->mode != MODE_INEQ
#endif
#if ENABLE_RATIO == 1
		&& a->mode != MODE_RATIO
#endif
		) {
		if (f_03660()) {
			f_0AF16();
			f_0B8B8(0x80);
		} else if (!(char)*a->input_area && v0 == 1) smart_strcpy(a->input_area, cache_area);
		else goto j_0bf04;
		cursor_pos_byte = last_key_keycode == K_RIGHT ? 0 : smart_strlen(a->input_area);
	} else goto j_0bf04;
	f_046AE();
	return 1;
}

// FUNCTION: GY454XE  Re 0BF8A
// FUNCTION: GY455XE  Im 0CD4C
// FUNCTION: GY460XF  Im 0C87E
static char f_0BF8A(void) {
	if (is_mov_x_keycode(last_key_keycode) && is_matho() && d_0812C) {
		f_046AE();
		if (last_key_keycode == K_RIGHT) ++cursor_pos_byte;
		else if (cursor_pos_byte) --cursor_pos_byte;
		else f_046C4();
		return 3;
	}
	return 4;
}

// FUNCTION: GY454XE  Re 0BFDC
// FUNCTION: GY455XE  Im 0CD9E
// FUNCTION: GY460XF  Im 0C8D0
static char _keyfunc_mov_y(f_09962_struct *a) {
	char v0;
	char v1;

	if (!is_mov_y_keycode(last_key_keycode))
j_0bff2:
		return 4;
	if (a->unk_0x0a) goto j_0bff2;
	v0 = f_03660();
	if (!v0 && *a->input_area) goto j_0bff2;
	v1 = get_num_replay_entries();
	if (v1 >= 1) {
		if (!(d_080FE & (1 << 5))) {
			if (last_key_keycode == K_UP) {
				replay_idx = v1;
				if (v0 && v1 > 1) replay_idx = v1 - 1;
			} else {
				if (!v0) replay_idx = 1;
				else goto j_0bff2;
			}
j_0c04a:
			read_replay_entry();
			return 2;
		} else if (last_key_keycode == K_UP) {
			if (--replay_idx < 1) replay_idx = 1;
			else goto j_0c04a;
		} else if (++replay_idx > v1) replay_idx = v1;
		else goto j_0c04a;
	}
	goto j_0bff2;
}

// FUNCTION: GY454XE  Re 0C084
// FUNCTION: GY455XE  Im 0CE46
// FUNCTION: GY460XF  Im 0C978
static void print_result_basic(void) {
	num_output_print(result);
}

// FUNCTION: GY454XE  Re 0C08C
// FUNCTION: GY455XE  Im 0CE4E
// FUNCTION: GY460XF  Im 0C980
static void f_0C08C(f_09962_struct *a) {

	if (is_table_func_input()) return;
	if (!a->unk_0x07) {
		char v0;
#if ENABLE_VERIF == 1
		if (mode == MODE_VERIF && !input_area[0] && !cache_area[0] && !mode_ram[0]) verif_draw_truefalse();
#endif
		if (a->mode != MODE_CMPLX) num_fromdigit(&a->result[10], 0);
		v0 = cursor_pos_byte;
		print_result(a->result);
		cursor_pos_byte = v0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0C0D0
// FUNCTION: GY455XE  Im 0CE92
// FUNCTION: GY460XF  Im 0C9C4
void f_0C0D0(void) {
	char v0;

	if (
		is_eqn_result()
#if ENABLE_RATIO == 1
		|| is_ratio_result()
#endif
		) f_10EF8();
	else if (table_mode == TABLE_SOLVE && d_080FD == 0x40) print_continue_prompt();
	else {
		v0 = f_02CB6();
		if (d_080FE == 1) {
			d_08122 = 1;
j_0c10a:
			if (!is_mathi()) input_print_linei();
			else {
				disable_ins();
#if ENABLE_VERIF == 1
				if (mode != MODE_VERIF || input_area[0] || last_key_keycode == K_DEL)
#endif
					input_print_mathi();
			}
		} else {
			d_08122 = 0;
			cursor_pos_byte = 0;
			if (table_mode == TABLE_NONE || table_mode == TABLE_RANGE || (table_mode & (1 << 7))) {
				if (v0) print_input_area();
				else goto j_0c10a;
			}
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0C148
// FUNCTION: GY455XE  Im 0CF0A
// FUNCTION: GY460XF  Im 0CA4C
// FUNCTION: GY465XG  Im 0C4C8
static char f_0C148(f_09962_struct *a) {
	char v0;

	v0 = d_080FE;
	d_080FE = 0x80;
	set_keycode(show_error(ERROR_MATH));
	clear_result();
	if (_keyfunc_mov_x(a) == 1) {
		if (v0 == 4) {
			f_0AF16();
			smart_strcpy(a->input_area, cache_area);
		} else cursor_pos_byte = smart_strlen(a->input_area);
	} else f_0B9C8(a);
}

// FUNCTION: GY454XE  Re 0C1A0
// FUNCTION: GY455XE  Im 0CF62
// FUNCTION: GY460XF  Im 0CAA4
static char f_0C1A0(f_09962_struct *a) {
	if (a->mode == MODE_BASE_N && submode == SMODE_BASE_N_BIN) {
		if (f_149D8(a->result)) return 4;
		else return f_0C148(a);
	} else return 4;
}

// FUNCTION: GY454XE  Re 0C1D4
// FUNCTION: GY455XE  Im 0CF96
// FUNCTION: GY460XF  Im 0CAD8
static char f_0C1D4(f_09962_struct *a) {
	char loc_m20[20];

	if (f_02C76() || get_result_disp_fmt() || d_0812C) return 4;
	num_cpy_im(loc_m20, a->result);
	if (num_invalid__(&loc_m20[10]) == 1 || !cmplx_abs(loc_m20)) return 4;
	return f_0C148(a);
}

// FUNCTION: GY454XE  Re 0C22E
// FUNCTION: GY455XE  Im 0CFF0
// FUNCTION: GY460XF  Im 0CB32
static char keyfunc_ac(f_09962_struct *a) {
	if (
		is_eqn_result()
#if ENABLE_INEQ == 1
		|| is_ineq_result()
#endif
#if ENABLE_RATIO == 1
		|| is_ratio_result()
#endif
		) {
		f_0BA28(a);
		return 0;
	} else if (!f_0B9C8(a)) return 0;
	else return 1;
}

// FUNCTION: GY454XE  Re 0C25A
// FUNCTION: GY455XE  Im 0D01C
// FUNCTION: GY460XF  Im 0CB6E
static char keyfunc_sto_rcl(f_09962_struct *a) {
	char v0;
	char v1;
	char v2;
	char v3;

	v0 = 0;
	v1 = last_key_keycode;
	v2 = is_rcl_keycode(v2);
	if (!v2) {
		if (a->unk_0x07)
j_0c27a:
			return 0;
		else if (
			a->mode != MODE_TABLE
#if ENABLE_VERIF == 1
			&& a->mode != MODE_VERIF
#endif
			&& !a->unk_0x0a) {
		} else goto j_0c27a;
	}
	set_char_keycode(get_storcl_tok(v1));
	f_0B736();
	if (f_03664()) {
		a->unk_0x07 = f_0A564();
		v3 = smart_strlen(a->input_area);
		if (v2) {
			if (v3) v0 = 1;
		} else if (v3 && v3 < 99) {
			cursor_pos_byte = v3;
		} else goto j_0c27a;
		insert_token(last_key_keycode, a->is_func);
		if (v2) {
			if (v0 == 1)
j_0c2e2:
				return 1;
			else if (
				a->mode != MODE_TABLE
#if ENABLE_VERIF == 1
				&& a->mode != MODE_VERIF
#endif
				&& !a->unk_0x07 && !a->unk_0x0a) {
			} else goto j_0c2e2;
		}
		v0 = K_APPROX;
		if (f_0B588()) v0 = K_EXECUTE;
		set_keycode(v0);
		return keyfunc_exe(a);
	} else goto j_0c27a;
}

// FUNCTION: GY454XE  Re 0C316
// FUNCTION: GY455XE  Im 0D0D8
// FUNCTION: GY460XF  Im 0CC2A
static char keyfunc_rcl(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// FUNCTION: GY454XE  Re 0C31A
// FUNCTION: GY455XE  Im 0D0DC
// FUNCTION: GY460XF  Im 0CC2E
static char keyfunc_sto(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// FUNCTION: GY454XE  Re 0C31E
// FUNCTION: GY455XE  Im 0D0E0
// FUNCTION: GY460XF  Im 0CC32
static char keyfunc_exe(f_09962_struct *a) {
	char v0;
	char v1;
	char *v2;
	char *loc_m2;
	char loc_m22[20];

	v0 = d_080FE & (1 << 6);
	if (
		is_eqn_result()
#if ENABLE_RATIO == 1
		|| is_ratio_result()
#endif
		) {
		v1 = get_num_replay_entries();
		if (++replay_idx > v1) {
			f_0BA28(a);
			goto j_0c396;
		} else {
			read_replay_entry();
			set_default_result_fmt();
		}
#if ENABLE_INEQ == 1
	} else if (is_ineq_result()) {
		if (get_result_disp_fmt()) {
			f_0BA28(a);
			goto j_0c396;
		} else {
			d_080FE = 3;
			set_default_result_fmt();
		}
#endif
	} else if (*a->input_area) {
		if (is_table_func_input()) {
			table_mode = TABLE_RANGE;
			d_080FD = 1;
			d_080FE = 3;
			smart_strcpy(cache_area, a->input_area);
j_0c390:
			a->unk_0x04 = 0;
j_0c396:
			return 0;
		} else {
			char v3;
			input_area_ptr = a->input_area;
			v1 = is_mathi_mode() ? f_0BB42(&loc_m2) : f_0BAF2(&loc_m2);
			v2 = loc_m2;
			if (v0) loc_m2 = d_0812A;
			num_cpy_im(loc_m22, a->result);
			if (v1) {
				f_04796();
				if (table_mode == TABLE_SOLVE && d_080FD == 4) v1 = num_solve(&loc_m2, a->result);
				else if (a->mode == MODE_TABLE && d_080FD == 4) {
					v1 = f_042AA(&loc_m2);
					if (!v1) {
						table_mode = TABLE_STAT_TABLE;
						f_0AF0A();
						goto j_0c390;
					}
				}
#if ENABLE_VERIF == 1
				else if (a->mode == MODE_VERIF) v1 = num_parse_verif(&loc_m2, a->result);
#endif
				else v1 = num_parse(&loc_m2, a->result);
				if (v1 > 0 && v1 < 32) {
					char tmp = loc_m2 - v2;
					if (a->is_mathi) math2line(a->input_area, v2, tmp, 0);
					else cursor_pos_byte = tmp;
				}
			} else v1 = ERROR_SYNTAX;
			if (v1 > 0 && v1 < 32) {
				d_080FE = 0x80;
				set_keycode(show_error(v1));
				clear_result();
				if (a->unk_0x07) num_cpy_im(a->result, loc_m22);
				if (_keyfunc_mov_x(a) == 1)
j_0c4a6:
					return 1;
				else if (f_0B9C8(a)) goto j_0c4a6;
				else goto j_0c396;
 			}
 			f_044B6();
 			set_default_result_fmt();
 			d_080FE = 3;
 			v3 = get_numtype(a->result);
 			// Matrix/Vector pointer
 			if (v3 == 0x60 || v3 == 0x90) {
 				char tmp;
 				if (v3 == 0x60) tmp = TABLE_MATRIX;
 				else tmp = TABLE_VECTOR;
 				table_mode = tmp;
 				f_15486(a->result);
 				submode = SMODE_MATVCT_ANS;
 				table_home();
 				goto j_0c390;
 			} else {
 				if (!v1 || v1 == 36) {
	 				if (table_mode == TABLE_SOLVE && !a->unk_0x0a) {
	 					char tmp;
	 					result_template = RESTMP_SOLVE;
	 					if (v1 == 36) tmp = 0x40;
	 					else tmp = 0x20;
	 					d_080FD = tmp;
	 				} else if (a->mode != MODE_CMPLX) num_fromdigit(&a->result[10], 0);
	 			} else if (v1 == 34) {
	 				if (a->mode != MODE_CMPLX) result_template = RESTMP_POL;
	 				else d_08101 = 2;
	 			} else if (v1 == 35) {
	 				if (a->mode != MODE_CMPLX) result_template = RESTMP_REC;
	 				else d_08101 = 1;
	 			} else if (v1 == 37) {
	 				if (a->result[10] == 0x70) num_fromdigit(&a->result[10], 0);
	 				else result_template = RESTMP_REMAINDER;
	 			}
	 			if (a->unk_0x0a || (table_mode != 1 && !(table_mode & (1 << 7)))) {
	 				if (result_template & (1 << 4)) num_fromdigit(&a->result[10], 0);
	 				result_template = 0;
	 				table_set_cur_cell(a->result);
 					goto j_0c390;
	 			} else {
	 				if (result_template & (1 << 4)) {
	 					num_cpy(loc_m22, a->result);
	 					num_fromdigit(&loc_m22[10], 0);
	 					st_var(VAR_ANS, loc_m22);
	 				} else st_var(VAR_ANS, a->result);
	 				if (table_mode != TABLE_SOLVE) {
	 					if (loc_m2[-1] == ':') {
	 						v0 = 1;
	 						d_080FE |= 1 << 6;
	 						d_0812A = loc_m2;
	 					} else if (v0) v0 = 0;
	 					if (!v0 && table_mode & (1 << 7) && d_080FD == 3) d_080FD = 0x10;
	 					write_replay_entry();
	 				}
	 			}
	 		}
		}
	} else goto j_0c396;
	return 2;
}

// FUNCTION: GY454XE  Re 0C64A
// FUNCTION: GY455XE  Im 0D40C
// FUNCTION: GY460XF  Im 0CF8C
static char keyfunc_fmt_dec(f_09962_struct *a) {
	char v0;
	char v1;
	char v2;

	if (result_template & (1 << 4))
j_0c658:
		return 0;
	v0 = get_result_disp_fmt();
	v1 = RESULT_DECIMAL;
	v2 = get_result_store_fmt();
	if (v0 <= RESULT_ENG4) v1 = RESULT_STANDARD;
	else if (v0 == RESULT_DECIMAL) {
		if (v2 >= RESULT_STANDARD) {
			if (!a->is_matho) goto j_0c658;
		} else v1 = RESULT_STANDARD;
	}
#if ENABLE_RDEC == 1
	else if (f_045C2_460F()) {
		if (v0 == RESULT_FRAC || v0 == RESULT_FRAC_MIX) v1 = RESULT_RDEC;
	}
#endif
	set_result_store_fmt(v1);
	return 2;
}

// FUNCTION: GY454XE  Re 0C692
// FUNCTION: GY455XE  Im 0D454
// FUNCTION: GY460XF  Im 0CFE8
static char keyfunc_fmt_frac(f_09962_struct *a) {
	char v0;
	char v1;
	char v2;

	v0 = get_result_disp_fmt();
	v1 = get_result_store_fmt();
	v2 = RESULT_FRAC_MIX;
	if (v0 <= RESULT_DECIMAL) {
		if (v0 != RESULT_DMS && v1 > RESULT_DECIMAL && v1 <= RESULT_STANDARD)
j_0c6b4:
			return 0;
		else if (setup_frac_result) v2 = RESULT_FRAC;
	} else if (v0 != RESULT_FRAC) {
		if (v0 == RESULT_FRAC_MIX) v2 = RESULT_FRAC;
		else if (v0 == RESULT_STANDARD) goto j_0c6b4;
	}
	set_result_store_fmt(v2);
	return 2;
}

// FUNCTION: GY454XE  Re 0C6DE
// FUNCTION: GY455XE  Im 0D4A0
// FUNCTION: GY460XF  Im 0D034
static char keyfunc_dms(f_09962_struct *a) {
	if (result_template & (1 << 4))
j_0c6ea:
		return 0;
	if (num_invalid__(&a->result[10]) == 1) {
		char tmp;
		char tmp2;
		if (!f_02C76() && num_invalid__(a->result) == 2) goto j_0c6ea;
		tmp = get_result_disp_fmt();
		tmp2 = RESULT_DMS;
		if (tmp == RESULT_DMS) tmp2 = RESULT_DECIMAL;
		set_result_store_fmt(tmp2);
		return 2;
	} else goto j_0c6ea;
}

// FUNCTION: GY454XE  Re 0C728
// FUNCTION: GY455XE  Im 0D4EA
// FUNCTION: GY460XF  Im 0D086
// FUNCTION: GY465XG  Im 0CAE8
static char keyfunc_fact(f_09962_struct *a) {
#if ENABLE_FACT == 1
	if (!f_08D3A_460F())
j_0caf8_465g:
		return 0;
	else if (f_1ADB6(a->result)) return f_0C148(a);
	else {
		char tmp, tmp2, tmp3;
		tmp = RESULT_FACT;
		tmp2 = get_result_disp_fmt();
		tmp3 = get_result_store_fmt();
		if (tmp2 == RESULT_FACT) tmp = RESULT_DECIMAL;
		else if (tmp3 == RESULT_FACT) goto j_0caf8_465g;
		set_result_store_fmt(tmp);
		return 2;
	}
#else
	return 0;
#endif
}

// FUNCTION: GY454XE  Re 0C72C
// FUNCTION: GY455XE  Im 0D4EE
// FUNCTION: GY460XF  Im 0D08A
static char f_0C72C(f_09962_struct *a) {
	if (a->mode == MODE_CMPLX)
j_0c736:
		return 1;
	if (
		!(result_template & (1 << 4))
		&& !is_eqn_result()
#if ENABLE_INEQ == 1
		&& !is_ineq_result()
#endif
		) return 0;
	else goto j_0c736;
}

// FUNCTION: GY454XE  Re 0C74C
// FUNCTION: GY455XE  Im 0D50E
// FUNCTION: GY460XF  Im 0D0B2
static char keyfunc_eng(f_09962_struct *a) {
	char v0;

	if (f_0C72C(a)) return 0;
	v0 = get_result_disp_fmt();
	if (v0 >= RESULT_ENGM4 && v0 <= RESULT_ENG4) {
		--v0;
		if (v0 < RESULT_ENGM4) v0 = RESULT_ENGM4;
	} else v0 = RESULT_ENGM1;
	set_result_store_fmt(v0);
	return 2;
}

// FUNCTION: GY454XE  Re 0C77A
// FUNCTION: GY455XE  Im 0D53C
// FUNCTION: GY460XF  Im 0D0E0
static char keyfunc_eng_r(f_09962_struct *a) {
	char v0;

	if (f_0C72C(a)) return 0;
	v0 = get_result_disp_fmt();
	if (v0 >= RESULT_ENGM4 && v0 <= RESULT_ENG4) {
		++v0;
		if (v0 > RESULT_ENG4) v0 = RESULT_ENGM4;
	} else v0 = RESULT_ENG1;
	set_result_store_fmt(v0);
	return 2;
}

// FUNCTION: GY454XE  Re 0C7A8
// FUNCTION: GY455XE  Im 0D56A
// FUNCTION: GY460XF  Im 0D10E
static char keyfunc_del(f_09962_struct *a) {
	if (!f_03664()) return 0;
	else {
		process_cursor_action(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C7C2
// FUNCTION: GY455XE  Im 0D584
// FUNCTION: GY460XF  Im 0D128
static char keyfunc_mov_x(f_09962_struct *a) {
	if (f_0BF8A() == 3) return 3;
	else if (d_080FE & (1 << 6)) return 0;
	else if (_keyfunc_mov_x(a) == 1) return 1;
	else if (!f_03664()) return 0;
	else {
		process_cursor_action(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C806
// FUNCTION: GY455XE  Im 0D5C8
// FUNCTION: GY460XF  Im 0D16C
static char keyfunc_mov_y(f_09962_struct *a) {
	if (d_080FE & (1 << 6)) return 0;
	else if (_keyfunc_mov_y(a) == 2) return 2;
	else if (!f_03664()) return 0;
	else {
		process_cursor_action(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C836
// FUNCTION: GY455XE  Im 0D5F8
// FUNCTION: GY460XF  Im 0D19C
static char keyfunc_base(f_09962_struct *a) {
	submode = base_n_submodes[last_key_keycode - K_BASE_BIN];
	if (f_03664()) {
		if (f_0C1A0(a) == 1) return 1;
		else print_result_basic();
	}
	return 2;
}

// FUNCTION: GY454XE  Re 0C86A
// FUNCTION: GY455XE  Im 0D62C
// FUNCTION: GY460XF  Im 0D1D0
static char keyfunc_mode(f_09962_struct *a) {
	print_result_0();
	return 2;
}

// FUNCTION: GY454XE  Re 0C874
// FUNCTION: GY455XE  Im 0D636
// FUNCTION: GY460XF  Im 0D1DA
static char keyfunc_setup(f_09962_struct *a) {
	if (is_matho()) f_085D2();
	if (f_03664()) {
		if (a->is_mathi)
j_0c894:
			return 1;
		else if (f_0C1D4(a) == 1) f_0C08C(a);
		else goto j_0c894;
	}
	return 2;
}
