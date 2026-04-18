TYPE(ML610CASESplus)
MODEL LARGE
ROMWINDOW 0, 7FFFH

; NOTE:
; generals may be partially written in C (at least 1 C source and 1 ASM source), however full assembly will be used for now.
; generals may be made up of multiple ASM files. For now, these are combined into a single ASM file.

_stop_type			EQU 8E00H

_pd_val_emu			EQU 0F050H
_real_screen		EQU 0F800H
_real_screen_10		EQU 0F80AH
_real_screen_11		EQU 0F80BH
_real_screen_end	EQU _real_screen+16*32-1

$$NCODgenerals SEGMENT CODE 2H ANY
$$NTABgenerals0 SEGMENT TABLE 2H #0
$$NTABgenerals1 SEGMENT TABLE 2H #0
$$NTABgenerals2 SEGMENT TABLE 2H #0
$$NTABgenerals3 SEGMENT TABLE 2H #0

RSEG $$NTABgenerals0

; DATA: GY454XE  Re 00778
; DATA: GY455XE  Im 00778
; DATA: GY460XF  Im 007EA
_matrix_dims:
	DB 3, 3
	DB 3, 2
	DB 3, 1
	DB 2, 3
	DB 2, 2
	DB 2, 1
	DB 1, 3
	DB 1, 2
	DB 1, 1

; DATA: GY454XE  Re 0078A
; DATA: GY455XE  Im 0078A
; DATA: GY460XF  Im 007FC
_unk_0078a:
	DW _mode_ram+0
	DW _mode_ram+10
	DW _mode_ram+20
	DW _mode_ram+30
	DW _mode_ram+40
	DW _mode_ram+50
	DW _mode_ram+60
	DW _mode_ram+70
	DW _mode_ram+80
	DW _mode_ram+90
	DW _mode_ram+100
	DW _mode_ram+110
	DW _mode_ram+120
	DW _mode_ram+130
	DW _mode_ram+140
	DW _mode_ram+150
	DW _mode_ram+160
	DW _mode_ram+170
	DW _mode_ram+180
	DW _mode_ram+190
	DW _mode_ram+200
	DW _mode_ram+210
	DW _mode_ram+220
	DW _mode_ram+230
	DW _mode_ram+240
	DW _mode_ram+250
	DW _mode_ram+260
	DW _mode_ram+270
	DW _mode_ram+280
	DW _mode_ram+290
	DW _mode_ram+300
	DW _mode_ram+310
	DW _mode_ram+320
	DW _mode_ram+330
	DW _mode_ram+340
	DW _mode_ram+350

; DATA: GY454XE  Re 007D2
; DATA: GY455XE  Im 007D2
; DATA: GY460XF  Im 00844
_vector_dims:
	DB 1, 3
	DB 1, 2

; DATA: GY454XE  Re 007D6
; DATA: GY455XE  Im 007D6
; DATA: GY460XF  Im 00848
_matvct_strings:
	DB "\xe0\x00\x00", 0	; "A"
	DB "\xe1\x00\x00", 0	; "B"
	DB "\xe2\x00\x00", 0	; "C"
	DB "\xe0\xf4\xf5", 0	; "Ans"

; DATA: GY454XE  Re 007E6
; DATA: GY455XE  Im 007E6
; DATA: GY460XF  Im 00858
_unk_007e6:
	DW _mode_ram+0
	DW _mode_ram+10
	DW _mode_ram+20
	DW _mode_ram+30
	DW _mode_ram+40
	DW _mode_ram+50
	DW _mode_ram+60
	DW _mode_ram+70
	DW _mode_ram+80
	DW _mode_ram+10
	DW _mode_ram+20
	DW _mode_ram+90
	DW _mode_ram+40
	DW _mode_ram+50
	DW _mode_ram+100
	DW _mode_ram+70
	DW _mode_ram+80
	DW _mode_ram+110

; DATA: GY454XE  Re 0080A
; DATA: GY455XE  Im 0080A
; DATA: GY460XF  Im 0087C
_unk_0080a:
	DB 2, 3
	DB 3, 3
	DB 1, 3
	DB 1, 3

; DATA: GY454XE  Re 00812
; DATA: GY455XE  Im 00812
; DATA: GY460XF  Im 00884
_unk_00812:
	DB 2, 3
	DB 3, 4
	DB 1, 3
	DB 1, 4

; DATA: GY454XE  Re 0081A
; DATA: GY455XE  Im 0081A
; DATA: GY460XF  Im 0088C
_table_param_ptrs:
	DW _mode_ram+0		; Start
	DW _mode_ram+10		; End
	DW _mode_ram+20		; Step

; DATA: GY454XE  Re 00820
; DATA: GY455XE  Im 00820
; DATA: GY460XF  Im 00892
_table_eqn_linel_params:
	DB  10, 7, 18, 11000000B
	DB  10, 7, 24, 11000000B
	DB  10, 7, 12, 11000000B
	DB  10, 7, 12, 11000000B

; DATA: GY454XE  Re 00830
; DATA: GY455XE  Im 00830
; DATA: GY460XF  Im 008A2
_table_eqn_liner_params:
	DB  94, 7, 18, 00110000B
	DB  94, 7, 24, 00110000B
	DB  94, 7, 12, 00110000B
	DB  94, 7, 12, 00110000B

; DATA: GY454XE  Re 00840
; DATA: GY455XE  Im 00840
; DATA: GY460XF  Im 008B2
_base_n_submodes:
	DB 1
	DB 7
	DB 9
	DB 15

; DATA: GY454XE  Re 00844
; DATA: GY455XE  Im 00844
; DATA: GY460XF  Im 008B6
_vars_list:
	DB 'M'
	DB 'A'
	DB 'A'
	DB 'B'
	DB 'C'
	DB 'D'
	DB 'E'
	DB 'F'
	DB 'X'
	DB 'Y'

; Character blacklists

