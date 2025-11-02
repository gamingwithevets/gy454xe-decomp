// == ES PLUS FEATURE CUSTOMIZATION FILE ==
// This file is used for configuring which features (modes, functions, etc.) will be
// included in the final ROM.

// To add/remove features, modify the number after the equals sign for the corresponding setting.
// 1 is true, 0 is false. Other values may cause instability.

// NOTE:
// - This file does not allow you to change the MODE and SETUP menu options,
//   as well as keyboard layouts and tokens.
//   + To modify the menus and keyboard layouts, edit unk4.c and consts.h.
//   + To modify tokens, edit generals.asm and unk5.asm.

// For default ROM settings and version info configuration, see config.asm.

// ==================================================================================

// Build real ROMs. Set to 0 to build emulator ROMs.
REAL=0

// ==== Functions ====

// Enable the integral function.
ENABLE_INTEGRAL=1
// Enable the summation function.
ENABLE_SUM=1
// Enable the derivative function.
ENABLE_DDX=1
// Enable recurring decimal.
ENABLE_RDEC=1
// Enable prime factor result format.
ENABLE_FACT=1
// Enable SOLVE.
ENABLE_SOLVE=1
// Enable scientific constants.
ENABLE_CONST=1
// Enable unit conversions.
ENABLE_CONV=1

// ==== Modes ====
ENABLE_BASE_N=1
ENABLE_CMPLX=1
ENABLE_MATRIX=1
ENABLE_VECTOR=1
ENABLE_INEQ=1
ENABLE_VERIF=1
ENABLE_RATIO=1
