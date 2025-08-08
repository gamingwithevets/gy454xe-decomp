#ifndef CONSTS_H
#define CONSTS_H

// Possibly a coordinate-related union?
typedef union {
	unsigned int word[2];
	char byte[4];
	long dword;
} mathi_bbox;

typedef union {
	struct {
		char ki;
		char ko;
	} b;
	unsigned int kio;
} scancode;

typedef union {
	struct {
		char m;
		char n;
	} b;
	unsigned int mn;
} dim;

// Struct used by num_to_str functions and some functions in generals.
typedef struct {
	char num[15];
	char negative;
	int exponent;
	int unk_0x12;
	char unk_0x14;
	char unk_0x15;
	char use_tiny_font;
	char unk_0x17;
} num_struct;

// Mode constants (mode 80F9H)
enum mode {
	MODE_COMP	= 0xC1,
	MODE_BASE_N	= 0x02,
	MODE_STAT	= 0x03,
	MODE_CMPLX	= 0xC4,
	MODE_EQN	= 0x45,
	MODE_MATRIX	= 0x06,
	MODE_VECTOR	= 0x07,
	MODE_TABLE	= 0x88,
	MODE_VERIF	= 0x89,
	MODE_RATIO	= 0x4A,
	MODE_INEQ	= 0x4B
};

// Submode constants (submode 80FAH)

// BASE-N
enum submode_base_n {
	SMODE_BASE_N_BIN	= 1,
	SMODE_BASE_N_OCT	= 7,
	SMODE_BASE_N_DEC	= 9,
	SMODE_BASE_N_HEX	= 15
};

// STAT
enum submode_stat {
	SMODE_STAT_1VAR			= 1,
	SMODE_STAT_REG_LIN		= 2,
	SMODE_STAT_REG_QUAD		= 3,
	SMODE_STAT_REG_LOG		= 4,
	SMODE_STAT_REG_E_EXP	= 5,
	SMODE_STAT_REG_AB_EXP	= 6,
	SMODE_STAT_REG_POW		= 7,
	SMODE_STAT_REG_INV		= 8
};

// EQN
enum submode_eqn {
	SMODE_EQN_SIMUL2	= 1,
	SMODE_EQN_SIMUL3	= 2,
	SMODE_EQN_POLY2		= 3,
	SMODE_EQN_POLY3		= 4
};

// MATRIX/VECTOR
enum submode_matvct {
	SMODE_MATVCT_A		= 0,
	SMODE_MATVCT_B		= 1,
	SMODE_MATVCT_C		= 2,
	SMODE_MATVCT_ANS	= 3
};

// Table constants (table_mode 80FCH)
enum table_mode {
	TABLE_NONE			= 0x01,
	TABLE_RANGE			= 0x06,
	TABLE_STAT_TABLE	= 0x12,
	TABLE_MATRIX		= 0x13,
	TABLE_VECTOR		= 0x14,
	TABLE_EQN			= 0x15,
	TABLE_RATIO			= 0x17,
	TABLE_INEQ			= 0x18,
	TABLE_CALC			= 0xA0,
	TABLE_SOLVE			= 0xC0
};