; DATA: GY454XE  Re 0084E
; DATA: GY455XE  Im 0084E
; DATA: GY460XF  Im 008C0
_blacklist_tables:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 6CH	; Pol(
	DB 6DH	; Rec(
	DB 3AH	; Multi-statement colon
	DB 5FH	; Remainder (÷R)
	DB 0A4H	; Recurring decimal
	DB 0

; DATA: GY454XE  Re 00856
; DATA: GY455XE  Im 00856
; DATA: GY460XF  Im 008C8
_blacklist_stat_mat_vct:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 3AH	; Multi-statement colon
	DB 0A4H	; Recurring decimal
	DB 0

; DATA: GY454XE  Re 0085B
; DATA: GY455XE  Im 0085B
; DATA: GY460XF  Im 008CD
_blacklist_table:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 6CH	; Pol(
	DB 6DH	; Rec(
	DB 69H	; Summation function (∑)
	DB 3AH	; Multi-statement colon
	DB 0A4H	; Recurring decimal
	DB 0A5H	; >Conv n
	DB 0

; DATA: GY454XE  Re 00864
; DATA: GY455XE  Im 00864
; DATA: GY460XF  Im 008D6
_blacklist_cmplx:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 6CH	; Pol(
	DB 6DH	; Rec(
	DB 69H	; Summation function (∑)
	DB 25H	; Percent sign
	DB 0A4H	; Recurring decimal
	DB 05FH	; Remainder (÷R)
	DB 0

; DATA: GY454XE  Re 0086D
; DATA: GY455XE  Im 0086D
; DATA: GY460XF  Im 008DF
; DATA: GY465XG  Im 00864
_blacklist_verif:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 69H	; Summation function (∑)
	DB 6CH	; Pol(
	DB 6DH	; Rec(
	DB 3AH	; Multi-statement colon
	DB 05FH	; Remainder (÷R)
	DB 0

; DATA: GY454XE  Re 00875
; DATA: GY455XE  Im 00875
; DATA: GY460XF  Im 008E7
_s_err_acbreak:
	DB "AC Break", 0

; DATA: GY454XE  Re 0087E
; DATA: GY455XE  Im 0087E
; DATA: GY460XF  Im 008F0
_s_err_syntax:
	DB "Syntax ERROR", 0

; DATA: GY454XE  Re 0088B
; DATA: GY455XE  Im 0088B
; DATA: GY460XF  Im 008FD
_s_err_math:
	DB "Math ERROR", 0

; DATA: GY454XE  Re 00896
; DATA: GY455XE  Im 00896
; DATA: GY460XF  Im 00908
_s_err_outofmem:
	DB "Insufficient MEM", 0

; DATA: GY454XE  Re 008A7
; DATA: GY455XE  Im 008A7
; DATA: GY460XF  Im 00919
_s_err_stack:
	DB "Stack ERROR", 0

; DATA: GY454XE  Re 008B3
; DATA: GY455XE  Im 008B3
; DATA: GY460XF  Im 00925
_s_err_argument:
	DB "Argument ERROR", 0

; DATA: GY454XE  Re 008C2
; DATA: GY455XE  Im 008C2
; DATA: GY460XF  Im 00934
_s_err_dimension:
	DB "Dimension ERROR", 0

; DATA: GY454XE  Re 008D2
; DATA: GY455XE  Im 008D2
; DATA: GY460XF  Im 00944
_s_err_solve:
	DB "Can't Solve", 0

; DATA: GY454XE  Re 008DE
; DATA: GY455XE  Im 008DE
; DATA: GY460XF  Im 00950
_s_err_timeout:
	DB "Time Out", 0

; DATA: GY454XE  Re 008E7
; DATA: GY455XE  Im 008E7
; DATA: GY460XF  Im 00959
_s_err_variable:
	DB "Variable ERROR", 0

; DATA: GY454XE  Re 008F6
; DATA: GY455XE  Im 008F6
; DATA: GY460XF  Im 00968
_s_err_null:
	DB "NULL ERROR", 0
	DB 0

; DATA: GY454XE  Re 00902
; DATA: GY455XE  Im 00902
; DATA: GY460XF  Im 0097E
_errors:
	DW _s_err_acbreak
	DW _s_err_syntax
	DW _s_err_math
	DW _s_err_outofmem
	DW _s_blank_line
	DW _s_blank_line
	DW _s_err_stack
	DW _s_err_argument
	DW _s_err_dimension
	DW _s_err_solve
	DW _s_err_timeout
	DW _s_err_variable
	DW _s_err_null

; DATA: GY454XE  Re 0091C
; DATA: GY455XE  Im 0091C
; DATA: GY460XF  Im 0098E
_s_prompt_goto:
	DB "[\x9f][\x9e]:Goto", 0

; DATA: GY454XE  Re 00928
; DATA: GY455XE  Im 00928
; DATA: GY460XF  Im 0099A
_s_table_x:
	;  "  X"
	DB "\xec\xec\xea", 0

; DATA: GY454XE  Re 0092C
; DATA: GY455XE  Im 0092C
; DATA: GY460XF  Im 0099E
_s_table_y:
	;  "  Y"
	DB "\xec\xec\xeb", 0
	
; DATA: GY454XE  Re 00930
; DATA: GY455XE  Im 00930
; DATA: GY460XF  Im 009A2
_s_table_freq:
	;  " FREQ"
	DB "\xec\xe5\xe9\xe4\xe8", 0
	

; Unused
; DATA: GY454XE  Re 00936
; DATA: GY455XE  Im 00936
; DATA: GY460XF  Im 009A8
_s_table_ans:
	;  " Ans"
	DB "\xec\xe0\xf4\xf5", 0

; DATA: GY454XE  Re 0093B
; DATA: GY455XE  Im 0093B
; DATA: GY460XF  Im 009AD
_s_table_fx:
	;  " F(X)"
	DB "\xec\xe5\xf6\xea\xf7", 0

; DATA: GY454XE  Re 00941
; DATA: GY455XE  Im 00941
; DATA: GY460XF  Im 009B3
_s_table_a:
	;  "  a"
	DB "\xec\xec\xf0", 0

; DATA: GY454XE  Re 00945
; DATA: GY455XE  Im 00945
; DATA: GY460XF  Im 009B7
_s_table_1:
	;  "1"
	DB "\xd1", 0

; DATA: GY454XE  Re 00947
; DATA: GY455XE  Im 00947
; DATA: GY460XF  Im 009B9
_s_blank_line:
	DB "                ", 0

; DATA: GY454XE  Re 00958
; DATA: GY455XE  Im 00958
; DATA: GY460XF  Im 009CA
_s_prompt_fix:
	DB "Fix 0~9?", 0
	
; DATA: GY454XE  Re 00961
; DATA: GY455XE  Im 00961
; DATA: GY460XF  Im 009D3
_s_prompt_sci:
	DB "Sci 0~9?", 0

; DATA: GY454XE  Re 0096A
; DATA: GY455XE  Im 0096A
; DATA: GY460XF  Im 009DC
_s_prompt_norm:
	DB "Norm 1~2?", 0

; DATA: GY454XE  Re 00974
; DATA: GY455XE  Im 00974
; DATA: GY460XF  Im 009E6
_contrast_screen:
	DB "CONTRAST", 0
	DB "", 0
	DB "LIGHT       DARK", 0
	DB " [\x9f]        [\x9e]", 0

; DATA: GY454XE  Re 0099F
; DATA: GY455XE  Im 0099F
; DATA: GY460XF  Im 00A11
_const_input_template:
	DB "[__]", 0
	
; DATA: GY454XE  Re 009A4
; DATA: GY455XE  Im 009A4
; DATA: GY460XF  Im 00A16
_const_screen:
	DB "CONSTANT", 0
	DB "Number 01~40?", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 009BE
; DATA: GY455XE  Im 009BE
; DATA: GY460XF  Im 00A30
_conv_screen:
	DB "CONVERSION", 0
	DB "Number 01~40?", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 009DA
; DATA: GY455XE  Im 009DA
; DATA: GY460XF  Im 00A4C
_menu_clr:
	DB "Clear?", 0
	DB "1:Setup 2:Memory", 0
	DB "3:All", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 009FA
; DATA: GY455XE  Im 009FA
; DATA: GY460XF  Im 00A6C
_s_clr_setup:
	DB "Clear Setup?", 0

; DATA: GY454XE  Re 00A07
; DATA: GY455XE  Im 00A07
; DATA: GY460XF  Im 00A79
_s_clr_mem:
	DB "Clear Memory?", 0

; DATA: GY454XE  Re 00A15
; DATA: GY455XE  Im 00A15
; DATA: GY460XF  Im 00A87
_s_reset_all:
	DB "Reset All?", 0

; DATA: GY454XE  Re 00A20
; DATA: GY455XE  Im 00A20
; DATA: GY460XF  Im 00A92
_s_clr_done_all:
	DB "   Reset All", 0

; DATA: GY454XE  Re 00A2D
; DATA: GY455XE  Im 00A2D
; DATA: GY460XF  Im 00A9F
_s_prompt_yes:
	DB "[=]   :Yes", 0

; DATA: GY454XE  Re 00A38
; DATA: GY455XE  Im 00A38
; DATA: GY460XF  Im 00AAA
_s_prompt_cancel:
	DB "[AC]  :Cancel", 0

; DATA: GY454XE  Re 00A46
; DATA: GY455XE  Im 00A46
; DATA: GY460XF  Im 00AB8
_s_clr_done:
	DB "   Complete!", 0
	
; DATA: GY454XE  Re 00A53
; DATA: GY455XE  Im 00A53
; DATA: GY460XF  Im 00AC5
_s_clr_prompt_ac:
	DB " Press [AC] key", 0
	
; DATA: GY454XE  Re 00A63
; DATA: GY455XE  Im 00A63
; DATA: GY460XF  Im 00AD5
_menu_matrix_table:
	DB "1:Dim   2:Data", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00A76
; DATA: GY455XE  Im 00A76
; DATA: GY460XF  Im 00AE8
_menu_matrix:
	DB "1:Dim   2:Data", 0
	DB "3:MatA  4:MatB", 0
	DB "5:MatC  6:MatAns", 0
	DB "7:det   8:Trn", 0
	DB 0
	
; DATA: GY454XE  Re 00AB4
; DATA: GY455XE  Im 00AB4
; DATA: GY460XF  Im 00B26
_menu_matrix_data:
	DB "1:MatA  2:MatB", 0
	DB "3:MatC", 0
	DB 0
	DB 0

; DATA: GY454XE  Re 00ACC
; DATA: GY455XE  Im 00ACC
; DATA: GY460XF  Im 00B3E
_menu_matrix_dim0:
	DB "1:3$3   2:3$2", 0
	DB "3:3$1   4:2$3", 0
	DB "5:2$2   6:2$1", 0

; DATA: GY454XE  Re 00AF6
; DATA: GY455XE  Im 00AF6
; DATA: GY460XF  Im 00B68
_menu_matrix_dim1:
	DB "1:1$3   2:1$2", 0
	DB "3:1$1", 0
	DB "", 0

; DATA: GY454XE  Re 00B0C
; DATA: GY455XE  Im 00B0C
; DATA: GY460XF  Im 00B7E
_menu_matrix_sel:
	DB "Matrix?", 0

; DATA: GY454XE  Re 00B14
; DATA: GY455XE  Im 00B14
; DATA: GY460XF  Im 00B86
_menu_mata_dim_sel:
	DB "MatA(m$n) m$n?", 0

; DATA: GY454XE  Re 00B23
; DATA: GY455XE  Im 00B23
; DATA: GY460XF  Im 00B95
_menu_matb_dim_sel:
	DB "MatB(m$n) m$n?", 0

; DATA: GY454XE  Re 00B32
; DATA: GY455XE  Im 00B32
; DATA: GY460XF  Im 00BA4
_menu_matc_dim_sel:
	DB "MatC(m$n) m$n?", 0

; DATA: GY454XE  Re 00B41
; DATA: GY455XE  Im 00B41
; DATA: GY460XF  Im 00BB3
_menu_vector_table:
	DB "1:Dim   2:Data", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00B54
; DATA: GY455XE  Im 00B54
; DATA: GY460XF  Im 00BC6
_menu_vector:
	DB "1:Dim   2:Data", 0
	DB "3:VctA  4:VctB", 0
	DB "5:VctC  6:VctAns", 0
	DB "7:Dot", 0
	DB 0
	
; DATA: GY454XE  Re 00B8A
; DATA: GY455XE  Im 00B8A
; DATA: GY460XF  Im 00BFC
_menu_vector_data:
	DB "1:VctA  2:VctB", 0
	DB "3:VctC", 0
	DB 0
	DB 0

; DATA: GY454XE  Re 00BA2
; DATA: GY455XE  Im 00BA2
; DATA: GY460XF  Im 00C14
_menu_vector_dim:
	DB "1:3     2:2", 0
	DB "", 0
	DB "", 0

; DATA: GY454XE  Re 00BB1
; DATA: GY455XE  Im 00BB1
; DATA: GY460XF  Im 00C23
_menu_vector_sel:
	DB "Vector?", 0

; DATA: GY454XE  Re 00BB9
; DATA: GY455XE  Im 00BB9
; DATA: GY460XF  Im 00C2B
_menu_vcta_dim_sel:
	DB "VctA(m) m?", 0

; DATA: GY454XE  Re 00BC4
; DATA: GY455XE  Im 00BC4
; DATA: GY460XF  Im 00C36
_menu_vctb_dim_sel:
	DB "VctB(m) m?", 0

; DATA: GY454XE  Re 00BCF
; DATA: GY455XE  Im 00BCF
; DATA: GY460XF  Im 00C41
_menu_vctc_dim_sel:
	DB "VctC(m) m?", 0

; DATA: GY454XE  Re 00BDA
; DATA: GY455XE  Im 00BDA
; DATA: GY460XF  Im 00C4C
_menu_cmplx:
	DB "1:arg   2:Conjg", 0
	DB "3:\x9er\x88\xc4  4:\x9ea+b\x80", 0
	DB "", 0
	DB "", 0

; DATA: GY454XE  Re 00BFC
; DATA: GY455XE  Im 00BFC
; DATA: GY460XF  Im 00C6E
_menu_base_n_0:
	DB "1:and   2:or", 0
	DB "3:xor   4:xnor", 0
	DB "5:Not   6:Neg", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C28
; DATA: GY455XE  Im 00C28
; DATA: GY460XF  Im 00C9A
_menu_base_n_1:
	DB "1:d     2:h", 0
	DB "3:b     4:o", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C43
; DATA: GY455XE  Im 00C43
; DATA: GY460XF  Im 00CB5
_menu_drg:
	DB "1:\x85     2:\x86", 0
	DB "3:\x87", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C56
; DATA: GY455XE  Im 00C56
; DATA: GY460XF  Im 00CC8
_menu_hyp:
	DB "1:sinh  2:cosh", 0
	DB "3:tanh  4:sinh\xaa", 0
	DB "5:cosh\xaa 6:tanh\xaa", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C87
; DATA: GY455XE  Im 00C87
; DATA: GY460XF  Im 00CF9
_menu_stat_data_edit:
	DB "1:Ins   2:Del-A", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00C9B
; DATA: GY455XE  Im 00C9B
; DATA: GY460XF  Im 00D0D
_menu_stat_1var_sum:
	DB "1:\xc0x\xa2   2:\xc0x", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00CAC
; DATA: GY455XE  Im 00CAC
; DATA: GY460XF  Im 00D1E
_menu_stat_2var_sum:
	DB "1:\xc0x\xa2   2:\xc0x", 0
	DB "3:\xc0y\xa2   4:\xc0y", 0
	DB "5:\xc0xy   6:\xc0x\xa3", 0
	DB "7:\xc0x\xa2y  8:\xc0x\xa4", 0

; DATA: GY454XE  Re 00CE2
; DATA: GY455XE  Im 00CE2
; DATA: GY460XF  Im 00D54
_menu_stat_1var_var:
	DB "1:n     2:\x89", 0
	DB "3:\xc8x    4:sx", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00CFE
; DATA: GY455XE  Im 00CFE
; DATA: GY460XF  Im 00D70
_menu_stat_2var_var:
	DB "1:n     2:\x89", 0
	DB "3:\xc8x    4:sx", 0
	DB "5:\x8a     6:\xc8y", 0
	DB "7:sy", 0
	
; DATA: GY454XE  Re 00D29
; DATA: GY455XE  Im 00D29
; DATA: GY460XF  Im 00D9B
_menu_stat_1var_minmax:
	DB "1:minX  2:maxX", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D3C
; DATA: GY455XE  Im 00D3C
; DATA: GY460XF  Im 00DAE
_menu_stat_2var_minmax:
	DB "1:minX  2:maxX", 0
	DB "3:minY  4:maxY", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D5D
; DATA: GY455XE  Im 00D5D
; DATA: GY460XF  Im 00DCF
_menu_stat_distr:
	DB "1:P(    2:Q(", 0
	DB "3:R(    4:\x9et", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D7A
; DATA: GY455XE  Im 00D7A
; DATA: GY460XF  Im 00DEC
_menu_stat_reg:
	DB "1:\x9a     2:\x9b", 0
	DB "3:r     4:\x8b", 0
	DB "5:\x8c", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D98
; DATA: GY455XE  Im 00D98
; DATA: GY460XF  Im 00E0A
_menu_stat_reg_quad:
	DB "1:\x9a     2:\x9b", 0
	DB "3:\x9c     4:\x8b\xb1", 0
	DB "5:\x8b\xb2    6:\x8c", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DBF
; DATA: GY455XE  Im 00DBF
; DATA: GY460XF  Im 00E31
_menu_ratio:
	DB "1:a:b=X:d", 0
	DB "2:a:b=c:X", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DD6
; DATA: GY455XE  Im 00DD6
; DATA: GY460XF  Im 00E48
_menu_verify:
	DB "1:=     2:\x95", 0
	DB "3:>     4:<", 0
	DB "5:\x96     6:\x94", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DFC
; DATA: GY455XE  Im 00DFC
; DATA: GY460XF  Im 00E6E
_menu_ineq:
	DB "1:aX\xa2+bX+c", 0
	DB "2:aX\xa3+bX\xa2+cX+d", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00E19
; DATA: GY455XE  Im 00E19
; DATA: GY460XF  Im 00E8B
_menu_ineq_poly2:
	DB "1:aX\xa2+bX+c>0", 0
	DB "2:aX\xa2+bX+c<0", 0
	DB "3:aX\xa2+bX+c\x960", 0
	DB "4:aX\xa2+bX+c\x940", 0
	DB 0

; DATA: GY454XE  Re 00E4E
; DATA: GY455XE  Im 00E4E
; DATA: GY460XF  Im 00EC0
_menu_ineq_poly3:
	DB "1:aX\xa3+bX\xa2+cX+d>0", 0
	DB "2:aX\xa3+bX\xa2+cX+d<0", 0
	DB "3:aX\xa3+bX\xa2+cX+d\x960", 0
	DB "4:aX\xa3+bX\xa2+cX+d\x940", 0
	DB 0

; DATA: GY454XE  Re 00E93
; DATA: GY455XE  Im 00E93
; DATA: GY460XF  Im 00F05
_menu_setup_stat_freq:
	DB "Frequency?", 0
	DB "1:ON    2:OFF", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00EAF
; DATA: GY455XE  Im 00EAF
; DATA: GY460XF  Im 00F21
_menu_setup_rdec:
	DB "Recur Decimal?", 0
	DB "1:ON    2:OFF", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00ECF
; DATA: GY455XE  Im 00ECF
; DATA: GY460XF  Im 00F41
_menu_setup_cmplx_result:
	DB "Complex Result?", 0
	DB "1:a+b\x80  2:r\x88\xc4", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00EF0
; DATA: GY455XE  Im 00EF0
; DATA: GY460XF  Im 00F62
_menu_setup_decimal_mark:
	DB "Decimal Point?", 0
	DB "1:Dot   2:Comma", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00F12
; DATA: GY455XE  Im 00F12
; DATA: GY460XF  Im 00F84
_menu_setup_decimalo:
	DB "Result Format?", 0
	DB "1:MathO 2:LineO", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00F34
; DATA: GY455XE  Im 00F34
; DATA: GY460XF  Im 00FA6
_s_prompt_solve:
	DB "Solve for ", 0

; DATA: GY454XE  Re 00F3F
; DATA: GY455XE  Im 00F3F
; DATA: GY460XF  Im 00FB1
_s_table_prompt_start:
	DB "Start?", 0

; DATA: GY454XE  Re 00F45
; DATA: GY455XE  Im 00F45
; DATA: GY460XF  Im 00FB8
_s_table_prompt_end:
	DB "End?  ", 0

; DATA: GY454XE  Re 00F4D
; DATA: GY455XE  Im 00F4D
; DATA: GY460XF  Im 00FBF
_s_table_prompt_step:
	DB "Step? ", 0

; DATA: GY454XE  Re 00F54
; DATA: GY455XE  Im 00F54
; DATA: GY460XF  Im 00FC6
_s_ratio_optn_c:
	DB "a:b=X:d", 0

; DATA: GY454XE  Re 00F5C
; DATA: GY455XE  Im 00F5C
; DATA: GY460XF  Im 00FCE
_s_ratio_optn_d:
	DB "a:b=c:X", 0

; DATA: GY454XE  Re 00F64
; DATA: GY455XE  Im 00F64
; DATA: GY460XF  Im 00FD6
_s_verif_result_false:
	DB "FALSE", 0

; DATA: GY454XE  Re 00F6A
; DATA: GY455XE  Im 00F6A
; DATA: GY460XF  Im 00FDC
_s_verif_result_true:
	DB "TRUE", 0

; DATA: GY454XE  Re 00F6F
; DATA: GY455XE  Im 00F6F
; DATA: GY460XF  Im 00FE1
_s_ineq_allreal:
	DB "All", 0

; DATA: GY454XE  Re 00F73
; DATA: GY455XE  Im 00F73
; DATA: GY460XF  Im 00FE5
_s_ineq_nosolution:
	DB "No-Solution", 0

RSEG $$NTABgenerals1

; DATA: GY454XE  Re 010F2
; DATA: GY455XE  Im 010F2
_tokens:
	DW _char_placeholder	; 00
	DW _char_cs01			; 01
	DW _char_cs02			; 02
	DW _char_cs03			; 03
	DW _char_cs04			; 04
	DW _char_cs05			; 05
	DW _char_cs06			; 06
	DW _char_cs07			; 07
	DW _char_cs08			; 08
	DW _char_cs09			; 09
	DW _char_cs10			; 0A
	DW _char_cs11			; 0B
	DW _char_cs12			; 0C
	DW _char_cs13			; 0D
	DW _char_cs14			; 0E
	DW _char_cs15			; 0F
	DW _char_sum_x2			; 10
	DW _char_sum_x			; 11
	DW _char_n				; 12
	DW _char_sum_y2			; 13
	DW _char_sum_y			; 14
	DW _char_sum_xy			; 15
	DW _char_sum_x3			; 16
	DW _char_sum_x2y		; 17
	DW _char_sum_x4			; 18
	DW _char_min_x			; 19
	DW _char_max_x			; 1A
	DW _char_min_y			; 1B
	DW _char_max_y			; 1C
	DW _char_cs16			; 1D
	DW _char_cs17			; 1E
	DW _char_cs18			; 1F
	DW _char_0x20			; 20
	DW _char_box			; 21
	DW _char_cs19			; 22
	DW _char_cs20			; 23
	DW _char_cs21			; 24
	DW _char_percent		; 25
	DW _char_cs22			; 26
	DW _char_cs23			; 27
	DW _char_paren_l		; 28
	DW _char_paren_r		; 29
	DW _char_cs24			; 2A
	DW _char_plus			; 2B
	DW _char_sep			; 2C
	DW _char_minus			; 2D
	DW _char_decimal		; 2E
	DW _char_placeholder	; 2F
	DW _char_0				; 30
	DW _char_1				; 31
	DW _char_2				; 32
	DW _char_3				; 33
	DW _char_4				; 34
	DW _char_5				; 35
	DW _char_6				; 36
	DW _char_7				; 37
	DW _char_8				; 38
	DW _char_9				; 39
	DW _char_colon			; 3A
	DW _char_cs25			; 3B
	DW _char_lt				; 3C
	DW _char_equals			; 3D
	DW _char_gt				; 3E
	DW _char_placeholder	; 3F
	DW _char_cs26			; 40
	DW _char_var_a			; 41
	DW _char_var_b			; 42
	DW _char_var_c			; 43
	DW _char_var_d			; 44
	DW _char_var_e			; 45
	DW _char_var_f			; 46
	DW _char_sto_a			; 47
	DW _char_sto_b			; 48
	DW _char_sto_c			; 49
	DW _char_sto_d			; 4A
	DW _char_sto_m			; 4B
	DW _char_sto_x			; 4C
	DW _char_sto_y			; 4D
	DW _char_mul			; 4E
	DW _char_div			; 4F
	DW _char_base_hex		; 50
	DW _char_base_dec		; 51
	DW _char_base_oct		; 52
	DW _char_base_bin		; 53
	DW _char_var_m			; 54
	DW _char_cmplx_rec		; 55
	DW _char_cmplx_pol		; 56
	DW _char_factorial		; 57
	DW _char_var_x			; 58
	DW _char_var_y			; 59
	DW _char_placeholder	; 5A
	DW _char_0x5b			; 5B
	DW _char_dms			; 5C
	DW _char_0x5d			; 5D
	DW _char_pow			; 5E
	DW _char_remainder		; 5F
	DW _char_negative		; 60
	DW _char_not			; 61
	DW _char_neg			; 62
	DW _char_abs			; 63
	DW _char_xhat1			; 64
	DW _char_xhat			; 65
	DW _char_yhat			; 66
	DW _char_xhat2			; 67
	DW _char_log			; 68
	DW _char_sum			; 69
	DW _char_integral		; 6A
	DW _char_ddx			; 6B
	DW _char_pol			; 6C
	DW _char_rec			; 6D
	DW _char_and			; 6E
	DW _char_or				; 6F
	DW _char_sinh			; 70
	DW _char_cosh			; 71
	DW _char_tanh			; 72
	DW _char_epow			; 73
	DW _char_exp			; 74
	DW _char_pow_2			; 75
	DW _char_pow_3			; 76
	DW _char_pow_m1			; 77
	DW _char_cs27			; 78
	DW _char_cs28			; 79
	DW _char_cs29			; 7A
	DW _char_0x7b			; 7B
	DW _char_placeholder	; 7C
	DW _char_0x7d			; 7D
	DW _char_xor			; 7E
	DW _char_xnor			; 7F
	DW _char_cmplx_i		; 80
	DW _char_euler			; 81
	DW _char_pi				; 82
	DW _char_sto_e			; 83
	DW _char_sto_f			; 84
	DW _char_d				; 85
	DW _char_r				; 86
	DW _char_g				; 87
	DW _char_conjg			; 88
	DW _char_xbar			; 89
	DW _char_ybar			; 8A
	DW _char_ans			; 8B
	DW _char_ran			; 8C
	DW _char_placeholder	; 8D
	DW _char_placeholder	; 8E
	DW _char_placeholder	; 8F
	DW _char_asinh			; 90
	DW _char_acosh			; 91
	DW _char_atanh			; 92
	DW _char_10pow			; 93
	DW _char_le				; 94
	DW _char_ne				; 95
	DW _char_ge				; 96
	DW _char_placeholder	; 97
	DW _char_sqrt			; 98
	DW _char_m_plus			; 99
	DW _char_stat_a			; 9A
	DW _char_stat_b			; 9B
	DW _char_stat_c			; 9C
	DW _char_stat_reg_r		; 9D
	DW _char_dot			; 9E
	DW _char_nth_rt			; 9F
	DW _char_sin			; A0
	DW _char_cos			; A1
	DW _char_tan			; A2
	DW _char_ln				; A3
	DW _char_placeholder	; A4
	DW _char_conv			; A5
	DW _char_placeholder	; A6
	DW _char_placeholder	; A7
	DW _char_cbrt			; A8
	DW _char_m_minus		; A9
	DW _char_stat_ox		; AA
	DW _char_stat_sx		; AB
	DW _char_stat_oy		; AC
	DW _char_stat_sy		; AD
	DW _char_frac			; AE
	DW _char_angle			; AF
	DW _char_asin			; B0
	DW _char_acos			; B1
	DW _char_atan			; B2
	DW _char_rnd			; B3
	DW _char_cs30			; B4
	DW _char_cs31			; B5
	DW _char_cs32			; B6
	DW _char_cs33			; B7
	DW _char_hex_a			; B8
	DW _char_hex_b			; B9
	DW _char_hex_c			; BA
	DW _char_hex_d			; BB
	DW _char_hex_e			; BC
	DW _char_hex_f			; BD
	DW _char_permu			; BE
	DW _char_combi			; BF
	DW _char_det			; C0
	DW _char_trn			; C1
	DW _char_ranint			; C2
	DW _char_arg			; C3
	DW _char_cs34			; C4
	DW _char_cs35			; C5
	DW _char_cs36			; C6
	DW _char_cs37			; C7
	DW _char_mata			; C8
	DW _char_matb			; C9
	DW _char_matc			; CA
	DW _char_matans			; CB
	DW _char_vcta			; CC
	DW _char_vctb			; CD
	DW _char_vctc			; CE
	DW _char_vctans			; CF
	DW _char_stat_p			; D0
	DW _char_stat_q			; D1
	DW _char_stat_r			; D2
	DW _char_stat_tot		; D3
	DW _char_cs38			; D4
	DW _char_cs39			; D5
	DW _char_cs40			; D6
	DW _char_cv01			; D7
	DW _char_cv02			; D8
	DW _char_cv03			; D9
	DW _char_cv04			; DA
	DW _char_cv05			; DB
	DW _char_cv06			; DC
	DW _char_cv07			; DD
	DW _char_cv08			; DE
	DW _char_cv09			; DF
	DW _char_cv10			; E0
	DW _char_cv11			; E1
	DW _char_cv12			; E2
	DW _char_cv13			; E3
	DW _char_cv14			; E4
	DW _char_cv15			; E5
	DW _char_cv16			; E6
	DW _char_cv17			; E7
	DW _char_cv18			; E8
	DW _char_cv19			; E9
	DW _char_cv20			; EA
	DW _char_cv21			; EB
	DW _char_cv22			; EC
	DW _char_cv23			; ED
	DW _char_cv24			; EE
	DW _char_cv25			; FF
	DW _char_cv26			; F0
	DW _char_cv27			; F1
	DW _char_cv28			; F2
	DW _char_cv29			; F3
	DW _char_cv30			; F4
	DW _char_cv31			; F5
	DW _char_cv32			; F6
	DW _char_cv33			; F7
	DW _char_cv34			; F8
	DW _char_cv35			; F9
	DW _char_cv36			; FA
	DW _char_cv37			; FB
	DW _char_cv38			; FC
	DW _char_cv39			; FD
	DW _char_cv40			; FE
	DW _char_placeholder	; FF

; Length = value & 0xF.
; If (value & 0xF0) == 0xF then append a left parenthesis at the end of the token.
; Else add it to the character data pointer. (Casio might be lazy here)
; DATA: GY454XE  Re 012F2
; DATA: GY455XE  Im 012F2
_token_lengths:
	DB 01H	; 00
	DB 02H	; 01
	DB 02H	; 02
	DB 02H	; 03
	DB 02H	; 04
	DB 02H	; 05
	DB 01H	; 06
	DB 02H	; 07
	DB 02H	; 08
	DB 01H	; 09
	DB 01H	; 0A
	DB 02H	; 0B
	DB 02H	; 0C
	DB 02H	; 0D
	DB 03H	; 0E
	DB 03H	; 0F
	DB 03H	; 10
	DB 02H	; 11
	DB 01H	; 12
	DB 03H	; 13
	DB 02H	; 14
	DB 03H	; 15
	DB 03H	; 16
	DB 04H	; 17
	DB 03H	; 18
	DB 04H	; 19
	DB 04H	; 1A
	DB 04H	; 1B
	DB 04H	; 1C
	DB 02H	; 1D
	DB 01H	; 1E
	DB 02H	; 1F
	DB 01H	; 20
	DB 01H	; 21
	DB 02H	; 22
	DB 02H	; 23
	DB 02H	; 24
	DB 01H	; 25
	DB 01H	; 26
	DB 01H	; 27
	DB 01H	; 28
	DB 01H	; 29
	DB 02H	; 2A
	DB 01H	; 2B
	DB 01H	; 2C
	DB 01H	; 2D
	DB 01H	; 2E
	DB 01H	; 2F
	DB 01H	; 30
	DB 01H	; 31
	DB 01H	; 32
	DB 01H	; 33
	DB 01H	; 34
	DB 01H	; 35
	DB 01H	; 36
	DB 01H	; 37
	DB 01H	; 38
	DB 01H	; 39
	DB 01H	; 3A
	DB 01H	; 3B
	DB 01H	; 3C
	DB 01H	; 3D
	DB 01H	; 3E
	DB 01H	; 3F
	DB 02H	; 40
	DB 01H	; 41
	DB 11H	; 42
	DB 01H	; 43
	DB 11H	; 44
	DB 11H	; 45
	DB 11H	; 46
	DB 02H	; 47
	DB 02H	; 48
	DB 02H	; 49
	DB 02H	; 4A
	DB 02H	; 4B
	DB 02H	; 4C
	DB 02H	; 4D
	DB 01H	; 4E
	DB 01H	; 4F
	DB 01H	; 50
	DB 01H	; 51
	DB 01H	; 52
	DB 01H	; 53
	DB 01H	; 54
	DB 05H	; 55
	DB 04H	; 56
	DB 01H	; 57
	DB 11H	; 58
	DB 11H	; 59
	DB 01H	; 5A
	DB 01H	; 5B
	DB 01H	; 5C
	DB 01H	; 5D
	DB 0F1H	; 5E
	DB 02H	; 5F
	DB 01H	; 60
	DB 0F3H	; 61
	DB 0F3H	; 62
	DB 0F3H	; 63
	DB 02H	; 64
	DB 01H	; 65
	DB 01H	; 66
	DB 02H	; 67
	DB 0F3H	; 68
	DB 0F1H	; 69
	DB 0F1H	; 6A
	DB 0F4H	; 6B
	DB 0F3H	; 6C
	DB 0F3H	; 6D
	DB 03H	; 6E
	DB 02H	; 6F
	DB 0F4H	; 70
	DB 0F4H	; 71
	DB 0F4H	; 72
	DB 0F2H	; 73
	DB 02H	; 74
	DB 01H	; 75
	DB 01H	; 76
	DB 01H	; 77
	DB 01H	; 78
	DB 02H	; 79
	DB 02H	; 7A
	DB 01H	; 7B
	DB 01H	; 7C
	DB 01H	; 7D
	DB 03H	; 7E
	DB 04H	; 7F
	DB 01H	; 80
	DB 01H	; 81
	DB 01H	; 82
	DB 02H	; 83
	DB 02H	; 84
	DB 01H	; 85
	DB 01H	; 86
	DB 01H	; 87
	DB 0F5H	; 88
	DB 01H	; 89
	DB 01H	; 8A
	DB 03H	; 8B
	DB 04H	; 8C
	DB 01H	; 8D
	DB 01H	; 8E
	DB 01H	; 8F
	DB 0F5H	; 90
	DB 0F5H	; 91
	DB 0F5H	; 92
	DB 0F2H	; 93
	DB 01H	; 94
	DB 01H	; 95
	DB 01H	; 96
	DB 01H	; 97
	DB 0F1H	; 98
	DB 02H	; 99
	DB 01H	; 9A
	DB 01H	; 9B
	DB 01H	; 9C
	DB 01H	; 9D
	DB 01H	; 9E
	DB 0F2H	; 9F
	DB 0F3H	; A0
	DB 0F3H	; A1
	DB 0F3H	; A2
	DB 0F2H	; A3
	DB 01H	; A4
	DB 06H	; A5
	DB 01H	; A6
	DB 01H	; A7
	DB 0F2H	; A8
	DB 02H	; A9
	DB 02H	; AA
	DB 02H	; AB
	DB 02H	; AC
	DB 02H	; AD
	DB 01H	; AE
	DB 01H	; AF
	DB 0F4H	; B0
	DB 0F4H	; B1
	DB 0F4H	; B2
	DB 0F3H	; B3
	DB 02H	; B4
	DB 01H	; B5
	DB 02H	; B6
	DB 02H	; B7
	DB 01H	; B8
	DB 01H	; B9
	DB 01H	; BA
	DB 01H	; BB
	DB 01H	; BC
	DB 01H	; BD
	DB 01H	; BE
	DB 01H	; BF
	DB 0F3H	; C0
	DB 0F3H	; C1
	DB 0F7H	; C2
	DB 0F3H	; C3
	DB 02H	; C4
	DB 01H	; C5
	DB 02H	; C6
	DB 02H	; C7
	DB 04H	; C8
	DB 04H	; C9
	DB 04H	; CA
	DB 06H	; CB
	DB 04H	; CC
	DB 04H	; CD
	DB 04H	; CE
	DB 06H	; CF
	DB 0F1H	; D0
	DB 0F1H	; D1
	DB 0F1H	; D2
	DB 02H	; D3
	DB 01H	; D4
	DB 01H	; D5
	DB 03H	; D6
	DB 35H	; D7
	DB 05H	; D8
	DB 24H	; D9
	DB 04H	; DA
	DB 24H	; DB
	DB 04H	; DC
	DB 37H	; DD
	DB 07H	; DE
	DB 28H	; DF
	DB 08H	; E0
	DB 37H	; E1
	DB 07H	; E2
	DB 29H	; E3
	DB 09H	; E4
	DB 29H	; E5
	DB 09H	; E6
	DB 35H	; E7
	DB 05H	; E8
	DB 48H	; E9
	DB 08H	; EA
	DB 24H	; EB
	DB 04H	; EC
	DB 35H	; ED
	DB 05H	; EE
	DB 36H	; EF
	DB 06H	; F0
	DB 37H	; F1
	DB 07H	; F2
	DB 35H	; F3
	DB 05H	; F4
	DB 3AH	; F5
	DB 0AH	; F6
	DB 27H	; F7
	DB 07H	; F8
	DB 4BH	; F9
	DB 0BH	; FA
	DB 35H	; FB
	DB 05H	; FC
	DB 45H	; FD
	DB 05H	; FE
	DB 01H	; FF

; Character String Table

; DATA: GY454XE  Re 013F2
; DATA: GY455XE  Im 013F2
_char_placeholder:
	DB "@"

; DATA: GY454XE  Re 013F3
; DATA: GY455XE  Im 013F3
_char_cs01:
	DB "m\xb6"

; DATA: GY454XE  Re 013F5
; DATA: GY455XE  Im 013F5
_char_cs02:
	DB "m\x9d"

; DATA: GY454XE  Re 013F7
; DATA: GY455XE  Im 013F7
_char_cs03:
	DB "me"

; DATA: GY454XE  Re 013F9
; DATA: GY455XE  Im 013F9
_char_cs04:
	DB "m\xb7"

; DATA: GY454XE  Re 013FB
; DATA: GY455XE  Im 013FB
_char_cs05:
	DB "a\xb0"

; DATA: GY454XE  Re 013FD
; DATA: GY455XE  Im 013FD
_char_cs06:
_char_base_hex:
	DB "h"

; DATA: GY454XE  Re 013FE
; DATA: GY455XE  Im 013FE
_char_cs07:
	DB "\xc6\xb5"

; DATA: GY454XE  Re 01400
; DATA: GY455XE  Im 01400
_char_cs08:
	DB "\xc6\x9b"

; DATA: GY454XE  Re 01402
; DATA: GY455XE  Im 01402
_char_cs09:
	DB "\xcb"

; DATA: GY454XE  Re 01403
; DATA: GY455XE  Im 01403
_char_cs10:
	DB "\xc1"

; DATA: GY454XE  Re 01404
; DATA: GY455XE  Im 01404
_char_cs11:
_char_stat_reg_r:
	DB "re"

; DATA: GY454XE  Re 01406
; DATA: GY455XE  Im 01406
_char_cs12:
	DB "\xc5c"

; DATA: GY454XE  Re 01408
; DATA: GY455XE  Im 01408
_char_cs13:
	DB "\xc2\xb6"

; DATA: GY454XE  Re 0140A
; DATA: GY455XE  Im 0140A
_char_cs14:
	DB "\xc5cp"

; DATA: GY454XE  Re 0140D
; DATA: GY455XE  Im 0140D
_char_cs15:
	DB "\xc5cn"

; DATA: GY454XE  Re 01410
; DATA: GY455XE  Im 01410
_char_sum_x2:
_char_sum_x:
_char_sum:
	DB "\xc0x\xa2"

; DATA: GY454XE  Re 01413
; DATA: GY455XE  Im 01413
_char_n:
	DB "n"

; DATA: GY454XE  Re 01414
; DATA: GY455XE  Im 01414
_char_sum_y2:
_char_sum_y:
	DB "\xc0y\xa2"

; DATA: GY454XE  Re 01417
; DATA: GY455XE  Im 01417
_char_sum_xy:
	DB "\xc0xy"

; DATA: GY454XE  Re 0141A
; DATA: GY455XE  Im 0141A
_char_sum_x3:
	DB "\xc0x\xa3"

; DATA: GY454XE  Re 0141D
; DATA: GY455XE  Im 0141D
_char_sum_x2y:
	DB "\xc0x\xa2y"

; DATA: GY454XE  Re 01421
; DATA: GY455XE  Im 01421
_char_sum_x4:
	DB "\xc0x\xa4"

; DATA: GY454XE  Re 01424
; DATA: GY455XE  Im 01424
_char_min_x:
	DB "minX"

; DATA: GY454XE  Re 01428
; DATA: GY455XE  Im 01428
_char_max_x:
	DB "maxX"

; DATA: GY454XE  Re 0142C
; DATA: GY455XE  Im 0142C
_char_min_y:
	DB "minY"

; DATA: GY454XE  Re 01430
; DATA: GY455XE  Im 01430
_char_max_y:
	DB "maxY"

; DATA: GY454XE  Re 01434
; DATA: GY455XE  Im 01434
_char_cs16:
	DB "R\x84"

; DATA: GY454XE  Re 01436
; DATA: GY455XE  Im 01436
_char_cs17:
	DB "u"

; DATA: GY454XE  Re 01437
; DATA: GY455XE  Im 01437
_char_cs18:
	DB "\xc6\xb6"

; DATA: GY454XE  Re 01439
; DATA: GY455XE  Im 01439
_char_0x20:
	DB " "

; DATA: GY454XE  Re 0143A
; DATA: GY455XE  Im 0143A
_char_box:
	DB "\xcd"

; DATA: GY454XE  Re 0143B
; DATA: GY455XE  Im 0143B
_char_cs19:
	DB "\xc6e"

; DATA: GY454XE  Re 0143D
; DATA: GY455XE  Im 0143D
_char_cs20:
	DB "\xc6\x9d"

; DATA: GY454XE  Re 0143F
; DATA: GY455XE  Im 0143F
_char_cs21:
	DB "\xc6\xb7"

; DATA: GY454XE  Re 01441
; DATA: GY455XE  Im 01441
_char_percent:
	DB "%"

; DATA: GY454XE  Re 01442
; DATA: GY455XE  Im 01442
_char_cs22:
	DB "F"

; DATA: GY454XE  Re 01443
; DATA: GY455XE  Im 01443
_char_cs23:
	DB "e"

; DATA: GY454XE  Re 01444
; DATA: GY455XE  Im 01444
_char_paren_l:
	DB "("

; DATA: GY454XE  Re 01445
; DATA: GY455XE  Im 01445
_char_paren_r:
	DB ")"

; DATA: GY454XE  Re 01446
; DATA: GY455XE  Im 01446
_char_cs24:
	DB "N\x9a"

; DATA: GY454XE  Re 01448
; DATA: GY455XE  Im 01448
_char_plus:
	DB "+"

; DATA: GY454XE  Re 01449
; DATA: GY455XE  Im 01449
_char_sep:
	DB ","

; DATA: GY454XE  Re 0144A
; DATA: GY455XE  Im 0144A
_char_minus:
	DB "-"

; DATA: GY454XE  Re 0144B
; DATA: GY455XE  Im 0144B
_char_decimal:
	DB "."

; DATA: GY454XE  Re 0144C
; DATA: GY455XE  Im 0144C
_char_0:
	DB "0"

; DATA: GY454XE  Re 0144D
; DATA: GY455XE  Im 0144D
_char_1:
	DB "1"

; DATA: GY454XE  Re 0144E
; DATA: GY455XE  Im 0144E
_char_2:
	DB "2"

; DATA: GY454XE  Re 0144F
; DATA: GY455XE  Im 0144F
_char_3:
	DB "3"

; DATA: GY454XE  Re 01450
; DATA: GY455XE  Im 01450
_char_4:
	DB "4"

; DATA: GY454XE  Re 01451
; DATA: GY455XE  Im 01451
_char_5:
	DB "5"

; DATA: GY454XE  Re 01452
; DATA: GY455XE  Im 01452
_char_6:
	DB "6"

; DATA: GY454XE  Re 01453
; DATA: GY455XE  Im 01453
_char_7:
	DB "7"

; DATA: GY454XE  Re 01454
; DATA: GY455XE  Im 01454
_char_8:
	DB "8"

; DATA: GY454XE  Re 01455
; DATA: GY455XE  Im 01455
_char_9:
	DB "9"

; DATA: GY454XE  Re 01456
; DATA: GY455XE  Im 01456
_char_colon:
	DB ":"

; DATA: GY454XE  Re 01457
; DATA: GY455XE  Im 01457
_char_equals:
	DB "="

; DATA: GY454XE  Re 01458
; DATA: GY455XE  Im 01458
_char_cs26:
	DB "Vm"

; DATA: GY454XE  Re 0145A
; DATA: GY455XE  Im 0145A
_char_sto_a:
	DB "\x8dA"

; DATA: GY454XE  Re 0145C
; DATA: GY455XE  Im 0145C
_char_var_b:
_char_sto_b:
	DB "\x8dB"

; DATA: GY454XE  Re 0145E
; DATA: GY455XE  Im 0145E
_char_sto_c:
	DB "\x8dC"

; DATA: GY454XE  Re 01460
; DATA: GY455XE  Im 01460
_char_var_d:
_char_sto_d:
	DB "\x8dD"

; DATA: GY454XE  Re 01462
; DATA: GY455XE  Im 01462
_char_sto_m:
	DB "\x8dM"

; DATA: GY454XE  Re 01464
; DATA: GY455XE  Im 01464
_char_sto_x:
_char_var_x:
	DB "\x8dX"

; DATA: GY454XE  Re 01466
; DATA: GY455XE  Im 01466
_char_sto_y:
_char_var_y:
	DB "\x8dY"

; DATA: GY454XE  Re 01468
; DATA: GY455XE  Im 01468
_char_mul:
	DB "$"

; DATA: GY454XE  Re 01469
; DATA: GY455XE  Im 01469
_char_div:
	DB "&"

; DATA: GY454XE  Re 0146A
; DATA: GY455XE  Im 0146A
_char_base_bin:
	DB "b"

; DATA: GY454XE  Re 0146B
; DATA: GY455XE  Im 0146B
_char_cmplx_rec:
	DB "\x9ea+b\x80"

; DATA: GY454XE  Re 01470
; DATA: GY455XE  Im 01470
_char_cmplx_pol:
	DB "\x9er\x88\xc4"

; DATA: GY454XE  Re 01474
; DATA: GY455XE  Im 01474
_char_factorial:
	DB "!"

; DATA: GY454XE  Re 01475
; DATA: GY455XE  Im 01475
_char_0x5b:
	DB "["

; DATA: GY454XE  Re 01476
; DATA: GY455XE  Im 01476
_char_dms:
	DB "\\"

; DATA: GY454XE  Re 01477
; DATA: GY455XE  Im 01477
_char_0x5d:
	DB "]"

; DATA: GY454XE  Re 01478
; DATA: GY455XE  Im 01478
_char_pow:
	DB "^"

; DATA: GY454XE  Re 01479
; DATA: GY455XE  Im 01479
_char_negative:
	DB "`"

; DATA: GY454XE  Re 0147A
; DATA: GY455XE  Im 0147A
_char_not:
	DB "Not"

; DATA: GY454XE  Re 0147D
; DATA: GY455XE  Im 0147D
_char_neg:
	DB "Neg"

; DATA: GY454XE  Re 01480
; DATA: GY455XE  Im 01480
_char_var_a:
_char_abs:
	DB "Abs"

; DATA: GY454XE  Re 01483
; DATA: GY455XE  Im 01483
_char_xhat1:
_char_xhat:
	DB "\x8b\xb1"

; DATA: GY454XE  Re 01485
; DATA: GY455XE  Im 01485
_char_yhat:
	DB "\x8c"

; DATA: GY454XE  Re 01486
; DATA: GY455XE  Im 01486
_char_xhat2:
	DB "\x8b\xb2"

; DATA: GY454XE  Re 01488
; DATA: GY455XE  Im 01488
_char_log:
	DB "log"

; DATA: GY454XE  Re 0148B
; DATA: GY455XE  Im 0148B
_char_integral:
	DB "\x99"

; DATA: GY454XE  Re 0148C
; DATA: GY455XE  Im 0148C
_char_base_dec:
_char_ddx:
	DB "d/dx"

; DATA: GY454XE  Re 01490
; DATA: GY455XE  Im 01490
_char_pol:
_char_stat_p:
	DB "Pol"

; DATA: GY454XE  Re 01493
; DATA: GY455XE  Im 01493
_char_rec:
_char_cs27:
_char_stat_r:
	DB "Rec"

; DATA: GY454XE  Re 01496
; DATA: GY455XE  Im 01496
_char_and:
	DB "and"

; DATA: GY454XE  Re 01499
; DATA: GY455XE  Im 01499
_char_base_oct:
_char_or:
	DB "or"

; DATA: GY454XE  Re 0149B
; DATA: GY455XE  Im 0149B
_char_epow:
_char_euler:
	DB "\x81^"

; DATA: GY454XE  Re 0149D
; DATA: GY455XE  Im 0149D
_char_exp:
	DB "\x82\x83"

; DATA: GY454XE  Re 0149F
; DATA: GY455XE  Im 0149F
_char_pow_2:
	DB "\xa2"

; DATA: GY454XE  Re 014A0
; DATA: GY455XE  Im 014A0
_char_pow_3:
	DB "\xa3"

; DATA: GY454XE  Re 014A1
; DATA: GY455XE  Im 014A1
_char_pow_m1:
	DB "\xaa"

; DATA: GY454XE  Re 014A2
; DATA: GY455XE  Im 014A2
_char_cs28:
	DB "c\xb0"

; DATA: GY454XE  Re 014A4
; DATA: GY455XE  Im 014A4
_char_cs29:
	DB "c\xb0"

; DATA: GY454XE  Re 014A6
; DATA: GY455XE  Im 014A6
_char_0x7b:
	DB "{"

; DATA: GY454XE  Re 014A7
; DATA: GY455XE  Im 014A7
_char_0x7d:
	DB "}"

; DATA: GY454XE  Re 014A8
; DATA: GY455XE  Im 014A8
_char_xor:
	DB "xor"

; DATA: GY454XE  Re 014AB
; DATA: GY455XE  Im 014AB
_char_xnor:
	DB "xnor"

; DATA: GY454XE  Re 014AF
; DATA: GY455XE  Im 014AF
_char_cmplx_i:
	DB "\x80"

; DATA: GY454XE  Re 014B0
; DATA: GY455XE  Im 014B0
_char_pi:
	DB "\xc7"

; DATA: GY454XE  Re 014B1
; DATA: GY455XE  Im 014B1
_char_d:
	DB "\x85"

; DATA: GY454XE  Re 014B2
; DATA: GY455XE  Im 014B2
_char_r:
	DB "\x86"

; DATA: GY454XE  Re 014B3
; DATA: GY455XE  Im 014B3
_char_g:
	DB "\x87"

; DATA: GY454XE  Re 014B4
; DATA: GY455XE  Im 014B4
_char_var_c:
_char_conjg:
	DB "Conjg"

; DATA: GY454XE  Re 014B9
; DATA: GY455XE  Im 014B9
_char_xbar:
	DB "\x89"

; DATA: GY454XE  Re 014BA
; DATA: GY455XE  Im 014BA
_char_ybar:
	DB "\x8a"

; DATA: GY454XE  Re 014BB
; DATA: GY455XE  Im 014BB
_char_ans:
	DB "Ans"

; DATA: GY454XE  Re 014BE
; DATA: GY455XE  Im 014BE
_char_ran:
	DB "Ran#"

; DATA: GY454XE  Re 014C2
; DATA: GY455XE  Im 014C2
_char_10pow:
	DB "\x83^"

; DATA: GY454XE  Re 014C4
; DATA: GY455XE  Im 014C4
_char_sqrt:
	DB "\x98"

; DATA: GY454XE  Re 014C5
; DATA: GY455XE  Im 014C5
_char_var_m:
_char_m_plus:
	DB "M+"

; DATA: GY454XE  Re 014C7
; DATA: GY455XE  Im 014C7
_char_stat_a:
	DB "\x9a"

; DATA: GY454XE  Re 014C8
; DATA: GY455XE  Im 014C8
_char_stat_b:
	DB "\x9b"

; DATA: GY454XE  Re 014C9
; DATA: GY455XE  Im 014C9
_char_stat_c:
	DB "\x9c"

; DATA: GY454XE  Re 014CA
; DATA: GY455XE  Im 014CA
_char_dot:
	DB "*"

; DATA: GY454XE  Re 014CB
; DATA: GY455XE  Im 014CB
_char_nth_rt:
	DB "\xab\x98"

; DATA: GY454XE  Re 014CD
; DATA: GY455XE  Im 014CD
_char_ln:
	DB "ln"

; DATA: GY454XE  Re 014CF
; DATA: GY455XE  Im 014CF
_char_conv:
	DB "\x9eConv "

; DATA: GY454XE  Re 014D5
; DATA: GY455XE  Im 014D5
_char_cbrt:
	DB "\xa3\x98"

; DATA: GY454XE  Re 014D7
; DATA: GY455XE  Im 014D7
_char_m_minus:
	DB "M-"

; DATA: GY454XE  Re 014D9
; DATA: GY455XE  Im 014D9
_char_stat_ox:
	DB "\xc8x"

; DATA: GY454XE  Re 014DB
; DATA: GY455XE  Im 014DB
_char_stat_sx:
	DB "sx"

; DATA: GY454XE  Re 014DD
; DATA: GY455XE  Im 014DD
_char_stat_oy:
	DB "\xc8y"

; DATA: GY454XE  Re 014DF
; DATA: GY455XE  Im 014DF
_char_stat_sy:
	DB "sy"

; DATA: GY454XE  Re 014E1
; DATA: GY455XE  Im 014E1
_char_frac:
	DB "\x93"

; DATA: GY454XE  Re 014E2
; DATA: GY455XE  Im 014E2
_char_angle:
	DB "\x88"

; DATA: GY454XE  Re 014E3
; DATA: GY455XE  Im 014E3
_char_cs38:
	DB "t"

; DATA: GY454XE  Re 014E4
; DATA: GY455XE  Im 014E4
_char_rnd:
	DB "Rnd"

; DATA: GY454XE  Re 014E7
; DATA: GY455XE  Im 014E7
_char_cs30:
	DB "c\xb2"

; DATA: GY454XE  Re 014E9
; DATA: GY455XE  Im 014E9
_char_cs31:
	DB "\xc8"

; DATA: GY454XE  Re 014EA
; DATA: GY455XE  Im 014EA
_char_cs32:
	DB "\xc3\xb0"

; DATA: GY454XE  Re 014EC
; DATA: GY455XE  Im 014EC
_char_cs33:
	DB "\xc6\xb0"

; DATA: GY454XE  Re 014EE
; DATA: GY455XE  Im 014EE
_char_hex_a:
	DB "\xb8"

; DATA: GY454XE  Re 014EF
; DATA: GY455XE  Im 014EF
_char_hex_b:
	DB "\xb9"

; DATA: GY454XE  Re 014F0
; DATA: GY455XE  Im 014F0
_char_combi:
_char_hex_c:
	DB "\xba"

; DATA: GY454XE  Re 014F1
; DATA: GY455XE  Im 014F1
_char_hex_d:
	DB "\xbb"

; DATA: GY454XE  Re 014F2
; DATA: GY455XE  Im 014F2
_char_hex_e:
	DB "\xbc"

; DATA: GY454XE  Re 014F3
; DATA: GY455XE  Im 014F3
_char_hex_f:
	DB "\xbd"

; DATA: GY454XE  Re 014F4
; DATA: GY455XE  Im 014F4
_char_permu:
	DB "\xbe"

; DATA: GY454XE  Re 014F5
; DATA: GY455XE  Im 014F5
_char_det:
	DB "det"

; DATA: GY454XE  Re 014F8
; DATA: GY455XE  Im 014F8
_char_trn:
	DB "Trn"

; DATA: GY454XE  Re 014FB
; DATA: GY455XE  Im 014FB
_char_ranint:
	DB "RanInt#"

; DATA: GY454XE  Re 01502
; DATA: GY455XE  Im 01502
_char_arg:
	DB "arg"

; DATA: GY454XE  Re 01505
; DATA: GY455XE  Im 01505
_char_cs34:
	DB "\xc9\xb0"

; DATA: GY454XE  Re 01507
; DATA: GY455XE  Im 01507
_char_cs36:
_char_cs39:
	DB "G\xb0"

; DATA: GY454XE  Re 01509
; DATA: GY455XE  Im 01509
_char_cs37:
	DB "Z\xb0"

; DATA: GY454XE  Re 0150B
; DATA: GY455XE  Im 0150B
_char_matb:
	DB "MatB"

; DATA: GY454XE  Re 0150F
; DATA: GY455XE  Im 0150F
_char_matc:
	DB "MatC"

; DATA: GY454XE  Re 01513
; DATA: GY455XE  Im 01513
_char_mata:
_char_matans:
	DB "MatAns"

; DATA: GY454XE  Re 01519
; DATA: GY455XE  Im 01519
_char_vctb:
	DB "VctB"

; DATA: GY454XE  Re 0151D
; DATA: GY455XE  Im 0151D
_char_vctc:
	DB "VctC"

; DATA: GY454XE  Re 01521
; DATA: GY455XE  Im 01521
_char_vcta:
_char_vctans:
	DB "VctAns"

; DATA: GY454XE  Re 01527
; DATA: GY455XE  Im 01527
_char_stat_q:
	DB "Q"

; DATA: GY454XE  Re 01528
; DATA: GY455XE  Im 01528
_char_stat_tot:
	DB "\x9et"

; DATA: GY454XE  Re 0152A
; DATA: GY455XE  Im 0152A
_char_cs40:
	DB "atm"

; DATA: GY454XE  Re 0152D
; DATA: GY455XE  Im 0152D
_char_cv01:
_char_cv02:
	DB "cm\x9ein\x9ecm"

; DATA: GY454XE  Re 01535
; DATA: GY455XE  Im 01535
_char_cv03:
_char_cv04:
	DB "m\x9eft\x9em"

; DATA: GY454XE  Re 0153B
; DATA: GY455XE  Im 0153B
_char_cv05:
_char_cv06:
	DB "m\x9eyd\x9em"

; DATA: GY454XE  Re 01541
; DATA: GY455XE  Im 01541
_char_cs25:
_char_cv07:
_char_cv08:
	DB "k"
	DB "m\x9emile\x9ekm"

; DATA: GY454XE  Re 0154B
; DATA: GY455XE  Im 0154B
_char_cv09:
_char_cv10:
	DB "m\x9en mile\x9em"

; DATA: GY454XE  Re 01555
; DATA: GY455XE  Im 01555
_char_cv11:
_char_cv12:
	DB "m\xa2\x9eacre\x9em\xa2"

; DATA: GY454XE  Re 0155F
; DATA: GY455XE  Im 0155F
_char_cv13:
_char_cv14:
	DB "\xca\x9egal(US)\x9e\xca"

; DATA: GY454XE  Re 0156A
; DATA: GY455XE  Im 0156A
_char_cv15:
_char_cv16:
	DB "\xca\x9egal(UK)\x9e\xca"

; DATA: GY454XE  Re 01575
; DATA: GY455XE  Im 01575
_char_cv17:
_char_cv18:
	DB "km\x9epc\x9ekm"

; DATA: GY454XE  Re 0157D
; DATA: GY455XE  Im 0157D
_char_cv19:
_char_cv20:
	DB "m/s\x9ekm/h\x9em/s"

; DATA: GY454XE  Re 01589
; DATA: GY455XE  Im 01589
_char_cs35:
_char_cv21:
_char_cv22:
	DB "g\x9eoz\x9eg"

; DATA: GY454XE  Re 0158F
; DATA: GY455XE  Im 0158F
_char_cv23:
_char_cv24:
	DB "kg\x9elb\x9ekg"

; DATA: GY454XE  Re 01597
; DATA: GY455XE  Im 01597
_char_cv25:
_char_cv26:
	DB "Pa\x9eatm\x9ePa"

; DATA: GY454XE  Re 015A0
; DATA: GY455XE  Im 015A0
_char_cv27:
_char_cv28:
	DB "Pa\x9emmHg\x9ePa"

; DATA: GY454XE  Re 015AA
; DATA: GY455XE  Im 015AA
_char_cv29:
_char_cv30:
	DB "kW\x9ehp\x9ekW"

; DATA: GY454XE  Re 015B2
; DATA: GY455XE  Im 015B2
_char_cv31:
_char_cv32:
	DB "Pa\x9ekgf/cm\xa2\x9ePa"

; DATA: GY454XE  Re 015BF
; DATA: GY455XE  Im 015BF
_char_cv33:
_char_cv34:
	DB "J\x9ekgf*m\x9eJ"

; DATA: GY454XE  Re 015C8
; DATA: GY455XE  Im 015C8
_char_cv35:
_char_cv36:
	DB "kPa\x9elbf/in\xa2\x9ekPa"

; DATA: GY454XE  Re 015D7
; DATA: GY455XE  Im 015D7
_char_cv37:
_char_cv38:
	DB "\x85C\x9e\x85F\x9e\x85C"

; DATA: GY454XE  Re 015DF
; DATA: GY455XE  Im 015DF
_char_cv39:
_char_cv40:
	DB "cal\x9eJ\x9ecal"

; DATA: GY454XE  Re 015E8
; DATA: GY455XE  Im 015E8
_char_var_e:
_char_sto_e:
	DB "\x8dE"

; DATA: GY454XE  Re 015EA
; DATA: GY455XE  Im 015EA
_char_var_f:
_char_sto_f:
	DB "\x8dF"

; DATA: GY454XE  Re 015EC
; DATA: GY455XE  Im 015EC
_char_remainder:
	DB "&R"

; DATA: GY454XE  Re 015EE
; DATA: GY455XE  Im 015EE
_char_lt:
	DB "<"

; DATA: GY454XE  Re 015EF
; DATA: GY455XE  Im 015EF
_char_gt:
	DB ">"

; DATA: GY454XE  Re 015F0
; DATA: GY455XE  Im 015F0
_char_le:
	DB "\x94"

; DATA: GY454XE  Re 015F1
; DATA: GY455XE  Im 015F1
_char_ne:
	DB "\x95"

; DATA: GY454XE  Re 015F2
; DATA: GY455XE  Im 015F2
_char_ge:
	DB "\x96"

; DATA: GY454XE  Re 015F3
; DATA: GY455XE  Im 015F3
_char_sinh:
_char_asinh:
	DB "sinh\xaa"

; DATA: GY454XE  Re 015F8
; DATA: GY455XE  Im 015F8
_char_cosh:
_char_acosh:
	DB "cosh\xaa"

; DATA: GY454XE  Re 015FD
; DATA: GY455XE  Im 015FD
_char_tanh:
_char_atanh:
	DB "tanh\xaa"

; DATA: GY454XE  Re 01602
; DATA: GY455XE  Im 01602
_char_sin:
_char_asin:
	DB "sin\xaa"

; DATA: GY454XE  Re 01606
; DATA: GY455XE  Im 01606
_char_cos:
_char_acos:
	DB "cos\xaa"

; DATA: GY454XE  Re 0160A
; DATA: GY455XE  Im 0160A
_char_tan:
_char_atan:
	DB "tan\xaa"


RSEG $$NTABgenerals2

; X position bitmask. Indexed with x_pos & 7. Used in char_print.
; DATA: GY454XE  Re 01C64
; DATA: GY455XE  Im 01C64
_bitmasks_x:
	DB 11111111B
	DB 01111111B
	DB 00111111B
	DB 00011111B
	DB 00001111B
	DB 00000111B
	DB 00000011B
	DB 00000001B

; Y position bitmask. Indexed with (4 + y_pos) & 7. Used in char_print.
; DATA: GY454XE  Re 01C6C
; DATA: GY455XE  Im 01C6C
_bitmasks_y:
	DB 11111111B
	DB 10000000B
	DB 11000000B
	DB 11100000B
	DB 11110000B
	DB 11111000B
	DB 11111100B
	DB 11111110B

; DATA: GY454XE  Re 01C74
; DATA: GY455XE  Im 01C74
_unk_01c74:
	DB 10010101B
	DB 00000000B
	DB 10010101B
	DB 00000101B
	DB 01010000B
	DB 00000000B
	DB 01010000B
	DB 00000101B
	DB 00000111B
	DB 00000001B
	DB 00000111B
	DB 00000110B

; DATA: GY454XE  Re 01C80
; DATA: GY455XE  Im 01C80
_num_gk_weight:
	DB 04H, 17H, 95H, 91H, 83H, 67H, 34H, 69H, 99H, 00H  ; 0.417959183673469

; DATA: GY454XE  Re 01C8A
; DATA: GY455XE  Im 01C8A
_unk_01c8a:
	DB 02H, 09H, 48H, 21H, 41H, 08H, 47H, 28H, 99H, 00H  ; 0.209482141084728

; DATA: GY454XE  Re 01C94
; DATA: GY455XE  Im 01C94
_num_gk_nodes_symm:
	DB 09H, 49H, 10H, 79H, 12H, 34H, 27H, 59H, 99H, 00H  ; 0.949107912342759
	DB 06H, 30H, 92H, 09H, 26H, 29H, 97H, 86H, 98H, 00H  ; 0.0630920926299786
	DB 01H, 29H, 48H, 49H, 66H, 16H, 88H, 70H, 99H, 00H  ; 0.12948496616887
	DB 07H, 41H, 53H, 11H, 85H, 59H, 93H, 94H, 99H, 00H  ; 0.741531185599394
	DB 01H, 40H, 65H, 32H, 59H, 71H, 55H, 26H, 99H, 00H  ; 0.140653259715526
	DB 02H, 79H, 70H, 53H, 91H, 48H, 92H, 77H, 99H, 00H  ; 0.279705391489277
	DB 04H, 05H, 84H, 51H, 51H, 37H, 73H, 97H, 99H, 00H  ; 0.405845151377397
	DB 01H, 90H, 35H, 05H, 78H, 06H, 47H, 85H, 99H, 00H  ; 0.190350578064785
	DB 03H, 81H, 83H, 00H, 50H, 50H, 51H, 19H, 99H, 00H  ; 0.381830050505119

; DATA: GY454XE  Re 01CEE
; DATA: GY455XE  Im 01CEE
_num_gk_nodes_expanded:
	DB 09H, 91H, 45H, 53H, 71H, 12H, 08H, 13H, 99H, 00H  ; 0.991455371120813
	DB 02H, 29H, 35H, 32H, 20H, 10H, 52H, 92H, 98H, 00H  ; 0.0229353220105292
	DB 08H, 64H, 86H, 44H, 23H, 35H, 97H, 69H, 99H, 00H  ; 0.864864423359769
	DB 01H, 04H, 79H, 00H, 10H, 32H, 22H, 50H, 99H, 00H  ; 0.10479001032225
	DB 05H, 86H, 08H, 72H, 35H, 46H, 76H, 91H, 99H, 00H  ; 0.586087235467691
	DB 01H, 69H, 00H, 47H, 26H, 63H, 92H, 68H, 99H, 00H  ; 0.169004726639268
	DB 02H, 07H, 78H, 49H, 55H, 00H, 78H, 99H, 99H, 00H  ; 0.207784955007899
	DB 02H, 04H, 43H, 29H, 40H, 07H, 52H, 99H, 99H, 00H  ; 0.204432940075299

RSEG $$NTABgenerals3

; 96x13 bitmap image, 12 bytes per row. Need I say more?
; DATA: GY454XE  Re 01F10
; DATA: GY455XE  Im 01F12
_casio_logo:
	DB	00000000B,	00000001B,	11111111B,	11000000B,	00111110B,	00000011B,	11111111B,	11000111B,	00001111B,	11111110B,	00000000B,	00000000B
	DB	00000000B,	00000011B,	11111111B,	11100000B,	00111110B,	00000111B,	11111111B,	11100111B,	00011111B,	11111111B,	00000000B,	00000000B
	DB	00000000B,	00000111B,	11111111B,	11110000B,	01111111B,	00000111B,	11111111B,	11100111B,	00111111B,	11111111B,	10000000B,	00000000B
	DB	00000000B,	00000111B,	10000000B,	11110000B,	01110111B,	00000111B,	00000000B,	11100111B,	00111100B,	00000111B,	10000000B,	00000000B
	DB	00000000B,	00000111B,	00000000B,	01110000B,	11110111B,	10000111B,	00000000B,	00000111B,	00111000B,	00000011B,	10000000B,	00000000B
	DB	00000000B,	00000111B,	00000000B,	00000000B,	11100011B,	10000111B,	11111111B,	11000111B,	00111000B,	00000011B,	10001110B,	00000000B
	DB	00000000B,	00000111B,	00000000B,	00000001B,	11100011B,	11000111B,	11111111B,	11100111B,	00111000B,	00000011B,	10010001B,	00000000B
	DB	00000000B,	00000111B,	00000000B,	00000001B,	11000001B,	11000011B,	11111111B,	11100111B,	00111000B,	00000011B,	10101100B,	10000000B
	DB	00000000B,	00000111B,	00000000B,	01110011B,	11111111B,	11100000B,	00000000B,	11100111B,	00111000B,	00000011B,	10101010B,	10000000B
	DB	00000000B,	00000111B,	10000000B,	11110011B,	11111111B,	11100111B,	00000000B,	11100111B,	00111100B,	00000111B,	10101100B,	10000000B
	DB	00000000B,	00000111B,	11111111B,	11110111B,	11111111B,	11110111B,	11111111B,	11100111B,	00111111B,	11111111B,	10101010B,	10000000B
	DB	00000000B,	00000011B,	11111111B,	11100111B,	00000000B,	01110111B,	11111111B,	11100111B,	00011111B,	11111111B,	00010001B,	00000000B
	DB	00000000B,	00000001B,	11111111B,	11000111B,	00000000B,	01110011B,	11111111B,	11000111B,	00001111B,	11111110B,	00001110B,	00000000B

RSEG $$NCODgenerals

; FUNCTION: GY454XE  Re 02676
; FUNCTION: GY455XE  Im 02808
; FUNCTION: GY460XF  Im 0249C
; FUNCTION: GY465XG  Im 023DA
; FUNCTION: GY468XB  Im 0247C
_f_02676:
	MOV R1, #0H
	SRL R0, #1
	ADDC R1, #0H
	SLL R1, #2
	MOV R2, #BYTE1 _unk_0268e
	MOV R3, #BYTE2 _unk_0268e
	ADD R2, R0
	ADDC R3, #0H
	L R0, [ER2]
	SRL R0, R1
	AND R0, #0FH
	RT

; DATA: GY454XE  Re 0268E
; DATA: GY455XE  Im 02820
; DATA: GY460XF  Im 024B4
_unk_0268e:
	DB  0BH,  00H,  00H,  00H,  00H,  00H,  00H,  00H
	DB  00H,  00H,  00H,  00H,  00H,  00H,  00H,  00H
	DB  00H,  00H,  00H,  00H,  98H, 0D0H, 0DCH,  00H
	DB  00H,  00H,  00H,  00H,  00H,  0CH, 0CCH,  0CH
	DB  00H,  00H,  00H, 0C0H, 0CCH, 0CCH, 0CCH, 0DDH
	DB  00H,  00H, 0C0H,  0CH,  00H,  00H,  00H, 0D1H
	DB 0ADH,  1AH,  00H,  00H,  11H,  11H, 0AAH,  00H
	DB 0AAH,  1AH,  00H,  00H,  00H,  00H,  01H,  00H
	DB  00H, 0C0H,  0CH,  00H,  0AH,  00H, 0C0H, 0CCH
	DB 0AAH,  1AH, 0CCH, 0CCH, 0C1H,  00H,  00H,  10H
	DB 0AAH, 0AAH,  01H,  00H, 0C0H,  00H,  00H, 0D1H
	DB 0AAH, 0AAH,  00H,  00H,  32H,  65H,  07H, 0DDH
	DB 0AAH, 0AAH,  00H,  00H,  00H,  00H,  00H,  00H
	DB 0AAH,  0AH,  00H,  00H,  00H,  00H,  00H,  00H
	DB  00H,  00H,  00H,  00H,  00H,  00H,  00H,  00H
	DB  00H,  00H,  00H,  00H,  00H,  00H,  00H,  00H


; FUNCTION: GY454XE  Re 0270E
; FUNCTION: GY455XE  Im 028A0
; FUNCTION: GY460XF  Im 02534
_get_mathi_draw_idx:
	MOV R1, #5EH       ; Pow
	SUB R0, R1
	MOV R1, R0         ; Compare with 0
	BEQ _$j_0274e
	MOV R0, #1
	CMP R1, #63H-5EH   ; Absolute value
	BEQ _$j_0274e
	MOV R0, #6
	CMP R1, #73H-5EH   ; e^n
	BEQ _$j_0274e
	MOV R0, #7
	CMP R1, #7CH-5EH   ; Mixed fraction
	BEQ _$j_0274e
	MOV R0, #8
	CMP R1, #93H-5EH   ; 10^n
	BEQ _$j_0274e
	MOV R0, #9
	CMP R1, #98H-5EH   ; Square root
	BEQ _$j_0274e
	MOV R0, #10
	CMP R1, #9FH-5EH   ; nth root
	BEQ _$j_0274e
	MOV R0, #11
IF ENABLE_RDEC == 1
	CMP R1, #0A4H-5EH  ; Recurring decimal
	BEQ _$j_0274e
	MOV R0, #12
ENDIF
	CMP R1, #0AEH-5EH  ; Fraction
	BEQ _$j_0274e
	MOV R0, #13
	CMP R1, #68H-5EH   ; logab, Sum, Integral, Derivative
	BLT _$j_0274e
	CMP R1, #6BH-5EH
	BGT _$j_0274e
	MOV R0, R1
	ADD R0, #-8        ; 2, 3, 4, 5 respectively
_$j_0274e:
	RT

; FUNCTION: GY454XE  Re 02750
; FUNCTION: GY455XE  Im 028E2
; FUNCTION: GY460XF  Im 0257C
_smart_strlen:
	MOV ER0, ER0
	BEQ _$j_02758
	B _strlen_n
_$j_02758:
	RT

; FUNCTION: GY454XE  Re 0275A
; FUNCTION: GY455XE  Im 028EC
; FUNCTION: GY460XF  Im 02586
_smart_strcpy:
	PUSH LR
	MOV ER0, ER0
	BEQ _$j_02768
	MOV ER2, ER2
	BEQ _$j_0276a
	BL _strcpy_nn
_$j_02768:
	POP PC
_$j_0276a:
	ST R2, [ER0]
	POP PC

; FUNCTION: GY454XE  Re 0276E
; FUNCTION: GY455XE  Im 02900
; FUNCTION: GY460XF  Im 0259A
_smart_strcat:
	PUSH LR
	MOV ER0, ER0
	BEQ _$j_02768
	MOV ER2, ER2
	BEQ _$j_0276a
	BL _strcat_nn
	POP PC

; FUNCTION: GY454XE  Re 0277E
; FUNCTION: GY455XE  Im 02910
; FUNCTION: GY460XF  Im 025AA
_init_num_struct:
	PUSH LR
	PUSH XR12
	PUSH XR4
	MOV BP, ER0
	MOV FP, ER2
	L R1, [ER0]
	MOV R5, R1
	MOV R0, #0H
	AND R1, #11110000B
	BNE _$j_027f4       ; If not floating point type, return 0
	LEA [FP]
	MOV ER2, #0H        ; Initialize the struct
	ST XR0, [EA+]
	ST XR0, [EA+]
	ST XR0, [EA+]
	ST XR0, [EA+]
	ST XR0, [EA+]
	ST XR0, [EA+]
	CMP R5, #0H         ; If first nibble of Area 2 is 0, return 1
	BEQ _$j_027f2
	L R5, 9H[BP]        ; Area 4
	L R0, 8H[BP]        ; Area 3
	MOV R6, R0          ; ==== Convert Area 3 from BCD to hex ====
	AND R6, #00001111B  ; Area 3's ones digit
	SRL R0, #4          ; Area 3's tens digit
	MOV R7, #0H
	MOV R2, #10
	MUL ER0, R2         ; Multiply tens digit by... 10
	ADD ER0, ER6        ; Add in the ones digit. Now we have Area 3 as hex
	CMP R5, #0H         ; If Area 4 is 0, subtract 100 from Area 3
	BEQ _$j_027c0
	CMP R5, #5H         ; Do the same if Area 4 is 5
	BNE _$j_027c4
_$j_027c0:
	ADD R0, #-64H
	ADDC R1, #-1H
_$j_027c4:
	ST ER0, 10H[FP]     ; Store Area 3 into the struct
	MOV ER0, #1H        ; Set R0 = 1, R1 = 0
	CMP R5, #5H         ; Set carry flag if Area 4 < 5 (positive)
	SUBC R0, R1         ; Subtract carry from 1. This will be 0 if Area 4 < 5, 1 otherwise
	ST R0, 0FH[FP]      ; Store result in struct
	MOV ER2, #0H
	MOV R6, #1H         ; Skip type nibble
	MOV ER4, #0H
_$j_027d4:
	MOV ER0, ER2
	ADD ER0, BP
	L R7, [ER0]         ; Load byte from number
	XOR R6, #1H         ; Invert R6
	BEQ _$j_027e2       ; If R6 == 0, jump
	SRL R7, #4          ; Shift top nibble to low nibble
	BAL _$j_027e6       ; Jump
_$j_027e2:
	AND R7, #0FH        ; AND with 0xF to clear top nibble
	ADD R2, #1H         ; Increment the load index
_$j_027e6:
	MOV ER0, ER4
	ADD ER0, FP
	ST R7, [ER0]        ; Copy to the struct
	ADD R4, #1H         ; Increment the store index
	CMP R4, #0FH        ; Repeat 15 times
	BLT _$j_027d4
_$j_027f2:
	MOV R0, #1H         ; Return 1
_$j_027f4:
	POP XR4
	POP XR12
	POP PC

; FUNCTION: GY454XE  Re 027FA
; FUNCTION: GY455XE  Im 0298C
; FUNCTION: GY460XF  Im 02626
_num_exp_to_str:
	PUSH LR
	PUSH QR8
	PUSH XR4
	LEA [ER2]
	MOV BP, ER0
	MOV R4, #90H   ; × in ×10
	L R9, _use_output_charset
	CMP R9, #1H
	BNE _$j_02812  ; If use_output_charset == 1...
	MOV R4, #0E0H  ; Use character 0xE0 (???)
	BAL _$j_0281a
_$j_02812:
	L R8, 16H[BP]  ; Otherwise, if use_tiny_font == 1, use character 0xDD (tiny)
	CMP R8, #1H
	BNE _$j_0281a
	MOV R4, #0DDH
_$j_0281a:
	ST R4, [EA+]   ; Copy to output string
	ADD R4, #1H    ; Go to the next character
	L ER6, 10H[BP]
	BPS _$j_0282a  ; If exponent < 0...
	ADD R4, #1H    ; Go to the next next character
	XOR R6, #11111111B  ; Invert to get -(exponent+1)
	XOR R7, #11111111B
	ADD ER6, #1H   ; Add 1
_$j_0282a:
	ST R4, [EA+]   ; Copy to output string
	MOV ER0, ER6
	MOV R2, #0AH
	DIV ER0, R2    ; Floor divide exponent by 10
	MOV R4, R0     ; Tens digit
	MOV R5, R2     ; Ones digit
	MOV R6, #0A0H  ; Start with superscript 0 (0xA0 is for big/small font)
	CMP R9, #1H    ; If use_output_charset == 1...
	BNE _$j_02840
	MOV R6, #0F0H  ; Start with character 0xF0 (???)
	BAL _$j_02846
_$j_02840:
	CMP R8, #1H    ; Otherwise, if use_tiny_font == 1, start with number 0 (0xD0 tiny)
	BNE _$j_02846
	MOV R6, #0D0H
_$j_02846:
	CMP R4, #0H    ; If tens digit is zero, skip copying tens digit
	BEQ _$j_0284e
	ADD R4, R6     ; Copy the corresponding character for tens digit to output string
	ST R4, [EA+]
_$j_0284e:
	ADD R5, R6     ; Same thing for ones digit
	ST R5, [EA+]
	MOV R2, #0H    ; Finish off with the terminator
	ST R2, [EA+]
	POP XR4
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 0285C
; FUNCTION: GY455XE  Im 029EE
; FUNCTION: GY460XF  Im 02688
_f_0285C:
	PUSH QR8
	PUSH XR4
	MOV BP, ER0
	LEA [ER2]
	L R4, 14H[BP]
	L R5, 15H[BP]
	L R2, 16H[BP]
	BNE _$j_0287a
	MOV R9, #30H
	MOV R8, #2EH
	L R0, _setup_decimal_mark
	BNE _$j_02886
	MOV R8, #2CH
	BAL _$j_02886
_$j_0287a:
	MOV R9, #0D0H
	MOV R8, #0DCH
	L R0, _setup_decimal_mark
	BNE _$j_02886
	MOV R8, #-13H
_$j_02886:
	L R0, [BP]
	BEQ _$j_02956
	L R0, 0FH[BP]
	CMP R0, #1H
	BNE _$j_0289c
	MOV R0, R2
	BNE _$j_02898
	MOV R2, #60H
	BAL _$j_0289a
_$j_02898:
	MOV R2, #-25H
_$j_0289a:
	ST R2, [EA+]
_$j_0289c:
	L ER10, 12H[BP]
	BPS _$j_028e8
	CMP R5, #1H
	BEQ _$j_028a8
	CMP R5, #3H
	BNE _$j_028c2
_$j_028a8:
	CMP R4, #0H
	BLE _$j_028c2
	MOV R6, R4
	MOV R7, #0H
	ADD R6, #-1H
	BEQ _$j_028c2
_$j_028b4:
	MOV ER0, ER6
	ADD ER0, BP
	L R0, [ER0]
	BNE _$j_028c2
	ADD R4, #-1H
	ADD R6, #-1H
	BNE _$j_028b4
_$j_028c2:
	ST R9, [EA+]
	ST R8, [EA+]
	MOV R0, #-1H
	SUB R0, R10
	BAL _$j_028d0
_$j_028cc:
	ST R9, [EA+]
	ADD R0, #-1H
_$j_028d0:
	BNE _$j_028cc
	MOV ER6, #0H
	BAL _$j_028e2
_$j_028d6:
	MOV ER0, BP
	ADD ER0, ER6
	L R0, [ER0]
	ADD R0, R9
	ST R0, [EA+]
	ADD R6, #1H
_$j_028e2:
	CMP R6, R4
	BLT _$j_028d6
	BAL _$j_0294c
_$j_028e8:
	CMP R5, #1H
	BEQ _$j_028f0
	CMP R5, #3H
	BNE _$j_02908
_$j_028f0:
	MOV R6, R4
	MOV R7, #0H
	ADD R6, #-1H
	BAL _$j_02904
_$j_028f8:
	MOV ER0, BP
	ADD ER0, ER6
	L R0, [ER0]
	BNE _$j_02908
	ADD R4, #-1H
	ADD R6, #-1H
_$j_02904:
	CMP ER6, ER10
	BGTS _$j_028f8
_$j_02908:
	MOV ER6, #0H
	MOV R0, R4
	MOV R1, #0H
	ADD ER0, #-1H
	CMP ER0, ER10
	BNE _$j_02948
	MOV ER2, ER10
	ADD R2, #1H
	BAL _$j_02926
_$j_0291a:
	MOV ER0, BP
	ADD ER0, ER6
	L R0, [ER0]
	ADD R0, R9
	ST R0, [EA+]
	ADD R6, #1H
_$j_02926:
	CMP R6, R2
	BLT _$j_0291a
	CMP R5, #2H
	BEQ _$j_02932
	CMP R5, #3H
	BNE _$j_0294c
_$j_02932:
	ST R8, [EA+]
	BAL _$j_0294c
_$j_02936:
	MOV ER0, BP
	ADD ER0, ER6
	L R0, [ER0]
	ADD R0, R9
	ST R0, [EA+]
	CMP ER10, ER6
	BNE _$j_02946
	ST R8, [EA+]
_$j_02946:
	ADD R6, #1H
_$j_02948:
	CMP R6, R4
	BLT _$j_02936
_$j_0294c:
	MOV R2, #0H
	ST R2, [EA]
	POP XR4
	POP QR8
	RT
_$j_02956:
	ST R9, [EA+]
	CMP R5, #0H
	BNE _$j_02972
	CMP R4, #1H
	BEQ _$j_0294c
	ST R8, [EA+]
	ADD R4, #-1H
	MOV R6, #0H
	BAL _$j_0296c
_$j_02968:
	ST R9, [EA+]
	ADD R6, #1H
_$j_0296c:
	CMP R6, R4
	BLT _$j_02968
	BAL _$j_0294c
_$j_02972:
	CMP R5, #2H
	BNE _$j_0297e
	ST R8, [EA+]
	ADD R4, #-1H
	MOV R6, #0H
	BAL _$j_0296c
_$j_0297e:
	CMP R5, #3H
	BNE _$j_0294c
	ST R8, [EA+]
	BAL _$j_0294c

; FUNCTION: GY454XE  Re 02986
; FUNCTION: GY455XE  Im 02B18
; FUNCTION: GY460XF  Im 027B2
_f_02986:
	PUSH LR
	PUSH XR12
	MOV FP, SP
	ADD SP, #-2AH
	PUSH ER8
	PUSH XR4
	MOV BP, ER2
	MOV ER8, ER0
	L R6, 8H[FP]
	MOV R7, #0H
	MOV R2, #0H
	ST R2, [BP]
	MOV ER2, ER0
	BNE _$j_029ac
_$j_029a2:
	POP XR4
	POP ER8
	MOV SP, FP
	POP XR12
	POP PC
_$j_029ac:
	MOV ER0, FP
	ADD ER0, #-0AH
	BL _num_cpy
	BL _num_to_str_std_lineo
	BEQ _$j_029dc
	CMP R6, #6H
	BEQ _$j_029c6
	MOV R0, #'E'
	MOV R1, #'R'
	MOV R2, #'O'
	BAL _$j_029cc
_$j_029c6:
	MOV R0, #0E4H  ; "Tiny" font
	MOV R1, #0E9H
	MOV R2, #0E7H
_$j_029cc:
	MOV R3, #0H
	LEA [BP]
	ST ER0, [EA+]  ; ER
	ST R1, [EA+]   ; R
	ST R2, [EA+]   ; O
	ST R1, [EA+]   ; R
	ST R3, [EA+]   ; [null]
	BAL _$j_029a2
_$j_029dc:
	MOV R2, #0AH
	ADD R2, #1H
	BL _f_1AFD8
	MOV ER4, ER6
	MOV ER2, FP
	ADD ER2, #-22H
	MOV ER0, FP
	ADD ER0, #-0AH
	BL _init_num_struct
	CMP R0, #0H
	BEQ _$j_029a2
	CMP R6, #6H
	BNE _$j_029fe
	MOV R0, #1H
	ST R0, -0CH[FP]
_$j_029fe:
	L R0, -13H[FP]
	BEQ _$j_02a06
	ADD R6, #-1H
	ADD ER4, #-1H
_$j_02a06:
	MOV R0, #1H
	ST R0, -0DH[FP]
	L ER0, -12H[FP]
	CMP R0, #0AH
	CMPC R1, #0H
	BGES _$j_02a28
	PUSH R2
	L R2, _result_template
	CMP R2, #14H
	POP R2
	BEQ _$j_02aa2
	CMP R0, #-2H
	CMPC R1, #-1H
	BLTS _$j_02a28
_$j_02a24:
	CMP ER0, ER4
	BLTS _$j_02a72
_$j_02a28:
	MOV ER2, FP
	ADD ER2, #-2AH
	MOV ER0, FP
	ADD ER0, #-22H
	BL _num_exp_to_str
	MOV ER0, #0H
	ST ER0, -10H[FP]
	MOV ER0, FP
	ADD ER0, #-2AH
	BL _smart_strlen
	MOV R4, R0
	L R0, -13H[FP]
	BEQ _$j_02a54
	MOV R0, R6
	SUB R0, R4
	CMP R0, #1H
	BGTS _$j_02a54
	MOV R0, R6
	SUB R0, R4
	BAL _$j_02a5a
_$j_02a54:
	MOV R0, R6
	SUB R0, R4
	ADD R0, #-1H
_$j_02a5a:
	ST R0, -0EH[FP]
	MOV ER2, BP
	MOV ER0, FP
	ADD ER0, #-22H
	BL _f_0285C
	MOV ER2, FP
	ADD ER2, #-2AH
	MOV ER0, BP
	BL _smart_strcat
	BAL _$j_029a2
_$j_02a72:
	ST ER0, -10H[FP]
	L ER0, -12H[FP]
	BPS _$j_02a80
	L R0, -12H[FP]
	ADD R0, R6
	ADD R0, #-1H
	BAL _$j_02a92
_$j_02a80:
	MOV ER0, ER6
	ADD ER0, #-2H
	L ER2, -10H[FP]
	CMP ER2, ER0
	BGTS _$j_02a90
	MOV R0, R6
	ADD R0, #-1H
	BAL _$j_02a92
_$j_02a90:
	MOV R0, R6
_$j_02a92:
	ST R0, -0EH[FP]
	MOV ER2, BP
	MOV ER0, FP
	ADD ER0, #-22H
	BL _f_0285C
	B _$j_029a2
_$j_02aa2:
	CMP R0, #-3H
	CMPC R1, #-1H
	BLTS _$j_02a28
	BAL _$j_02a24

; FUNCTION: GY454XE  Re 02AAA
; FUNCTION: GY455XE  Im 02C3C
; FUNCTION: GY460XF  Im 028D6
_get_result_disp_fmt:
	L R0, _result_format
	SRL R0, #4
	RT

; FUNCTION: GY454XE  Re 02AB2
; FUNCTION: GY455XE  Im 02C44
; FUNCTION: GY460XF  Im 028DE
_get_result_store_fmt:
	L R0, _result_format
	AND R0, #0FH
	RT

; FUNCTION: GY454XE  Re 02ABA
; FUNCTION: GY455XE  Im 02C4C
; FUNCTION: GY460XF  Im 028E6
_set_result_store2disp:
	L R1, _result_format
	AND R1, #0FH
	SLL R0, #4
	OR R1, R0
	ST R1, _result_format
	RT

; FUNCTION: GY454XE  Re 02ACA
; FUNCTION: GY455XE  Im 02C5C
; FUNCTION: GY460XF  Im 028F6
_set_result_store_fmt:
	L R1, _result_format
	AND R1, #0F0H
	OR R0, R1
	ST R0, _result_format
	MOV R0, #0H
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 02ADE
; FUNCTION: GY455XE  Im 02C70
; FUNCTION: GY460XF  Im 0290A
_set_result_fmt:
	ST R0, _result_format
	MOV R0, #0H
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 02AEA
; FUNCTION: GY455XE  Im 02C7C
; FUNCTION: GY460XF  Im 02916
_result_str_print:
	PUSH LR
	PUSH ER0
	PUSH R2
	BL _smart_strlen
	POP R2
	MOV R1, #6
	MUL ER0, R1
	CMP R2, #0
	BEQ _$j_02b02
	MOV R1, #22
	BAL _$j_02b04
_$j_02b02:
	MOV R1, #12
_$j_02b04:
	POP ER2
	NEG R0
	ADD R0, #96
	BL _line_print
	POP PC

; FUNCTION: GY454XE  Re 02B10
; FUNCTION: GY455XE  Im 02CA2
; FUNCTION: GY460XF  Im 0293C
_basen_base_print:
	PUSH LR
	L R0, _submode
	SRL R0, #2
	MOV R1, #0H
	SLLC R1, #2
	SLL R0, #2
	MOV R2, #BYTE1 _base_strs
	MOV R3, #BYTE2 _base_strs
	ADD ER2, ER0
	MOV R1, #12
	MOV R0, #78
	BL _line_print
	POP PC

; DATA: GY454XE  Re 02B2E
; DATA: GY455XE  Im 02CC0
; DATA: GY460XF  Im 0295A
_base_strs:
	DB "Bin", 0
	DB "Oct", 0
	DB "Dec", 0
	DB "Hex", 0

; FUNCTION: GY454XE  Re 02B3E
; FUNCTION: GY455XE  Im 02CD0
; FUNCTION: GY460XF  Im 0296A
_f_02B3E:
	LEA [ER0]
	L R3, [EA+]
	CMP R3, #31H
	BNE _$j_02b4e
	L R3, [EA+]
	BNE _$j_02b66
	ST R2, [ER0]
	RT
_$j_02b4e:
	CMP R3, #60H
	BNE _$j_02b62
	L R3, [EA+]
	CMP R3, #31H
	BNE _$j_02b62
	L R3, [EA+]
	BNE _$j_02b66
	ST R2, 1H[ER0]
	RT
_$j_02b62:
	CMP R3, #0H
	BEQ _$j_02b6a
_$j_02b66:
	L R3, [EA+]
	BNE _$j_02b4e
_$j_02b6a:
	ST R3, [EA]
	PUSH EA
	POP ER0
	ST R2, -1H[ER0]
	RT

; FUNCTION: GY454XE  Re 02B76
; FUNCTION: GY455XE  Im 02D08
; FUNCTION: GY460XF  Im 029A2
_get_token:
	PUSH ER4
	LEA [ER2]
	MOV R1, R0
	BEQ _$j_02bb8
	MOV R1, #0H
	MOV R4, #BYTE1 _tokens
	MOV R5, #BYTE2 _tokens
	ADD ER4, ER0
	ADD ER4, ER0
	L ER2, [ER4]
	MOV R4, #BYTE1 _token_lengths
	MOV R5, #BYTE2 _token_lengths
	ADD ER4, ER0
	L R0, [ER4]
	MOV R4, R0
	AND R0, #0FH
	SRL R4, #4
	MOV R1, R0
	CMP R4, #0FH
	BEQ _$j_02ba2
	MOV R5, #0H
	ADD ER2, ER4
_$j_02ba2:
	L R5, [ER2]
	NOP
	ST R5, [EA+]
	ADD ER2, #1H
	ADD R1, #-1H
	BNE _$j_02ba2
	CMP R4, #0FH
	BNE _$j_02bb8
	MOV R5, #28H
	ST R5, [EA+]
	ADD R0, #1H
_$j_02bb8:
	ST R1, [EA]
	POP ER4
	RT

; FUNCTION: GY454XE  Re 02BBE
; FUNCTION: GY455XE  Im 02D50
; FUNCTION: GY460XF  Im 029EA
_get_token_length:
	MOV R1, #0H
	MOV R2, #BYTE1 _token_lengths
	MOV R3, #BYTE2 _token_lengths
	ADD ER2, ER0
	L R0, [ER2]
	MOV R1, R0
	AND R0, #0FH
	CMP R1, #-10H
	BLT _$j_02bd2
	ADD R0, #1H
_$j_02bd2:
	RT

; FUNCTION: GY454XE  Re 02BD4
; FUNCTION: GY455XE  Im 02D66
; FUNCTION: GY460XF  Im 02A00
_print_continue_prompt:
	PUSH LR
	BL _buffer_clear
	MOV R2, #BYTE1 _s_continue_prompt
	MOV R3, #BYTE2 _s_continue_prompt
	MOV R1, #1H
	MOV R0, #0H
	BL _line_print
	POP PC

; FUNCTION: GY454XE  Re 02BE8
; FUNCTION: GY455XE  Im 02D7A
; FUNCTION: GY460XF  Im 02A14
_print_input_area:
	PUSH LR
	MOV R2, #BYTE1 _s_blank_line
	MOV R3, #BYTE2 _s_blank_line
	MOV R1, #2H
	MOV R0, #0H
	BL _line_print
	MOV R2, #BYTE1 _input_area
	MOV R3, #BYTE2 _input_area
	MOV R1, #1H
	MOV R0, #0H
	BL _line_print
	POP PC

; FUNCTION: GY454XE  Re 02C04
; FUNCTION: GY455XE  Im 02D96
; FUNCTION: GY460XF  Im 02A30
_num_output_print:
	PUSH LR
	PUSH FP
	MOV FP, SP
	ADD SP, #-1AH
	PUSH XR8
	MOV ER8, ER0
	L R0, _mode
	CMP R0, #2H  ; BASE-N mode
	BNE _$j_02c1c
	BL _basen_base_print
_$j_02c1c:
	MOV ER8, ER8
	BNE _$j_02c26
	MOV R0, #0H
	ST R0, -1AH[FP]
	BAL _$j_02c36
_$j_02c26:
	MOV R0, #0H
	PUSH R0
	MOV ER2, FP
	ADD ER2, #-1AH
	MOV ER0, ER8
	BL _num_to_str
	ADD SP, #2H
_$j_02c36:
	BL _set_result_store2disp
	L R0, _font_size
	CMP R0, #0AH
	BNE _$j_02c46
	MOV R10, #0AH
	BAL _$j_02c48
_$j_02c46:
	MOV R10, #7H
_$j_02c48:
	MOV R0, #32
	SUB R0, R10
	MOV R10, R0
	BL _buffer_clear_lastnline
	MOV ER0, FP
	ADD ER0, #-1AH
	BL _smart_strlen
	MOV R2, #6H
	MUL ER0, R2
	MOV R11, R0
	MOV ER2, FP
	ADD ER2, #-1AH
	MOV R1, R10
	MOV R0, #96
	SUB R0, R11
	BL _line_print
	POP XR8
	MOV SP, FP
	POP FP
	POP PC

; FUNCTION: GY454XE  Re 02C76
; FUNCTION: GY455XE  Im 02E08
; FUNCTION: GY460XF  Im 02AA2
_f_02C76:
	PUSH LR
	PUSH R8
	MOV R8, #1H
	MOV R0, #0H
	BL _is_eqn_result
	L R1, _mode
	CMP R1, #0C4H  ; CMPLX mode
	BEQ _$j_02c8e
	CMP R0, #0H
	BEQ _$j_02cb0
_$j_02c8e:
	CMP R0, #0H
	BEQ _$j_02c9a
	L R0, _submode
	CMP R0, #2H
	BLE _$j_02cb0
_$j_02c9a:
	L R8, _setup_cmplx_result
	L R0, _d_08101
	CMP R0, #1H
	BNE _$j_02caa
	MOV R8, #1H
	BAL _$j_02cb0
_$j_02caa:
	CMP R0, #2H
	BNE _$j_02cb0
	MOV R8, #0H
_$j_02cb0:
	MOV R0, R8
	POP R8
	POP PC

; FUNCTION: GY454XE  Re 02CB6
; FUNCTION: GY455XE  Im 02E48
; FUNCTION: GY460XF  Im 02AE2
_f_02CB6:
	PUSH ER8
	L R8, _table_mode
	L R9, _d_080FD
	TB R8.7
	BEQ _$j_02cce
	CMP R9, #2H
	BNE _$j_02cce
_$j_02cc8:
	MOV R0, #1H
_$j_02cca:
	POP ER8
	RT
_$j_02cce:
	CMP R8, #6H  ; TABLE_RANGE
	BNE _$j_02cdc
	CMP R9, #1H  ; return 1 <= [80FDH] <= 3 ? 1 : 0;
	BLT _$j_02cdc
	CMP R9, #3H
	BGT _$j_02cdc
	BAL _$j_02cc8
_$j_02cdc:
	MOV R0, #0H
	BAL _$j_02cca

; ==== POTENTIAL SPLIT ===
; Extra code found at this area in LY710XA. Needs more investigation.

; FUNCTION: GY454XE  Re 02CE0
; FUNCTION: GY455XE  Im 02E72
; FUNCTION: GY460XF  Im 02B0C
_buffer_clear_lastnline:
	PUSH LR
	MOV R2, #BYTE1 (32*12)
	MOV R3, #BYTE2 (32*12)
	MOV R1, #12
	MUL ER0, R1
	SUB R2, R0
	SUBC R3, R1
	PUSH ER2
	MOV R2, #BYTE1 _screen_buffer
	MOV R3, #BYTE2 _screen_buffer
	ADD ER0, ER2
	POP ER2
	BL _memzero
	POP PC

; FUNCTION: GY454XE  Re 02CFE
; FUNCTION: GY455XE  Im 02E90
; FUNCTION: GY460XF  Im 02B2A
_fill_screen:
	PUSH LR
	PUSH ER4
	MOV ER4, ER0
	TB R5.0
	BEQ _$j_02d1c
	MOV R0, #80H
	MOV R1, #1H
	PUSH ER0
	MOV R2, R4
	MOV R3, #0H
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	BL _memset_n
	POP ER0
_$j_02d1c:
	TB R5.1
	BEQ _$j_02d34
	MOV R0, #0H
	MOV R1, #2H
	PUSH ER0
	MOV R2, R4
	MOV R3, #0H
	MOV R0, #BYTE1 _real_screen
	MOV R1, #BYTE2 _real_screen
	BL _memset_n
	POP ER0
_$j_02d34:
	POP ER4
	POP PC

; FUNCTION: GY454XE  Re 02D38
; FUNCTION: GY455XE  Im 02ECA
; FUNCTION: GY460XF  Im 02B64
_buffer_clear:
	PUSH QR8
	MOV ER8, #0H
	MOV ER10, #0H
	MOV BP, #0H
	MOV FP, #0H
	LEA _screen_buffer
	MOV R0, #30H
_$j_02d48:
	ST QR8, [EA+]
	ADD R0, #-1H
	BNE _$j_02d48
	POP QR8
	RT

; FUNCTION: GY454XE  Re 02D52
; FUNCTION: GY455XE  Im 02EE4
; FUNCTION: GY460XF  Im 02B7E
_f_02D52:
	PUSH LR
	PUSH XR4
	MOV R4, #80H
	MOV R5, #1H
	MOV R6, #BYTE1 _screen_buffer
	MOV R7, #BYTE2 _screen_buffer
	ADD R0, #1H
	MOV R2, #0CH
	MUL ER2, R0
	MOV ER0, ER4
	SUB R0, R2
	SUBC R1, R3
	PUSH ER0
	PUSH ER0
	MOV ER0, #0CH
	ADD ER0, ER6
	ADD ER2, ER6
	BL _memmove_nn
	POP ER0
	POP ER0
	SUB R4, R0
	SUBC R5, R1
	ADD ER4, #-0CH
	MOV ER2, ER4
	ADD ER0, #0CH
	ADD ER0, ER6
	BL _memzero
	POP XR4
	POP PC

; Draws a vertical line and adds pixels on the top and bottom based on flags (R3/R11).
;     Bit 5 -> ### <- Bit 7
;               #
;               #
;               #
;               #
;               #
;     Bit 4 -> ### <- Bit 6
; FUNCTION: GY454XE  Re 02D90
; FUNCTION: GY455XE  Im 02F22
; FUNCTION: GY460XF  Im 02BBC
_draw_line_vert:
	PUSH LR
	PUSH XR8
	MOV ER8, ER0
	MOV ER10, ER2
	TB R11.7
	BEQ _$j_02da2
	ADD R0, #1H
	BL _plot_pixel_unsafe
_$j_02da2:
	TB R11.5
	BEQ _$j_02dae
	MOV ER0, ER8
	ADD R0, #-1H
	BL _plot_pixel_unsafe
_$j_02dae:
	MOV ER0, ER8
	BL _plot_pixel_unsafe
	CMP R9, R10
	BEQ _$j_02dbc
	ADD R9, #1H
	BAL _$j_02dae
_$j_02dbc:
	TB R11.6
	BEQ _$j_02dc8
	MOV ER0, ER8
	ADD R0, #1H
	BL _plot_pixel_unsafe
_$j_02dc8:
	TB R11.4
	BEQ _$j_02dd4
	MOV ER0, ER8
	ADD R0, #-1H
	BL _plot_pixel_unsafe
_$j_02dd4:
	POP XR8
	POP PC

; FUNCTION: GY454XE  Re 02DD8
; FUNCTION: GY455XE  Im 02F6A
; FUNCTION: GY460XF  Im 02C04
_draw_line:
	PUSH LR
	PUSH XR4
	PUSH QR8
	MOV FP, SP
	ADD SP, #-4H
	MOV ER6, ER0
	MOV R10, R2
	EXTBW ER10
	MOV R8, R0
	EXTBW ER8
	SUB R10, R8
	SUBC R11, R9
	MOV R12, R3
	EXTBW BP
	MOV R8, R1
	EXTBW ER8
	SUB R12, R8
	SUBC R13, R9
	BL _plot_pixel
	MOV R2, #1H
	MOV R3, #1H
	MOV ER8, #0H
	CMP ER10, ER8
	BGES _$j_02e12
	XOR R10, #11111111B
	XOR R11, #11111111B
	ADD ER10, #1H
	MOV R2, #-1H
_$j_02e12:
	CMP BP, ER8
	BGES _$j_02e1e
	XOR R12, #11111111B
	XOR R13, #11111111B
	ADD BP, #1H
	MOV R3, #-1H
_$j_02e1e:
	MOV ER0, #0H
	MOV ER8, ER10
	CMP BP, ER10
	BC LE, _$j_02e3a
	MOV R4, R7
	MOV R7, R6
	MOV R6, R4
	MOV ER8, BP
	MOV BP, ER10
	MOV ER10, ER8
	MOV R4, R2
	MOV R2, R3
	MOV R3, R4
	MOV R1, #1H
_$j_02e3a:
	ST ER0, -4H[FP]
	ST ER2, -2H[FP]
	MOV ER4, ER10
	SLLC R11, #1
	SLL R10, #1
	SLLC R13, #1
	SLL R12, #1
_$j_02e48:
	CMP R8, #0H
	BC EQ, _$j_02e70
	SUB R4, R12
	SUBC R5, R13
	ADD ER4, #0H
	BGES _$j_02e5a
	ADD ER4, ER10
	L R0, -1H[FP]
	ADD R7, R0
_$j_02e5a:
	L R0, -2H[FP]
	ADD R6, R0
	MOV ER0, ER6
	L R2, -3H[FP]
	BC EQ, _$j_02e68
	MOV R0, R7
	MOV R1, R6
_$j_02e68:
	BL _plot_pixel
	ADD R8, #-1H
	BC AL, _$j_02e48
_$j_02e70:
	MOV SP, FP
	POP QR8
	POP XR4
	POP PC

; FUNCTION: GY454XE  Re 02E78
; FUNCTION: GY455XE  Im 0300A
; FUNCTION: GY460XF  Im 02CA4
; FUNCTION: GY465XG  Im 02BE2
_line_print_col_0:
	MOV R0, #0H

; FUNCTION: GY454XE  Re 02E7A
; FUNCTION: GY455XE  Im 0300C
; FUNCTION: GY460XF  Im 02CA6
; FUNCTION: GY465XG  Im 02BE4
_line_print:
	PUSH LR
	PUSH QR8
	MOV ER8, ER0
	MOV ER10, ER2
	MOV R13, #4H
	MOV R14, #18H
	L R0, _font_size
	CMP R0, #6H
	BEQ _$j_02e92
	MOV R13, #6H
	MOV R14, #10H
_$j_02e92:
	MOV R15, #60H
	SUB R15, R13
	MOV R12, #0H
_$j_02e98:
	CMP R8, R15
	BGT _$j_02eb2
	MOV R0, #0H
	L R2, [ER10]
	BEQ _$j_02eb4
	MOV ER0, ER8
	BL _char_print
	ADD ER10, #1H
	ADD R8, R13
	ADD R12, #1H
	CMP R12, R14
	BLT _$j_02e98
_$j_02eb2:
	MOV R0, R12
_$j_02eb4:
	MOV ER2, ER10
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 02EBA
; FUNCTION: GY455XE  Im 0304C
; FUNCTION: GY460XF  Im 02CE6
_char_print:
	PUSH LR
	PUSH XR4
	PUSH QR8
	MOV FP, SP
	ADD SP, #-15H  ; Oddity: Odd number subtracted from SP
	CMP R0, #5FH
	BGT _$j_02fac
	CMP R1, #20H
	BGES _$j_02fac
	MOV R3, #0CH
	MOV R4, #BYTE1 (_screen_buffer+12*32-1)
	MOV R5, #BYTE2 (_screen_buffer+12*32-1)
	L R7, _use_rambuf
	BNE _$j_02ede
	MOV R3, #10H
	MOV R4, #BYTE1 _real_screen_end
	MOV R5, #BYTE2 _real_screen_end
_$j_02ede:
	ST R3, -1H[FP]
	ST ER4, -4H[FP]
	L R3, _font_size
	MOV R4, #4H
	MOV R5, #4H
	CMP R3, #6H
	BEQ _$j_02ef2
	MOV R4, #6H
	MOV R5, #2H
_$j_02ef2:
	ST ER4, -6H[FP]
	CMP R3, #7H
	BNE _$j_02f00
	CMP R2, #20H
	BGE _$j_02f00
	ADD R3, #1H
	ADD R1, #-1H
_$j_02f00:
	PUSH R3
	MOV ER6, ER0
	MOV R0, R2
	MOV ER2, FP
	ADD ER2, #-15H
	BL _copy_char_data
	ST ER0, -0CH[FP]
	MOV R12, #BYTE1 _font_big_0
	MOV R13, #BYTE2 _font_big_0
	MOV ER0, ER6
	MOV ER2, FP
	ADD ER2, #-8H
	BL _get_screen_addr
	L ER10, -8H[FP]
	MOV R9, R0
	MOV R1, #0H
	L R7, _bitmasks_x[ER0]
	ADD R4, R6
	MOV R5, #0H
	MOV R0, R4
	AND R0, #00000111B
	L R6, _bitmasks_y[ER0]
	POP R0
	ADD R0, #-1H
	ST ER0, -0AH[FP]
_$j_02f3a:
	CMP R9, #0H
	BNE _$j_02f4c
	MOV ER0, ER10
	MOV ER2, BP
	PUSH R6
	BL _draw_byte
	POP R0
	BAL _$j_02f90
_$j_02f4c:
	L R0, -5H[FP]
	CMP R9, R0
	BGT _$j_02f56
	MOV R0, R7
	AND R0, R6
_$j_02f56:
	L R8, [BP]
	SRL R8, R9
	AND R8, R7
	MOV ER0, ER10
	ST R8, -2H[FP]
	MOV ER2, FP
	ADD ER2, #-2H
	PUSH R7
	BL _draw_byte
	POP R0
	L R0, -5H[FP]
	ADD R0, #1H
	CMP R9, R0
	BLT _$j_02f90
	L R8, [BP]
	MOV R1, #8H
	SUB R1, R9
	SLL R8, R1
	AND R8, R6
	MOV ER0, ER10
	ADD ER0, #1H
	ST R8, -2H[FP]
	MOV ER2, FP
	ADD ER2, #-2H
	PUSH R6
	BL _draw_byte
	POP R0
_$j_02f90:
	L R0, -1H[FP]
	ADD R10, R0
	ADDC R11, #0H
	L ER0, -4H[FP]
	CMP ER10, ER0
	BGT _$j_02fac
	L ER0, -0AH[FP]
	CMP R1, #0H
	BEQ _$j_02fb4
	ADD BP, #1H
_$j_02fa4:
	ADD R1, #1H
	ST ER0, -0AH[FP]
	CMP R1, R0
	BLE _$j_02f3a
_$j_02fac:
	MOV SP, FP
	POP QR8
	POP XR4
	POP PC
_$j_02fb4:
	L BP, -0CH[FP]
	BAL _$j_02fa4

; FUNCTION: GY454XE  Re 02FB8
; FUNCTION: GY455XE  Im 0314A
; FUNCTION: GY460XF  Im 02DE4
_plot_pixel:
	CMP R0, #0
	BLTS _$j_02fea
	CMP R0, #95
	BGTS _$j_02fea
	CMP R1, #0
	BLTS _$j_02fea
	CMP R1, #31
	BGTS _$j_02fea

; FUNCTION: GY454XE  Re 02FC8
; FUNCTION: GY455XE  Im 0315A
; FUNCTION: GY460XF  Im 02DF4
_plot_pixel_unsafe:
	PUSH ER4
	MOV R3, #10000000B
	SRL R3, R0
	MOV R4, R1
	MOV R1, #0H
	SRL R0, #3
	MOV R2, #12
	MUL ER4, R2
	ADD ER4, ER0
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	ADD ER0, ER4
	LEA [ER0]
	L R0, [EA]
	OR R0, R3
	ST R0, [EA]
	POP ER4
_$j_02fea:
	RT

; FUNCTION: GY454XE  Re 02FEC
; FUNCTION: GY455XE  Im 0317E
; FUNCTION: GY460XF  Im 02E18
_copy_char_data:
	PUSH ER8
	L R1, _font_size
	CMP R1, #6H
	BEQ _$j_03084
	CMP R1, #7H
	BEQ _$j_0300c
	LEA _font_big_0
	MOV R8, #9H
	CMP R0, #20H
	BGE _$j_0301e
	LEA _font_big_1
	ADD R0, #1FH
	BAL _$j_0301e
_$j_0300c:
	LEA _font_small_0
	MOV R8, #6H
	CMP R0, #20H
	BGE _$j_0301e
	LEA _font_small_1
	ADD R0, #1FH
	ADD R8, #1H
_$j_0301e:
	PUSH ER2
	PUSH ER4
	CMP R0, #7CH
	BNE _$j_03034
	MOV R1, R8
	MOV R4, #11000000B  ; Hardcode character 0x7C
_$j_0302a:
	ST R4, [ER2]
	ADD ER2, #1H
	ADD R1, #-1H
	BNE _$j_0302a
	BAL _$j_030b4
_$j_03034:
	PUSH ER6
	PUSH ER2
	ADD R0, #-20H
	MOV R4, #5H
	MUL ER0, R4
	MOV R4, #8H
	DIV ER0, R4
	MOV R5, R8
	MUL ER0, R5
	PUSH EA
	POP ER2
	ADD ER0, ER2
	PUSH ER0
	POP EA
	POP ER2
_$j_03052:
	MOV R1, #8H
	L R0, [EA+]
	SLL R0, R4
	SRL R0, #1
	SUB R1, R4
	CMP R1, #5H
	BGE _$j_03074
	PUSH EA
	POP ER6
	MOV R9, #0H
	ADD ER6, ER8
	ADD ER6, #-1H
	L R1, [ER6]
	MOV R6, #9H
	SUB R6, R4
	SRL R1, R6
	OR R0, R1
_$j_03074:
	AND R0, #01111100B
	ST R0, [ER2]
	ADD ER2, #1H
	INC [EA]            ; Oddity: Random ROM address increment?
	ADD R5, #-1H
	BNE _$j_03052
	POP ER6
	BAL _$j_030b4
_$j_03084:
	LEA _font_tiny
	PUSH ER2
	PUSH ER4
	ADD R0, #30H
	MOV R8, R0
	SRL R0, #1
	MOV R9, #5H
	MUL ER0, R9
	PUSH EA
	POP ER4
	ADD ER0, ER4
	PUSH ER0
	POP EA
	MOV R9, #5H
_$j_030a2:
	L R0, [EA+]
	TB R8.0
	BEQ _$j_030aa
	SLL R0, #4
_$j_030aa:
	AND R0, #11110000B
	ST R0, [ER2]
	ADD R2, #1H
	ADD R9, #-1H
	BNE _$j_030a2
_$j_030b4:
	POP ER4
	POP ER2
	POP ER8
	MOV ER0, ER2
	RT

; FUNCTION: GY454XE  Re 030BE
; FUNCTION: GY455XE  Im 03250
; FUNCTION: GY460XF  Im 02EEA
_draw_byte:
	PUSH FP
	MOV R14, #BYTE1 _screen_buffer_y1_neg
	MOV R15, #BYTE2 _screen_buffer_y1_neg
	ADD FP, ER0
	BGE _$j_030e8
	MOV FP, SP
	LEA [ER0]
	L R0, [ER2]
	L R1, 2H[FP]
	BEQ _$j_030d4
	AND R0, R1
_$j_030d4:
	MOV FP, ER0
	L R0, _draw_mode
	BNE _$j_030ec
	MOV R0, R15
	XOR R0, #-1H
	L R1, [EA]
	AND R0, R1
	OR R0, R14
	ST R0, [EA]
_$j_030e8:
	POP FP
	RT
_$j_030ec:
	CMP R0, #4H
	BNE _$j_03106
	MOV R0, R15
	MOV R2, R0
	XOR R0, #-1H
	L R1, [EA]
	AND R0, R1
	MOV R1, R14
	XOR R1, #-1H
	AND R1, R2
	OR R1, R0
	ST R1, [EA]
	BAL _$j_030e8
_$j_03106:
	CMP R0, #1H
	BNE _$j_03112
	L R0, [EA]
	OR R0, R14
	ST R0, [EA]
	BAL _$j_030e8
_$j_03112:
	CMP R0, #2H
	BNE _$j_03124
	MOV R0, R15
	XOR R0, #-1H
	OR R0, R14
	L R1, [EA]
	AND R0, R1
	ST R0, [EA]
	BAL _$j_030e8
_$j_03124:
	L R0, [EA]
	XOR R0, R14
	ST R0, [EA]
	BAL _$j_030e8

; FUNCTION: GY454XE  Re 0312C
; FUNCTION: GY455XE  Im 032BE
; FUNCTION: GY460XF  Im 02F58
_render:
	PUSH XR4
	PUSH QR8
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	PUSH ER0
	MOV R2, #BYTE1 _real_screen
	MOV R3, #BYTE2 _real_screen
	MOV R0, #20H
_$j_0313c:
	POP EA
	L XR4, [EA+]
	L QR8, [EA+]
	PUSH EA
	LEA [ER2]
	ST XR4, [EA+]
	ST QR8, [EA+]
	ADD ER2, #10H
	ADD R0, #-1H
	BNE _$j_0313c
	POP ER0
	POP QR8
	POP XR4
	RT

; FUNCTION: GY454XE  Re 03158
; FUNCTION: GY455XE  Im 032EA
; FUNCTION: GY460XF  Im 02F84
_get_screen_addr:
	PUSH XR8
	MOV R8, #0CH
	MOV R10, #BYTE1 _screen_buffer
	MOV R11, #BYTE2 _screen_buffer
	L R9, _use_rambuf
	BNE _$j_0316c
	MOV R8, #10H
	MOV R10, #BYTE1 _real_screen
	MOV R11, #BYTE2 _real_screen
_$j_0316c:
	LEA [ER2]
	TB R1.7
	BNE _$j_03188
	MUL ER8, R1
	ADD ER10, ER8
_$j_03176:
	MOV R1, #0H
	MOV R2, R0
	AND R2, #7H
	SRL R0, #3
	ADD ER10, ER0
	ST ER10, [EA]
	MOV R0, R2
	POP XR8
	RT
_$j_03188:
	NEG R1
	MUL ER8, R1
	SUB R10, R8
	SUBC R11, R9
	BAL _$j_03176

; FUNCTION: GY454XE  Re 03192
; FUNCTION: GY455XE  Im 03324
; FUNCTION: GY460XF  Im 02FBE
_setup_status_bar:
	PUSH LR
	MOV ER2, #12
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	BL _memzero
	TB _modifiers.3  ; SHIFT
	BEQ _$j_031a8
	SB _screen_buffer.4  ; [S]
_$j_031a8:
	TB _modifiers.2  ; ALPHA
	BEQ _$j_031b2
	SB _screen_buffer.2  ; [A]
_$j_031b2:
	TB _modifiers.0
	BEQ _$j_031bc
	SB (_screen_buffer+1).1  ; STO
_$j_031bc:
	TB _modifiers.1
	BEQ _$j_031c6
	SB (_screen_buffer+2).6  ; RCL
_$j_031c6:
	MOV R0, #BYTE1 _var_m
	MOV R1, #BYTE2 _var_m
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_031d6
	SB (_screen_buffer+1).4  ; M
_$j_031d6:
	L R0, _mode
	CMP R0, #0C4H  ; CMPLX mode
	BNE _$j_031f2
	SB (_screen_buffer+4).7  ; CMPLX
	MOV R0, #BYTE1 (_mode_ram+362)
	MOV R1, #BYTE2 (_mode_ram+362)
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_031f2
	SB (_screen_buffer+1).4  ; M
_$j_031f2:
	L R0, _mode
	CMP R0, #3H  ; STAT mode
	BNE _$j_031fe
	SB (_screen_buffer+3).6  ; STAT
_$j_031fe:
	CMP R0, #6H  ; MATRIX mode
	BNE _$j_03206
	SB (_screen_buffer+5).6  ; MAT
_$j_03206:
	CMP R0, #7H  ; VECTOR mode
	BNE _$j_0320e
	SB (_screen_buffer+5).1  ; VCT
_$j_0320e:
	L R0, _mode
	CMP R0, #2H  ; Skip if in BASE-N mode
	BEQ _$j_03246
	L R0, _setup_angle_unit
	CMP R0, #4H  ; Degree
	BNE _$j_03222
	SB (_screen_buffer+7).5  ; [D]
_$j_03222:
	CMP R0, #5H  ; Radian
	BNE _$j_0322a
	SB (_screen_buffer+7).1  ; [R]
_$j_0322a:
	CMP R0, #6H  ; Gradian
	BNE _$j_03232
	SB (_screen_buffer+8).4  ; [G]
_$j_03232:
	L R0, _setup_num_fmt
	CMP R0, #8H  ; Fix
	BNE _$j_0323e
	SB (_screen_buffer+8).0  ; FIX
_$j_0323e:
	CMP R0, #9H  ; Sci
	BNE _$j_03246
	SB (_screen_buffer+9).5  ; SCI
_$j_03246:
	L R0, _setup_mathi
	BEQ _$j_03258
	L R0, _mode
	AND R0, #11000000B  ; If current mode has MathIO enabled
	BEQ _$j_03258
	SB (_screen_buffer+10).6  ; Math
_$j_03258:
	TB _d_080FE.6
	BEQ _$j_03262
	SB (_screen_buffer+11).4  ; Disp
_$j_03262:
	TB _d_080FE.7
	BNE _$j_03290
	TB _arrow_state.0
	BEQ _$j_03272
	SB (_screen_buffer+11).7  ; ^
_$j_03272:
	TB _arrow_state.1
	BEQ _$j_0327c
	SB (_screen_buffer+10).3  ; v
_$j_0327c:
	TB _arrow_state.2
	BEQ _$j_03286
	SB _screen_buffer.6  ; Unused (Left?)
_$j_03286:
	TB _arrow_state.3
	BEQ _$j_03290
	SB (_screen_buffer+11).1  ; Unused (Right?)
_$j_03290:
	MOV ER0, #12
	PUSH ER0
	MOV R2, #BYTE1 _screen_buffer
	MOV R3, #BYTE2 _screen_buffer
	MOV R0, #BYTE1 _real_screen
	MOV R1, #BYTE2 _real_screen
	BL _memcpy_nn  ; memcpy to real screen
	POP ER0
	POP PC

; FUNCTION: GY454XE  Re 032A4
; FUNCTION: GY455XE  Im 03436
; FUNCTION: GY460XF  Im 030D0
_f_032A4:
	PUSH LR
	PUSH ER6
	PUSH QR8
	MOV FP, SP
	L ER8, 0EH[FP]
	MOV FP, ER0
	MOV ER6, ER2
	MOV R10, #1H
_$j_032b4:
	MOV R11, #1H
	MOV R12, R15
_$j_032b8:
	MOV R0, R10
	ADD R0, R0
	ADD R0, R10
	ADD R0, #-3H
	MOV R1, #0H
	ADD R0, R11
	ADD R0, #-1H
	ADD R0, R0
	ADD ER0, ER8
	L ER0, [ER0]
	MOV R2, #0H
	CMP ER10, ER6
	BNE _$j_032d4
	MOV R2, #1H
_$j_032d4:
	PUSH R2
	MOV ER2, ER10
	BL _f_032F0
	POP R2
	ADD R11, #1H
	ADD R12, #-1H
	BNE _$j_032b8
	ADD R10, #1H
	ADD R14, #-1H
	BNE _$j_032b4
	POP QR8
	POP ER6
	POP PC

; FUNCTION: GY454XE  Re 032F0
; FUNCTION: GY455XE  Im 03482
; FUNCTION: GY460XF  Im 0311C
_f_032F0:
	PUSH LR
	PUSH XR12
	MOV FP, SP
	ADD SP, #-10H
	MOV BP, FP
	ADD BP, #-0AH
	PUSH ER2
	MOV R2, #0ECH
	MOV R3, #0H
	ST ER2, -0AH[FP]
	MOV ER0, ER0
	BEQ _$j_03314
	MOV R2, #6H
	PUSH R2
	MOV ER2, BP
	BL _f_02986
	POP R2
_$j_03314:
	MOV R0, #-14H
	MOV R1, R0
	LEA -6H[BP]
	ST ER0, [EA+]
	ST ER0, [EA+]
	ST ER0, [EA+]
	MOV ER0, BP
	BL _strlen_n
	MOV ER2, #6H
	SUB R2, R0
	SUB R12, R2
	SUBC R13, R3
	MOV R0, #6H
	ST R0, _font_size
	L R0, 8H[FP]
	BEQ _$j_03340
	MOV R0, #4H
	ST R0, _draw_mode
_$j_03340:
	POP ER2
	MOV R1, #1CH
	MOV R0, R3
	ADD R0, #-1H
	MUL ER0, R1
	ADD R0, #0CH
	ADD R2, #-1H
	MOV R1, #7H
	ADD R2, R2
	ADD R1, R2
	ADD R2, R2
	ADD R1, R2
	MOV ER2, BP
	BL _line_print
	MOV R0, #0H
	ST R0, _draw_mode
	MOV SP, FP
	POP XR12
	POP PC

; FUNCTION: GY454XE  Re 0336A
; FUNCTION: GY455XE  Im 034FC
; FUNCTION: GY460XF  Im 03196
_f_0336A:
	PUSH LR
	PUSH QR8
	ADD SP, #-6H
	MOV FP, SP
	MOV R1, #6H
	ST R1, _font_size
	MOV R13, #3H
	MOV R12, R0
	MOV R11, #7H
	MOV R10, #0H
	MOV R8, #0ECH
_$j_03382:
	MOV ER2, FP
	MOV R0, R12
	BL _f_033AC
	CMP R12, #9H
	BGTS _$j_03394
	LEA [FP]
	L R9, [EA]
	ST XR8, [EA]
_$j_03394:
	MOV R0, #1H
	MOV R1, R11
	MOV ER2, FP
	BL _line_print
	ADD R11, #6H
	ADD R12, #1H
	ADD R13, #-1H
	BNE _$j_03382
	ADD SP, #6H
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 033AC
; FUNCTION: GY455XE  Im 0353E
; FUNCTION: GY460XF  Im 031D8
_f_033AC:
	PUSH R4
	LEA [ER2]
	CMP R0, #0H
	BGES _$j_033ba
	NEG R0
	MOV R1, #0DBH
	ST R1, [EA+]
_$j_033ba:
	MOV R4, #0H
	MOV R2, #100
	MOV R1, #0H
	DIV ER0, R2
	BEQ _$j_033ca
	ADD R0, #-30H
	ST R0, [EA+]
	SB R4.0
_$j_033ca:
	MOV R0, #0AH
	MOV R3, #0H
	DIV ER2, R0
	BEQ _$j_033e0
	ADD R2, #-30H
	ST R2, [EA+]
_$j_033d6:
	ADD R0, #-30H
	ST R0, [EA+]
	ST R3, [EA+]
	POP R4
	RT
_$j_033e0:
	TB R4.0
	BEQ _$j_033d6
	MOV R1, #-30H
	ST R1, [EA+]
	BAL _$j_033d6

; FUNCTION: GY454XE  Re 033EA
; FUNCTION: GY455XE  Im 0357C
; FUNCTION: GY460XF  Im 03216
_print_4lines_4str:
	PUSH LR
	PUSH QR8
	MOV FP, SP
	L ER10, 0CH[FP]
	L BP, 0EH[FP]
	PUSH BP
	PUSH ER10
	PUSH ER2
	PUSH ER0
	BL _f_0345E
	MOV R10, #1H
_$j_03402:
	POP ER2
	MOV R1, R10
	BL _line_print_col_0
	ADD R10, #8H
	CMP R10, #19H
	BLE _$j_03402
_$j_03410:
	MOV R0, #1H
	ST R0, _use_rambuf
	BL _set_scr_normal
	POP QR8
	POP PC

; FUNCTION: GY454XE  Re 0341E
; FUNCTION: GY455XE  Im 035B0
; FUNCTION: GY460XF  Im 0324A
_print_4lines_head:
	PUSH LR
	PUSH QR8
	MOV ER8, ER0
	MOV ER10, ER2
	BL _f_0345E
	MOV ER2, ER8
	MOV R1, #1H
	BL _line_print_col_0
	MOV ER2, ER10
	BEQ _$j_03410
	MOV R10, #9H
	MOV R11, #3H
	BAL _$j_0344e

; FUNCTION: GY454XE  Re 0343C
; FUNCTION: GY455XE  Im 035CE
; FUNCTION: GY460XF  Im 03268
_print_4lines:
	MOV R2, #4H
	PUSH LR
	PUSH QR8
	MOV R11, R2
	MOV ER8, ER0
	BL _f_0345E
	MOV ER2, ER8
	MOV R10, #1H
_$j_0344e:
	MOV R1, R10
	BL _line_print_col_0
	ADD ER2, #1H
	ADD R10, #8H
	ADD R11, #-1H
	BNE _$j_0344e
	BAL _$j_03410

; FUNCTION: GY454XE  Re 0345E
; FUNCTION: GY455XE  Im 035F0
; FUNCTION: GY460XF  Im 0328A
_f_0345E:
	PUSH LR
	MOV R0, #7H
	ST R0, _font_size
	MOV R0, #0H
	ST R0, _use_rambuf
	BL _set_scr_calculating
	MOV R0, #0H
	MOV R1, #2H
	BL _fill_screen
	POP PC

; FUNCTION: GY454XE  Re 0347A
; FUNCTION: GY455XE  Im 0360C
; FUNCTION: GY460XF  Im 032A6
_set_up_arrow:
	SB _real_screen_11.7
	RT

; FUNCTION: GY454XE  Re 03480
; FUNCTION: GY455XE  Im 03612
; FUNCTION: GY460XF  Im 032AC
_set_down_arrow:
	SB _real_screen_10.3
	RT

; FUNCTION: GY454XE  Re 03486
; FUNCTION: GY455XE  Im 03618
; FUNCTION: GY460XF  Im 032B2
_pd_value:
IF REAL == 1
	PUSH LR
	PUSH QR8
	MOV R0, #0H				; Disable keyboard interrupts
	ST R0, KIMASK
	MOV R0, #0H
	ST R0, KICON
	MOV ER10, #0H			; Pull all KO pins low...
	ST R10, KOD0
	ST R11, KOD1
	MOV R0, #01111111B
	MOV R1, #11111100B
	ST R0, KOMASK0			; Then lock all KO bits except for KO.7, KO.8, KO.9
	ST R1, KOMASK1
	MOV R15, #0H
	MOV R8, #3H
	MOV R11, #00000010B		; We'll start with KO.9 first
	MOV R10, #00000000B
_$j_034b4:
	MOV R12, #0H
	MOV R13, #3H
	MOV R14, #0H
	ST R10, KOD0
	ST R11, KOD1
	NOP
_$j_034c4:
	L R0, KID		; If KI8 is currently pulled low, add 1 to R12
	TB R0.7
	BNE _$j_034ce
	ADD R12, #1H
_$j_034ce:
	ADD R13, #-1H	; Repeat 3 times
	BNE _$j_034c4
	CMP R12, #2H	; If there are at least 2 KO pins that pulled KI8 low...
	BLT _$j_034d8
	MOV R14, #1H	; Set R14 to 1
_$j_034d8:
	MOV R0, R8
	ADD R0, #-1H
	SLL R14, R0		; Set corresponding bit of R15
	ADD R15, R14
	MOV ER0, #0H	; Pull all KO pins low again
	ST R0, KOD0
	ST R1, KOD1
	MOV R12, #3H
	MOV R13, #-1H
_$j_034ee:
	L R0, KID		; Check if KI8 is currently pulled low for at least 3 ticks
	TB R0.7
	BEQ _$j_034fc
	ADD R12, #-1H
	BNE _$j_034ee
	BAL _$j_03504
_$j_034fc:
	MOV R12, #3H	; If KI8 is high we repeat until R13 is 0 (255 times)
	ADD R13, #-1H
	BNE _$j_034ee
	BAL _$j_03504
_$j_03504:
	ADD R8, #-1H	; If we've gone through all 3 KO pins then return R15
	BEQ _$j_0350e
	SRLC R10, #1	; If not, we shift the KO value to check for the next pin
	SRL R11, #1
	BAL _$j_034b4
_$j_0350e:
	BL _set_ko1_ko9
	MOV R0, R15
	POP QR8
	POP PC
ELSE
	PUSH ER2
	MOV R3, #BYTE2 _pd_val_emu  ; These are probably the P pins on ES boards
	MOV R2, #BYTE1 _pd_val_emu
	L R0, [ER2]
	POP ER2
	RT
ENDIF

; FUNCTION: GY454XE  Re 03518
; FUNCTION: GY455XE  Im 03624
; FUNCTION: GY460XF  Im 032BE
_set_ko1_ko9:
	PUSH ER0
	MOV R0, #10000000B
	MOV R1, #11111111B
	ST R0, KOMASK0
	ST R1, KOMASK1
	POP ER0
	RT

; FUNCTION: GY454XE  Re 0352A
; FUNCTION: GY455XE  Im 03636
; FUNCTION: GY460XF  Im 032D0
_print_error:
	PUSH LR
	MOV R1, #7H
	ST R1, _font_size
	MOV R2, #BYTE1 _s_prompt_goto
	MOV R3, #BYTE2 _s_prompt_goto
	PUSH ER2
	MOV R2, #BYTE1 _s_prompt_cancel
	MOV R3, #BYTE2 _s_prompt_cancel
	PUSH ER2
	ADD R0, #-1H
	MOV R1, #0H
	ADD ER0, ER0
	MOV R2, #BYTE1 _errors
	MOV R3, #BYTE2 _errors
	ADD ER2, ER0
	L ER0, [ER2]
	MOV R2, #BYTE1 _s_blank_line
	MOV R3, #BYTE2 _s_blank_line
	BL _print_4lines_4str
	ADD SP, #4H
	POP PC

; FUNCTION: GY454XE  Re 03558
; FUNCTION: GY455XE  Im 03664
; FUNCTION: GY460XF  Im 032FE
_f_03558:
	PUSH LR
	MOV ER0, #1H
	ST R0, _table_mode
	ST R1, _d_080F4
	MOV R2, #0E6H  ; AC key
	ST R2, _last_key_keycode
	MOV R3, #1H
	ST R3, _force_nochar
	ST R1, _d_08126
	ST R1, _arrow_state
	L R2, _modifiers
	AND R2, #10000000B
	ST R2, _modifiers
	MOV R0, #0H
	ST R0, _draw_mode
	MOV R0, #1H
	ST R0, _use_rambuf
	BL _pd_value
	ST R0, _d_080DC
	MOV R0, #0H
	ST R0, _screen_state
	L R0, _setup_contrast
	BL _clr_port0
	BL _f_0AF0A
	MOV R0, #2H
	BL _f_0B8B8
	BL _f_0B05A
	BL _render
	POP PC

; FUNCTION: GY454XE  Re 035B8
; FUNCTION: GY455XE  Im 036C4
; FUNCTION: GY460XF  Im 0335E
_is_char_keycode:
	MOV R1, R0
	BEQ _$j_035ce
	L R0, _force_nochar
	BEQ _$j_035ca
	CMP R1, #1FH   ; <= K_STO_M
	BLE _$j_035ce
	CMP R1, #0DFH  ; > K_SUM
	BGT _$j_035ce
_$j_035ca:
	MOV R0, #1H
	RT
_$j_035ce:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 035D2
; FUNCTION: GY455XE  Im 036DE
; FUNCTION: GY460XF  Im 03378
_is_format_keycode:
	MOV R1, R0
	BEQ _$j_035ce
	L R0, _force_nochar
	BEQ _$j_035ce
	CMP R1, #0F1H  ; If keycode == K_FACT, return 1
	BEQ _$j_035ca
	CMP R1, #0F6H  ; If K_DMS <= keycode <= K_FMT_FRAC (display format keycodes), return 1
	BLT _$j_035e8
	CMP R1, #0FBH
	BLE _$j_035ca
_$j_035e8:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 035EC
; FUNCTION: GY455XE  Im 036F8
; FUNCTION: GY460XF  Im 03392
_f_035EC:
	PUSH LR
	MOV ER2, ER0
	MOV R1, #0H
	ST R1, [ER2]
	L R1, _mode
	CMP R1, #6H   ; MATRIX
	BEQ _$j_03622
	CMP R1, #7H   ; VECTOR
	BEQ _$j_03622
	CMP R1, #45H  ; EQN
	BEQ _$j_03648
	CMP R1, #4BH  ; INEQ
	BEQ _$j_03648
	CMP R1, #4AH  ; RATIO
	BEQ _$j_03648
	CMP R1, #88H  ; TABLE
	BNE _$j_03618
	L R0, _table_mode
	CMP R0, #12H  ; TABLE_STAT_TABLE
	BEQ _$j_03652
_$j_03618:
	TB _d_080FE.6
	BNE _$j_03652
_$j_0361e:
	MOV R0, #1H
	POP PC
_$j_03622:
	L R0, _d_080FE
	CMP R0, #3H
	BEQ _$j_0362e
	CMP R0, #0H
	BNE _$j_03618
_$j_0362e:
	L R0, _table_mode
	CMP R0, #1H
	BEQ _$j_0361e
	L R0, _submode
	CMP R0, #3H
	BNE _$j_03618
	ST R1, [ER2]
	BL _f_0AF16
	MOV R0, #2H
	POP PC
_$j_03648:
	L R0, _table_mode
	CMP R0, #1H
	BEQ _$j_03652
	BAL _$j_0361e
_$j_03652:
	MOV R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 03656
; FUNCTION: GY455XE  Im 03762
; FUNCTION: GY460XF  Im 033FC
_is_ac_keycode:
	CMP R0, #0E6H  ; K_AC
	BEQ _$j_0365c
	BAL _is_mov_x_keycode
_$j_0365c:
	MOV R0, #1H
	RT

; FUNCTION: GY454XE  Re 03660
; FUNCTION: GY455XE  Im 0376C
; FUNCTION: GY460XF  Im 03406
_f_03660:
	MOV R1, #3H
	BAL _$j_03666

; FUNCTION: GY454XE  Re 03664
; FUNCTION: GY455XE  Im 03770
; FUNCTION: GY460XF  Im 0340A
_f_03664:
	MOV R1, #1H
_$j_03666:
	L R0, _d_080FE
	AND R0, #0FH
	CMP R0, R1
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 03672
; FUNCTION: GY455XE  Im 0377E
; FUNCTION: GY460XF  Im 03418
_is_mov_keycode:
	L R1, _force_nochar
	BEQ _$j_03684
	CMP R0, #0E0H  ; >= K_UP
	BLT _$j_03684
	CMP R0, #0E3H  ; <= K_LEFT
	BGT _$j_03684
_$j_03680:
	MOV R0, #1H
	RT
_$j_03684:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 03688
; FUNCTION: GY455XE  Im 03794
; FUNCTION: GY460XF  Im 0342E
_is_mov_y_keycode:
	L R1, _force_nochar
	BEQ _$j_03684
	CMP R0, #0E0H  ; K_UP
	BEQ _$j_03680
	CMP R0, #0E1H  ; K_DOWN
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 03698
; FUNCTION: GY455XE  Im 037A4
; FUNCTION: GY460XF  Im 0343E
_is_mov_x_keycode:
	L R1, _force_nochar
	BEQ _$j_03684
	CMP R0, #0E3H  ; K_LEFT
	BEQ _$j_03680
	CMP R0, #0E2H  ; K_RIGHT
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 036A8
; FUNCTION: GY455XE  Im 037B4
; FUNCTION: GY460XF  Im 0344E
_is_sto_abc_keycode:
	L R1, _force_nochar
	BEQ _$j_03684
	CMP R0, #17H  ; >= K_STO_A
	BLT _$j_03684
	CMP R0, #19H  ; <= K_STO_C
	BGT _$j_03684
	BAL _$j_03680

; FUNCTION: GY454XE  Re 036B8
; FUNCTION: GY455XE  Im 037C4
; FUNCTION: GY460XF  Im 0345E
_is_func_table:
	L R0, _mode
	CMP R0, #88H  ; TABLE mode
	BNE _$j_03684
	L R0, _table_mode
	CMP R0, #12H  ; TABLE_STAT_TABLE
	BNE _$j_03684
	BAL _$j_03680

; FUNCTION: GY454XE  Re 036CA
; FUNCTION: GY455XE  Im 037D6
; FUNCTION: GY460XF  Im 03470
_filter_chars_stat_mat_vct:
	L R1, _table_mode
	CMP R1, #1H
	BNE _$j_036f6
	L R1, _mode
	CMP R1, #3H  ; STAT mode
	BEQ _$j_036e2
	CMP R1, #6H  ; MATRIX mode
	BEQ _$j_036e2
	CMP R1, #7H  ; VECTOR mode
	BNE _$j_036f6
_$j_036e2:
	LEA _blacklist_stat_mat_vct
	BAL _$j_036ec

; FUNCTION: GY454XE  Re 036E8
; FUNCTION: GY455XE  Im 037F4
; FUNCTION: GY460XF  Im 0348E
_filter_chars_tables:
	LEA _blacklist_tables
_$j_036ec:
	L R1, [EA+]
	BEQ _$j_036f6
	CMP R1, R0
	BNE _$j_036ec
	MOV R0, #0H
_$j_036f6:
	RT

; FUNCTION: GY454XE  Re 036F8
; FUNCTION: GY455XE  Im 03804
; FUNCTION: GY460XF  Im 0349E
_filter_chars_table:
	L R1, _mode
	CMP R1, #88H  ; TABLE mode
	BNE _$j_036f6
	LEA _blacklist_table
	BAL _$j_036ec

; FUNCTION: GY454XE  Re 03706
; FUNCTION: GY455XE  Im 03812
; FUNCTION: GY460XF  Im 034AC
_filter_chars_cmplx:
	L R1, _mode
	CMP R1, #0C4H  ; CMPLX mode
	BNE _$j_036f6
	LEA _blacklist_cmplx
	BAL _$j_036ec

; FUNCTION: GY465XG  Im 033F8
_filter_chars_verif:
; Casio didn't add an else clause here so in ROMs without VERIF it runs
; the very next function instead. What a joke!
IF ENABLE_VERIF == 1
	L R1, _mode
	CMP R1, #89H
	BNE _$j_036f6
	LEA _blacklist_verif
	BAL _$j_036ec
ENDIF

; FUNCTION: GY454XE  Re 03714
; FUNCTION: GY455XE  Im 03820
; FUNCTION: GY460XF  Im 034BA
_f_03714:
	PUSH LR
	MOV ER0, BP
	BL _f_1AE06
	ADD R0, #-1H
	BLT _$j_03724
	L R0, 8H[BP]
	ADD R0, #-10H
_$j_03724:
	POP PC

; FUNCTION: GY454XE  Re 03726
; FUNCTION: GY455XE  Im 03832
; FUNCTION: GY460XF  Im 034CC
_num_sum:
IF ENABLE_SUM == 1
	PUSH LR
	CMP R6, #0H
	BEQ _$j_03730
	MOV R2, #2H
	POP PC
_$j_03730:
	PUSH ER8
	ADD SP, #-1EH
	MOV ER8, SP
	PUSH FP
	MOV R6, #-1H
	BL _num_eval__
	BGT _$j_0380a
	BNE _$j_0380c
	MOV R6, #2H
	BL _num_eval__
	BGT _$j_0380a
	BNE _$j_0380c
	MOV ER0, ER8
	ADD ER0, #0AH
	BL _num_cpy_cmplx_er0_bp
	BL _num_eval__
	BLT _$j_0380c
	BNE _$j_0375e
	ADD FP, #-1H
_$j_0375e:
	BL _f_03714
	BLT _$j_0378a
	PUSH BP
	MOV BP, ER8
	ADD BP, #0AH
	BL _f_03714
	POP BP
	BLT _$j_0378a
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV ER2, ER0
	MOV ER0, ER8
	ADD ER0, #0AH
	BL _num_to_str_std_lineo
	BL _num_cmp
	CMP R0, #4H
	BNE _$j_0378e
_$j_0378a:
	MOV R0, #8H
	BAL _$j_0380c
_$j_0378e:
	BL _set_scr_calculating
	MOV ER0, ER8
	ADD ER0, #14H
	BL _num_cpy_cmplx_er0_bp
	MOV R0, #BYTE1 _num_0
	MOV R1, #BYTE2 _num_0
	MOV ER2, ER8
	BL _num_cpy_cmplx
	POP ER0
	PUSH FP
	PUSH ER0
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #0AH
	BL _st_var
_$j_037b4:
	BL _check_ac
	CMP R0, #0H
	BNE _$j_03802
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_03802
	MOV ER0, ER8
	MOV ER2, BP
	BL _num_add_1
	MOV ER0, ER8
	BL _f_1B238
	CMP R0, #0H
	BNE _$j_03802
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	MOV ER2, ER8
	ADD ER2, #14H
	PUSH ER0
	BL _num_cmp
	CMP R0, #1H
	BEQ _$j_037f8
	POP ER0
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _num_add_1
	BAL _$j_037b4
_$j_037f8:
	ADD SP, #2H
	MOV ER0, ER8
	BL _num_cpy_cmplx_bp_er0
	MOV R0, #0H
_$j_03802:
	ADD SP, #2H
	POP FP
	ADD SP, #1EH
	BAL _$j_0380e
_$j_0380a:
	MOV R0, #2H
_$j_0380c:
	ADD SP, #20H
_$j_0380e:
	MOV R2, R0
	POP ER8
	POP PC
ELSE
	RT
ENDIF

IF ENABLE_INTEGRAL == 1
; FUNCTION: GY454XE  Re 03814
; FUNCTION: GY455XE  Im 03920
_f_03814:
	PUSH LR
	PUSH ER0
	MOV ER0, ER8
	MOV R2, #BYTE1 (_mode_ram+790)
	MOV R3, #BYTE2 (_mode_ram+790)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+780)
	MOV R3, #BYTE2 (_mode_ram+780)
	BL _f_1A438
	POP ER0
	MOV R2, #BYTE1 (_mode_ram+790)
	MOV R3, #BYTE2 (_mode_ram+790)
	BL _f_1A410
	ADD ER8, #0AH
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 0383C
; FUNCTION: GY455XE  Im 03948
_f_0383C:
	MOV FP, SP
	L FP, 2H[FP]
	PUSH LR
	PUSH FP
	MOV ER0, ER8
	MOV R2, #BYTE1 (_mode_ram+780)
	MOV R3, #BYTE2 (_mode_ram+780)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+740)
	BL _f_1A438
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	MOV R2, #BYTE1 (_mode_ram+790)
	MOV R3, #BYTE2 (_mode_ram+790)
	BL _num_cpy_cmplx
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+750)
	BL _f_1A3FC
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	BL _num_negate
	MOV R2, #BYTE1 (_mode_ram+780)
	MOV R3, #BYTE2 (_mode_ram+780)
_$j_03878:
	MOV R0, #8H
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_038c0
	SB R8.0
	BNE _$j_038ac
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	BL _num_cpy_cmplx_er0_bp
	MOV R0, #BYTE1 (_mode_ram+790)
	MOV R1, #BYTE2 (_mode_ram+790)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+750)
	BL _f_1A410
	CMP R0, #0H
	BNE _$j_038c0
	MOV R2, #BYTE1 (_mode_ram+790)
	MOV R3, #BYTE2 (_mode_ram+790)
	BAL _$j_03878
_$j_038ac:
	RB R8.0
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	MOV ER2, BP
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	BL _f_1B238
_$j_038c0:
	ADD SP, #2H
	ADD ER8, #0AH
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 038C8
; FUNCTION: GY455XE  Im 039D4
_f_038C8:
	POP FP
	PUSH FP
	PUSH LR
	PUSH FP
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+740)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+650)
	MOV R3, #BYTE2 (_mode_ram+650)
	BL _f_1A3FC
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+740)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+740)
	BL _num_sign
	CMP R0, #0F0H
	BEQ _$j_039ac
	CMP R0, #1H
	BEQ _$j_039ac
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+750)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+650)
	MOV R3, #BYTE2 (_mode_ram+650)
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+750)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	CMP R0, #3H
	BEQ _$j_039ac
	MOV R0, #8H
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+750)
	BL _st_var
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_039ac
	MOV R0, #BYTE1 _num_gk_weight
	MOV R1, #BYTE2 _num_gk_weight
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+760)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV ER2, BP
	BL _f_1A438
	MOV R0, #BYTE1 _unk_01c8a
	MOV R1, #BYTE2 _unk_01c8a
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+770)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV ER2, BP
	BL _f_1A438
	PUSH ER8
	MOV R8, #BYTE1 _num_gk_nodes_symm
	MOV R9, #BYTE2 _num_gk_nodes_symm
