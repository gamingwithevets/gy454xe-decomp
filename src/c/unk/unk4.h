#include "../consts.h"

char keyfunc_nop(keyfunc_struct *a);
char f_09962(char no_keyfunc);
char f_09BDC(char a);
void f_09D54(char tmode);
char f_09D84(void);
char display_menu(char *val, char *head);
char display_token_menu(char m, char menu_idx);
char table_range_handler(char a);
void table_stat_handler(void);
void table_set_cur_cell(char *num);
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