// Keycodes (last_key_keycode 80F5H)
enum keycode {
	K_SD		= 0x01,  // STAT/DIST mode menu
	K_CMPLX		= 0x02,  // CMPLX mode menu
	K_BASE		= 0x03,  // BASE-N mode menu
	K_MATRIX	= 0x04,  // MATRIX mode menu
	K_VECTOR	= 0x05,  // VECTOR mode menu
	K_CONST		= 0x06,  // Constant input
	K_CONV		= 0x07,  // Conversion input
	K_CLR		= 0x08,  // Data clear menu
	K_HYP		= 0x09,  // Hyperbolic function select
	K_DRG		= 0x0A,  // Angle unit select
	K_VERIFY	= 0x0B,  // VERIF mode menu
	K_ATOMIC	= 0x0C,  // Atomic weight menu
	K_RCL_A		= 0x0E,  // Variable recall: A
	K_RCL_B		= 0x0F,  // Variable recall: B
	K_RCL_C		= 0x10,  // Variable recall: C
	K_RCL_D		= 0x11,  // Variable recall: D
	K_RCL_E		= 0x12,  // Variable recall: E
	K_RCL_F		= 0x13,  // Variable recall: F
	K_RCL_X		= 0x14,  // Variable recall: X
	K_RCL_Y		= 0x15,  // Variable recall: Y
	K_RCL_M		= 0x16,  // Variable recall: M
	K_STO_A		= 0x17,  // Variable store: A
	K_STO_B		= 0x18,  // Variable store: B
	K_STO_C		= 0x19,  // Variable store: C
	K_STO_D		= 0x1A,  // Variable store: D
	K_STO_E		= 0x1B,  // Variable store: E
	K_STO_F		= 0x1C,  // Variable store: F
	K_STO_X		= 0x1D,  // Variable store: X
	K_STO_Y		= 0x1E,  // Variable store: Y
	K_STO_M		= 0x1F,  // Variable store: M
	K_PERCENT	= 0x25,  // Percent sign (%)
	K_PAREN_L	= 0x28,  // Opening parenthesis (
	K_PAREN_R	= 0x29,  // Closing parenthesis )
	K_PLUS		= 0x2B,  // Plus sign (+)
	K_COMMA		= 0x2C,  // Function argument seperator (comma/semicolon depending on model)
	K_MINUS		= 0x2D,  // Minus sign (not to be confused with the negative sign)
	K_PERIOD	= 0x2E,  // Decimal point (dot/comma depending on model)
	K_0			= 0x30,  // Number 0
	K_1			= 0x31,  // Number 1
	K_2			= 0x32,  // Number 2
	K_3			= 0x33,  // Number 3
	K_4			= 0x34,  // Number 4
	K_5			= 0x35,  // Number 5
	K_6			= 0x36,  // Number 6
	K_7			= 0x37,  // Number 7
	K_8			= 0x38,  // Number 8
	K_9			= 0x39,  // Number 9
	K_COLON		= 0x3A,  // Multi-statement colon
	K_EQUALS	= 0x3D,  // Equals sign (=) used in equations
	K_VAR_A		= 0x41,  // Variable A
	K_VAR_B		= 0x42,  // Variable B
	K_VAR_C		= 0x43,  // Variable C
	K_VAR_D		= 0x44,  // Variable D
	K_VAR_E		= 0x45,  // Variable E
	K_VAR_F		= 0x46,  // Variable F
	K_MUL		= 0x4E,  // Multiply sign (×)
	K_DIV		= 0x4F,  // Division sign (÷)
	K_VAR_M		= 0x54,  // Variable M
	K_FCTRIAL	= 0x57,  // Factorial (x!)
	K_VAR_X		= 0x58,  // Variable X
	K_VAR_Y		= 0x59,  // Variable Y
	K_NEGATIVE	= 0x60,  // Negative sign (-)
	K_LOG		= 0x68,  // log₁₀ n
	K_POL		= 0x6C,  // Pol( function
	K_REC		= 0x6D,  // Rec( function
	K_EXP		= 0x74,  // ×10 character
	K_EULER		= 0x81,  // Euler's number (e)
	K_PI		= 0x82,  // Pi
	K_ANS		= 0x8B,  // Ans variable
	K_RAN		= 0x8C,  // Ran#
	K_SIN		= 0xA0,  // Sine function
	K_COS		= 0xA1,  // Cosine function
	K_TAN		= 0xA2,  // Tangent function
	K_LN		= 0xA3,  // Natural logarithm function
	K_CONV_N	= 0xA5,  // >Conv n (exclusive to Japanese models)
	K_ARCSIN	= 0xB0,  // sin⁻¹ / Asn / Arcsin
	K_ARCCOS	= 0xB1,  // cos⁻¹ / Acs / Arccos
	K_ARCTAN	= 0xB2,  // tan⁻¹ / Atn / Arctan
	K_RND		= 0xB3,  // Rnd( function
	K_HEX_A		= 0xB8,  // Hexadecimal A
	K_HEX_B		= 0xB9,  // Hexadecimal B
	K_HEX_C		= 0xBA,  // Hexadecimal C
	K_HEX_D		= 0xBB,  // Hexadecimal D
	K_HEX_E		= 0xBC,  // Hexadecimal E
	K_HEX_F		= 0xBD,  // Hexadecimal F
	K_PERMU		= 0xBE,  // nPr
	K_COMBI		= 0xBF,  // nCr
	K_RANINT	= 0xC2,  // RanInt#( function
	K_FRAC		= 0xD0,  // Fraction
	K_FRAC_ABC	= 0xD1,  // Mixed fraction
	K_POW		= 0xD2,  // xⁿ
	K_SQRT		= 0xD3,  // Square root √
	K_POW_M1	= 0xD4,  // x⁻¹
	K_POW_2		= 0xD5,  // x²
	K_POW_3		= 0xD6,  // x³
	K_CBRT		= 0xD7,  // Cube root ∛
	K_NTH_RT	= 0xD8,  // nth root ⁿ√
	K_E_POW		= 0xD9,  // eⁿ
	K_10_POW	= 0xDA,  // 10ⁿ
	K_LOGAB		= 0xDB,  // logₐ b
	K_ABS		= 0xDC,  // Absolute value
	K_INTEGRAL	= 0xDD,  // Integral function (∫)
	K_DDX		= 0xDE,  // Derivative function (d/dx)
	K_SUM		= 0xDF,  // Summation function (∑)
	K_UP		= 0xE0,  // Cursor Key Up
	K_DOWN		= 0xE1,  // Cursor Key Down
	K_RIGHT		= 0xE2,  // Cursor Key Right
	K_LEFT		= 0xE3,  // Cursor Key Left
	K_MODE		= 0xE4,  // MODE menu
	K_SETUP		= 0xE5,  // SETUP menu
	K_AC		= 0xE6,  // AC key
	K_OFF		= 0xE7,  // Shutdown
	K_ALPHA		= 0xE8,  // ALPHA modifier toggle
	K_SHIFT		= 0xE9,  // SHIFT modifier toggle
	K_RCL		= 0xEA,  // RCL modifier toggle
	K_STO		= 0xEB,  // STO modifier toggle
	K_INS		= 0xEC,  // Insert mode toggle (LineI only)
	K_APPROX	= 0xED,  // Force decimal result *
	K_M_PLUS	= 0xEE,  // M+
	K_M_MINUS	= 0xEF,  // M-
	K_EXECUTE	= 0xF0,  // Execute key **
	K_FACT		= 0xF1,  // Prime factor display
	K_BASE_BIN	= 0xF2,  // BASE-N: Binary
	K_BASE_OCT	= 0xF3,  // BASE-N: Octal
	K_BASE_DEC	= 0xF4,  // BASE-N: Decimal
	K_BASE_HEX	= 0xF5,  // BASE-N: Hexadecimal
	K_DMS		= 0xF6,  // Degrees Minutes Seconds
	K_DMS_R		= 0xF7,  // Ditto but reverse (pretty much useless),
	K_ENG		= 0xF8,  // Engineering notation. Divides the result by 10^-3 on each press (max 10^-9)
	K_ENG_R		= 0xF9,  // Ditto but reverse. Multiples the result by 10^3 on each press (max 10^9),
	K_FMT_DEC	= 0xFA,  // Standard/decimal format toggle [S⇔D]
	K_FMT_FRAC	= 0xFB,  // Improper fraction/Mixed fraction toggle
	K_CALC		= 0xFC,  // CALC
	K_SOLVE		= 0xFD,  // SOLVE
	K_DEL		= 0xFE   // DEL
};