_$j_03968:
	BL _check_ac
	CMP R0, #0H
	BNE _$j_039b0
	BL _f_0383C
	BNE _$j_039aa
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+770)
	BL _f_03814
	BNE _$j_039aa
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	BL _f_03814
	BNE _$j_039aa
	MOV R0, #BYTE1 _num_gk_nodes_expanded
	MOV R1, #BYTE2 _num_gk_nodes_expanded
	CMP ER8, ER0
	BLT _$j_03968
_$j_03992:
	BL _check_ac
	CMP R0, #0H
	BNE _$j_039b0
	BL _f_0383C
	BNE _$j_039aa
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+770)
	BL _f_03814
	BEQ _$j_039b4
_$j_039aa:
	POP ER8
_$j_039ac:
	MOV R0, #3H
	BAL _$j_03a14
_$j_039b0:
	POP ER8
	BAL _$j_03a14
_$j_039b4:
	MOV R0, #2AH
	MOV R1, #1DH
	CMP ER8, ER0
	BLE _$j_03992
	POP ER8
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	BL _f_1B238
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	BL _num_to_str_std_lineo
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+740)
	BL _f_1A438
	CMP R0, #0H
	BNE _$j_039ac
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+770)
	BL _f_1B238
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+770)
	BL _num_to_str_std_lineo
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+740)
	BL _f_1A438
	CMP R0, #0H
	BNE _$j_039ac
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+770)
	BL _f_1A3FC
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	BL _num_abs
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+760)
	BL _f_1B238
	MOV R0, #0H
