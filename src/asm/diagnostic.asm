TYPE(ML610CASESplus)
MODEL LARGE

_version			EQU 1:0FFF4H
_checksum			EQU 1:0FFFCH

EXTRN CODE	: _fill_screen
EXTRN CODE	: _buffer_clear
EXTRN CODE	: _line_print_col_0
EXTRN CODE	: _line_print
EXTRN CODE	: _render
EXTRN CODE	: _pd_value
EXTRN CODE	: _f_03558
EXTRN CODE	: _clr_port0
EXTRN CODE	: _set_contrast_sfr
EXTRN CODE	: _clr_all_ko
EXTRN CODE	: _check_ac
EXTRN CODE	: _memset_n
EXTRN CODE	: _set_contrast
EXTRN CODE	: _reset_all
EXTRN CODE	: _getscancode
EXTRN CODE	: _set_modifiers
EXTRN CODE	: _wait_shift
EXTRN CODE	: _diag_initloop
EXTRN DATA	: _cursor_noflash
EXTRN DATA	: _last_key_scancode
EXTRN DATA	: _setup_contrast
EXTRN DATA	: _font_size
EXTRN DATA	: _use_rambuf
EXTRN DATA	: _magic_string
EXTRN DATA	: _screen_buffer

$$NCODdiagnostic SEGMENT CODE 2H ANY
$$NTABdiagnostic SEGMENT TABLE 2H #0

RSEG $$NTABdiagnostic

; All keys in the key check in order. High nibble = KI, low nibble = KO.
; The null byte is actually unnecessary as diag_check_key returns if the key count is
; exactly 49, thus only the first 49 entries are ever used.
; DATA: GY454XE  Re 01EB6
; DATA: GY455XE  Im 01EB8
_diag_keys:
	DB 81H, 82H, 83H, 84H, 85H
	DB 71H, 72H, 73H, 74H, 75H, 76H
	DB 61H, 62H, 63H, 64H, 65H, 66H
	DB 51H, 52H, 53H, 54H, 55H, 56H
	DB 41H, 42H, 43H, 44H, 45H, 46H
	DB 31H, 32H, 33H, 34H, 35H
	DB 21H, 22H, 23H, 24H, 25H
	DB 11H, 12H, 13H, 14H, 15H
	DB 57H, 47H, 37H, 27H, 17H
	DB 0

; DATA: GY454XE  Re 01EE8
; DATA: GY455XE  Im 01EEA
_s_dpressac:
	DB "Press AC", 0

; DATA: GY454XE  Re 01EF1
; DATA: GY455XE  Im 01EF3
_s_dtestok:
	DB "TEST OK", 0

; DATA: GY454XE  Re 01EF9
; DATA: GY455XE  Im 01EFB
_s_dresetall:
	DB "Reset All", 0

; DATA: GY454XE  Re 01F03
; DATA: GY455XE  Im 01F05
_s_diagnostic:
	DB "DIAGNOSTIC ", 0

RSEG $$NCODdiagnostic

; FUNCTION: GY454XE  Re 04898
; FUNCTION: GY455XE  Im 049A6
_diag_init_check:
	PUSH LR
	PUSH XR4
	PUSH QR8
	MOV R8, #BYTE1 KID
	MOV R9, #BYTE2 KID
	MOV R2, #1H
	ST R2, KOD0
	MOV R0, #5H
_$j_048aa:
	L R3, [ER8]
	CMP R3, #01111011B
	BNE _$j_048bc
	ADD R0, #-1H
	BNE _$j_048aa
	MOV R2, #10000000B
	MOV R3, #00000001B
	ST ER2, _last_key_scancode
_$j_048bc:
	BL _clr_all_ko
	BAL _$j_04980

