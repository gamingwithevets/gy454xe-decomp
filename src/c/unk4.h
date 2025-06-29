#include <string.h>
#include "consts.h"
#include "generals.h"

// 09962
char f_09962(char a);
// 09BDC
char f_09BDC(char a);
// 09D54
char f_09D54(char tmode);
// 09D84
char f_09D84(void);
// 09DB6
void f_09DB6(void);
// 09E70
char display_menu(char *val, char *head);
// 0A010
char f_0A010(char m, char b);
// 0A050
void f_0A050(char a);
// 0A0BC
void f_0A0BC(void);
// 0A24E
void f_0A24E(void);
// 0A372
void f_0A372(void);
// 0A3B4
void f_0A3B4(void);
// 0A410
void f_0A410(void);
// 0A564
char f_0A564(void);
// 0A57A
void f_0A57A(void);
// 0A594
void f_0A594(char a);
// 0A5E4
void f_0A5E4(void);
// 0A6DA
void f_0A6DA(void);
// 0A782
void f_0A782(void);
// 0A7FA
void f_0A7FA(void);
// 0A936
void f_0A936(char a);
// 0A93A
void f_0A93A(char a);
// 0A978
void f_0A978(void);
// 0AA7C
void f_0AA7C(void);
// 0AB44
void f_0AB44(void);
// 0AB6A
void f_0AB6A(void);
// 0ABA8
char *f_0ABA8(char a, char b, char c);
// 0ABDE
void f_0ABDE(void);
// 0ABFC
void f_0ABFC(void);
// 0AC28
void f_0AC28(void);
// 0AC36
void f_0AC36(void);
// 0AC44
void f_0AC44(void);
// 0AD08
void f_0AD08(void);
// 0AE14
void f_0AE14(void);
// 0AECE
void setup_stat(void);
// 0AEFA
void table_home(void);
// 0AF0A
void f_0AF0A(void);
// 0AF16
void f_0AF16(void);
// 0AF30
void f_0AF30(void);
// 0AFB0
void reset_all(void);
// 0AFE0
void clear_setup(void);
// 0B00E
void clear_mem(void);
// 0B042
void clear_input_area(void);
// 0B04E
void clear_cache_area(void);
// 0B05A
void f_0B05A(void);
// 0B08A
void f_0B08A(void);
// 0B0C6
void getscancode(int *scancode);
// 0B292
char check_key_kio(char *kio);
// 0B2E0
char get_key_kio(char *kio);
// 0B31E
char check_key_kio2(char *kio);
// 0B3EC
char f_0B3EC(void);
// 0B410
char scancode_to_int(char *a, char *keycodes);
// 0B45E
char getkeycode(char a);
// 0B588
char f_0B588(void);
// 0B5C6
void set_modifiers(char keycode);
// 0B620
char is_modifier_keycode(char keycode);
// 0B634
char prompt_yes_no(void);
// 0B654
void wait_shift(void);
// 0B6B6
char f_0B6B6(void);
// 0B6F0
void f_0B6F0(void);
// 0B736
void f_0B736(void);
// 0B7B6
char f_0B7B6(void);
// 0B804
char diag_initloop(void);
// 0B8B8
void f_0B8B8(char a);
// 0B968
void f_0B968(void);
// 0B984
char f_0B984(char keycode);
// 0B998
void f_0B998(void);
// 0BA50
void f_0BA50(void);
// 0BAA8
void f_0BAA8(void);
// 0BADA
void f_0BADA(void);
// 0BAF2
void f_0BAF2(void);
// 0BBDA
void f_0BBDA(void);
// 0BC34
char f_0BC34(void);
// 0BC6C
void f_0BC6C(void);
// 0BC90
void f_0BC90(void);
// 0BDFA
void f_0BDFA(void);
// 0C084
void f_0C084(void);
// 0C0D0
void f_0C0D0(void);
