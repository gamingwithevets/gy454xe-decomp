#include <string.h>
#include "consts.h"
#include "generals.h"
#include "emu_kb.h"
#include "input.h"
#include "unk2.h"
#include "unk3.h"
#include "unk4.h"
#include "unk5.h"

// Weird struct used in f_09962 and the functions that it calls from a jump table.
typedef struct {
	char *input_area_ptr;
	char *result_ptr;
	char unk_0x04;
	char unk_0x05;
	char mode;
	char unk_0x07;
	char unk_0x08;
	char unk_0x09;
	char unk_0x0a;
	char unk_0x0b;
} f_09962_struct;

// Only getscancode and its neighboring functions seem to use this. There may be more functions found to be using
// this struct type as the decomp goes on.
typedef struct {
	char cursor_on[20];
	char cursor_off[20];
	char cursor_toggle;
	char cursor_enable_flash;
	char *screen_addr;
	char unk_0x2c;
	char unk_0x2d;
	int shutdown_timer;
} getscancode_struct;

// Struct used by the menu list.
typedef struct {
	const char *string;	// String printed when menu is displayed.
	char ret_mode;		// Return value in values if the corresponding bit (MSB->LSB = 0->7) is set; otherwise use it as index
	char char_mode;		// Set bits (MSB->LSB = 0->7) denote tokens. display_menu returns 2 
	char values[8];		// Values corresponding to the (up to) 8 choices.
	char prev_page;		// Index of menu to go to when pressing up. NULL = none.
	char next_page;		// Index of menu to go to when pressing down. NULL = none.
	char parent;		// Index of menu to go to when pressing left.
	char keycode;		// Keycode associated with this menu.
} menu;

// Static declarations (placed first so we can define jump table)
static char keyfunc_nop(f_09962_struct *a);
static void f_0A410(char *a);
static void f_0B226(getscancode_struct *a);
static void copy_cursor_from_scr(char *cursor, char *scr);
static void copy_cursor_to_scr(char *scr, char *cursor);
static void f_0B67E(void);
static char is_rcl_keycode(char keycode);
static char is_sto_keycode(char keycode);
static char show_error(char idx);
static char conv_func_token(char keycode);
static char f_0B9C8(f_09962_struct *a);
static void f_0BA28(f_09962_struct *a);
static char f_0BADA(void);
static char f_0BAF2(char **a);
static char f_0BB42(char **a);
static char *f_0BBDA(char *a);
static char f_0BC6C(char *a);
static char f_0BEEE(f_09962_struct *a);
static char f_0BF8A(void);
static char f_0BFDC(f_09962_struct *a);
static void f_0C08C(f_09962_struct *a);
static char f_0C148(f_09962_struct *a);
static char f_0C1A0(f_09962_struct *a);
static char f_0C1D4(f_09962_struct *a);
static char keyfunc_ac(f_09962_struct *a);
static char keyfunc_sto_rcl(f_09962_struct *a);
static char keyfunc_rcl(f_09962_struct *a);
static char keyfunc_sto(f_09962_struct *a);
static char keyfunc_exe(f_09962_struct *a);
static char keyfunc_abc(f_09962_struct *a);
static char keyfunc_dc(f_09962_struct *a);
static char keyfunc_dms(f_09962_struct *a);
static char keyfunc_fact(f_09962_struct *a);
static void f_0C72C(f_09962_struct *a);
static char keyfunc_eng(f_09962_struct *a);
static char keyfunc_eng_r(f_09962_struct *a);
static char keyfunc_del(f_09962_struct *a);
static char keyfunc_mov_x(f_09962_struct *a);
static char keyfunc_mov_y(f_09962_struct *a);
static char keyfunc_base(f_09962_struct *a);
static char keyfunc_mode(f_09962_struct *a);
static char keyfunc_setup(f_09962_struct *a);

// DATA: GY454XE  Re 01FB6
static char (* const keyfuncs[])(f_09962_struct *) = {
	keyfunc_mov_y,	// K_UP
	keyfunc_mov_y,	// K_DOWN
	keyfunc_mov_x,	// K_RIGHT
	keyfunc_mov_x,	// K_LEFT
	keyfunc_mode,	// K_MODE
	keyfunc_setup,	// K_SETUP
	keyfunc_ac,		// K_AC
	keyfunc_nop,	// K_OFF
	keyfunc_nop,	// K_ALPHA
	keyfunc_nop,	// K_SHIFT
	keyfunc_rcl,	// K_RCL
	keyfunc_sto,	// K_STO
	keyfunc_nop,	// K_INS
	keyfunc_exe,	// K_APPROX
	keyfunc_sto,	// K_M_PLUS
	keyfunc_sto,	// K_M_MINUS
	keyfunc_exe,	// K_EXECUTE
	keyfunc_fact,	// K_FACT
	keyfunc_base,	// K_BASE_BIN
	keyfunc_base,	// K_BASE_OCT
	keyfunc_base,	// K_BASE_DEC
	keyfunc_base,	// K_BASE_HEX
	keyfunc_dms,	// K_DMS
	keyfunc_dms,	// K_DMS_R
	keyfunc_eng,	// K_ENG
	keyfunc_eng_r,	// K_ENG_R
	keyfunc_abc,	// K_FMT_DEC
	keyfunc_dc,		// K_FMT_FRAC
	keyfunc_nop,	// K_CALC
	keyfunc_nop,	// K_SOLVE
	keyfunc_del		// K_DEL
};

// As the emulator ROM's switch-case table for show_error and the error string table comes after the function table above,
// forward definitions are needed for keycode and menu arrays.

extern const char keycodes[64];
extern const char keycodes_shift[64];
extern const char keycodes_alpha[64];
extern const char keycodes_rcl[64];
extern const char keycodes_sto[64];
extern const char keycodes_base_n[64];
extern const char keycodes_shift_base_n[64];
extern const menu menus[];