; FUNCTION: GY454XE  Re 048C2
; FUNCTION: GY455XE  Im 049D0
_diagnostic_mode:
	PUSH LR
	PUSH QR0
	MOV R1, #1H
	MOV R0, #0H
	BL _fill_screen
	MOV R0, #1H
	MOV R2, #BYTE1 _s_diagnostic
	MOV R3, #BYTE2 _s_diagnostic
	BL _line_print_small
	MOV R0, #22
	MOV R2, #BYTE1 _s_dpressac
	MOV R3, #BYTE2 _s_dpressac
	BL _line_print_small
	BL _render
	BL _diag_initloop
	CMP R0, #0H
	BEQ _$j_048f2
	BL _diagnostic
_$j_048f2:
	POP QR0
	POP PC

; FUNCTION: GY454XE  Re 048F6
; FUNCTION: GY455XE  Im 04A04
_diagnostic:
	PUSH LR
	PUSH XR4
	PUSH QR8
	ADD SP, #-4H
	MOV FP, SP
	LEA _magic_string
	MOV ER0, #0H
	ST ER0, [EA+]
	ST ER0, [EA+]
	MOV R0, #1H
	ST R0, _cursor_noflash
	MOV R4, #1H
	MOV R0, #11111111B
	BL _diag_scr_fill_ws
	MOV R0, #00000000B
	BL _diag_scr_fill_ws
	BL _diag_scr_box_ws
	BL _diag_scr_ckb1_ws
	BL _diag_scr_ckb2_ws
	BL _diag_checksum
	MOV R0, #0H
	BL _set_modifiers
	MOV R0, #1H
	BL _set_contrast
	BL _diag_check_key
	MOV R2, #BYTE1 _s_dtestok
	MOV R3, #BYTE2 _s_dtestok
	MOV R0, #1
	BL _line_print_small
	MOV R2, #BYTE1 _s_dresetall
	MOV R3, #BYTE2 _s_dresetall
	MOV R0, #15
	BL _line_print_small
	MOV R2, #BYTE1 _s_dpressac
	MOV R3, #BYTE2 _s_dpressac
	MOV R0, #22
	BL _line_print_small
	BL _render
	MOV ER0, FP
	BL _getscancode
	L R0, _setup_contrast
	PUSH R0
	BL _reset_all
	POP R0
	ST R0, _setup_contrast
	BL _set_contrast_sfr
	BL _f_03558
	ADD SP, #4
_$j_04980:
	POP QR8
	POP XR4
	POP PC

; FUNCTION: GY454XE  Re 04986
; FUNCTION: GY455XE  Im 04A94
_diag_check_key:
	PUSH LR
	PUSH XR4
	PUSH QR8
	ADD SP, #-0AH
	MOV FP, SP
	BL _buffer_clear
	MOV ER8, #0H
	ST ER8, 2[FP]
_$j_04998:
	MOV R2, R8
	MOV BP, #1H
_$j_0499c:
	SRL R2, #1H
	BGE _$j_049a4
	ADD R13, R12
	DAA R13
_$j_049a4:
	ADD R12, R12
	DAA R12
	CMP R2, #0H
	BNE _$j_0499c
	MOV R12, R13
	SRL R12, #4
	AND R13, #00001111B
	ADD R12, #30H
	ADD R13, #30H
	ST BP, [FP]
	MOV R0, #1H
	MOV ER2, FP
	BL _line_print_small
	BL _render
	MOV ER0, FP
	BL _getscancode
	L R0, [FP]
	BL _find_bit
	MOV R2, R0
	SLL R2, #4
	L R0, 1[FP]
	BL _find_bit
	OR R2, R0
	MOV R0, #BYTE1 _diag_keys
	MOV R1, #BYTE2 _diag_keys
	ADD ER0, ER8
	L R0, [ER0]
	CMP R0, R2
	BNE _$j_04998
	ADD R8, #1
	CMP R8, #49
	BNE _$j_04998
	ADD SP, #0AH
	BAL _$j_04980