/*

* The decimal result function ([SHIFT] [=]) is present since ES models, but up until ClassWiz models, it is not
labeled on the calculator's front side, and is only mentioned in the User's Guide for some reason. 

** The Execute key is labeled as [=] on most ES, ES PLUS and ClassWiz EX models, and labeled as [EXE] on French . However,
most ClassWiz CW models had this key renamed to [EXE] to avoid confusion with the equals sign.

*/

// Setup constants

// Number format constants (setup_num_fmt 8102H)
enum num_format {
	NUM_FMT_FIX		= 8,
	NUM_FMT_SCI		= 9,
	NUM_FMT_NORM1	= 0,
	NUM_FMT_NORM2	= 4
};

// Angle unit constants (setup_angle_unit 8105H)
enum angle_unit {
	ANGLE_DEG	= 4,
	ANGLE_RAD	= 5,
	ANGLE_GRA	= 6
};

// Fraction Result (setup_frac_result 8107H)
enum frac_result {
	FRAC_RESULT_IMP = 0,
	FRAC_RESULT_MIX = 1
};

// Errors
enum error {
	ERROR_ACBREAK		= 1,
	ERROR_SYNTAX		= 2,
	ERROR_MATH			= 3,
	ERROR_OUTOFMEM		= 4,
	ERROR_STACK			= 7,
	ERROR_ARGUMENT		= 8,
	ERROR_DIMENSION		= 9,
	ERROR_SOLVE			= 10,
	ERROR_TIMEOUT		= 11,
	ERROR_VARIABLE		= 12,
	ERROR_NULL			= 13
};

