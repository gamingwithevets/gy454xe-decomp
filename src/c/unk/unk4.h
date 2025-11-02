#include <string.h>
#include "../consts.h"
#include "../generals.h"

char f_09962(char no_keyfunc);
char f_09BDC(char a);
char f_09D54(char tmode);
char f_09D84(void);
char display_menu(char *val, char *head);
char display_token_menu(char m, char menu_idx);
char table_range_handler(char a);
void table_stat_handler(void);
char f_0A564(void);
char f_0A57A(void);
char table_eqn_handler(char a);
void f_0A6DA(void);
void f_0A782(void);
char table_mat_handler(char a);
char table_vct_handler(char a);
#if ENABLE_INEQ == 1
char table_ineq_handler(char a);
#endif
#if ENABLE_RATIO == 1
char table_ratio_handler(char a);
#endif
char *set_dim(char idx, char m, char n);
void table_eqn_init(void);
void table_eqn_setup(void);
void table_ratio_setup(void);
void table_ineq_setup(void);
char f_0AD08(char a);
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
char f_0B6B6(void);
char is_eqn_result(void);
#if ENABLE_RATIO == 1
char is_ratio_result(void);
#endif
#if ENABLE_INEQ == 1
char is_ineq_result(void);
#endif
char diag_initloop(void);
void f_0B8B8(char a);
void f_0C0D0(void);