; FUNCTION: GY454XE  Re 049F2
; FUNCTION: GY455XE  Im 04B00
_line_print_small:
	PUSH LR
	MOV R1, R0
	MOV R0, #7H
	ST R0, _font_size
	BL _line_print_col_0
	POP PC

; FUNCTION: GY454XE  Re 04A02
; FUNCTION: GY455XE  Im 04B10
_diag_scr_fill_ws:
	PUSH LR
	MOV R1, #1H
	BL _fill_screen
_$j_04a0a:
	BL _render_waitshift
	POP PC

; FUNCTION: GY454XE  Re 04A10
; FUNCTION: GY455XE  Im 04B1E
_diag_scr_ckb1_ws:
	MOV R2, #01010101B
_$j_04a12:
	PUSH LR
	PUSH R2
	MOV R1, #BYTE2 _screen_buffer
	MOV R0, #BYTE1 _screen_buffer
	PUSH XR0
	BL _f_04A34
	POP XR0
	MOV ER2, #0CH
	ADD ER0, ER2
	POP R2
	XOR R2, #-1H
	BL _f_04A34
	BAL _$j_04a0a

; FUNCTION: GY454XE  Re 04A30
; FUNCTION: GY455XE  Im 04B3E
_diag_scr_ckb2_ws:
	MOV R2, #10101010B
	BAL _$j_04a12

; FUNCTION: GY454XE  Re 04A34
; FUNCTION: GY455XE  Im 04B42
_f_04A34:
	PUSH LR
	PUSH QR8
	MOV ER8, ER0
	MOV R10, R2
	MOV R11, #0H
	MOV BP, #18H
	MOV FP, #1FH
_$j_04a42:
	MOV ER0, #0CH
	PUSH ER0
	MOV ER2, ER10
	MOV ER0, ER8
	BL _memset_n
	POP ER0
	ADD ER8, BP
	ADD R14, #-2H
	BPS _$j_04a42
	BAL _$j_04a9a

; FUNCTION: GY454XE  Re 04A58
; FUNCTION: GY455XE  Im 04B66
_diag_scr_box_ws:
	PUSH LR
	PUSH QR8
	MOV ER8, #0CH
	PUSH ER8
	MOV R2, #-1H
	MOV R1, #BYTE2 (_screen_buffer+12)
	MOV R0, #BYTE1 (_screen_buffer+12)
	BL _memset_n
	POP ER0
	PUSH ER8
	MOV R2, #-1H
	MOV R1, #BYTE2 (_screen_buffer+12*31)
	MOV R0, #BYTE1 (_screen_buffer+12*31)
	BL _memset_n
	POP ER0
	MOV R1, #BYTE2 (_screen_buffer+12)
	MOV R0, #BYTE1 (_screen_buffer+12)
	MOV R2, #1FH
_$j_04a80:
	L R8, [ER0]
	OR R8, #10000000B
	ST R8, [ER0]
	L R8, 0BH[ER0]
	OR R8, #00000001B
	ST R8, 0BH[ER0]
	ADD ER0, #0CH
	ADD R2, #-1H
	BNE _$j_04a80
	BL _render_waitshift
_$j_04a9a:
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 04A9E
; FUNCTION: GY455XE  Im 04BAC
_diag_checksum:
	PUSH LR
	PUSH QR0
	PUSH QR8
	MOV FP, SP
	ADD SP, #-10H
	BL _buffer_clear
	MOV R0, #7
	ST R0, _font_size
	LEA OFFSET _version
	L XR0, SEG _version:[EA+]
	L ER4, SEG _version:[EA+]
	MOV R6, #' '
	MOV R7, #'V'
	MOV R8, #'e'
	MOV R9, #'r'
	L ER10, SEG _version:[EA+]
	MOV BP, #0H
	BL _store_regs_to_stack
	MOV R1, #1H
	MOV ER2, SP
	BL _line_print_col_0
	BL _render
	BL _diag_get_checksum
	LEA OFFSET _checksum
	L ER2, SEG _checksum:[EA+]
	CMP ER0, ER2
	BNE _$j_04af4
	MOV R8, #' '
	MOV R9, #'O'
	MOV R10, #'K'
	BAL _$j_04afa