_$j_03a14:
	ADD SP, #2H
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 03A1A
; FUNCTION: GY455XE  Im 03B26
_f_03A1A:
	PUSH LR
	PUSH BP
	ADD SP, #-0AH
	MOV BP, SP
	MOV R0, #BYTE1 (_mode_ram+640)
	MOV R1, #BYTE2 (_mode_ram+640)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+760)
	PUSH ER2
	BL _num_cpy_cmplx_bp_er0
	MOV ER0, BP
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+770)
	BL _f_1A438
	CMP R0, #0H
	BNE _$j_03a6c
	MOV ER0, BP
	BL _num_abs
	L ER2, 8H[BP]
	CMP R3, #0H
	BNE _$j_03a5e
	CMP R2, #-70H
	BGE _$j_03a5e
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	MOV ER2, BP
	BL _num_cpy_cmplx
	MOV R0, #90H
	MOV R1, #0H
	ST ER0, 8H[BP]
_$j_03a5e:
	MOV ER0, BP
	POP ER2
	BL _f_1B0DC
_$j_03a66:
	ADD SP, #0AH
	POP BP
	POP PC
_$j_03a6c:
	ADD SP, #2H
	MOV R0, #4H
	BAL _$j_03a66
ENDIF

; FUNCTION: GY454XE  Re 03A72
; FUNCTION: GY455XE  Im 03B7E
; FUNCTION: GY460XF  Im 034CE
_num_integral:
IF ENABLE_INTEGRAL == 1
	PUSH LR
	CMP R6, #0H
	BEQ _$j_03a7c
	MOV R2, #2H
	POP PC
