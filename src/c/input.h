#include "consts.h"
#include "emu_kb.h"

void input_print_linei(void);
void mathi_draw_line_horz(int x1, int x2, char y);
void mathi_plot_pixel(int x, char y);
void mathi_line_print(char *string, int x, char y);
char f_05658(char *input);
void input_print_mathi(void);
char *mathi_draw_expr(char *input, mathi_bbox *b, int x, char y);
char f_06B52(char *a);
void math2line(char *input, char *b, char c, char d);
void insert_token(char token, char is_func);
void f_07488(char keycode);
char num_to_str_std(char *out, char *num, char c);
#if REAL == 0
int f_082A2_E(emu_kb *kb);
#endif
void print_result(char *result);