_$j_04af4:
	MOV R8, #' '
	MOV R9, #'N'
	MOV R10, #'G'
_$j_04afa:
	MOV R11, #0
	MOV R0, #'S'
	MOV R1, #'U'
	MOV R2, #'M'
	MOV R3, #' '
	BL _store_regs_to_stack
	MOV R1, #8H
	MOV ER2, SP
	BL _line_print_col_0
	BL _pd_value
	MOV R2, #'-'
	MOV R0, R0
	BEQ _$j_04b1e
	OR R0, #30H
	MOV R2, R0
_$j_04b1e:
	MOV R0, #'P'
	MOV R1, #'d'
	MOV R3, #0H
	BL _store_regs_to_stack
	MOV R1, #15
	MOV ER2, SP
	BL _line_print_col_0
	BL _render
	BL _diag_pd_check
	CMP R0, #0A5H
	BNE _$j_04b44
	MOV R4, #' '
	MOV R5, #'O'
	MOV R6, #'K'
	BAL _$j_04b4a
_$j_04b44:
	MOV R4, #' '
	MOV R5, #'N'
	MOV R6, #'G'
_$j_04b4a:
	MOV R7, #0H
	MOV R0, #'R'
	MOV R1, #'e'
	MOV R2, #'a'
	MOV R3, #'d'
	BL _store_regs_to_stack
	MOV R0, #24
	MOV R1, #15
	MOV ER2, SP
	BL _line_print
	MOV R0, #22
	MOV R2, #BYTE1 _s_dpressac
	MOV R3, #BYTE2 _s_dpressac
	BL _line_print_small
	LEA P0MOD
	MOV ER0, #1H
	ST R0, [EA+]
	ST R0, [EA+]
	ST R0, [EA+]
	ST R1, [EA]
_$j_04b7a:
	BL _render
	BL _check_ac
	BEQ _$j_04b7a
	BL _clr_port0
	MOV SP, FP
	POP QR8
	POP QR0
	POP PC

; FUNCTION: GY454XE  Re 04B90
; FUNCTION: GY455XE  Im 04C9E
_store_regs_to_stack:
	PUSH BP
	MOV BP, FP
	ADD BP, #-10H
	LEA [BP]
	POP BP
	ST QR0, [EA+]
	ST QR8, [EA+]
	RT

; FUNCTION: GY454XE  Re 04BA0
; FUNCTION: GY455XE  Im 04CAE
_diag_get_checksum:
	PUSH LR
	PUSH QR8
	MOV ER8, #0H
	MOV FP, #0H
	MOV ER10, #0H
	LEA [ER10]
_$j_04bac:
	L R10, 8:[EA+]
	NOP
	SUB R8, R10
	SUBC R9, R11
	ADD FP, #-1H
	BNE _$j_04bac
	MOV R14, #-4H
	MOV R15, #-1H
	MOV ER10, #0H
	LEA [ER10]
_$j_04bc2:
	L R10, 1:[EA+]
	NOP
	SUB R8, R10
	SUBC R9, R11
	ADD FP, #-1H
	BNE _$j_04bc2
	MOV R0, R9
	BL _hex_to_char
	MOV ER4, ER0
	MOV R0, R8
	BL _hex_to_char
	MOV ER6, ER0
	MOV ER0, ER8
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 04BE6
; FUNCTION: GY455XE  Im 04CF4
_diag_pd_check:
	PUSH LR
	PUSH QR8
	MOV R0, #0H
	MOV ER8, #0H
	MOV FP, #0H
	MOV ER10, #0H
	LEA [ER10]