_$j_03a7c:
	PUSH FP
	MOV R6, #-1H
	BL _num_eval__
	BLE _$j_03a8a
	B _$j_03dde
_$j_03a8a:
	BEQ _$j_03a90
	B _$j_03de0
_$j_03a90:
	MOV R6, #2H
	BL _num_eval__
	BLE _$j_03a9c
	B _$j_03dde
_$j_03a9c:
	BEQ _$j_03aa2
	B _$j_03de0
_$j_03aa2:
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV R0, #BYTE1 (_mode_ram+620)
	MOV R1, #BYTE2 (_mode_ram+620)
	BL _num_cpy_cmplx_er0_bp
	BL _num_eval__
	BGE _$j_03aba
	B _$j_03de0
_$j_03aba:
	MOV R2, R0
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV R0, #BYTE1 (_mode_ram+630)
	MOV R1, #BYTE2 (_mode_ram+630)
	BL _num_cpy_cmplx_er0_bp
	CMP R2, #-1H
	BEQ _$j_03af0
	BL _num_eval__
	BGE _$j_03ad8
	B _$j_03de0
_$j_03ad8:
	BNE _$j_03adc
	ADD FP, #-1H
_$j_03adc:
	MOV ER0, BP
	BL _num_to_str_std_lineo
	BL _num_sign
	CMP R0, #2H
	BNE _$j_03afe
	MOV R0, #8H
	B _$j_03de0
