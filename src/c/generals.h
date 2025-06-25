#include <string.h>

// 00947
extern const char s_blank_line[];
// 00974
extern const char contrast_screen[];
// 009A4
extern const char const_screen[];
// 009BE
extern const char conv_screen[];
// 009DA
extern const char menu_clr[];
// 009FA
extern const char s_clr_setup[];
// 00A07
extern const char s_clr_mem[];
// 00A15
extern const char s_reset_all[];
// 00A20
extern const char s_clr_done_all[];
// 00A2D
extern const char s_prompt_yes[];
// 00A38
extern const char s_prompt_cancel[];
// 00A46
extern const char s_clr_done[];
// 00A53
extern const char s_clr_prompt_ac[];
// 00A63
extern const char menu_matrix_table[];
// 00A76
extern const char menu_matrix[];
// 00AB4
extern const char menu_matrix_data[];
// 00ACC
extern const char menu_matrix_dim0[];
// 00AF6
extern const char menu_matrix_dim1[];
// 00B41
extern const char menu_vector_table[];
// 00B54
extern const char menu_vector[];
// 00B8A
extern const char menu_vector_data[];
// 00BA2
extern const char menu_vector_dim[];
// 00BDA
extern const char menu_cmplx[];
// 00BFC
extern const char menu_base_n_0[];
// 00C28
extern const char menu_base_n_1[];
// 00C43
extern const char menu_drg[];
// 00C56
extern const char menu_hyp[];
// 00C87
extern const char menu_stat_data_edit[];
// 00C9B
extern const char menu_stat_1var_sum[];
// 00CAC
extern const char menu_stat_2var_sum[];
// 00CE2
extern const char menu_stat_1var_var[];
// 00CFE
extern const char menu_stat_2var_var[];
// 00D29
extern const char menu_stat_1var_minmax[];
// 00D3C
extern const char menu_stat_2var_minmax[];
// 00D5D
extern const char menu_stat_distr[];
// 00D7A
extern const char menu_stat_reg[];
// 00D98
extern const char menu_stat_reg_quad[];
// 00DBF
extern const char menu_ratio[];
// 00DD6
extern const char menu_verify[];
// 00DFC
extern const char menu_ineq[];
// 00E19
extern const char menu_ineq_poly2[];
// 00E4E
extern const char menu_ineq_poly3[];
// 00E93
extern const char menu_setup_stat_freq[];
// 00EAF
extern const char menu_setup_rdec[];
// 00ECF
extern const char menu_setup_cmplx_result[];
// 00EF0
extern const char menu_setup_decimal_mark[];
// 00F12
extern const char menu_setup_decimalo[];

// 02676
extern char f_02676(char a);
// 0270E
extern char f_0270E(char a);
// 02750
extern int smart_strlen(const char *str);
// 0275A
extern char *smart_strcpy(char *dest, char *src);
// 0276E
extern char *smart_strcat(char *dest, char *src);
// 02B76
extern char get_token(char id, char *dest);
// 02BD4
extern void print_continue_prompt(void);
// 02C04
extern void num_output_print(char *a);
// 02CB6
extern char f_02CB6(void);
// 02CE0
extern char buffer_clear_lastnline(char n);
// 02D38
extern void buffer_clear(void);
// 02DD8
extern void f_02DD8(char a, char b, char c, char d);
// 02E7A
extern void line_print(char x, char y, char *str);
// 02EBA
extern void char_print(char x, char y, char chr);
// 03158
extern char get_screen_addr(char x, char y, int *addr);
// 03192
extern void setup_status_bar(void);
// 033EA
extern void print_4lines_4str(char *a, char *b, char *c, char *d);
// 0341E
extern char print_4lines_head(char *head, char *content);
// 0343C
extern char print_4lines(char *string);
// 0347A
extern void set_up_arrow(void);
// 03480
extern void set_down_arrow(void);
// 03486
extern char pd_value(void);
// 0352A
extern void print_error(char idx);
// 03558
extern void f_03558(void);
// 035B8
extern char is_char_keycode(char keycode);
// 03656
extern char is_ac_key(char keycode);
// 036B8
extern char f_036B8(void);
// 036CA
extern char filter_chars_stat_mat_vct(char keycode);
// 036E8
extern char filter_chars(char keycode);
// 036F8
extern char filter_chars_table(char keycode);
// 03706
extern char filter_chars_cmplx(char keycode);
// 03714
extern char f_03714(char keycode);
// 03F78
extern char need_reset(void);
// 0448A
extern void f_0448A(char *a, char *b);
// 0449A
extern void clear_result(void);
// 044A8
extern void memzero(void *s, size_t n);
// 044B6
extern void	f_044B6(void);
// 044CE
extern void	f_044CE(void);
// 044D2
extern void	f_044D2(void);
// 044D6
extern void	f_044D6(char keycode);
// 044E2
extern void f_044E2(char keycode);
// 0458C
extern size_t get_remaining_stack_space(void);
// 04598
extern void reset_magic_string(void);
// 045EE
extern void shutdown(void);
// 04640
extern void delay(int ticks);
// 04686
extern void waitkey(void);
// 046A6
extern char get_IRQ0(void);
// 046C0
extern void	f_046C0(void);
// 046C4
extern void f_046C4(void);
// 046CC
extern char f_046CC(void);
// 046D4
extern char f_046D4(void);
// 04776
extern void set_contrast_sfr(char val);
// 04776
extern void set_scr_normal(void);
// 047C0
extern void set_disp_indicator(char enable);
// 047D2
extern void set_all_kimask(void);
// 047CA
extern void set_contrast2_0(void);
// 047DA
extern void clr_all_kimask(void);
// 047E2
extern void set_all_ko(void);
// 047EA
extern void clr_all_ko(void);
// 047F2
extern char is_key_pressed(void);