_$j_04bf4:
	L R10, 8:[EA+]
	ADD R8, #0H
	MOV PSW, #01000100B
	MOV R12, PSW
	AND R12, #11110100B
	CMP R12, #01000100B
	BNE _$j_04c30
	ADD FP, #-1H
	BNE _$j_04bf4
	MOV R9, #5AH
	MOV R14, #0FFH
	MOV R15, #0FCH
	MOV ER10, #0H
	LEA [ER10]
_$j_04c12:
	L R10, 1:[EA+]
	ADD R8, #0H
	MOV PSW, #01000100B
	MOV R12, PSW
	AND R12, #11110100B
	CMP R12, #01000100B
	BNE _$j_04c30
	ADD FP, #-1H
	BNE _$j_04c12
	CMP R9, #5AH
	BNE _$j_04c30
	MOV R0, #0A5H
_$j_04c2c:
	POP QR8
	POP PC
_$j_04c30:
	MOV R0, #0H
	BAL _$j_04c2c

; FUNCTION: GY454XE  Re 04C34
; FUNCTION: GY455XE  Im 04D42
_hex_to_char:
	MOV R1, R0
	AND R1, #0FH
	CMP R1, #0AH
	BGE _$j_04c40
	ADD R1, #30H
	BAL _$j_04c42
_$j_04c40:
	ADD R1, #37H
_$j_04c42:
	SRL R0, #4
	CMP R0, #0AH
	BGE _$j_04c4c
	ADD R0, #30H
	BAL _$j_04c4e
_$j_04c4c:
	ADD R0, #37H
_$j_04c4e:
	RT

; FUNCTION: GY454XE  Re 04C50
; FUNCTION: GY455XE  Im 04D5E
_diag_print_hex:
	PUSH LR
	PUSH QR8
	MOV FP, SP
	ADD SP, #-0AH
	MOV R9, R0
	MOV R8, R0
	SRL R8, #4
	OR R8, #30H
	OR R9, #30H
	CMP R9, #39H
	BC LES, _$j_04c68
	ADD R9, #7H
_$j_04c68:
	MOV R10, #'h'
	MOV R11, #0H
	MOV R0, #0H
	ST R0, _use_rambuf
	MOV R0, #8H
	MOV ER2, SP
	LEA [ER2]
	ST XR8, [EA]
	BL _line_print_small
	MOV R0, #1H
	ST R0, _use_rambuf
	MOV SP, FP
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 04C8A
; FUNCTION: GY455XE  Im 04D98
_find_bit:
	CMP R0, #3H
	BLT _$j_04c9c
	MOV R1, #9H
_$j_04c90:
	ADD R1, #-1H
	ADD R0, R0
	BGE _$j_04c90
	BEQ _$j_04c9a
	MOV R1, #10H
_$j_04c9a:
	MOV R0, R1
_$j_04c9c:
	RT

; FUNCTION: GY454XE  Re 04C9E
; FUNCTION: GY455XE  Im 04DAC
_render_waitshift:
	PUSH LR
	BL _render
	BL _wait_shift
	POP PC

; FUNCTION: GY454XE  Re 04CAA
; FUNCTION: GY455XE  Im 04DB8
_diag_factory_test:
	PUSH LR
	MOV R1, #3H
_$j_04cae:
	LEA KID
	L R0, [EA]
	BEQ _$j_04cda
	AND R0, #00011000B
	BNE _$j_04cda
	ADD R1, #-1H
	BNE _$j_04cae
	LEA _magic_string
	MOV ER0, #0H
	ST ER0, [EA+]
	ST ER0, [EA+]
	MOV R0, #1H
	ST R0, _cursor_noflash
	BL _diag_checksum
	BL _reset_all
	BL _f_03558
_$j_04cda:
	POP PC
	RT

PUBLIC _diag_init_check
PUBLIC _diagnostic_mode
PUBLIC _diag_print_hex
PUBLIC _diag_factory_test