// Variables
enum var {
	VAR_M		= 0,
	VAR_ANS		= 1,
	VAR_A		= 2,
	VAR_B		= 3,
	VAR_C		= 4,
	VAR_D		= 5,
	VAR_E		= 6,
	VAR_F		= 7,
	VAR_X		= 8,
	VAR_Y		= 9,
	VAR_PREANS	= 10
};

// Result template (result_template 80FFH)
enum result_template {
	RESTMP_REC			= 0x11,
	RESTMP_POL			= 0x12,
	RESTMP_SOLVE		= 0x13,
	RESTMP_REMAINDER	= 0x14,
	RESTMP_SIMP			= 0x15
};

// Result format (result_format 8100H)
enum result_fmt {
	RESULT_DMS		= 1,
	RESULT_ENGM4	= 2,
	RESULT_ENGN3	= 3,
	RESULT_ENGM2	= 4,
	RESULT_ENGM1	= 5,
	RESULT_ENG1		= 6,
	RESULT_ENG2		= 7,
	RESULT_ENG3		= 8,
	RESULT_ENG4		= 9,
	RESULT_DECIMAL	= 10,
	RESULT_FRAC		= 11,
	RESULT_FRAC_MIX	= 12,
	RESULT_STANDARD	= 13,
	RESULT_RDEC		= 14,
	RESULT_FACT		= 15
};

// RAM addresses

extern void *ram_start;
extern unsigned int d_08000;
extern unsigned int d_08002;
extern char d_08004;
extern char d_08005;
extern char d_08006;
extern char d_08007;
extern char d_08008;
extern char mathi_enable_draw;
extern char d_0800A;
extern char d_080DC;
extern char cursor_noflash;
extern char d_080DE;
extern char d_080DF;
extern dim matvct_dims[9];
extern scancode last_key_scancode;
extern char last_key_keycode;
extern char force_nochar;
extern char modifiers;
extern char mode;
extern char submode;
extern char screen_state;
extern char table_mode;
extern char d_080FD;
extern char d_080FE;
extern char result_template;
extern char result_format;
extern char d_08101;
extern char setup_num_fmt;
extern char setup_num_fmt_n;
extern char setup_decimal_mark;
extern char setup_angle_unit;
extern char setup_mathi;
extern char setup_frac_result;
extern char setup_cmplx_result;
extern char setup_stat_freq;
extern char setup_rdec;
extern char setup_decimalo;
extern char setup_contrast;
extern char replay_idx;
extern char cursor_pos_byte;
extern char d_08111;
extern char formula_x;
extern char formula_y;
extern char cursor_x;
extern char cursor_y;
extern char cursor_char;
extern char d_08117;
extern char table_viewport;
extern char table_y;
extern char table_x;
extern char font_size;
extern char draw_mode;
extern char use_rambuf;
extern char d_08120;
extern char d_08122;
extern char use_output_charset;
extern char d_08124;
extern char arrow_state;
extern char d_08126;
extern char *input_area_ptr;
extern char *d_0812A;
extern char d_0812C;
extern char d_0812D;
extern char d_0812E;
extern char result[20];
extern char input_area[100];
extern char cache_area[100];
extern int timer;
extern void *vars_start;
extern char mode_ram[880];
extern char magic_string[16];
extern mathi_bbox d_08640[100];
extern void *stack_start;

// SFR address defines
#define KID_ADDR 0xF040
#define KOD_ADDR 0xF046

#endif