_$j_03af0:
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _num_cpy_cmplx_bp_er0
	MOV R0, #95H
	MOV R1, #0H
	ST ER0, 8H[BP]
_$j_03afe:
	MOV R0, #BYTE1 (_mode_ram+640)
	MOV R1, #BYTE2 (_mode_ram+640)
	BL _num_cpy_cmplx_er0_bp
	ST FP, (_mode_ram+810)
	MOV R0, #BYTE1 (_mode_ram+620)
	MOV R1, #BYTE2 (_mode_ram+620)
	MOV R2, #BYTE1 (_mode_ram+630)
	MOV R3, #BYTE2 (_mode_ram+630)
	BL _f_1B0DC
	CMP R0, #1H
	BNE _$j_03b5a
	MOV R0, #8H
	MOV R2, #BYTE1 (_mode_ram+630)
	MOV R3, #BYTE2 (_mode_ram+630)
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b34
	B _$j_03dda
_$j_03b34:
	MOV R0, #8H
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b4e
	B _$j_03dda
_$j_03b4e:
	MOV R0, #BYTE1 _num_0
	MOV R1, #BYTE2 _num_0
	BL _num_cpy_cmplx_bp_er0
	B _$j_03dc8
_$j_03b5a:
	MOV R0, #8H
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b74
	B _$j_03dda
_$j_03b74:
	MOV R0, #8H
	MOV R2, #BYTE1 (_mode_ram+630)
	MOV R3, #BYTE2 (_mode_ram+630)
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b8e
	B _$j_03dda
_$j_03b8e:
	MOV R0, #BYTE1 (_mode_ram+620)
	MOV R1, #BYTE2 (_mode_ram+620)
	MOV R2, #BYTE1 (_mode_ram+650)
	MOV R3, #BYTE2 (_mode_ram+650)
	BL _num_cpy_cmplx
	ADD ER0, #0AH
	ADD ER2, #0AH
	BL _num_cpy_cmplx
	MOV R0, #BYTE1 _num_0
	MOV R1, #BYTE2 _num_0
	MOV R2, #BYTE1 (_mode_ram+680)
	MOV R3, #BYTE2 (_mode_ram+680)
	BL _num_cpy_cmplx
	ADD ER0, #0AH
	ADD ER2, #14H
	BL _num_cpy_cmplx
	ADD ER2, #0AH
	BL _num_cpy_cmplx
	MOV R0, #BYTE1 (_mode_ram+815)
	MOV R1, #BYTE2 (_mode_ram+815)
	MOV R2, #2FH
	ST R2, [ER0]
	MOV R1, #1H
	MOV R0, #44H
	MOV R2, #BYTE1 (_mode_ram+812)
	MOV R3, #BYTE2 (_mode_ram+812)
	ST ER0, [ER2]
	MOV R0, #BYTE1 (_mode_ram+630)
	MOV R1, #BYTE2 (_mode_ram+630)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+730)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _f_1A3FC
	CMP R0, #3H
	BNE _$j_03bec
	B _$j_03dda
_$j_03bec:
	BL _f_038C8
	BEQ _$j_03bf6
	B _$j_03dd6
_$j_03bf6:
	BL _f_03A1A
	CMP R0, #4H
	BNE _$j_03c0a
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+770)
	BL _num_cpy_cmplx_bp_er0
	B _$j_03dc8
_$j_03c0a:
	LEA (_mode_ram+812)
	L ER0, [EA]
	ADD ER0, #-1H
	ST ER0, [EA]
	BEQ _$j_03c62
	BL _f_038C8
	BEQ _$j_03c20
	B _$j_03dd6
_$j_03c20:
	BL _f_03A1A
	CMP R0, #2H
	BNE _$j_03cd2
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+710)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A438
	LEA (_mode_ram+815)
	DEC [EA]
	BNE _$j_03c68
	MOV R0, #BYTE1 (_mode_ram+680)
	MOV R1, #BYTE2 (_mode_ram+680)
	MOV R2, #BYTE1 (_mode_ram+780)
	MOV R3, #BYTE2 (_mode_ram+780)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+770)
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+780)
	MOV R1, #BYTE2 (_mode_ram+780)
	MOV R2, #BYTE1 (_mode_ram+680)
	MOV R3, #BYTE2 (_mode_ram+680)
	BL _num_cmp
	CMP R0, #1H
	BEQ _$j_03cc0
_$j_03c62:
	MOV R0, #0BH
	B _$j_03de0
_$j_03c68:
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A438
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 (_mode_ram+660)
	MOV R3, #BYTE2 (_mode_ram+660)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+710)
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+730)
	BL _f_1A438
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _f_1A410
	CMP R0, #3H
	BNE _$j_03cbe
	B _$j_03dda
