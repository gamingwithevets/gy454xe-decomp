#include "../consts.h"

const char (* const keyfuncs[])(keyfunc_struct *);

char *set_dim(char idx, char m, char n);
char table_matvct_set_cell(char *num, char sm, char ty, char tx);
void table_eqn_init(void);
void table_eqn_setup(void);
void table_ratio_setup(void);
void table_ineq_setup(void);
char f_0AD08(char a);
char f_0AE14(char a);
void setup_stat(void);
void table_home(void);
void f_0AF0A(void);
void f_0AF16(void);
char table_stat_set_cell(char a, char b, char *num);
void reset_all(void);
void clear_setup(void);
void clear_mem(void);
void clear_input_area(void);
void clear_cache_area(void);
void f_0B05A(void);
void set_result(char *a);
void getscancode(int *sc);
char check_key_kio(scancode *kio);
char get_key_kio(scancode *kio);
char check_key_kio2(scancode *kio);
char f_0B3EC(void);
char scancode_to_int(scancode *a, char *keycodes);
char getkeycode(char a);
char f_0B588(void);
void set_modifiers(char keycode);
char is_modifier_keycode(char keycode);
char prompt_yes_no(void);
void wait_shift(void);
void f_0B67E(void);
char f_0B6B6(void);
char is_meta_keycode(void);
void f_0B736(void);
char is_rcl_keycode(char keycode);
char is_sto_keycode(char keycode);
char is_eqn_result(void);
#if ENABLE_RATIO == 1
char is_ratio_result(void);
#endif
#if ENABLE_INEQ == 1
char is_ineq_result(void);
#endif
char show_error(char idx);
char diag_initloop(void);
void f_0B8B8(char a);
char get_func_tok(char keycode);
char f_0BB42(char **a);
void f_0C0D0(void);
char f_0C1A0(keyfunc_struct *a);
char f_0C1D4(keyfunc_struct *a);
