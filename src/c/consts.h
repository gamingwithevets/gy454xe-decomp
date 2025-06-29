#ifndef CONSTS_H
#define CONSTS_H

#include "ML610CASESplus.H"

// Struct used by f_058DC and its child functions. Also used by f_089AC.
typedef union {
	int word[2];
	char byte[4];
} f_058DC_union;

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
	K_EXP		= 0x74,  // ×₁₀ character
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
	K_HEX_A		= 0xB3,  // Hexadecimal A
	K_HEX_B		= 0xB8,  // Hexadecimal B
	K_HEX_C		= 0xB9,  // Hexadecimal C
	K_HEX_D		= 0xBA,  // Hexadecimal D
	K_HEX_E		= 0xBB,  // Hexadecimal E
	K_HEX_F		= 0xBC,  // Hexadecimal F
	K_RND		= 0xBD,  // Rnd( function
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
	K_DEL		= 0xFE  // DEL
};

/*

* The decimal result function ([SHIFT] [=]) is present since ES models, but up until ClassWiz models, it is not
labeled on the calculator's front side, and is only mentioned in the User's Guide for some reason. 

** The Execute key is labeled as [=] on all ES and ES PLUS models, and most ClassWiz EX models. However,
most ClassWiz CW models had this key renamed to [EXE] to avoid confusion with the equals sign.

*/

// Setup constants

// Number format constants (setup_num_fmt 8102H)
enum num_format {
	NUM_FMT_FIX		= 8,
	NUM_FMT_SCI		= 9,
	NUM_FMT_NORM1	= 0,
	NUM_FMT_NORM2	= 1
};

// Angle unit constants (setup_angle_unit 8105H)
enum angle_unit {
	ANGLE_DEG	= 4,
	ANGLE_RAD	= 5,
	ANGLE_GRA	= 6
};

// Errors
enum error {
	ERROR_ACBREAK		= 1,
	ERROR_SYNTAX		= 2,
	ERROR_MATH			= 3,
	ERROR_OUTOFMEM		= 4,
	ERROR_STACK			= 5,
	ERROR_ARGUMENT		= 6,
	ERROR_DIMENSION		= 7,
	ERROR_SOLVE			= 8,
	ERROR_TIMEOUT		= 9,
	ERROR_VARIABLE		= 10,
	ERROR_NULL			= 11
}

// RAM addresses

// 08000
extern void *ram_start;
extern char reg0[10];
// 0800A
extern char d_0800A;
// 080DC
extern char d_080DC;
// 080DD
extern char cursor_noflash;
// 080DE
extern char d_080DE;
// 080DF
extern char d_080DF;
// 080E0
extern char (*d_080E0)[2];
// 080F2
extern int  last_key_scancode;
// 080F5
extern char last_key_keycode;
// 080F7
extern char d_080F7;
// 080F8
extern char modifiers;
// 080F9
extern char mode;
// 080FA
extern char submode;
// 080FB
extern char screen_state;
// 080FC
extern char table_mode;
// 080FD
extern char d_080FD;
// 080FE
extern char d_080FE;
// 080FF
extern char d_080FF;
// 08102
extern char setup_num_fmt;
// 08104
extern char setup_decimal_mark;
// 08105
extern char setup_angle_unit;
// 08106
extern char setup_mathi;
// 08107
extern char setup_frac_result;
// 08108
extern char setup_cmplx_result;
// 08109
extern char setup_stat_freq;
// 0810A
extern char setup_rdec;
// 0810C
extern char setup_decimalo;
// 0810E
extern char setup_contrast;
// 08110
extern char cursor_pos_byte;
// 08111
extern char d_08111;
// 08112
extern char formula_x;
// 08113
extern char formula_y;
// 08114
extern char cursor_x;
// 08115
extern char cursor_y;
// 08116
extern char cursor_char;
// 08117
extern char d_08117;
// 08118
extern char table_viewport;
// 08119
extern char table_y;
// 0811A
extern char table_x;
// 0811B
extern char font_size;
// 0811C
extern char draw_mode;
// 0811D
extern char use_rambuf;
// 08120
extern char d_08120;
// 08122
extern char d_08122;
// 08123
extern char use_output_charset;
// 08124
extern char d_08124;
// 08125
extern char d_08125;
// 08126
extern char d_08126;
// 08128
extern char (*input_area_ptr)[100];
// 0812C
extern char d_0812C;
// 08140
extern char result[20];
// 08154
extern char input_area[100];
// 081B7
extern char cache_area[100];
// 08224
extern int timer;
// 08224
extern void *vars_start;
// 0829E
extern char mode_ram[880];
// 0860E
extern char magic_string[16];
// 08640
extern f_058DC_union *d_08640;
// 08A18
extern void *stack_start;

#endif