_$j_03cbe:
	BAL _$j_03c0a
_$j_03cc0:
	LEA (_mode_ram+815)
	INC [EA]
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+710)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
_$j_03cd2:
	MOV R0, #BYTE1 (_mode_ram+680)
	MOV R1, #BYTE2 (_mode_ram+680)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+770)
	BL _f_1A410
	CMP R0, #3H
	BEQ _$j_03dda
_$j_03ce2:
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1B0DC
	CMP R0, #1H
	BNE _$j_03cfc
	MOV R0, #BYTE1 (_mode_ram+680)
	MOV R1, #BYTE2 (_mode_ram+680)
	BL _num_cpy_cmplx_bp_er0
	BAL _$j_03dc8
_$j_03cfc:
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	BL _f_1B4D0
	CMP R0, #0H
	BEQ _$j_03d6c
	LEA (_mode_ram+815)
	INC [EA]
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+710)
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	ADD ER2, #0AH
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 (_mode_ram+650)
	MOV R3, #BYTE2 (_mode_ram+650)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+710)
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+650)
	MOV R1, #BYTE2 (_mode_ram+650)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+730)
	BL _f_1A438
	MOV R0, #BYTE1 (_mode_ram+650)
	MOV R1, #BYTE2 (_mode_ram+650)
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _f_1A410
	CMP R0, #3H
	BEQ _$j_03dda
	BAL _$j_03ce2
_$j_03d6c:
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+650)
	MOV R3, #BYTE2 (_mode_ram+650)
	BL _num_cpy_cmplx
	MOV ER2, ER0
	MOV R0, #BYTE1 (_mode_ram+740)
	MOV R1, #BYTE2 (_mode_ram+700)
	BL _num_cpy_cmplx
	MOV ER0, ER2
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+710)
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+740)
	MOV R3, #BYTE2 (_mode_ram+730)
	BL _f_1A438
	MOV R0, #BYTE1 (_mode_ram+660)
	MOV R1, #BYTE2 (_mode_ram+660)
	MOV R2, #BYTE1 (_mode_ram+620)
	MOV R3, #BYTE2 (_mode_ram+620)
	BL _f_1A410
	B _$j_03c0a
_$j_03dc8:
	MOV ER0, BP
	BL _f_1B238
	MOV R0, #0H
_$j_03dd0:
	L FP, (_mode_ram+810)
	BAL _$j_03de0
_$j_03dd6:
	CMP R0, #1H
	BEQ _$j_03dd0
_$j_03dda:
	MOV R0, #3H
	BAL _$j_03de0
_$j_03dde:
	MOV R0, #2H
_$j_03de0:
	ADD SP, #2H
	MOV R2, R0
	POP PC
ELSE
	RT
ENDIF

IF ENABLE_DDX == 1
; FUNCTION: GY454XE  Re 03DE6
; FUNCTION: GY455XE  Im 03EF2
_f_03DE6:
	POP FP
	PUSH FP
	PUSH LR
	PUSH FP
	PUSH ER0
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #-4H
	BL _st_var
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	MOV ER2, ER8
	ADD ER2, #-18H
	BL _f_1A410
	POP ER0
	PUSH ER0
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_03e68
	POP ER0
	PUSH ER0
	BL _num_cpy_cmplx_er0_bp
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #-4H
	BL _st_var
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	MOV ER2, ER8
	ADD ER2, #-18H
	BL _f_1A3FC
	POP ER0
	POP FP
	PUSH FP
	PUSH ER0
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_03e68
	POP ER0
	PUSH ER0
	MOV ER2, BP
	BL _f_1A3FC
	POP ER0
	PUSH ER0
	MOV ER2, ER8
	ADD ER2, #1EH
	BL _f_1A44C
	POP ER0
	BL _num_to_str_std_lineo
	BL _f_1B238
	CMP R0, #0H
	BEQ _$j_03e6c
	MOV R0, #-1H
	BAL _$j_03e6c
_$j_03e68:
	MOV R0, #3H
	ADD SP, #2H
_$j_03e6c:
	ADD SP, #2H
	CMP R0, #3H
	POP PC
ENDIF

; FUNCTION: GY454XE  Re 03E72
; FUNCTION: GY455XE  Im 03F7E
; FUNCTION: GY460XF  Im 034D0
_num_ddx:
IF ENABLE_DDX == 1
	PUSH LR
	CMP R6, #0H
	BEQ _$j_03e7c
	MOV R2, #2H
	POP PC
_$j_03e7c:
	PUSH ER8
	MOV R8, #BYTE1 (_mode_ram+790)
	MOV R9, #BYTE2 (_mode_ram+790)
	PUSH FP
	MOV R6, #-1H
	BL _num_eval__
	BLE _$j_03e90
	B _$j_04146
_$j_03e90:
	BEQ _$j_03e96
	B _$j_04148
_$j_03e96:
	MOV R6, #2H
	BL _num_eval__
	BGE _$j_03ea2
	B _$j_04148
_$j_03ea2:
	MOV R2, R0
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV ER0, ER8
	ADD ER0, #-4H
	BL _num_cpy_cmplx_er0_bp
	CMP R2, #-1H
	BEQ _$j_03eee
	BL _num_eval__
	BGE _$j_03ec0
	B _$j_04148
_$j_03ec0:
	BNE _$j_03ec4
	ADD FP, #-1H
_$j_03ec4:
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV ER0, BP
	BL _num_sign
	CMP R0, #4H
	BEQ _$j_03eda
	MOV R0, #8H
	B _$j_04148
_$j_03eda:
	MOV ER0, ER8
	ADD ER0, #32H
	BL _num_cpy_cmplx_er0_bp
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	MOV R2, #-1H
	ST R2, [ER0]
	BAL _$j_03f18
_$j_03eee:
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _num_cpy_cmplx_bp_er0
	MOV R0, #90H
	MOV R1, #0H
	ST ER0, 8H[BP]
	MOV ER0, ER8
	ADD ER0, #32H
	BL _num_cpy_cmplx_er0_bp
	MOV R0, #93H
	ST R0, 8H[BP]
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	BL _num_cpy_cmplx_er0_bp
	MOV R2, #-1H
	ADD ER0, #0AH
	ST R2, [ER0]
_$j_03f18:
	POP ER0
	PUSH FP
	PUSH ER0
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #-4H
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03f38
	B _$j_04140
_$j_03f38:
	MOV ER0, ER8
	ADD ER0, #-4H
	BL _num_cpy_cmplx_bp_er0
	MOV ER0, BP
	BL _num_sign
	MOV R2, #98H
	CMP R0, #1H
	BEQ _$j_03f82
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _num_cpy_cmplx_bpp20_er0
	MOV R2, #97H
	MOV R3, #0H
	ST ER2, 1CH[BP]
	MOV ER0, BP
	MOV ER2, BP
	ADD ER2, #14H
	BL _f_1A438
	MOV ER0, BP
	MOV R2, #1H
	BL _f_1AFC8
	L R0, [BP]
	BEQ _$j_03f80
	L ER0, 8H[BP]
	CMP R1, #5H
	BLT _$j_03f78
	ADD R1, #-5H
_$j_03f78:
	CMP R1, #0H
	BNE _$j_03f88
	CMP R0, #7H
	BGE _$j_03f88
_$j_03f80:
	MOV R2, #7H
_$j_03f82:
	ST R2, 8H[BP]
	MOV R0, #1H
	ST R0, [BP]
_$j_03f88:
	MOV ER0, ER8
	ADD ER0, #-18H
	BL _num_cpy_cmplx_er0_bp
	B _$j_04150
_$j_03f94:
	MOV ER0, ER8
	ADD ER0, #-18H
	BL _num_abs
	MOV ER0, ER8
	ADD ER0, #-18H
	MOV ER2, ER8
	ADD ER2, #1EH
	BL _num_cpy_cmplx
	PUSH ER0
	MOV ER0, ER2
	POP ER2
	BL _f_1A410
	MOV ER0, ER8
	BL _f_03DE6
	BLT _$j_03fc0
	MOV R0, #3H
	B _$j_04140
_$j_03fc0:
	MOV ER6, #-0AH
_$j_03fc2:
	BL _check_ac
	CMP R0, #0H
	BEQ _$j_03fce
	B _$j_04140
_$j_03fce:
	MOV ER0, ER8
	ADD ER0, #-18H
	MOV ER2, ER8
	ADD ER2, #1EH
	BL _num_cpy_cmplx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV ER0, ER8
	MOV ER2, ER8
	ADD ER2, #0AH
	BL _num_cpy_cmplx
	MOV ER0, ER8
	ADD ER0, ER6
	POP FP
	PUSH ER6
	PUSH FP
	BL _f_03DE6
	POP FP
	POP ER6
	PUSH FP
	BLT _$j_04008
	MOV R0, #3H
	B _$j_04140
_$j_04008:
	MOV R0, #BYTE1 _num_3
	MOV R1, #BYTE2 _num_3
	MOV ER2, ER8
	ADD ER2, #14H
	BL _num_cpy_cmplx
	MOV R0, #6H
	ST R0, 9H[ER2]
	PUSH ER6
_$j_0401c:
	MOV ER4, ER6
	ADD ER4, #0AH
	MOV ER0, ER8
	ADD ER0, ER4
	MOV ER2, ER8
	ADD ER2, ER6
	PUSH XR0
	BL _f_1A3FC
	MOV ER0, ER8
	ADD ER0, ER4
	MOV ER2, ER8
	ADD ER2, #14H
	BL _f_1A44C
	POP XR0
	BL _f_1A410
	CMP R0, #0H
	BEQ _$j_04048
	ADD SP, #2H
	BAL _$j_04140
_$j_04048:
	CMP R4, #0H
	BEQ _$j_04068
	MOV ER0, ER8
	ADD ER0, #14H
	MOV R2, #BYTE1 _num_4
	MOV R3, #BYTE2 _num_4
	BL _f_1A438
	MOV ER0, ER8
	ADD ER0, #14H
	MOV R2, #BYTE1 _num_3
	MOV R3, #BYTE2 _num_3
	BL _f_1A3FC
	ADD ER6, #0AH
	BAL _$j_0401c
_$j_04068:
	POP ER6
	L R0, [ER8]
	BNE _$j_04076
	L R0, 0AH[ER8]
	BEQ _$j_04138
	BAL _$j_04132
_$j_04076:
	MOV ER0, ER8
	ADD ER0, #0AH
	MOV ER2, ER8
	BL _f_1A3FC
	MOV ER0, ER8
	ADD ER0, #0AH
	MOV ER2, ER8
	BL _f_1A44C
	MOV ER0, ER8
	ADD ER0, #0AH
	BL _num_abs
	CMP R0, #0H
	BNE _$j_040fa
	MOV ER0, ER8
	ADD ER0, #0AH
	MOV ER2, ER8
	ADD ER2, #32H
	BL _f_1B0DC
	CMP R0, #4H
	BNE _$j_04138
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	L R2, [ER0]
	CMP R2, #-1H
	BEQ _$j_040ea
	MOV ER2, ER8
	ADD R2, #46H
	ADDC R3, #0H
	L R0, [ER2]
	BEQ _$j_040ea
_$j_040bc:
	MOV ER0, ER8
	ADD ER0, #0AH
	BL _f_1B0DC
	CMP R0, #4H
	BEQ _$j_040ea
	MOV ER0, ER8
	MOV ER2, ER8
	ADD R2, #50H
	ADDC R3, #0H
	BL _num_cpy_cmplx
	ADD ER2, #-0AH
	L R0, 8H[ER2]
	CMP R0, #-6FH
	BEQ _$j_040e6
	ADD R0, #-1H
	ST R0, 8H[ER2]
	BAL _$j_040bc
_$j_040e6:
	MOV R0, #0H
	ST R0, [ER2]
_$j_040ea:
	L ER0, 8H[ER8]
	CMP R1, #0H
	BEQ _$j_040f6
	CMP R1, #5H
	BNE _$j_040fa
_$j_040f6:
	CMP R0, #-70H
	BLT _$j_04132
_$j_040fa:
	ADD ER6, #-0AH
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	L R2, [ER0]
	CMP R2, #-1H
	BNE _$j_04112
	CMP R6, #60H
	BEQ _$j_04110
	B _$j_03fc2
_$j_04110:
	BAL _$j_0412e
_$j_04112:
	CMP R6, #-6EH
	BEQ _$j_0411a
	B _$j_03fc2
_$j_0411a:
	MOV ER0, ER8
	ADD R0, #50H
	ADDC R1, #0H
	L R2, [ER0]
	CMP R2, #-1H
	BEQ _$j_0412e
	MOV ER2, ER8
	BL _num_cpy_cmplx
	BAL _$j_04138
_$j_0412e:
	MOV R0, #0BH
	BAL _$j_04140
_$j_04132:
	MOV R0, #-42H
	MOV R1, #17H
	BAL _$j_0413a
_$j_04138:
	MOV ER0, ER8
_$j_0413a:
	BL _num_cpy_cmplx_bp_er0
	MOV R0, #0H
_$j_04140:
	ADD SP, #2H
	POP FP
	BAL _$j_0414a
_$j_04146:
	MOV R0, #2H
_$j_04148:
	ADD SP, #2H
_$j_0414a:
	MOV R2, R0
	POP ER8
	POP PC
_$j_04150:
	MOV ER0, ER8
	ADD ER0, #-0AH
	MOV R2, #0AH
	BL _num_frombyte
	MOV ER0, ER8
	ADD ER0, #32H
	MOV R2, #2H
	BL _num_fromdigit
	MOV ER0, ER8
	ADD ER0, #32H
	MOV ER2, #-1H
	BL _num_mulxp__
	MOV ER0, ER8
	ADD ER0, #-1EH
	MOV R2, #9H
	BL _num_fromdigit
	MOV ER0, ER8
	ADD ER0, #-1EH
	MOV ER2, #-1H
	BL _num_mulxp__
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #-4H
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_0419c
	B _$j_042a0
_$j_0419c:
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV ER0, ER8
	ADD ER0, #0EH
	BL _num_cpy_cmplx_er0_bp
	MOV ER0, ER8
	ADD ER0, #4H
	MOV R2, #0H
	BL _num_fromdigit
	MOV R6, #7H
_$j_041b6:
	MOV ER0, ER8
	ADD ER0, #-4H
	MOV ER2, ER8
	ADD ER2, #18H
	BL _num_cpy_r
	MOV ER0, ER2
	MOV ER2, ER8
	ADD ER2, #-18H
	BL _f_1A410
	CMP R0, #0H
	BNE _$j_042a0
	MOV ER0, ER8
	ADD ER0, #4H
	BL _get_num_err_type
	CMP R0, #0H
	BEQ _$j_041ec
	MOV ER0, ER8
	ADD ER0, #18H
	MOV ER2, ER8
	ADD ER2, #-4H
	BL _f_1B0DC
	CMP R0, #1H
	BEQ _$j_042a0
_$j_041ec:
	MOV R0, #8H
	MOV ER2, ER8
	ADD ER2, #18H
	BL _st_var
	POP FP
	PUSH FP
	PUSH R6
	MOV R6, #1H
	BL _num_eval__
	POP R6
	MOV ER0, BP
	BL _num_to_str_std_lineo
	MOV ER0, ER8
	ADD ER0, #4H
	BL _num_cpy_cmplx_er0_bp
	MOV ER0, ER8
	ADD ER0, #4H
	BL _get_num_err_type
	CMP R0, #0H
	BEQ _$j_04224
	CMP R0, #3H
	BEQ _$j_0428e
	BAL _$j_042a0
_$j_04224:
	MOV ER0, ER8
	ADD ER0, #0EH
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_042a6
	MOV ER0, ER8
	ADD ER0, #4H
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_042a6
	MOV ER0, ER8
	ADD ER0, #4H
	BL _num_cpy_cmplx_bp_er0
	MOV ER2, ER8
	ADD ER2, #0EH
	MOV ER0, BP
	BL _f_1A44C
	MOV ER0, BP
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_0425c
	CMP R0, #4H
	BNE _$j_0428e
_$j_0425c:
	MOV ER0, BP
	BL _num_abs
	MOV ER2, ER8
	ADD ER2, #-1EH
	MOV ER0, BP
	BL _f_1A424
	MOV ER0, BP
	BL _num_sign
	CMP R0, #1H
	BEQ _$j_0427a
	CMP R0, #4H
	BNE _$j_0428e
_$j_0427a:
	MOV ER2, ER8
	ADD ER2, #32H
	MOV ER0, BP
	BL _num_cmp
	CMP R0, #1H
	BEQ _$j_0428c
	CMP R0, #2H
	BNE _$j_0428e
_$j_0428c:
	BAL _$j_042a6
_$j_0428e:
	MOV ER0, ER8
	ADD ER0, #-18H
	MOV ER2, ER8
	ADD ER2, #-0AH
	BL _f_1A44C
	ADD R6, #-1H
	BNE _$j_041b6
	BAL _$j_042a6
_$j_042a0:
	MOV R0, #3H
	B _$j_04140
_$j_042a6:
	B _$j_03f94
ELSE
	RT
ENDIF

; FUNCTION: GY454XE  Re 042AA
; FUNCTION: GY455XE  Im 043B6
; FUNCTION: GY460XF  Im 034D2
_f_042AA:
	PUSH LR
	PUSH QR8
	PUSH XR4
	PUSH ER0
	L FP, [ER0]
	ADD SP, #-3CH
	MOV BP, SP
	MOV R0, #BYTE1 _mode_ram
	MOV R1, #BYTE2 _mode_ram
	MOV R2, #BYTE1 (_mode_ram+10)
	MOV R3, #BYTE2 (_mode_ram+10)
	BL _f_1B0DC
	CMP R0, #0F0H
	BEQ _$j_043a4
	CMP R0, #4H
	BEQ _$j_043a4
	MOV R0, #BYTE1 (_mode_ram+20)
	MOV R1, #BYTE2 (_mode_ram+20)
	BL _num_sign
	CMP R0, #4H
	BNE _$j_043a4
	MOV R0, #BYTE1 (_mode_ram+10)
	MOV R1, #BYTE2 (_mode_ram+10)
	BL _num_cpy_cmplx_bp_er0
	MOV ER0, BP
	MOV R2, #BYTE1 _mode_ram
	MOV R3, #BYTE2 _mode_ram
	BL _f_1A3FC
	MOV ER0, BP
	MOV R2, #BYTE1 (_mode_ram+20)
	MOV R3, #BYTE2 (_mode_ram)
	BL _f_1A44C
	MOV ER0, BP
	BL _f_1B238
	CMP R0, #3H
	BEQ _$j_043a4
	MOV ER0, BP
	BL _num_trunc__
	L ER2, [BP]
	AND R2, #0FH
	L R0, 8H[BP]
	BEQ _$j_0431e
	CMP R0, #1H
	BNE _$j_043a8
	CMP R2, #3H
	BGE _$j_043a8
	SRL R3, #4
	MOV R0, #0AH
	MUL ER0, R2
	ADD R0, R3
	BAL _$j_04320
_$j_0431e:
	MOV R0, R2
_$j_04320:
	ADD R0, #1H
	MOV R8, R0
	MOV R9, #1H
	BL _f_04468
	MOV R0, #BYTE1 _mode_ram
	MOV R1, #BYTE2 _mode_ram
	MOV R2, #BYTE1 _var_x
	MOV R3, #BYTE2 _var_x
	BL _num_cpy_cmplx
	PUSH FP
_$j_04338:
	MOV ER10, #0H
	ST R11, _d_08121
	MOV R6, #1H
	POP FP
	PUSH FP
	BL _num_eval__
	CMP R0, #-1H
	BEQ _$j_0435a
	ADD FP, #-1H
	CMP R0, #3H
	BNE _$j_04396
	MOV R2, #3H
	MOV ER0, BP
	BL _num_errorval
_$j_0435a:
	MOV R0, #0H
	MOV R1, R9
	MOV R2, #BYTE1 _var_x
	MOV R3, #BYTE2 _var_x
	BL _table_stat_set_cell
	MOV R0, #1H
	MOV R1, R9
	MOV ER2, BP
	BL _table_stat_set_cell
	CMP R9, R8
	BEQ _$j_04394
	ADD R9, #1H
	BL _check_ac
	CMP R0, #0H
	BNE _$j_04396
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	MOV R2, #BYTE1 (_mode_ram+20)
	MOV R3, #BYTE2 (_mode_ram)
	BL _f_1A410
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	BL _f_1B238
	BAL _$j_04338
_$j_04394:
	MOV R0, #0H
_$j_04396:
	ADD SP, #2H
_$j_04398:
	ADD SP, #3CH
	POP BP
	ST FP, [BP]
	POP XR4
	POP QR8
	POP PC
_$j_043a4:
	MOV R0, #3H
	BAL _$j_04398
_$j_043a8:
	MOV R0, #4H
	BAL _$j_04398

; FUNCTION: GY454XE  Re 043AC
; FUNCTION: GY455XE  Im 044B8
; FUNCTION: GY460XF  Im 035D4
_f_043AC:
	PUSH R0
	ADD R1, #-1
	MOV R0, #3
	MUL ER0, R1
	ADD R2, #-1
	MOV R3, #0H
	ADD ER2, ER0
	POP R0
	MOV R1, #9H
	MUL ER0, R1
	ADD R2, R0
	MOV R0, #10
	MUL ER2, R0
	MOV R0, #BYTE1 _mode_ram
	MOV R1, #BYTE2 _mode_ram
	ADD ER0, ER2
	RT

; FUNCTION: GY454XE  Re 043CE
; FUNCTION: GY455XE  Im 044DA
; FUNCTION: GY460XF  Im 035F6
_table_stat_get_cell_addr:
	PUSH LR
	PUSH XR8
	MOV ER8, ER2
	MOV ER10, ER0
	BL _table_stat_get_col_addr
	CMP R0, R11
	BLT _$j_04414
	MOV R0, #1H
	CMP R10, #3H
	BEQ _$j_043e8
	BL _table_stat_get_num_cols
_$j_043e8:
	MOV R2, R11
	ADD R2, #-1H
	MUL ER2, R0
	MOV R1, #0AH
	MUL ER2, R1
	CMP R10, #3H
	BEQ _$j_04408
	L R0, _submode
	CMP R0, #1H
	BNE _$j_04404
	CMP R10, #2H
	BNE _$j_04404
	ADD R10, #-1H
_$j_04404:
	MUL ER10, R1
	ADD ER2, ER10
_$j_04408:
	MOV R0, #0H
	L ER10, [ER8]
	ADD ER2, ER10
_$j_0440e:
	ST ER2, [ER8]
	POP XR8
	POP PC
_$j_04414:
	MOV ER2, #0H
	MOV R0, #2H
	BAL _$j_0440e

; FUNCTION: GY454XE  Re 0441A
; FUNCTION: GY455XE  Im 04526
; FUNCTION: GY460XF  Im 03642
_table_stat_get_col_addr:
	PUSH QR8
	MOV ER10, ER2
	MOV R8, #BYTE1 (_mode_ram+80)
	MOV R9, #BYTE2 (_mode_ram+80)
	L R12, _d_080DE
	CMP R0, #3H
	BEQ _$j_04432
_$j_0442a:
	MOV R0, R12
	ST ER8, [ER10]
	POP QR8
	RT
_$j_04432:
	PUSH LR
	BL _table_stat_get_num_cols
	MUL BP, R0
	MOV R0, #0AH
	MUL BP, R0
	ADD ER8, BP
	L R12, _d_080DF
	POP LR
	BAL _$j_0442a

; FUNCTION: GY454XE  Re 04448
; FUNCTION: GY455XE  Im 04554
; FUNCTION: GY460XF  Im 03670
_table_stat_get_num_cols:
	MOV R0, #2H
	L R1, _mode
	CMP R1, #88H  ; If TABLE mode, return 2
	BEQ _$j_04466
	MOV R0, #1H   ; Start with 1
	L R1, _submode
	CMP R1, #1H   ; If not in 1-VAR submode, add 1
	BEQ _$j_0445e
	ADD R0, #1H
_$j_0445e:
	L R1, _setup_stat_freq
	BEQ _$j_04466  ; If frequency is enabled, add 1
	ADD R0, #1H
_$j_04466:
	RT

; FUNCTION: GY454XE  Re 04468
; FUNCTION: GY455XE  Im 04574
; FUNCTION: GY460XF  Im 03690
_f_04468:
	PUSH LR
	ST R0, _d_080DE
	BL _f_0AF0A
	LEA 600
	PUSH EA
	MOV ER2, #0H
	ST R2, _d_080DF
	MOV R0, #BYTE1 (_mode_ram+80)
	MOV R1, #BYTE2 (_mode_ram+80)
	BL _memset_n
	POP ER0
	POP PC

; FUNCTION: GY454XE  Re 0448A
; FUNCTION: GY455XE  Im 04596
; FUNCTION: GY460XF  Im 036B2
_smart_num_cpy:
	PUSH LR
	ADD ER0, #0H
	BEQ _$j_04498
	ADD ER2, #0H
	BEQ _$j_04498
	BL _num_cpy
_$j_04498:
	POP PC

; FUNCTION: GY454XE  Re 0449A
; FUNCTION: GY455XE  Im 045A6
; FUNCTION: GY460XF  Im 036C2
_clear_result:
	PUSH LR
	MOV ER2, #20
	MOV R0, #BYTE1 _result
	MOV R1, #BYTE2 _result
	BL _memzero
	POP PC

; FUNCTION: GY454XE  Re 044A8
; FUNCTION: GY455XE  Im 045B4
; FUNCTION: GY460XF  Im 036D0
_memzero:
	PUSH LR
	PUSH ER2
	MOV ER2, #0H
	BL _memset_n
	POP ER2
	POP PC

; FUNCTION: GY454XE  Re 044B6
; FUNCTION: GY455XE  Im 045C2
; FUNCTION: GY460XF  Im 036DE
_f_044B6:
	MOV R0, #0H
	ST R0, _d_080FE
	ST R0, _result_template
_$j_044c0:
	ST R0, _d_08101
_$j_044c4:
	ST R0, _result_format
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 044CE
; FUNCTION: GY455XE  Im 045DA
; FUNCTION: GY460XF  Im 036F6
_f_044CE:
	MOV R0, #0H
	BAL _$j_044c0

; FUNCTION: GY454XE  Re 044D2
; FUNCTION: GY455XE  Im 045DE
; FUNCTION: GY460XF  Im 036FA
_f_044D2:
	MOV R0, #0H
	BAL _$j_044c4

; FUNCTION: GY454XE  Re 044D6
; FUNCTION: GY455XE  Im 045E2
; FUNCTION: GY460XF  Im 036FE
_set_keycode:
	ST R0, _last_key_keycode
	MOV R0, #1H
	ST R0, _force_nochar
	RT

; FUNCTION: GY454XE  Re 044E2
; FUNCTION: GY455XE  Im 045EE
; FUNCTION: GY460XF  Im 0370A
_set_char_keycode:
	ST R0, _last_key_keycode
	MOV R0, #0H
	ST R0, _force_nochar
	RT

; FUNCTION: GY454XE  Re 044EE
; FUNCTION: GY455XE  Im 045FA
; FUNCTION: GY460XF  Im 03716
_l_var:
	PUSH LR
	PUSH QR8
	PUSH R0
	BL _f_16CF0
	MOV R14, R0
	POP R0
	MOV ER10, ER2
	MOV ER8, #0AH
	MOV BP, ER8
	PUSH BP
	MUL ER8, R0
	MOV R2, #BYTE1 _vars_start
	MOV R3, #BYTE2 _vars_start
	ADD ER2, ER8
	MOV ER0, ER10
	BL _memcpy_nn
	POP ER0
	CMP R14, #1H
	BEQ _$j_0451e
	MOV ER0, ER10
	BL _f_15EE4