// FUNCTION: GY454XE  Re 0995E
static char keyfunc_nop(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 09962
char f_09962(char a) {
	f_09962_struct v0;
	char v1;
	char v2;

	f_0B67E();
	font_size = 10;
	if (table_mode & (1 << 4)) font_size = 7;

	memzero(&v0, 12);
	v0.unk_0x04 = 1;
	v0.input_area_ptr = input_area;
	v0.result_ptr = result;
	v0.mode = mode;
	v0.unk_0x07 = f_0A564();
	v0.unk_0x08 = is_mathi();
	v0.unk_0x09 = is_matho();
	v0.unk_0x0a = f_02CB6();
	if (!(d_080FE & 0x60)) d_08125 = 0;
	if (!a) {
		if (!is_char_keycode(last_key_keycode)) {
			v1 = last_key_keycode - 0xe0;
			if (is_rcl_keycode(last_key_keycode)) v1 = K_RCL - 0xe0;
			else if (is_sto_keycode(last_key_keycode)) v1 = K_STO - 0xe0; 
			else if (last_key_keycode < 0xe0) goto j_09b9c;
			switch (keyfuncs[v1](&v0)) {
				case 0: goto j_09b9c;
				case 1: goto j_09b6c;
				case 2: goto j_09b72;
				case 3: goto j_09b76;
			}
		} else if (d_080F7 && 0xcf <= last_key_keycode) {
			f_044E2(conv_func_token(last_key_keycode));
			if (v0.unk_0x08) v0.unk_0x05 = 1;
		} else if (last_key_keycode == 0x7c) last_key_keycode = 0;
		if (!(d_080FE & (1 << 6))) {
			if (table_mode & (1 << 7) && d_080FD == 2 && (last_key_keycode == K_COLON || last_key_keycode == 0xa4)) last_key_keycode = 0;
			else if (table_mode & (1 << 7) && d_080FD == 2 && last_key_keycode == K_EQUALS) last_key_keycode = 0;
			else if (table_mode & (1 << 4)) last_key_keycode = filter_chars(last_key_keycode);
			else if (v0.mode == MODE_STAT || v0.mode == MODE_MATRIX || v0.mode == MODE_VECTOR) last_key_keycode = filter_chars_stat_mat_vct(last_key_keycode);
			else if (v0.mode == MODE_TABLE) last_key_keycode = filter_chars_table(last_key_keycode);
			else if (v0.mode == MODE_CMPLX) last_key_keycode = filter_chars_cmplx(last_key_keycode);
			else if (v0.mode == MODE_VERIF) last_key_keycode = f_03714(last_key_keycode);
			if (last_key_keycode == K_EQUALS && v0.mode != MODE_COMP && v0.mode != MODE_CMPLX && v0.mode != MODE_VERIF) last_key_keycode = 0;
			if (last_key_keycode == 0xa4 && !setup_mathi) last_key_keycode = 0;
			if (last_key_keycode) {
				if (is_char_keycode(last_key_keycode)) f_0B736();
				if (d_080FE == 1) {
					f_07470(last_key_keycode, v0.unk_0x05);
					goto j_09b6c;
				}
			}
		}
		goto j_09b9c;
	} else goto j_09b72;
j_09b6c:
	d_080FE = 1;
j_09b72:
	f_0C0D0();
j_09b76:
	if (d_080FE != 1) {
		if (f_0C1A0(&v0) != 1 && f_0C1D4(&v0) != 1) f_07B60(v0.result_ptr);
		else goto j_09b6c;
	}
j_09b9c:
	if (!(d_080FE & (1 << 5)) && f_0BC34() > 0) d_08125 = 1;
	return v0.unk_0x04;
}

// STUB: GY454XE  Re 09BDC
char f_09BDC(char a) {
	return 0;
}

// FUNCTION: GY454XE  Re 09D54
char f_09D54(char tmode) {
	if (table_mode & (1 << 7)) smart_strcpy(input_area, cache_area);
	table_mode = tmode;
	d_080FD = 1;
	d_080FE = 4;
	f_044CE();
}

// FUNCTION: GY454XE  Re 09D84
char f_09D84(void) {
	char v0 = 0;

	if (d_080FE & (1 << 6)) v0 = 1;
	if (table_mode & (1 << 7) && d_080FD == 2) v0 = 1;
	if (!smart_strlen(input_area)) v0 = 1;
	return v0;
}

// STUB: GY454XE  Re 09DB6
void f_09DB6(void) {
	return;
}

// FUNCTION: GY454XE  Re 09E70
char display_menu(char *val, char *head) {
	char v0;
	char v00;
	char v000;
	char v0000;
	int idx;
	int idx2;
	char v3;
	char v4;

	v4 = 0;
	if (head == 0xffff) {
		v4 = 1;
		head = NULL;
	}
	idx2 = *val;
	idx = *val;
	while (1) {
		if (idx) {
			idx2 = idx;
			if (head) print_4lines_head(head, menus[idx].string);
			else print_4lines(menus[idx].string);
			if (menus[idx].prev_page) set_up_arrow();
			if (menus[idx].next_page) set_down_arrow();
		}
		idx = 0;
		v0 = getkeycode(1);
		if (v0) {
			// Check for keys 1-8
			if (0x31 <= v0 && v0 <= 0x38) {
				v3 = menus[idx2].values[v0 - 0x31];
				v00 = 7 - (v0 - 0x31);
				v0 = menus[idx2].ret_mode;
				if (v000 = v00) {
					do v0 >>= 1;
					while (--v000);
				}
				v000 = v0 & 1;
				v0 = menus[idx2].char_mode;
				if (v00) do v0 >>= 1; while (--v00);
				v0000 = v0 & 1;
				if (!v3) { /* jump to below */ }
				else if (v000 == 0) idx = v3;
				else {
					*val = v3;
					if (v0000 == 0) return 3;
					else return 2;
				}
			} else if (v0 == K_AC) {
				last_key_keycode = 0;
				// Handles the Disp indicator (AC pressed)
				set_disp_indicator(1);
				delay(0x770);
				set_disp_indicator(0);
				return 0;
			} else if (menus[idx2].keycode == v0) {
				if (!v4 && screen_state == 1) { /* jump to below */ }
				else {
					if (menus[idx2].keycode) {
						*val = v0;
						last_key_keycode = 0;
						return 1;
					}
				}
			} else if (v0 == K_UP) idx = menus[idx2].prev_page;
			else if (v0 == K_DOWN) idx = menus[idx2].next_page;
			else if (v0 == K_LEFT) {
				idx = menus[idx2].next_page;
				if (idx) head = NULL;
			}
			if (idx & (1 << 7)) {
				*val = (char)idx;
				return 1;
			}
		}
		if (!idx) {
			// Handles the Disp indicator (invalid key pressed)
			set_disp_indicator(1);
			delay(0x770);
			set_disp_indicator(0);
		} else continue;
	}
}

// FUNCTION: GY454XE  Re 0A010
char f_0A010(char m, char b) {
	char loc_m1;

	if (m != NULL && mode != m) return 0;
	else {
		loc_m1 = b;
		if (display_menu(&loc_m1, NULL) == 2) {
			last_key_keycode = loc_m1;
			return 1;
		} else return 0;
	}
}

// STUB: GY454XE  Re 0A050
void f_0A050(char a) {
	return;
}

// STUB: GY454XE  Re 0A0BC
void f_0A0BC(void) {
	return;
}

// STUB: GY454XE  Re 0A24E
void f_0A24E(void) {
	return;
}

// STUB: GY454XE  Re 0A372
void f_0A372(void) {
	return;
}

// STUB: GY454XE  Re 0A3B4
void f_0A3B4(void) {
	return;
}

// STUB: GY454XE  Re 0A410
static void f_0A410(char *a) {
	return;
}

// FUNCTION: GY454XE  Re 0A564
char f_0A564(void) {
	if (table_mode & (1 << 4) && d_080FE == 1) return 1;
	else return 0;
}

// STUB: GY454XE  Re 0A57A
char f_0A57A(void) {
	if (table_mode & (1 << 4) && (d_080FE == 3 || !d_080FE)) return 1;
	return 0;
}

// STUB: GY454XE  Re 0A594
void f_0A594(char a) {
	return;
}

// STUB: GY454XE  Re 0A5E4
void f_0A5E4(void) {
	return;
}

// STUB: GY454XE  Re 0A6DA
void f_0A6DA(void) {
	return;
}

// STUB: GY454XE  Re 0A782
void f_0A782(void) {
	return;
}

// STUB: GY454XE  Re 0A7FA
void f_0A7FA(void) {
	return;
}

// STUB: GY454XE  Re 0A936
void f_0A936(char a) {
	return;
}

// STUB: GY454XE  Re 0A93A
void f_0A93A(char a) {
	return;
}

// STUB: GY454XE  Re 0A978
void f_0A978(void) {
	return;
}

// STUB: GY454XE  Re 0AA7C
void f_0AA7C(void) {
	return;
}

// STUB: GY454XE  Re 0AB44
void f_0AB44(void) {
	return;
}

// STUB: GY454XE  Re 0AB6A
void f_0AB6A(void) {
	return;
}

// FUNCTION: GY454XE  Re 0ABA8
char *f_0ABA8(char a, char b, char c) {
	char *v0;

	d_080E0[a][0] = b;
	d_080E0[a][1] = c;
	v0 = f_043AC(a, 1, 1);
	memzero(v0, 0x5a);
	return v0;
}

// STUB: GY454XE  Re 0ABDE
void f_0ABDE(void) {
	return;
}

// STUB: GY454XE  Re 0ABFC
void f_0ABFC(void) {
	return;
}

// STUB: GY454XE  Re 0AC28
void f_0AC28(void) {
	return;
}

// STUB: GY454XE  Re 0AC36
void f_0AC36(void) {
	return;
}

// STUB: GY454XE  Re 0AC44
void f_0AC44(void) {
	return;
}

// STUB: GY454XE  Re 0AD08
void f_0AD08(void) {
	return;
}

// STUB: GY454XE  Re 0AE14
void f_0AE14(void) {
	return;
}

// FUNCTION: GY454XE  Re 0AECE
void setup_stat(void) {
	if (mode == MODE_STAT) {
		d_080DE = 0;
		d_080DF = 0;
		memzero(&mode_ram[8], 800);
		d_08126 = 0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0AEFA
void table_home(void) {
	table_viewport = 1;
	table_x = 1;
	table_y = 1;
	return;
}

// FUNCTION: GY454XE  Re 0AF0A
void f_0AF0A(void) {
	table_home();
	f_044B6();
	return;
}

// FUNCTION: GY454XE  Re 0AF16
void f_0AF16(void) {
	table_mode = 1;
	d_080FD = 0;
	d_080FE = 1;
	d_080FF = 0;
}

// STUB: GY454XE  Re 0AF30
void f_0AF30(void) {
	return;
}

// FUNCTION: GY454XE  Re 0AFB0
void reset_all(void) {
	memzero(&ram_start, 0xa17);
	setup_contrast = 0x11;
	set_contrast_sfr(0x11);
	use_rambuf = 1;
	d_080DC = pd_value();
	clear_setup();
	reset_magic_string();
	return;
}


// FUNCTION: GY454XE  Re 0AFE0
void clear_setup(void) {
	clear_input_area();
	clear_cache_area();
	set_default_settings();
	mode = MODE_COMP;
	submode = 0;
	screen_state = 3;
	table_mode = 1;
	f_0B8B8(2);
}

// FUNCTION: GY454XE  Re 0B00E
void clear_mem(void) {
	memzero(&vars_start, 100);
	if (mode == MODE_CMPLX) memzero(&mode_ram[362], 100);
	if (f_1B288(result) != 13) clear_result();
	return;
}

// FUNCTION: GY454XE  Re 0B042
void clear_input_area(void) {
	memzero(input_area, 100);
}

// FUNCTION: GY454XE  Re 0B04E
void clear_cache_area(void) {
	memzero(cache_area, 100);
}

// FUNCTION: GY454XE  Re 0B05A
void f_0B05A(void) {
	char m = mode;
	
	d_08125 = 0;
	if (m == MODE_COMP || m == MODE_BASE_N || m == MODE_CMPLX || m == MODE_VERIF) memzero(mode_ram, 250);
	clear_cache_area();
	return;
}

// STUB: GY454XE  Re 0B08A
void f_0B08A(void) {
	return;
}

// FUNCTION: GY454XE  Re 0B0C6
// FUNCTION: GY455XE  Im 0B9EA
void getscancode(scancode *sc) {
#if REAL == 1
	char v0;
	getscancode_struct gsc;
	scancode loc_m50;
	scancode loc_m52;
#else
	int t;
	getscancode_struct gsc;
	char v0;
	emu_kb kb;
#endif

	gsc.shutdown_timer = 0x4ab;
	v0 = 48;
	if (f_046D4()) v0 = 0xee;
#if REAL == 1
	loc_m50.kio = last_key_scancode.kio;
#endif
	gsc.cursor_enable_flash = f_0B3EC();
	font_size = d_08117;
	draw_mode = 1;
	use_rambuf = 0;
	get_screen_addr(cursor_x, cursor_y, &gsc.screen_addr);
	copy_cursor_from_scr(gsc.cursor_off, gsc.screen_addr);
	if (gsc.cursor_enable_flash) char_print(cursor_x, cursor_y, cursor_char);
	copy_cursor_from_scr(gsc.cursor_on, gsc.screen_addr);
	use_rambuf = 1;
	draw_mode = 0;
#if REAL == 1
	gsc.cursor_toggle = 1;
	gsc.unk_0x2c = 0xac;
	gsc.unk_0x2d = 0xac;
	do {
		f_0B226(&gsc);
		if (!(v0--) && f_046CC()) {
			f_046C0();
			loc_m52.kio = loc_m50.kio;
			sc->kio = loc_m50.kio;
			goto j_0b19c;
		} else delay(19);
	} while (check_key_kio(&loc_m50));
#endif
	gsc.unk_0x2c = 1;
	gsc.unk_0x2d = 1;
	gsc.cursor_toggle = 0;
#if REAL == 1
	goto j_0b1ee;
j_0b19c:
	clr_all_ko();
	last_key_scancode = loc_m52;
	copy_cursor_to_scr(gsc.screen_addr, gsc.cursor_off);
	return;
j_0b1ee:
	while (set_all_kimask(), set_all_ko(), 1) {
#else
	init_emu_kb(&kb);
	*kb.stop_type = 1;
	while (1) {
#endif
		f_0B226(&gsc);
		delay(4762);
#if REAL == 1
j_0b1cc:
		if (get_IRQ0() & (1 << 1)) {
			if (!is_key_pressed()) {
				waitkey();
				goto j_0b1cc;
			}
			break;
		}
		++timer;
	}
	clr_all_ko();
	clr_all_kimask();
	delay(1);
	if (get_key_kio(&loc_m52) && check_key_kio2(&loc_m52)) {
		*sc = loc_m52;
		goto j_0b19c;
	}
	goto j_0b1ee;
#else
		if (*kb.ki | *kb.ko) break;
		t = ++timer;
	}
	*kb.stop_type = 0;
	sc->b.ki = *kb.ki;
	sc->b.ko = *kb.ko;
	copy_cursor_to_scr(gsc.screen_addr, gsc.cursor_off);
	return;
#endif
}

// FUNCTION: GY454XE  Re 0B226
static void f_0B226(getscancode_struct *a) {
	--a->unk_0x2d;
	if (!a->unk_0x2d) {
		a->cursor_toggle ^= 1;
		a->unk_0x2d = a->unk_0x2c;
#if REAL == 1
		if (!a->shutdown_timer--) shutdown();
#endif
	}
	if (a->cursor_enable_flash) {
		if (a->cursor_toggle) copy_cursor_to_scr(a->screen_addr, a->cursor_on);
		else copy_cursor_to_scr(a->screen_addr, a->cursor_off);
	} else delay(5);
	return;
}

// FUNCTION: GY454XE  Re 0B292
char check_key_kio(scancode *kio) {
	char i;
	char v1;
	volatile char *_KOD;
	volatile char *_KID;

	v1 = 0;
	_KOD = 0xf046;
	_KID = 0xf040;

	*_KOD = kio->b.ko;
	i = 1;
	do {
		if ((char)((*_KID ^ 0xff) & kio->b.ki)) {
			v1 = 1;
			break;
		} else delay(13);
	} while (++i <= 10);

	*_KOD = 0;
	if (v1) f_046C4();
	return v1;
}

// FUNCTION: GY454XE  Re 0B2E0
char get_key_kio(scancode *kio) {
	char v0;
	volatile char *_KOD;
	volatile char *_KID;
	char v1;
	char v2;

	v0 = 1;
	_KOD = 0xf046;
	_KID = 0xf040;
	v1 = 1;
	v2 = 1;
	do {
		*_KOD = v1;
		if ((char)*_KID != (char)0xff) {
			kio->b.ko = v1;
			kio->b.ki = *_KID ^ 0xff;
			goto j_0b312;
		}
		v1 <<= 1;
	} while (++v2 <= 7);
	v0 = 0;
j_0b312:
	clr_all_ko();
	return v0;
}

// FUNCTION: GY454XE  Re 0B31E
char check_key_kio2(scancode *kio) {
	char v0;
	char v1;
	char v2;
	volatile char *_KOD;
	volatile char *_KID;
	char v3;

	v0 = 1;
	v2 = 0;
	_KOD = 0xf046;
	_KID = 0xf040;
	v3 = 1;
	do {
		delay(13);
		*_KOD = kio->b.ko;
		v1 = *_KID ^ 0xff;
		clr_all_ko();
		if ((char)(v1 & kio->b.ki)) ++v2;
	} while (++v3 <= 5);
	if (v2 != 5) v0 = 0;
	return v0;
}

// FUNCTION: GY454XE  Re 0B370
static void copy_cursor_from_scr(char *cursor, char *scr) {
	char v0;

	v0 = 0;
	while (v0 < d_08117) {
		cursor[v0*2] = scr[0];
		cursor[v0*2+1] = scr[1];
		scr += 0x10;
		++v0;
	}
}

// FUNCTION: GY454XE  Re 0B3B2
static void copy_cursor_to_scr(char *scr, char *cursor) {
	char v0;

	v0 = 0;
	while (v0 < d_08117) {
		scr[0] = cursor[v0*2];
		scr[1] = cursor[v0*2+1];
		scr += 0x10;
		++v0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0B3EC
char f_0B3EC(void) {
	if (cursor_noflash) return 0;
	if (screen_state) return 0;
	if (d_080FE != 1) return 0;
	return 1;
}

// FUNCTION: GY454XE  Re 0B410
char scancode_to_int(scancode *a, char *keycodes) {
	char v0;
	char v1;
	char v2;

	v0 = a->b.ki;
	v1 = a->b.ko;
	if (!v1 || !v0) return 0;
	v2 = 0;
	while (v0 > 0) {
		++v2;
		v0 >>= 1;
	}
	v0 = v2;
	v2 = 0;
	while (v1 > 0) {
		++v2;
		v1 >>= 1;
	}
	v1 = v2 - 1;
	--v0;
	return keycodes[(char)((v0 << 3) + v1)];
}

// FUNCTION: GY454XE  Re 0B45E
char getkeycode(char a) {
	scancode v0;
	char v1;
	char *v2;

	while (1) {
		if (!screen_state) setup_status_bar();
		getscancode(&v0);
		if (modifiers & (1 << 2)) {
			v1 = scancode_to_int(&v0, keycodes_alpha);
			if (mode == MODE_BASE_N && (v1 == K_EULER || v1 == K_CONV_N || v1 == K_RANINT)) v1 = 0;
		}
		else if (modifiers & (1 << 1)) v1 = scancode_to_int(&v0, keycodes_rcl);
		else if (modifiers & (1 << 0)) v1 = scancode_to_int(&v0, keycodes_sto);
		else if (!(modifiers & (1 << 3))) v1 = scancode_to_int(&v0, mode != MODE_BASE_N ? keycodes : keycodes_base_n);
		else v1 = scancode_to_int(&v0, mode != MODE_BASE_N ? keycodes_shift : keycodes_shift_base_n);

		if (mode == MODE_CMPLX) {
			if (v1 != K_NEGATIVE) if (v1 == K_ENG || v1 == K_ENG_R) v1 = 0x80;
			else if (modifiers & (1 << 3)) v1 = 0xaf;
		}
		if (v1 == K_OFF) shutdown();
		if (is_modifier_keycode(v1)) set_modifiers(v1);
		else {
			set_modifiers(0);
			if (v1 == K_EXECUTE && !f_0B588()) v1 = K_APPROX;
			return v1;
		}
	}
}

// FUNCTION: GY454XE  Re 0B588
char f_0B588(void) {
	if (!setup_mathi) return 0;
	if (setup_decimalo) return 0;
	if (!(mode & (1 << 6))) return 0;
	if (mode == MODE_EQN && d_080FE == 1 && submode <= 2) return 0;
	return 1;
}

// FUNCTION: GY454XE  Re 0B5C6
void set_modifiers(char keycode) {
	char mods;

	mods = modifiers;
	switch (keycode) {
		case K_ALPHA:
			mods = (mods ^ 4) & 0xf4;
			break;
		case K_SHIFT:
			mods = (mods ^ 8) & 0xf8;
			break;
		case K_RCL:
			mods = (mods ^ 2) & 0xf2;
			break;
		case K_STO:
			mods = (mods ^ 1) & 0xf1;
			break;
		case K_INS:
			if (!screen_state) mods += 0x80;
			mods &= 0xf7;
			modifiers = mods;
			f_10FD2();
			break;
		default:
			mods &= 0xf0;
			break;
	}
	modifiers = mods;
	return;
}

// FUNCTION: GY454XE  Re 0B620
char is_modifier_keycode(char keycode) {
	if (0xe8 <= keycode && keycode <= 0xec) return 1;
	return 0;
}

// 0 = AC, 1 = EXE
// FUNCTION: GY454XE  Re 0B634
char prompt_yes_no(void) {
	char v0;

	do {
		v0 = getkeycode(1);
		if (v0 == K_APPROX || v0 == K_EXECUTE) return 1;
		else if (v0 == K_AC) return 0;
	} while (1);
}

// FUNCTION: GY454XE  Re 0B654
void wait_shift(void) {
	scancode sc;

	do getscancode(&sc);
	while (scancode_to_int(&sc, keycodes) != K_SHIFT);
}

// FUNCTION: GY454XE  Re 0B67E
static void f_0B67E(void) {
	if (d_080FE == 1 && d_080F7 == 1) {
		if (last_key_keycode == K_DMS) {
			// Original version jumps to `f_044E2` with B. Currently there is no known way to replicate this in CCU8
			f_044E2(0x5c); // Degs-Mins-Secs
			return;
		}
		if ((K_DMS_R <= last_key_keycode && last_key_keycode <= K_FMT_FRAC) || last_key_keycode == K_FACT) last_key_keycode = 0;
	}
}

// FUNCTION: GY454XE  Re 0B6B6
char f_0B6B6(void) {
	char v0 = 1;

	if (table_mode != 1 && d_080FE != 1 && !(d_080FE & 0xf0) && !is_char_keycode(last_key_keycode) && !is_rcl_keycode(last_key_keycode)) v0 = 0;
	return v0;
}

// STUB: GY454XE  Re 0B6F0
void f_0B6F0(void) {
	return;
}

// STUB: GY454XE  Re 0B736
void f_0B736(void) {
	return;
}

// FUNCTION: GY454XE  Re 0B782
static char is_rcl_keycode(char keycode) {
	if (d_080F7 && K_RCL_A <= keycode && keycode <= K_RCL_M) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0B79C
static char is_sto_keycode(char keycode) {
	if (d_080F7 && K_STO_A <= keycode && keycode <= K_STO_M) return 1;
	return 0;
}

// FUNCTION: GY454XE  Re 0B7B6
char f_0B7B6(void) {
	if (mode == MODE_EQN && table_mode == 1) return 1;
	return 0;
}

#if REAL == 0

// Struct used in emulator ROM's log_error.
typedef struct {
	int index;
	int add_template;
	char *string;
} error_emu;

// We can't define these yet because there is a switch-case in log_error.
// We need the jump table to appear before the strings.

extern const char s_err_emu_acbreak[];
extern const char s_err_emu_syntax[];
extern const char s_err_emu_math[];
extern const char s_err_emu_mem[];
extern const char s_err_emu_go[];
extern const char s_err_emu_nesting[];
extern const char s_err_emu_stack[];
extern const char s_err_emu_argument[];
extern const char s_err_emu_dimension[];
extern const char s_err_emu_com[];
extern const char s_err_emu_transmit[];
extern const char s_err_emu_receive[];
extern const char s_err_emu_outofmem[];
extern const char s_err_emu_undefined[];
extern const char s_err_emu_overflow[];
extern const char s_err_emu_domain[];
extern const char s_err_emu_nonreal[];
extern const char s_err_emu_nosolution[];
extern const char s_err_emu_mismatch[];
extern const char s_err_emu_novar[];
extern const char s_err_emu_notfound[];
extern const char s_err_emu_app[];
extern const char s_err_emu_sys[];
extern const char s_err_emu_exists[];
extern const char s_err_emu_cmplx[];
extern const char s_err_emu_solve[];
extern const char s_err_emu_range[];
extern const char s_err_emu_iter[];
extern const char s_err_emu_cond[];
extern const char s_err_emu_blank[];
extern const char s_err_emu_circular[];
extern const char s_err_emu_imroot[];
extern const char s_err_emu_ver[];
extern const char s_err_emu_sd[];
extern const char s_err_emu_sdro[];
extern const char s_err_emu_sdinvalid[];
extern const char s_err_emu_nosd[];
extern const char s_err_emu_timeout[];
extern const char s_err_emu_template[];
extern const char s_err_emu_unknown[];

// FUNCTION: GY455XE  Im 0C10C
static char log_error(unsigned int idx) {
	char *error_buf;
	int i;
	error_emu errors[47];
	emu_kb kb;
	
	errors[0].index = 1;
	errors[0].add_template = 0;
	errors[0].string = s_err_emu_acbreak;
	errors[1].index = 2;
	errors[1].add_template = 1;
	errors[1].string = s_err_emu_syntax;
	errors[2].index = 3;
	errors[2].add_template = 1;
	errors[2].string = s_err_emu_math;
	errors[3].index = 4;
	errors[3].add_template = 1;
	errors[3].string = s_err_emu_mem;
	errors[4].index = 5;
	errors[4].add_template = 1;
	errors[4].string = s_err_emu_go;
	errors[5].index = 6;
	errors[5].add_template = 1;
	errors[5].string = s_err_emu_nesting;
	errors[6].index = 7;
	errors[6].add_template = 1;
	errors[6].string = s_err_emu_stack;
	errors[7].index = 8;
	errors[7].add_template = 1;
	errors[7].string = s_err_emu_argument;
	errors[8].index = 9;
	errors[8].add_template = 1;
	errors[8].string = s_err_emu_dimension;
	errors[9].index = 10;
	errors[9].add_template = 1;
	errors[9].string = s_err_emu_com;
	errors[10].index = 11;
	errors[10].add_template = 1;
	errors[10].string = s_err_emu_transmit;
	errors[11].index = 12;
	errors[11].add_template = 1;
	errors[11].string = s_err_emu_receive;
	errors[12].index = 13;
	errors[12].add_template = 0;
	errors[12].string = s_err_emu_outofmem;
	errors[13].index = 14;
	errors[13].add_template = 0;
	errors[13].string = s_err_emu_undefined;
	errors[14].index = 15;
	errors[14].add_template = 1;
	errors[14].string = s_err_emu_overflow;
	errors[15].index = 16;
	errors[15].add_template = 1;
	errors[15].string = s_err_emu_domain;
	errors[16].index = 17;
	errors[16].add_template = 1;
	errors[16].string = s_err_emu_nonreal;
	errors[17].index = 18;
	errors[17].add_template = 0;
	errors[17].string = s_err_emu_nosolution;
	errors[18].index = 19;
	errors[18].add_template = 0;
	errors[18].string = s_err_emu_mismatch;
	errors[19].index = 20;
	errors[19].add_template = 0;
	errors[19].string = s_err_emu_novar;
	errors[20].index = 21;
	errors[20].add_template = 0;
	errors[20].string = s_err_emu_notfound;
	errors[21].index = 22;
	errors[21].add_template = 1;
	errors[21].string = s_err_emu_app;
	errors[22].index = 23;
	errors[22].add_template = 1;
	errors[22].string = s_err_emu_sys;
	errors[23].index = 24;
	errors[23].add_template = 0;
	errors[23].string = s_err_emu_exists;
	errors[24].index = 25;
	errors[24].add_template = 0;
	errors[24].string = s_err_emu_cmplx;
	errors[25].index = 26;
	errors[25].add_template = 0;
	errors[25].string = s_err_emu_cmplx;
	errors[26].index = 27;
	errors[26].add_template = 0;
	errors[26].string = s_err_emu_solve;
	errors[27].index = 28;
	errors[27].add_template = 1;
	errors[27].string = s_err_emu_range;
	errors[28].index = 29;
	errors[28].add_template = 1;
	errors[28].string = s_err_emu_iter;
	errors[29].index = 30;
	errors[29].add_template = 1;
	errors[29].string = s_err_emu_cond;
	errors[30].index = 31;
	errors[30].add_template = 0;
	errors[30].string = s_err_emu_blank;
	errors[31].index = 32;
	errors[31].add_template = 0;
	errors[31].string = s_err_emu_blank;
	errors[32].index = 33;
	errors[32].add_template = 1;
	errors[32].string = s_err_emu_range;
	errors[33].index = 34;
	errors[33].add_template = 1;
	errors[33].string = s_err_emu_circular;
	errors[34].index = 35;
	errors[34].add_template = 0;
	errors[34].string = s_err_emu_imroot;
	errors[35].index = 36;
	errors[35].add_template = 1;
	errors[35].string = s_err_emu_ver;
	errors[36].index = 37;
	errors[36].add_template = 1;
	errors[36].string = s_err_emu_sd;
	errors[37].index = 38;
	errors[37].add_template = 0;
	errors[37].string = s_err_emu_sdro;
	errors[38].index = 39;
	errors[38].add_template = 0;
	errors[38].string = s_err_emu_sdinvalid;
	errors[39].index = 40;
	errors[39].add_template = 0;
	errors[39].string = s_err_emu_nosd;
	errors[40].index = 41;
	errors[40].add_template = 0;
	errors[40].string = s_err_emu_blank;
	errors[41].index = 42;
	errors[41].add_template = 0;
	errors[41].string = s_err_emu_blank;
	errors[42].index = 43;
	errors[42].add_template = 0;
	errors[42].string = s_err_emu_blank;
	errors[43].index = 44;
	errors[43].add_template = 0;
	errors[43].string = s_err_emu_blank;
	errors[44].index = 45;
	errors[44].add_template = 0;
	errors[44].string = s_err_emu_blank;
	errors[45].index = 46;
	errors[45].add_template = 0;
	errors[45].string = s_err_emu_blank;
	errors[46].index = 47;
	errors[46].add_template = 0;
	errors[46].string = s_err_emu_timeout;

	init_emu_kb(&kb);
	switch (idx) {
		case ERROR_ACBREAK:
			idx = 1;  // ACBREAK
			break;
		case ERROR_SYNTAX:
			idx = 2;  // SyntaxERROR
			break;
		case ERROR_MATH:
			idx = 3;  // MaERROR
			break;
		case ERROR_OUTOFMEM:
			idx = 4;  // MemoryERROR
			break;
		case ERROR_STACK:
			idx = 7;  // StackERROR
			break;
		case ERROR_ARGUMENT:
			idx = 8;  // ArgumentERROR
			break;
		case ERROR_DIMENSION:
			idx = 9;  // DimensionERROR
			break;
		case ERROR_SOLVE:
			idx = 27;  // Can't Solve!
			break;
		case ERROR_TIMEOUT:
			idx = 47;  // Time out
			break;
		case ERROR_VARIABLE:
			idx = 20;  // No Variable
			break;
		default:
			idx = 23;  // SystemERROR
			break;
	}
	error_buf = kb.error_buf;
	if (!idx) smart_strcpy(error_buf, s_err_emu_blank);
	else {
		for (i = 0; i < 47; i++) {
			if (errors[i].index == idx) {
				smart_strcpy(error_buf, errors[i].string);
				if (errors[i].add_template) smart_strcat(error_buf, s_err_emu_template);
				f_082A2_E(&kb);
				return;
			}
		}
		smart_strcpy(error_buf, s_err_emu_unknown);  // ??? ERROR (impossible)
		f_082A2_E(&kb);
	}
	return;
}

// FUNCTION: GY455XE  Im 0204C
const char s_err_emu_acbreak[] = "ACBREAK";

// FUNCTION: GY455XE  Im 02054
const char s_err_emu_syntax[] = "Syntax";

// FUNCTION: GY455XE  Im 0205B
const char s_err_emu_math[] = "Ma";

// FUNCTION: GY455XE  Im 0205E
const char s_err_emu_mem[] = "Memory";

// FUNCTION: GY455XE  Im 02065
const char s_err_emu_go[] = "Go";

// FUNCTION: GY455XE  Im 02068
const char s_err_emu_nesting[] = "Nesting";

// FUNCTION: GY455XE  Im 02070
const char s_err_emu_stack[] = "Stack";

// FUNCTION: GY455XE  Im 02076
const char s_err_emu_argument[] = "Argument";

// FUNCTION: GY455XE  Im 0207F
const char s_err_emu_dimension[] = "Dimension";

// FUNCTION: GY455XE  Im 02089
const char s_err_emu_com[] = "Com";

// FUNCTION: GY455XE  Im 0208D
const char s_err_emu_transmit[] = "Transmit";

// FUNCTION: GY455XE  Im 02096
const char s_err_emu_receive[] = "Receive";

// FUNCTION: GY455XE  Im 0209E
const char s_err_emu_outofmem[] = "Memory Full";

// FUNCTION: GY455XE  Im 020AA
const char s_err_emu_undefined[] = "Undefined";

// FUNCTION: GY455XE  Im 020B4
const char s_err_emu_overflow[] = "Overflow";

// FUNCTION: GY455XE  Im 020BD
const char s_err_emu_domain[] = "Domain";

// FUNCTION: GY455XE  Im 020C4
const char s_err_emu_nonreal[] = "Non-Real";

// FUNCTION: GY455XE  Im 020CD
const char s_err_emu_nosolution[] = "No Solution";

// FUNCTION: GY455XE  Im 020D9
const char s_err_emu_mismatch[] = "Mismatch";

// FUNCTION: GY455XE  Im 020E2
const char s_err_emu_novar[] = "No Variable";

// FUNCTION: GY455XE  Im 020EE
const char s_err_emu_notfound[] = "Not Found";

// FUNCTION: GY455XE  Im 020F8
const char s_err_emu_app[] = "Application";

// FUNCTION: GY455XE  Im 02104
const char s_err_emu_sys[] = "System";

// FUNCTION: GY455XE  Im 0210B
const char s_err_emu_exists[] = "Already Exists";

// FUNCTION: GY455XE  Im 0211A
const char s_err_emu_cmplx[] = "Complex Number In List";

// FUNCTION: GY455XE  Im 02131
const char s_err_emu_solve[] = "Can't Solve!";

// FUNCTION: GY455XE  Im 0213E
const char s_err_emu_range[] = "Range";

// FUNCTION: GY455XE  Im 02144
const char s_err_emu_iter[] = "Iteration";

// FUNCTION: GY455XE  Im 0214E
const char s_err_emu_cond[] = "Condition";

// FUNCTION: GY455XE  Im 02158
const char s_err_emu_blank[] = "";

// FUNCTION: GY455XE  Im 02159
const char s_err_emu_circular[] = "Circular";

// FUNCTION: GY455XE  Im 02162
const char s_err_emu_imroot[] = "No Real Roots";

// FUNCTION: GY455XE  Im 02170
const char s_err_emu_ver[] = "Version";

// FUNCTION: GY455XE  Im 02178
const char s_err_emu_sd[] = "SD Card";

// FUNCTION: GY455XE  Im 02180
const char s_err_emu_sdro[] = "SD Card is protected";

// FUNCTION: GY455XE  Im 02195
const char s_err_emu_sdinvalid[] = "invarid Card";

// FUNCTION: GY455XE  Im 021A2
const char s_err_emu_nosd[] = "No Card";

// FUNCTION: GY455XE  Im 021AA
const char s_err_emu_timeout[] = "Time out";

// FUNCTION: GY455XE  Im 021B3
const char s_err_emu_template[] = "ERROR";

// FUNCTION: GY455XE  Im 021B9
const char s_err_emu_unknown[] = "??? ERROR";

#endif

// FUNCTION: GY454XE  Re 0B7E6
// FUNCTION: GY455XE  Im 0C59A
static char show_error(char idx) {
	char keycode;

#if REAL == 0
	log_error(idx);
#endif
	print_error(idx);
	do keycode = getkeycode(1);
	while (!is_ac_key(keycode));
	return keycode;
}

// ====== DEFINITIONS (declared above) ======

// FUNCTION: GY454XE  Re 02044
const char tokens_map[16] = {
	0xAE,	// K_FRAC
	0x7C,	// K_FRAC_ABC
	0x5E,	// K_POW
	0x98,	// K_SQRT
	0x77,	// K_POW_M1
	0x75,	// K_POW_2
	0x76,	// K_POW_3
	0xA8,	// K_CBRT
	0x9F,	// K_NTH_RT
	0x73,	// K_E_POW
	0x93,	// K_10_POW
	0x68,	// K_LOGAB
	0x63,	// K_ABS
	0x6A,	// K_INTEGRAL
	0x6B,	// K_DDX
	0x69	// K_SUM
};

// FUNCTION: GY454XE  Re 02054
const char s_colon[] = ":";

// For keycode lists, every row is KI(1-8), every column is KO(1-8)

// FUNCTION: GY454XE  Re 02056
const char keycodes[64] = {
	K_1,		K_2,		K_3,		K_PLUS,		K_MINUS,	NULL,		K_EXECUTE,	NULL,
	K_4,		K_5,		K_6,		K_MUL,		K_DIV,		NULL,		K_ANS,		NULL,
	K_7,		K_8,		K_9,		K_DEL,		K_AC,		NULL,		K_EXP,		NULL,
	K_RCL,		K_ENG,		K_PAREN_L,	K_PAREN_R,	K_FMT_DEC,	K_M_PLUS,	K_PERIOD,	NULL,
	K_NEGATIVE,	K_DMS,		K_HYP,		K_SIN,		K_COS,		K_TAN,		K_0,		NULL,
	K_FRAC,		K_SQRT,		K_POW_2,	K_POW,		K_LOG,		K_LN,		NULL,		NULL,
	K_CALC,		K_INTEGRAL,	K_LEFT,		K_DOWN,		K_POW_M1,	K_LOGAB,	NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		NULL,		NULL
};

// FUNCTION: GY454XE  Re 02096
const char keycodes_shift[64] = {
	K_SD,		K_CMPLX,	K_BASE,		K_POL,		K_REC,		NULL,		K_APPROX,	NULL,
	K_MATRIX,	K_VECTOR,	NULL,		K_PERMU,	K_COMBI,	NULL,		K_DRG,		NULL,
	K_CONST,	K_CONV,		K_CLR,		K_INS,		K_OFF,		NULL,		K_PI,		NULL,
	K_STO,		K_ENG_R,	K_PERCENT,	K_COMMA,	K_FMT_FRAC,	K_M_MINUS,	K_RAN,		NULL,
	K_NEGATIVE,	K_DMS_R,	K_ABS,		K_ARCSIN,	K_ARCCOS,	K_ARCTAN,	K_RND,		NULL,
	K_FRAC_ABC,	K_CBRT,		K_POW_3,	K_NTH_RT,	K_10_POW,	K_E_POW,	NULL,		NULL,
	K_SOLVE,	K_DDX,		K_LEFT,		K_DOWN,		K_FCTRIAL,	K_SUM,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_SETUP,	NULL,		NULL,		NULL
};

// FUNCTION: GY454XE  Re 020D6
const char keycodes_alpha[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		K_EULER,	NULL,
	K_RCL,		NULL,		NULL,		K_VAR_X,	K_VAR_Y,	K_VAR_M,	K_RANINT,	NULL,
	K_VAR_A,	K_VAR_B,	K_VAR_C,	K_VAR_D,	K_VAR_E,	K_VAR_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	K_EQUALS,	K_COLON,	K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		K_COLON,	K_EQUALS,	NULL
};

// FUNCTION: GY454XE  Re 02116
const char keycodes_rcl[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		NULL,		K_RCL_X,	K_RCL_Y,	K_RCL_M,	NULL,		NULL,
	K_RCL_A,	K_RCL_B,	K_RCL_C,	K_RCL_D,	K_RCL_E,	K_RCL_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		K_CALC,		NULL
};

// FUNCTION: GY454XE  Re 02156
const char keycodes_sto[64] = {
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		K_EXECUTE,	K_APPROX,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		NULL,		K_STO_X,	K_STO_Y,	K_STO_M,	NULL,		NULL,
	K_STO_A,	K_STO_B,	K_STO_C,	K_STO_D,	K_STO_E,	K_STO_F,	NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		K_CALC,		NULL
};

// FUNCTION: GY454XE  Re 02196
const char keycodes_base_n[64] = {
	K_1,		K_2,		K_3,		K_PLUS,		K_MINUS,	NULL,		K_APPROX,	K_APPROX,
	K_4,		K_5,		K_6,		K_MUL,		K_DIV,		NULL,		K_ANS,		NULL,
	K_7,		K_8,		K_9,		K_DEL,		K_AC,		NULL,		NULL,		NULL,
	K_RCL,		NULL,		K_PAREN_L,	K_PAREN_R,	NULL,		K_M_PLUS,	NULL,		NULL,
	K_HEX_A,	K_HEX_B,	K_HEX_C,	K_HEX_D,	K_HEX_E,	K_HEX_F,	K_0,		NULL,
	NULL,		NULL,		K_BASE_DEC,	K_BASE_HEX,	K_BASE_BIN,	K_BASE_OCT,	NULL,		NULL,
	K_CALC,		NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_MODE,		NULL,		NULL,		NULL
};

// FUNCTION: GY454XE  Re 021D6
const char keycodes_shift_base_n[64] = {
	NULL,		NULL,		K_BASE,		NULL,		NULL,		NULL,		K_APPROX,	NULL,
	NULL,		K_VECTOR,	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		K_CLR,		K_INS,		K_OFF,		NULL,		NULL,		NULL,
	K_STO,		NULL,		NULL,		NULL,		NULL,		K_M_MINUS,	NULL,		NULL,
	K_HEX_A,	K_HEX_B,	K_HEX_C,	K_HEX_D,	K_HEX_E,	K_HEX_F,	NULL,		NULL,
	NULL,		NULL,		K_BASE_DEC,	K_BASE_HEX,	K_BASE_BIN,	K_BASE_OCT,	NULL,		NULL,
	K_SOLVE,	NULL,		K_LEFT,		K_DOWN,		NULL,		NULL,		NULL,		NULL,
	K_SHIFT,	K_ALPHA,	K_UP,		K_RIGHT,	K_SETUP,	NULL,		NULL,		NULL
};

// FUNCTION: GY454XE  Re 02216
const char menu_mode[] = {
	"1:COMP  2:CMPLX \0"
	"3:STAT  4:BASE-N\0"
	"5:EQN   6:MATRIX\0"
	"7:TABLE 8:VECTOR"
};

// FUNCTION: GY454XE  Re 0225A
const char menu_setup_0[] = {
	"1:MthIO 2:LineIO\0"
	"3:Deg   4:Rad   \0"
	"5:Gra   6:Fix   \0"
	"7:Sci   8:Norm  \0"
};

// FUNCTION: GY454XE  Re 0229F
const char menu_setup_1[] = {
	"1:ab/c  2:d/c   \0"
	"3:CMPLX 4:STAT  \0"
	"5:Disp  6:" "\x9f" "CONT" "\x9e" "\0"
	"\0"
};

// FUNCTION: GY454XE  Re 022D4
const char menu_stat_type[] = {
	"1:1-VAR 2:" "\x9a" "+" "\x9b" "X\0"
	"3:" "\x5f" "+" "\x9c" "X" "\xa2" " 4:ln X\0"
	"5:" "\x81" "^X   6:" "\x9a" "*" "\x9b" "^X\0"
	"7:" "\x9a" "*X^" "\x9b" " 8:1/X"
};

// FUNCTION: GY454XE  Re 02310
const char menu_stat_table[] = {
	"1:Type  2:Data\0"
	"3:Edit\0"
	"\0"
	"\0"
};

// FUNCTION: GY454XE  Re 02329
const char menu_stat_1var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Distr 6:MinMax\0"
	"\0"
};

// FUNCTION: GY454XE  Re 02359
const char menu_stat_2var[] = {
	"1:Type  2:Data\0"
	"3:Sum   4:Var\0"
	"5:Reg   6:MinMax\0"
	"\0"
};

// FUNCTION: GY454XE  Re 02389
const char menu_eqn[] = {
	"1:a" "\x9d" "X+b" "\x9d" "Y=c" "\x9d" "\0"
	"2:a" "\x9d" "X+b" "\x9d" "Y+c" "\x9d" "Z=d" "\x9d" "\0"
	"3:aX" "\xa2" "+bX+c=0\0"
	"4:aX" "\xa3" "+bX" "\xa2" "+cX+d=0\0"
};

// FUNCTION: GY454XE  Re 023C6
const menu menus[] = {
// n		 String						Return	Char	Option values/Menu indexes/Tokens (up to 8)			Up		Down	Left	Keycode
// Placeholder
/* 0  */	{NULL,						0xff,	0,		{0},												NULL,	NULL,	NULL,	NULL},

// CLR menu (SHIFT 9)
/* 1  */	{menu_clr,					0xff,	0,		{1, 2, 3},											NULL,	NULL,	NULL,	K_CLR},

// SETUP menu (SHIFT MODE)
/* 2  */	{menu_setup_0,				0x7f,	0,		{4, 2, 3, 4, 5, 6, 7, 8},							NULL,	3,		NULL,	K_SETUP},
/* 3  */	{menu_setup_1,				0xc7,	0,		{9, 10, 5, 6, 7, 11},								2,		NULL,	NULL,	K_SETUP},
/* 4  */	{menu_setup_decimalo,		0xff,	0,		{22, 23},											NULL,	NULL,	2,		K_SETUP},
/* 5  */	{menu_setup_cmplx_result,	0xff,	0,		{12, 13},											NULL,	NULL,	3,		K_SETUP},
/* 6  */	{menu_setup_stat_freq,		0xff,	0,		{14, 15},											NULL,	NULL,	3,		K_SETUP},
/* 7  */	{menu_setup_decimal_mark,	0xff,	0,		{18, 19},											NULL,	NULL,	3,		K_SETUP},

// MATRIX mode (SHIFT 4)
/* 8  */	{menu_matrix_table,			0xff,	0,		{1, 2},												NULL,	NULL,	NULL,	K_MATRIX},
/* 9  */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 10 */	{menu_matrix_data,			0x1f,	0,		{6, 7, 8},											NULL,	NULL,	0xfd,	K_MATRIX},
/* 11 */	{menu_matrix_dim0,			0xff,	0,		{9, 10, 11, 12, 13, 14},							NULL,	12,		0xff,	K_MATRIX},
/* 12 */	{menu_matrix_dim1,			0xff,	0,		{15, 16, 17},										11,		NULL,	0xff,	K_MATRIX},
/* 13 */	{menu_matrix,				0xff,	0x3f,	{1, 2, 0xc8, 0xc9, 0xca, 0xcb, 0xc0, 0xc1},			NULL,	NULL,	NULL,	K_MATRIX},

// VECTOR mode (SHIFT 5)
/* 14 */	{menu_vector_table,			0xff,	0,		{18, 19},											NULL,	NULL,	NULL,	K_VECTOR},
/* 15 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 16 */	{menu_vector_data,			0x1f,	0,		{23, 24, 25},										NULL,	NULL,	0xfa,	K_VECTOR},
/* 17 */	{menu_vector_dim,			0xff,	0,		{26, 27},											NULL,	NULL,	0xfc,	K_VECTOR},
/* 18 */	{menu_vector,				0xff,	0x3f,	{18, 19, 0xcc, 0xcd, 0xce, 0xcf, 0x9e},				NULL,	NULL,	NULL,	K_VECTOR},

// CMPLX mode (SHIFT 2)
/* 19 */	{menu_cmplx,				0xff,	0xf0,	{0xc3, 0x88, 0x56, 0x55},							NULL,	NULL,	NULL,	K_CMPLX},

// BASE-N mode (SHIFT 3)
/* 20 */	{menu_base_n_0,				0xff,	0xfc,	{0x6e, 0x6f, 0x7e, 0x7f, 0xc1, 0x62},				NULL,	21,		NULL,	K_BASE},
/* 21 */	{menu_base_n_1,				0xff,	0xfc,	{0x51, 0x50, 0x53, 0x52},							20,		NULL,	NULL,	K_BASE},

// Angle unit (DRG>) menu (SHIFT Ans)
/* 22 */	{menu_drg,					0xff,	0xe0,	{0x85, 0x86, 0x87},									NULL,	NULL,	NULL,	K_DRG},

// Hyperbolic functions (hyp) menu
/* 23 */	{menu_hyp,					0xff,	0xfc,	{0x70, 0x71, 0x72, 0x90, 0x91, 0x92},				NULL,	NULL,	NULL,	K_HYP},

// STAT mode (SHIFT 1)
/* 24 */	{menu_stat_table,			0x5f,	0,		{27, 20, 28},										NULL,	NULL,	NULL,	K_SD},
/* 25 */	{menu_stat_1var,			0x43,	0,		{27, 20, 29, 31, 35, 33},							NULL,	NULL,	NULL,	K_SD},
/* 26 */	{menu_stat_2var,			0x4b,	0,		{27, 20, 30, 32, 23, 34},							NULL,	NULL,	NULL,	K_SD},
/* 27 */	{menu_stat_type,			0xff,	0,		{1, 2, 3, 4, 5, 6, 7, 8},							NULL,	NULL,	0xff,	K_SD},
/* 28 */	{menu_stat_data_edit,		0xff,	0,		{21, 22},											NULL,	NULL,	24,		K_SD},
/* 29 */	{menu_stat_1var_sum,		0xff,	0,		{0x10, 0x11},										NULL,	NULL,	25,		K_SD},
/* 30 */	{menu_stat_2var_sum,		0xff,	0,		{0x10, 0x11, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18},	NULL,	NULL,	26,		K_SD},
/* 31 */	{menu_stat_1var_var,		0xff,	0,		{0x12, 0x89, 0xaa, 0xab},							NULL,	NULL,	25,		K_SD},
/* 32 */	{menu_stat_2var_var,		0xff,	0,		{0x12, 0x89, 0xaa, 0xab, 0x8a, 0xac, 0xad},			NULL,	NULL,	26,		K_SD},
/* 33 */	{menu_stat_1var_minmax,		0xff,	0xc0,	{0x19, 0x1a},										NULL,	NULL,	25,		K_SD},
/* 34 */	{menu_stat_2var_minmax,		0xff,	0xf0,	{0x19, 0x1a, 0x1b, 0x1c},							NULL,	NULL,	26,		K_SD},
/* 35 */	{menu_stat_distr,			0xff,	0xf0,	{0xd0, 0xd1, 0xd2, 0xd3},							NULL,	NULL,	25,		K_SD},
/* 36 */	{menu_stat_reg,				0xff,	0xf8,	{0x9a, 0x9b, 0x9d, 0x65, 0x66},						NULL,	NULL,	26,		K_SD},
/* 37 */	{menu_stat_reg_quad,		0xff,	0xfc,	{0x9a, 0x9b, 0x9c, 0x64, 0x67, 0x66},				NULL,	NULL,	26,		K_SD},

// EQN mode
/* 38 */	{menu_eqn,					0xff,	0,		{1, 2, 3, 4},										NULL,	NULL,	NULL,	NULL},

// MATRIX mode (submode select)
/* 39 */	{menu_matrix_data,			0xff,	0,		{3, 4, 5},											NULL,	NULL,	NULL,	NULL},

// VECTOR mode (submode select)
/* 40 */	{menu_vector_data,			0xff,	0,		{20, 21, 22},										NULL,	NULL,	NULL,	NULL},


// STAT mode (submode select)
/* 41 */	{menu_stat_type,			0xff,	0,		{1, 2, 3, 4, 5, 6, 7, 8},							NULL,	NULL,	NULL,	NULL},

// MENU mode
/* 42 */	{menu_mode,					0xff,	0,		{MODE_COMP, MODE_CMPLX, MODE_STAT, MODE_BASE_N,
														MODE_EQN, MODE_MATRIX, MODE_TABLE, MODE_VECTOR},	NULL,	NULL,	NULL,	K_MODE}, 
};

// ====== END DEFINITIONS ======

// FUNCTION: GY454XE  Re 0B804
char diag_initloop(void) {
	char v0;
	int v1;
	scancode v2;
	scancode v3;

	v0 = 0;
	v1 = 2381;
	v2.b.ki = 0x80;
	v2.b.ko = 1;
j_0b81c:
	if (v1--) {
		delay(20);
		if (!check_key_kio(&v2)) {
j_0b850:
			clr_all_ko();
			clr_all_kimask();
			delay(1);
			if (get_key_kio(&v3) && check_key_kio2(&v3)) {
				if (v3.b.ki == 4 && v3.b.ko == 4) {
					v0 = 1;
					goto j_0b838;
				} else if (v3.b.ki != 4 || v3.b.ko != 0x10) {
					v2.kio = v3.kio;
					goto j_0b81c;
				} else goto j_0b838;
			}
		}
		set_all_kimask();
		set_all_ko();
j_0b898:
		if (!--v1) {
			clr_all_ko();
			clr_all_kimask();
			goto j_0b838;
		}
		delay(20);
		if (is_key_pressed()) goto j_0b850;
		else goto j_0b898;
	}

j_0b838:
	clr_all_ko();
	return v0;
}

// FUNCTION: GY454XE  Re 0B8B8
void f_0B8B8(char a) {
	char v0;

	v0 = 1;
	if (a & (1 << 7)) v0 = 0;

	a &= 0xf;
	d_08120 = 0;
	d_080FE = 1;
	d_080FF = 0;
	if (v0) {
		memzero(input_area, 100);
		cursor_pos_byte = 0;
	}
	formula_x = 0;
	formula_y = 1;
	font_size = 10;
	if (!f_0A564()) {
		if (a) buffer_clear();
	} else {
		font_size = 7;
		formula_y = 25;
		line_print(formula_x, formula_y, s_blank_line);
	}
	cursor_x = 0;
	cursor_y = 1;
	f_10FD2();
	d_08117 = font_size;
	f_044CE();
	d_08124 = 0;
	use_output_charset = 0;
	input_area_ptr = input_area;
	if (a == 2) f_0B998();
	return;
}

// STUB: GY454XE  Re 0B968
void f_0B968(void) {
	return;
}

// FUNCTION: GY454XE  Re 0B984
static char conv_func_token(char keycode) {
	if (keycode < K_FRAC || keycode > K_SUM) return 0;
	else return tokens_map[keycode - 0xd0];
}

// FUNCTION: GY454XE  Re 0B998
void f_0B998(void) {
	char num[10];

	if (!f_08ADC() && !is_mathi()) {
		num_fromdigit(&num, 0);
		num_output_print(&num);
	}
	return;
}

// FUNCTION: GY454XE  Re 0B9C8
static char f_0B9C8(f_09962_struct *a) {
	f_0B8B8(2);
	if (a->unk_0x07) {
		f_044B6();
		num_output_print(a->result_ptr);
		a->unk_0x04 = 1;
		return 0;
	} else {
		clear_result();
		if (a->mode == MODE_TABLE) {
			if (d_080FD) smart_strcpy(a->input_area_ptr, cache_area);
			else memzero(cache_area, 100);
			buffer_clear();
		}
		f_0AF16();
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0BA28
static void f_0BA28(f_09962_struct *a) {
	f_0B8B8(1);
	f_0AF0A();
	table_mode = TABLE_EQN;
	last_key_keycode = NULL;
	d_08125 = 0;
	a->unk_0x04 = 0;
	return;
}

// STUB: GY454XE  Re 0BA50
void f_0BA50(void) {
	return;
}

// FUNCTION: GY454XE  Re 0BAA8
void f_0BAA8(char *a) {
	if (table_mode == 1 && smart_strlen(a) == 1 && f_14516(*a)) {
		a[1] = 0x8b;  // Ans
		a[2] = 0;
	}
	return;
}

// FUNCTION: GY454XE  Re 0BADA
static char f_0BADA(void) {
	if (!f_02CB6())
j_0bae4:
		return 0;
	else if (!(mode & (1 << 7))) return 1;
	else goto j_0bae4;
}

// FUNCTION: GY454XE  Re 0BAF2
static char f_0BAF2(char **a) {
	*a = input_area;
	if (!f_02CB6()) {
		if (d_080FE & (1 << 6)) input_area_ptr = d_0812A;
		else {
			f_0BAA8(*a);
			if (!(mode & (1 << 7)) && (table_mode == 1 || table_mode & (1 << 7))) smart_strcpy(cache_area, input_area);
		}
	}
	return 1;
}

// FUNCTION: GY454XE  Re 0BB42
static char f_0BB42(char **a) {
	char v0;
	char *v1;

	v0 = 1;
	v1 = input_area;
	*a = f_11030();
	if (table_mode == TABLE_RANGE && d_080FD == 4) smart_strcpy(v1, cache_area);
	else if (!(d_080FE & (1 << 6))) {
		f_0BAA8(v1);
		smart_strcpy(cache_area, v1);
	}
	if (!is_mathi()) {
		smart_strcpy(*a, v1);
		if (d_080FE & (1 << 6)) input_area_ptr += (char)(d_0812A - *a);
	} else if (v0 = f_06B52(v1)) {
		f_06C54(v1, *a, 0, 1);
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0BBDA
static char *f_0BBDA(char *a) {
	char v0;
	char *v1;

	v0 = 10;
	if (!(v1 = get_calc_history_addr())) return NULL;
	if (*a++ & 1) v0 = 20;
	a += 2;
	a += v0;
	a = strchr(a, ':');
	if (a > v1 + 250) return NULL;
	if (!a) return NULL;
	return ++a;
}

// FUNCTION: GY454XE  Re 0BC34
char f_0BC34(void) {
	char v0;
	char *v1;

	v0 = 0;
	if ((v1 = get_calc_history_addr()) && (mode == MODE_EQN || f_0B7B6())) {
		v0 = 0;
		do if (v1 = f_0BBDA(v1)) break;
		while (1);
	}
	return v0;
}

// FUNCTION: GY454XE  Re 0BC6C
static char f_0BC6C(char *a) {
	char *v0;
	char *v1;

	v0 = a;
	do {
		v1 = a;
		if (!(a = f_0BBDA(a))) break;
	} while (1);
	return v1 - v0;
}

// FUNCTION: GY454XE  Re 0BC90
void f_0BC90(void) {
	char *v0;
	char v1;
	char v2;
	char *v3;
	char v4;
	char v5;
	char *v6;
	char loc_m1;
	char loc_m2;
	char *loc_m4;

	if (loc_m4 = get_calc_history_addr()) {
		v0 = input_area_ptr;
		v1 = 10;
		loc_m1 = d_080FF;
		if (num_invalid__(&result[10]) != 1) {
			loc_m1 |= 0x80;
			v1 = 20;
		}
		v2 = d_080FE & (1 << 6) ? strchr(v0, ':') - v0 : smart_strlen(v0);
		loc_m2 = v1 + v2 + 4;
		if (loc_m2 <= 250) {
			v3 = loc_m4;
			v4 = f_0BC6C(loc_m4);
			v5 = 0;
			while (v4 + loc_m2 > v5 + 250) {
				v6 = v3;
				v3 = f_0BBDA(v3);
				v5 += v3 - v6;
			}
			v4 -= v5;
			memmove(loc_m4, v3, v4);
			loc_m4 += v4;
			memzero(loc_m4, v5);
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			loc_m1 = d_08100;
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			loc_m1 = d_08101;
			v3 = loc_m4++;
			memmove(v3, &loc_m1, 1);
			memcpy(loc_m4, result, v1);
			memcpy(loc_m4, v0, v2);
			loc_m4 += v2;
			memcpy(loc_m4, s_colon, 1);
		}
	}
	return;
}

// STUB: GY454XE  Re 0BDFA
void f_0BDFA(void) {
	return;
}

// FUNCTION: GY454XE  Re 0BEEE
static char f_0BEEE(f_09962_struct *a) {
	char v0;

	if (!f_03698(last_key_keycode))
j_0bf04:
		return 4;
	v0 = table_mode;
	if (d_080FE & (1 << 7)) {
		if (!f_02CB6()) {
			if (!(v0 & (1 << 7)) && v0 == 6) f_0AF16();
		}
		f_0B8B8(0x82);
	} else if (a->mode != MODE_TABLE && a->mode != MODE_EQN) {
		if (f_03660()) {
			f_0AF16();
			f_0B8B8(0x80);
		} else if (!(char)*a->input_area_ptr && v0 == 1) smart_strcpy(a->input_area_ptr, cache_area);
		else goto j_0bf04;
		cursor_pos_byte = last_key_keycode == K_RIGHT ? 0 : smart_strlen(a->input_area_ptr);
	} else goto j_0bf04;
	f_046AE();
	return 1;
}

// FUNCTION: GY454XE  Re 0BF8A
static char f_0BF8A(void) {
	if (f_03698(last_key_keycode) && is_matho() && d_0812C) {
		f_046AE();
		if (last_key_keycode == K_RIGHT) ++cursor_pos_byte;
		else if (cursor_pos_byte) --cursor_pos_byte;
		else f_046C4();
		return 3;
	}
	return 4;
}

// STUB: GY454XE  Re 0BFDC
static char f_0BFDC(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 0C084
void print_result(void) {
	num_output_print(result);
}

// FUNCTION: GY454XE  Re 0C08C
static void f_0C08C(f_09962_struct *a) {
	char v0;

	if (f_08ADC() && !a->unk_0x07) {
		if (a->mode != MODE_CMPLX) num_fromdigit(&a->result_ptr[10], 0);
		v0 = cursor_pos_byte;
		f_07B60(a->result_ptr);
		cursor_pos_byte = v0;
	}

	return;
}

// FUNCTION: GY454XE  Re 0C0D0
void f_0C0D0(void) {
	char v0;

	if (f_0B7B6()) f_10EF8();
	else if (table_mode == TABLE_SOLVE && d_080FD == 0x40) print_continue_prompt();
	else {
		v0 = f_02CB6();
		if (d_080FE == 1) {
			d_08122 = 1;
			if (!is_mathi()) f_04F6E();
			else {
				f_10FC0();
				f_058DC();
			}
		} else {
			d_08122 = 0;
			cursor_pos_byte = 0;
			if (table_mode == 1 || table_mode == TABLE_RANGE || (table_mode & (1 << 7))) if (v0) f_02BE8();
		}
	}
	return;
}

// FUNCTION: GY454XE  Re 0C148
static char f_0C148(f_09962_struct *a) {
	char v0;

	v0 = d_080FE;
	d_080FE = 0x80;
	f_044D6(show_error(ERROR_MATH));
	clear_result();
	if (f_0BEEE(a) == 1) {
		if (v0 == 4) {
			f_0AF16();
			smart_strcpy(a->input_area_ptr, cache_area);
		} else cursor_pos_byte = smart_strlen(a->input_area_ptr);
	} else f_0B9C8(a);
}

// FUNCTION: GY454XE  Re 0C1A0
static char f_0C1A0(f_09962_struct *a) {
	if (a->mode == MODE_BASE_N && submode == SMODE_BASE_N_BIN) {
		if (f_149D8(a->result_ptr)) return 4;
		else return f_0C148(a);
	} else return 4;
}

// FUNCTION: GY454XE  Re 0C1D4
static char f_0C1D4(f_09962_struct *a) {
	char loc_m20[20];

	if (f_02C76() || f_02AAA() || d_0812C) return 4;
	num_cpy_im(loc_m20, a->result_ptr);
	if (num_invalid__(&loc_m20[10]) == 1 || !cmplx_abs(loc_m20)) return 4;
	return f_0C148(a);
}

// FUNCTION: GY454XE  Re 0C22E
static char keyfunc_ac(f_09962_struct *a) {
	if (f_0B7B6()) {
		f_0BA28(a);
		return 0;
	} else if (!f_0B9C8(a)) return 0;
	else return 1;
}

// STUB: GY454XE  Re 0C25A
static char keyfunc_sto_rcl(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 0C316
static char keyfunc_rcl(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// FUNCTION: GY454XE  Re 0C31A
static char keyfunc_sto(f_09962_struct *a) {
	return keyfunc_sto_rcl(a);
}

// FUNCTION: GY454XE  Re 0C31E
static char keyfunc_exe(f_09962_struct *a) {
	char v0;
	char v1;
	char *v2;
	char *loc_m2;
	char loc_m22[20];

	v0 = d_080FE & (1 << 6);
	if (f_0B7B6()) {
		v1 = f_0BC34();
		if (++d_0810F > v1) {
			f_0BA28(a);
			goto j_0c396;
		} else {
			f_0BDFA();
			f_085C0();
		}
	} else if (*a->input_area_ptr) {
		if (f_08ADC()) {
			table_mode = TABLE_RANGE;
			d_080FD = 1;
			d_080FE = 3;
			smart_strcpy(cache_area, a->input_area_ptr);
j_0c390:
			a->unk_0x04 = 0;
j_0c396:
			return 0;
		} else {
			char v3;
			input_area_ptr = a->input_area_ptr;
			v1 = f_0BADA() ? f_0BB42(&loc_m2) : f_0BAF2(&loc_m2);
			v2 = loc_m2;
			if (v0) loc_m2 = d_0812A;
			num_cpy_im(loc_m22, a->result_ptr);
			if (v1) {	
				f_04796();
				if (table_mode == TABLE_SOLVE && d_080FD == 4) v1 = f_10000(&loc_m2, a->result_ptr);
				else if (a->mode == MODE_TABLE && d_080FD == 4) {
					v1 = f_042AA(&loc_m2);
					if (!v1) {
						table_mode = TABLE_STAT_TABLE;
						f_0AF0A();
						goto j_0c390;
					}
				} else v1 = f_15CEE(&loc_m2, a->result_ptr);
				if (v1 > 0 && v1 < 32) {
					char tmp = loc_m2 - v2;
					if (a->unk_0x08) f_06C54(a->input_area_ptr, v2, tmp, 0);
					else cursor_pos_byte = tmp;
				}
			} else v1 = ERROR_SYNTAX;
			if (v1 > 0 && v1 < 32) {
				d_080FE = 0x80;
				f_044D6(show_error(v1));
				clear_result();
				if (a->unk_0x07) num_cpy_im(a->result_ptr, loc_m22);
				if (f_0BEEE(a) == 1)
j_0c4a6:
					return 1;
				else if (f_0B9C8(a)) goto j_0c4a6;
				else return 0;
 			}
 			f_044B6();
 			f_085C0();
 			d_080FE = 3;
 			v3 = get_numtype(a->result_ptr);
 			// Matrix/Vector pointer
 			if (v3 == 0x60 || v3 == 0x90) {
 				table_mode = v3 == 0x60 ? TABLE_MATRIX : TABLE_VECTOR;
 				f_15486(a->result_ptr);
 				submode = SMODE_MATVCT_ANS;
 				table_home();
 				goto j_0c390;
 			} else {
 				if (!v1 || v1 == 36) {
	 				if (table_mode == TABLE_SOLVE && !a->unk_0x0a) {
	 					d_080FF = 19;
	 					d_080FD = v1 == 36 ? (char)0x40 : (char)0x20;
	 				} else if (a->mode != MODE_CMPLX) num_fromdigit(&a->result_ptr[10], 0);
	 			} else if (v1 == 34) {
	 				if (a->mode != MODE_CMPLX) d_080FF = 18;
	 				else d_08101 = 2;
	 			} else if (v1 == 35) {
	 				if (a->mode != MODE_CMPLX) d_080FF = 17;
	 				else d_08101 = 1;
	 			} else if (v1 == 37) {
	 				if (a->result_ptr[10] == 0x70) num_fromdigit(&a->result_ptr[10], 0);
	 				else d_080FF = 20;
	 			}
	 			if (a->unk_0x0a || (table_mode != 1 && !(table_mode & (1 << 7)))) {
	 				if (d_080FF & (1 << 4)) num_fromdigit(&a->result_ptr[10], 0);
	 				d_080FF = 0;
	 				f_0A410(a->result_ptr);
 					goto j_0c390;
	 			} else {
	 				if (d_080FF & (1 << 4)) {
	 					num_cpy(loc_m22, a->result_ptr);
	 					num_fromdigit(&loc_m22[10], 0);
	 					st_var(VAR_ANS, loc_m22);
	 				} else st_var(VAR_ANS, a->result_ptr);
	 				if (table_mode != TABLE_SOLVE) {
	 					if (loc_m2[-1] == ':') {
	 						v0 = 1;
	 						d_080FE |= 1 << 6;
	 						d_0812A = loc_m2;
	 					} else if (v0) v0 = 0;
	 					if (!v0 && table_mode & (1 << 7) && d_080FD == 3) d_080FD = 0x10;
	 					f_0BC90();
	 				}
	 			}
	 		}
		}
	} else goto j_0c396;
	return 2;
}

// STUB: GY454XE  Re 0C64A
static char keyfunc_abc(f_09962_struct *a) {
	return 0;
}

// STUB: GY454XE  Re 0C692
static char keyfunc_dc(f_09962_struct *a) {
	return 0;
}

// STUB: GY454XE  Re 0C6DE
static char keyfunc_dms(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 0C728
static char keyfunc_fact(f_09962_struct *a) {
	// fx-570/991ES PLUS does not have prime factor
	return 0;
}

// STUB: GY454XE  Re 0C72C
static void f_0C72C(f_09962_struct *a) {
	return;
}

// STUB: GY454XE  Re 0C74C
static char keyfunc_eng(f_09962_struct *a) {
	return 0;
}

// STUB: GY454XE  Re 0C77A
static char keyfunc_eng_r(f_09962_struct *a) {
	return 0;
}

// FUNCTION: GY454XE  Re 0C7A8
static char keyfunc_del(f_09962_struct *a) {
	if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C7C2
static char keyfunc_mov_x(f_09962_struct *a) {
	if (f_0BF8A() == 3) return 3;
	else if (d_080FE & (1 << 6)) return 0;
	else if (f_0BEEE(a) == 1) return 1;
	else if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C806
static char keyfunc_mov_y(f_09962_struct *a) {
	if (d_080FE & (1 << 6)) return 0;
	else if (f_0BFDC(a) == 2) return 2;
	else if (!f_03664()) return 0;
	else {
		f_07488(last_key_keycode);
		return 1;
	}
}

// FUNCTION: GY454XE  Re 0C836
static char keyfunc_base(f_09962_struct *a) {
	submode = base_n_submodes[last_key_keycode - K_BASE_BIN];
	if (f_03664()) {
		if (f_0C1A0(a) == 1) return 1;
		else print_result();
	}
	return 2;
}

// FUNCTION: GY454XE  Re 0C86A
static char keyfunc_mode(f_09962_struct *a) {
	f_0B998();
	return 2;
}

// FUNCTION: GY454XE  Re 0C874
static char keyfunc_setup(f_09962_struct *a) {
	if (is_matho()) f_085D2();
	if (f_03664()) {
		if (a->unk_0x08 && f_0C1D4(a) != 1) f_0C08C(a);
		else return 1;
	}
	return 2;
}
