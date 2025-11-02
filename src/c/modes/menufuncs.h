char f_09014(void);
char stat_display_menu(char val, char noclr);
char show_menu_matvct(char idx, char mode_enter);
#if ENABLE_RATIO == 1
char show_menu_ratio(char idx, char mode_enter);
#endif
#if ENABLE_INEQ == 1
char show_menu_ineq(char idx, char mode_enter);
#endif