_$j_0451e:
	PUSH BP
	ADD ER10, #0AH
	MOV ER0, ER10
	L R2, _mode
	CMP R2, #0C4H  ; CMPLX mode
	BEQ _$j_04538
	MOV ER2, #0H
	BL _memset_n
_$j_04532:
	POP ER0
	POP QR8
	POP PC
_$j_04538:
	MOV R2, #BYTE1 (_mode_ram+362)
	MOV R3, #BYTE2 (_mode_ram+362)
	ADD ER2, ER8
	BL _memcpy_nn
	CMP R14, #1H
	BEQ _$j_0454c
	MOV ER0, ER10
	BL _f_15EE4
_$j_0454c:
	BAL _$j_04532

; FUNCTION: GY454XE  Re 0454E
; FUNCTION: GY455XE  Im 0465A
; FUNCTION: GY460XF  Im 03776
_st_var:
	PUSH LR
	PUSH QR8
	MOV ER10, ER2
	MOV ER8, #0AH
	MOV BP, ER8
	PUSH BP
	MUL ER8, R0
	MOV R0, #BYTE1 _vars_start
	MOV R1, #BYTE2 _vars_start
	ADD ER0, ER8
	MOV ER2, ER10
	BL _memcpy_nn
	POP ER0
	L R0, _mode
	CMP R0, #0C4H  ; CMPLX mode
	BNE _$j_04584
	PUSH BP
	MOV ER2, ER10
	ADD ER2, #0AH
	MOV R0, #BYTE1 (_mode_ram+362)
	MOV R1, #BYTE2 (_mode_ram+362)
	ADD ER0, ER8
	BL _memcpy_nn
	POP ER0
_$j_04584:
	POP QR8
	POP PC

; ==== POTENTIAL SPLIT ===
; Extra code found at this area in LY710XA. Needs more investigation.

; Unused since ES
; FUNCTION: GY454XE  Re 04588
; FUNCTION: GY455XE  Im 04694
; FUNCTION: GY460XF  Im 037B0
_f_04588:
	MOV ER0, SP
	RT

; FUNCTION: GY454XE  Re 0458C
; FUNCTION: GY455XE  Im 04698
; FUNCTION: GY460XF  Im 037B4
_get_remaining_stack_space:
	MOV R2, #BYTE1 _stack_start
	MOV R3, #BYTE2 _stack_start
	MOV ER0, SP
	SUB R0, R2
	SUBC R1, R3
	RT

; FUNCTION: GY454XE  Re 04598
; FUNCTION: GY455XE  Im 046A4
; FUNCTION: GY460XF  Im 037C0
_reset_magic_string:
	LEA _magic_string
	MOV R0, #0FH
_$rms_loop:
	ST R0, [EA+]
	ADD R0, #-1
	BNE _$rms_loop
	RT

; FUNCTION: GY454XE  Re 045A6
; FUNCTION: GY455XE  Im 046B2
; FUNCTION: GY460XF  Im 037CE
_need_reset:
	PUSH LR
	PUSH XR4
	LEA _magic_string
	MOV R0, #0FH
_$nr_loop:
	L R1, [EA+]
	CMP R1, R0
	BNE _$nr_yes
	ADD R0, #-1
	BNE _$nr_loop
	L R0, _setup_contrast
	CMP R0, #1DH
	BGTS _$nr_yes
	CMP R0, #4
	BLTS _$nr_yes
	MOV R6, #0AH
	MOV R4, #BYTE1 _vars_start
	MOV R5, #BYTE2 _vars_start
_$nr_loop2:
	MOV ER0, ER4
	BL _invalid_var
	BNE _$nr_yes
	ADD ER4, #0AH
	ADD R6, #-1
	BNE _$nr_loop2
	L R0, _d_080DC
	AND R0, #11111000B
	BEQ _$nr_no
_$nr_yes:
	POP XR4
	MOV R0, #1
	POP PC
_$nr_no:
	POP XR4
	MOV R0, #0
	POP PC

; FUNCTION: GY454XE  Re 045EE
; FUNCTION: GY455XE  Im 046FA
; FUNCTION: GY460XF  Im 03816
_shutdown:
	L R0, _cursor_noflash
	BNE _$stop_exit
	BL _buffer_clear
	MOV R0, #BYTE1 156
	MOV R1, #BYTE2 156
	PUSH ER0
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	MOV R2, #10*12  ; Start at row #10 (11th row from the top)
	MOV R3, #0
	ADD ER0, ER2
	MOV R2, #BYTE1 _casio_logo
	MOV R3, #BYTE2 _casio_logo
	BL _memcpy_nn
	POP ER0
	BL _render
	MOV R0, #BYTE1 5000
	MOV R1, #BYTE2 5000
	BL _delay
	MOV ER0, #3
	ST R0, 0F031H
	ST R1, FCON
	LEA IE0
	MOV ER2, #0
	ST ER2, [EA+]
	BL _clr_all_kimask
	BL _clr_all_ko
	BL _stop_enable
	B $$start_up

; FUNCTION: GY454XE  Re 04640
; FUNCTION: GY455XE  Im 0474C
; FUNCTION: GY460XF  Im 03868
_delay:
	MOV ER2, ER0
	LEA TM0CON
	MOV ER0, #1
	ST R0, [EA+]
	ST R1, [EA]
	LEA TM0C
	MOV R0, #0
	ST R0, [EA+]
	ST R0, [EA]
	LEA TM0D
	ST R2, [EA+]
	ST R3, [EA]
	LEA TM0CON1
	MOV R0, #1
	ST R0, [EA]
	LEA IRQ0
	MOV R0, #0
	ST R0, [EA+]
	ST R0, [EA]

; FUNCTION: GY454XE  Re 04670
; FUNCTION: GY455XE  Im 0477C
; FUNCTION: GY460XF  Im 03898
_stop_enable:
	LEA STPACP
	MOV R2, #50H
	MOV R3, #0A0H
	ST R2, [EA]
	ST R3, [EA+]
	MOV R0, #2
	ST R0, [EA]
	NOP
	NOP
_$stop_exit:
	RT
	
; FUNCTION: GY454XE  Re 04686
; FUNCTION: GY455XE  Im 04792
; FUNCTION: GY460XF  Im 038AE
_waitkey:
	LEA IRQ0
	L R0, [EA]
	AND R0, #11111101B
	ST R0, [EA]
	LEA STPACP
	MOV R2, #50H
	MOV R3, #0A0H
	ST R2, [EA]
	ST R3, [EA+]
	MOV R0, #2
	ST R0, [EA]
	NOP
	NOP
	RT
	
; FUNCTION: GY454XE  Re 046A6
; FUNCTION: GY455XE  Im 047B2
; FUNCTION: GY460XF  Im 038CE
_get_IRQ0:
	LEA IRQ0
	L R0, [EA]
	RT
	
; FUNCTION: GY454XE  Re 046AE
; FUNCTION: GY455XE  Im 047BA
; FUNCTION: GY460XF  Im 038D6
_f_046AE:
	TB _d_080F4.7
	BNE _$j_046bc
	MOV R0, #88H
_$j_046b6:
	ST R0, _d_080F4
	RT
_$j_046bc:
	MOV R0, #1
	RT
	
; FUNCTION: GY454XE  Re 046C0
; FUNCTION: GY455XE  Im 047CC
; FUNCTION: GY460XF  Im 038E8
_f_046C0:
	MOV R0, #80H
	BAL _$j_046b6
	
; FUNCTION: GY454XE  Re 046C4
; FUNCTION: GY455XE  Im 047D0
; FUNCTION: GY460XF  Im 038EC
_f_046C4:
	MOV R0, #0
	ST R0, _d_080F4
	RT

; FUNCTION: GY454XE  Re 046CC
; FUNCTION: GY455XE  Im 047D8
; FUNCTION: GY460XF  Im 038F4
_f_046CC:
	TB _d_080F4.7
	BEQ _$j_046dc
	BAL _$j_046bc
	
; FUNCTION: GY454XE  Re 046D4
; FUNCTION: GY455XE  Im 047E0
; FUNCTION: GY460XF  Im 038FC
_f_046D4:
	TB _d_080F4.3
	BEQ _$j_046dc
	BAL _$j_046bc
_$j_046dc:
	MOV R0, #0
	RT

; FUNCTION: GY454XE  Re 046E0
; FUNCTION: GY455XE  Im 047EC
; FUNCTION: GY460XF  Im 03908
_f_046E0:
	PUSH LR
	MOV ER2, #1H
	ST R2, FCON
	MOV R0, #34H
_$j_046ea:
	ADD R0, #-1H
	BNE _$j_046ea
	MOV R0, #0F4H
	MOV R1, #1H
_$j_046f2:
	ADD ER0, #-1H
	BNE _$j_046f2
	MOV R0, #5H
	ST R0, P1DIR
	MOV R0, #4H
	ST R0, P1CON0
	MOV R0, #1H
	ST R0, P1CON1
	BL _f_04760
	BL _clr_port0
	BL _f_04738
	MOV R0, #0H
	ST R0, KICON
	BL _set_ko1_ko9
	BL _clr_all_kimask
	BL _clr_all_ko
	POP PC

; FUNCTION: GY454XE  Re 04728
; FUNCTION: GY455XE  Im 04834
; FUNCTION: GY460XF  Im 03950
_clr_port0:
	LEA P0D
	MOV ER0, #0H
	MOV ER2, #7H
	ST ER0, [EA+]
	ST ER2, [EA+]
	ST R2, [EA]
	RT

; FUNCTION: GY454XE  Re 04738
; FUNCTION: GY455XE  Im 04844
; FUNCTION: GY460XF  Im 03960
_f_04738:
	LEA IE0
	MOV ER0, #00100010B
	ST ER0, [EA]
	MOV R0, #3H
	ST R0, EXICON0
	RT

; Used in ES. Segment 8 was repurposed for checksum calculation from ES PLUS LY onwards,
; so this function is not used anymore.
; FUNCTION: GY454XE  Re 04748
; FUNCTION: GY455XE  Im 04854
; FUNCTION: GY460XF  Im 03970
_f_04748:
	MOV ER0, #0H
	LEA 40H
_$j_0474e:
	L R2, 8:[EA+]
	CMP R1, R2
	BNE _$j_0475e
	ADD R1, #1H
	CMP R1, #20H
	BNE _$j_0474e
	MOV R0, #1H
_$j_0475e:
	RT

; FUNCTION: GY454XE  Re 04760
; FUNCTION: GY455XE  Im 0486C
; FUNCTION: GY460XF  Im 03988
_f_04760:
	MOV ER0, #3H
	ST R0, 0F033H
	ST R1, 0F030H
	ST R1, 0F034H
	MOV R0, #7H
	ST R0, 0F031H
	MOV R0, #11H

; FUNCTION: GY454XE  Re 04776
; FUNCTION: GY455XE  Im 04882
; FUNCTION: GY460XF  Im 0399E
_set_contrast_sfr:
	ST R0, 0F032H
	RT

; Used in ES and ES PLUS early GY.
; FUNCTION: GY454XE  Re 0477C
; FUNCTION: GY455XE  Im 04888
; FUNCTION: GY460XF  Im 039A4
_f_0477C:
	L R0, 0F031H
	AND R0, #11111011B
	ST R0, 0F031H
	OR R0, #00000100B
	ST R0, 0F031H
	RT

; FUNCTION: GY454XE  Re 0478E
; FUNCTION: GY455XE  Im 0489A
; FUNCTION: GY460XF  Im 039B6
_set_scr_normal:
	MOV R0, #5H
	ST R0, 0F031H
	RT

; FUNCTION: GY454XE  Re 04796
; FUNCTION: GY455XE  Im 048A2
; FUNCTION: GY460XF  Im 039BE
_f_04796:
	TB _table_mode.4
	BNE _$j_047a2

; FUNCTION: GY454XE  Re 0479C
; FUNCTION: GY455XE  Im 048A8
; FUNCTION: GY460XF  Im 039C4
_set_scr_calculating:
	MOV R0, #6H
	ST R0, 0F031H  ; Hide main screen area
_$j_047a2:
	RB 0F800H.4    ; Turn off [S]
	RB 0F800H.2    ; Turn off [A]
	RB 0F801H.1    ; Turn off STO
	RB 0F802H.6    ; Turn off RCL
	RB 0F80BH.7    ; Turn off ▲
	RB 0F80AH.3    ; Turn off ▼
_$j_047ba:
	RB 0F80BH.4    ; Turn off Disp
	RT

; FUNCTION: GY454XE  Re 047C0
; FUNCTION: GY455XE  Im 048CC
; FUNCTION: GY460XF  Im 039E8
_set_disp_indicator:
	CMP R0, #0H
	BEQ _$j_047ba
	SB 0F80BH.4    ; Turn on Disp
	RT

; FUNCTION: GY454XE  Re 047CA
; FUNCTION: GY455XE  Im 048D6
; FUNCTION: GY460XF  Im 039F2
_set_contrast2_0:
	MOV R0, #0H
	ST R0, 0F033H
	RT

; FUNCTION: GY454XE  Re 047D2
; FUNCTION: GY455XE  Im 048DE
; FUNCTION: GY460XF  Im 039FA
_set_all_kimask:
	MOV R0, #11111111B
	ST R0, KIMASK
	RT

; FUNCTION: GY454XE  Re 047DA
; FUNCTION: GY455XE  Im 048E6
; FUNCTION: GY460XF  Im 03A02
_clr_all_kimask:
	MOV R0, #0H
	ST R0, KIMASK
	RT

; FUNCTION: GY454XE  Re 047E2
; FUNCTION: GY455XE  Im 048EE
; FUNCTION: GY460XF  Im 03A0A
_set_all_ko:
	MOV R0, #01111111B
	ST R0, KOD0
	RT

; FUNCTION: GY454XE  Re 047EA
; FUNCTION: GY455XE  Im 048F6
; FUNCTION: GY460XF  Im 03A12
_clr_all_ko:
	MOV R2, #0H
	ST R2, KOD0
	RT

; FUNCTION: GY454XE  Re 047F2
; FUNCTION: GY455XE  Im 048FE
; FUNCTION: GY460XF  Im 03A1A
_is_key_pressed:
	MOV R0, #7FH
	ST R0, KOD0
	MOV ER0, #0H
	LEA KID
	L R2, [EA]
	CMP R2, #11111111B
	ADDC R0, #0H
	RT

; FUNCTION: GY454XE  Re 04806
; FUNCTION: GY455XE  Im 04912
; FUNCTION: GY460XF  Im 03A2E
_check_ac:
	PUSH LR
IF REAL == 1
	PUSH ER10
	MOV R2, #10H
	ST R2, KOD0
	MOV ER0, #0H
	MOV R2, #3H
_$j_04814:
	TB KID.2
	BNE _$j_04828
	ADD R2, #-1H
	BNE _$j_04814
	MOV R0, #4H
	MOV R1, #10H
	ST ER0, _last_key_scancode
	MOV ER0, #1H
_$j_04828:
	BL _clr_all_ko
	MOV R0, R0
	POP ER10
ELSE
	PUSH ER4
	MOV R4, #BYTE1 _stop_type
	MOV R5, #BYTE2 _stop_type
	MOV R2, #2H
	ST R2, [ER4]
	MOV R0, #BYTE1 4762
	MOV R1, #BYTE2 4762
	BL _delay
	MOV ER0, #0H
	L R2, [ER4]
	BC EQ, _$j_04936_e
	MOV R0, #4H
	MOV R1, #10H
	ST ER0, _last_key_scancode
	MOV ER0, #1H
_$j_04936_e:
	MOV R2, #0H
	ST R2, [ER4]
	MOV R0, R0
	POP ER4
ENDIF
	POP PC

; FUNCTION: GY454XE  Re 04832
; FUNCTION: GY455XE  Im 04940
; FUNCTION: GY460XF  Im 03A5C
_interrupt_stub:
	RTI

PUBLIC _matrix_dims
PUBLIC _unk_0078a
PUBLIC _vector_dims
PUBLIC _matvct_strings
PUBLIC _unk_007e6
PUBLIC _unk_0080a
PUBLIC _unk_00812
PUBLIC _table_param_ptrs
PUBLIC _table_eqn_linel_params
PUBLIC _table_eqn_liner_params
PUBLIC _base_n_submodes
PUBLIC _vars_list
PUBLIC _s_table_x
PUBLIC _s_table_y
PUBLIC _s_table_freq
PUBLIC _s_table_fx
PUBLIC _s_table_a
PUBLIC _s_table_1
PUBLIC _s_blank_line
PUBLIC _s_prompt_fix
PUBLIC _s_prompt_sci
PUBLIC _s_prompt_norm
PUBLIC _contrast_screen
PUBLIC _const_input_template
PUBLIC _const_screen
PUBLIC _conv_screen
PUBLIC _menu_clr
PUBLIC _s_clr_setup
PUBLIC _s_clr_mem
PUBLIC _s_reset_all
PUBLIC _s_clr_done_all
PUBLIC _s_prompt_yes
PUBLIC _s_prompt_cancel
PUBLIC _s_clr_done
PUBLIC _s_clr_prompt_ac
PUBLIC _menu_matrix_table
PUBLIC _menu_matrix
PUBLIC _menu_matrix_data
PUBLIC _menu_matrix_dim0
PUBLIC _menu_matrix_dim1
PUBLIC _menu_matrix_sel
PUBLIC _menu_mata_dim_sel
PUBLIC _menu_matb_dim_sel
PUBLIC _menu_matc_dim_sel
PUBLIC _menu_vector_table
PUBLIC _menu_vector
PUBLIC _menu_vector_data
PUBLIC _menu_vector_dim
PUBLIC _menu_vector_sel
PUBLIC _menu_vcta_dim_sel
PUBLIC _menu_vctb_dim_sel
PUBLIC _menu_vctc_dim_sel
PUBLIC _menu_cmplx
PUBLIC _menu_base_n_0
PUBLIC _menu_base_n_1
PUBLIC _menu_drg
PUBLIC _menu_hyp
PUBLIC _menu_stat_data_edit
PUBLIC _menu_stat_1var_sum
PUBLIC _menu_stat_2var_sum
PUBLIC _menu_stat_1var_var
PUBLIC _menu_stat_2var_var
PUBLIC _menu_stat_1var_minmax
PUBLIC _menu_stat_2var_minmax
PUBLIC _menu_stat_distr
PUBLIC _menu_stat_reg
PUBLIC _menu_stat_reg_quad
PUBLIC _menu_ratio
PUBLIC _menu_verify
PUBLIC _menu_ineq
PUBLIC _menu_ineq_poly2
PUBLIC _menu_ineq_poly3
PUBLIC _menu_setup_stat_freq
PUBLIC _menu_setup_rdec
PUBLIC _menu_setup_cmplx_result
PUBLIC _menu_setup_decimal_mark
PUBLIC _menu_setup_decimalo
PUBLIC _s_prompt_solve
PUBLIC _s_table_prompt_start
PUBLIC _s_table_prompt_end
PUBLIC _s_table_prompt_step
PUBLIC _s_ratio_optn_c
PUBLIC _s_ratio_optn_d
PUBLIC _s_verif_result_false
PUBLIC _s_verif_result_true
PUBLIC _s_ineq_allreal
PUBLIC _s_ineq_nosolution
PUBLIC _unk_01c74

PUBLIC _f_02676
PUBLIC _get_mathi_draw_idx
PUBLIC _smart_strlen
PUBLIC _smart_strcpy
PUBLIC _smart_strcat
PUBLIC _init_num_struct
PUBLIC _num_exp_to_str
PUBLIC _f_0285C
PUBLIC _f_02986
PUBLIC _get_result_disp_fmt
PUBLIC _get_result_store_fmt
PUBLIC _set_result_store2disp
PUBLIC _set_result_store_fmt
PUBLIC _set_result_fmt
PUBLIC _result_str_print
PUBLIC _basen_base_print
PUBLIC _f_02B3E
PUBLIC _get_token
PUBLIC _get_token_length
PUBLIC _print_continue_prompt
PUBLIC _print_input_area
PUBLIC _num_output_print
PUBLIC _f_02C76
PUBLIC _f_02CB6
PUBLIC _buffer_clear_lastnline
PUBLIC _fill_screen
PUBLIC _buffer_clear
PUBLIC _f_02D52
PUBLIC _draw_line_vert
PUBLIC _draw_line
PUBLIC _line_print_col_0
PUBLIC _line_print
PUBLIC _char_print
PUBLIC _plot_pixel
PUBLIC _render
PUBLIC _get_screen_addr
PUBLIC _setup_status_bar
PUBLIC _f_032A4
PUBLIC _f_0336A
PUBLIC _print_4lines_4str
PUBLIC _print_4lines_head
PUBLIC _print_4lines
PUBLIC _set_up_arrow
PUBLIC _set_down_arrow
PUBLIC _pd_value
PUBLIC _print_error
PUBLIC _f_03558
PUBLIC _is_char_keycode
PUBLIC _is_format_keycode
PUBLIC _f_035EC
PUBLIC _is_ac_keycode
PUBLIC _f_03660
PUBLIC _f_03664
PUBLIC _is_mov_keycode
PUBLIC _is_mov_y_keycode
PUBLIC _is_mov_x_keycode
PUBLIC _is_sto_abc_keycode
PUBLIC _is_func_table
PUBLIC _filter_chars_stat_mat_vct
PUBLIC _filter_chars_tables
PUBLIC _filter_chars_table
PUBLIC _filter_chars_cmplx
PUBLIC _filter_chars_verif
PUBLIC _f_03714
PUBLIC _num_sum
PUBLIC _num_integral
PUBLIC _num_ddx
PUBLIC _f_042AA
PUBLIC _f_043AC
PUBLIC _table_stat_get_cell_addr
PUBLIC _table_stat_get_col_addr
PUBLIC _table_stat_get_num_cols
PUBLIC _smart_num_cpy
PUBLIC _clear_result
PUBLIC _memzero
PUBLIC _f_044B6
PUBLIC _f_044CE
PUBLIC _f_044D2
PUBLIC _set_keycode
PUBLIC _set_char_keycode
PUBLIC _l_var
PUBLIC _st_var
PUBLIC _get_remaining_stack_space
PUBLIC _reset_magic_string
PUBLIC _need_reset
PUBLIC _shutdown
PUBLIC _delay
PUBLIC _waitkey
PUBLIC _get_IRQ0
PUBLIC _f_046AE
PUBLIC _f_046C0
PUBLIC _f_046C4
PUBLIC _f_046CC
PUBLIC _f_046D4
PUBLIC _f_046E0
PUBLIC _clr_port0
PUBLIC _set_contrast_sfr
PUBLIC _set_scr_normal
PUBLIC _f_04796
PUBLIC _set_scr_calculating
PUBLIC _set_disp_indicator
PUBLIC _set_contrast2_0
PUBLIC _set_all_kimask
PUBLIC _clr_all_kimask
PUBLIC _set_all_ko
PUBLIC _clr_all_ko
PUBLIC _is_key_pressed
PUBLIC _check_ac

EXTRN TABLE	: _font_big_0
EXTRN TABLE	: _font_small_0
EXTRN TABLE	: _font_tiny
EXTRN TABLE	: _font_big_1
EXTRN TABLE	: _font_small_1
EXTRN TABLE	: _s_continue_prompt
EXTRN TABLE	: _num_0
EXTRN TABLE	: _num_1
EXTRN TABLE	: _num_2
EXTRN TABLE	: _num_3
EXTRN TABLE	: _num_4

EXTRN CODE	: $$start_up
EXTRN CODE	: _memmove_nn
EXTRN CODE	: _strcpy_nn
EXTRN CODE	: _memset_n
EXTRN CODE	: _memcpy_nn
EXTRN CODE	: _num_to_str
EXTRN CODE	: _strlen_n
EXTRN CODE	: _strcat_nn
EXTRN CODE	: _f_0AF0A
EXTRN CODE	: _f_0AF16
EXTRN CODE	: _table_stat_set_cell
EXTRN CODE	: _f_0B05A
EXTRN CODE	: _is_eqn_result
EXTRN CODE	: _f_0B8B8
EXTRN CODE	: _num_to_str_std_lineo
EXTRN CODE	: _num_cpy_r
EXTRN CODE	: _num_cpy_cmplx_er0_bp
EXTRN CODE	: _num_cpy_cmplx_bp_er0
EXTRN CODE	: _num_cpy_cmplx_bpp20_er0
EXTRN CODE	: _num_cpy_cmplx
EXTRN CODE	: _num_eval__
EXTRN CODE	: _f_15EE4
EXTRN CODE	: _f_16CF0
EXTRN CODE	: _f_1A3FC
EXTRN CODE	: _f_1A410
EXTRN CODE	: _f_1A424
EXTRN CODE	: _f_1A438
EXTRN CODE	: _f_1A44C
EXTRN CODE	: _num_abs
EXTRN CODE	: _num_add_1
EXTRN CODE	: _f_1AE06
EXTRN CODE	: _f_1AFC8
EXTRN CODE	: _f_1AFD8
EXTRN CODE	: _num_sign
EXTRN CODE	: _num_cmp
EXTRN CODE	: _f_1B0DC
EXTRN CODE	: _num_fromdigit
EXTRN CODE	: _num_errorval
EXTRN CODE	: _num_negate
EXTRN CODE	: _f_1B238
EXTRN CODE	: _get_num_err_type
EXTRN CODE	: _num_mulxp__
EXTRN CODE	: _num_trunc__
EXTRN CODE	: _num_frombyte
EXTRN CODE	: _num_cpy
EXTRN CODE	: _f_1B4D0
EXTRN CODE	: _invalid_var
EXTRN DATA	: _d_080DC
EXTRN DATA	: _cursor_noflash
EXTRN DATA	: _d_080DE
EXTRN DATA	: _d_080DF
EXTRN DATA	: _last_key_scancode
EXTRN DATA	: _d_080F4
EXTRN DATA	: _last_key_keycode
EXTRN DATA	: _force_nochar
EXTRN DATA	: _modifiers
EXTRN DATA	: _mode
EXTRN DATA	: _submode
EXTRN DATA	: _screen_state
EXTRN DATA	: _table_mode
EXTRN DATA	: _d_080FD
EXTRN DATA	: _d_080FE
EXTRN DATA	: _result_template
EXTRN DATA	: _result_format
EXTRN DATA	: _d_08101
EXTRN DATA	: _setup_num_fmt
EXTRN DATA	: _setup_decimal_mark
EXTRN DATA	: _setup_angle_unit
EXTRN DATA	: _setup_mathi
EXTRN DATA	: _setup_cmplx_result
EXTRN DATA	: _setup_stat_freq
EXTRN DATA	: _setup_contrast
EXTRN DATA	: _font_size
EXTRN DATA	: _draw_mode
EXTRN DATA	: _use_rambuf
EXTRN DATA	: _d_08121
EXTRN DATA	: _use_output_charset
EXTRN DATA	: _arrow_state
EXTRN DATA	: _d_08126
EXTRN DATA	: _d_0812C
EXTRN DATA	: _result
EXTRN DATA	: _input_area
EXTRN DATA	: _vars_start
EXTRN DATA	: _var_m
EXTRN DATA	: _var_x
EXTRN DATA	: _mode_ram
EXTRN DATA	: _magic_string
EXTRN DATA	: _screen_buffer
EXTRN NUMBER: _screen_buffer_y1_neg
EXTRN DATA	: _stack_start


CSEG #0 AT 8H
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub
	DW _interrupt_stub

END
