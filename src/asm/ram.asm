TYPE(ML610CASESplus)
MODEL LARGE
ROMWINDOW 0, 7FFFH

DSEG #0 AT 8000H
; 08000
_ram_start:
; 8000H - 8009H is multipurpose
_reg0:
_d_08000:
	DS 2
; 08002
_d_08002:
	DS 2
; 08004
_d_08004:
	DS 1
; 08005
_d_08005:
	DS 1
; 08006
_d_08006:
	DS 1
; 08007
_d_08007:
	DS 1
; 08008
_d_08008:
	DS 1
; 08009
_d_08009:
_mathi_enable_draw:
	DS 1
; 0800A
_d_0800A:
	DS 1
; 0800B
_d_0800B:
	DS 1
; 0800C
_d_0800C:
	DS 1
; 0800D
_d_0800D:
	DS 1
; 0800E
_d_0800E:
	DS 1
; 0800F
_d_0800F:
	DS 1
; 08010
_reg1:
	DS 10
; 0801A
_d_0801A:
	DS 1
; 0801B
_d_0801B:
	DS 1
; 0801C
_d_0801C:
	DS 1
; 0801D
_d_0801D:
	DS 1
; From user202729's labelfile: (-, +, ÷, ×) = 9, 8, 7, 6
; 0801E
_arith_op:
	DS 1
; 0801F
_d_0801F:
	DS 1
; 08020
_reg2:
	DS 10
; 0802A
_d_0802A:
	DS 1
; 0802B
_d_0802B:
	DS 1
; 0802C
_d_0802C:
	DS 1
; 0802D
_d_0802D:
	DS 1
; 0802E
_d_0802E:
	DS 1
; 0802F
_d_0802F:
	DS 1
; 08030
_reg3:
	DS 10
; 0803A
_d_0803A:
	DS 1
; 0803B
_d_0803B:
	DS 1
; 0803C
_reg4:
	DS 10
; 08046
_reg5:
	DS 10
; 08050
_reg6:
	DS 10
_d_0805A:
	DS 1
; 0805B
_d_0805B:
	DS 1
; 0805C
_arg0_ref:
	DS 2
; 0805E
_arg1_ref:
	DS 2
; Operators are stored by ID.
; 08060
_operator_stack:
	DS 24
; 08078
_number_stack:
	DS 100
; 080DC
_d_080DC:
	DS 1
; 080DD
_cursor_noflash:
	DS 1
; 080DE
_d_080DE:
	DS 1
; 080DF
_d_080DF:
	DS 1
; 080E0
_matvct_dims:
	DS 18
; 080F2
_last_key_scancode:
	DS 2
; 080F4
_d_080F4:
	DS 1
; 080F5
_last_key_keycode:
	DS 1
; 080F6
_d_080F6:
	DS 1
; 080F7
_force_nochar:
	DS 1
; 080F8
_modifiers:
	DS 1
; 080F9
_mode:
	DS 1
; 080FA
_submode:
	DS 1
; 080FB
_screen_state:
	DS 1
; 080FC
_table_mode:
	DS 1
; 080FD
_d_080FD:
	DS 1
; 080FE
_d_080FE:
	DS 1
; 080FF
_result_template:
	DS 1
; 08100
_result_format:
	DS 1
; 08101
_d_08101:
	DS 1
; 08102
_setup_start:
_setup_num_fmt:
	DS 1
; 08103
_setup_num_fmt_n:
	DS 1
; 08104
_setup_decimal_mark:
	DS 1
; 08105
_setup_angle_unit:
	DS 1
; 08106
_setup_mathi:
	DS 1
; 08107
_setup_frac_result:
	DS 1
; 08108
_setup_cmplx_result:
	DS 1
; 08109
_setup_stat_freq:
	DS 1
; 0810A
_setup_rdec:
	DS 1
; 0810B
_setup_simp:
	DS 1
; 0810C
_setup_decimalo:
	DS 1
; 0810D
_d_0810D:
	DS 1
; 0810E
_setup_contrast:
	DS 1
; 0810F
_replay_idx:
	DS 1
; 08110
_cursor_pos_byte:
	DS 1
; 08111
_d_08111:
	DS 1
; 08112
_formula_x:
	DS 1
; 08113
_formula_y:
	DS 1
; 08114
_cursor_x:
	DS 1
; 08115
_cursor_y:
	DS 1
; 08116
_cursor_char:
	DS 1
; 08117
_d_08117:
	DS 1
; 08118
_table_viewport:
	DS 1
; 08119
_table_y:
	DS 1
; 0811A
_table_x:
	DS 1
; 0811B
_font_size:
	DS 1
; 0811C
_draw_mode:
	DS 1
; 0811D
_use_rambuf:
	DS 1
; 0811E
_d_0811E:
	DS 1
; 0811F
_d_0811F:
	DS 1
; 08120
_d_08120:
	DS 1
; 08121
_d_08121:
	DS 1
; 08122
_d_08122:
	DS 1
; 08123
_use_output_charset:
	DS 1
; 08124
_d_08124:
	DS 1
; 08125
_arrow_state:
	DS 1
; 08126
_d_08126:
	DS 1
; 08127
_d_08127:
	DS 1
; 08128
_input_area_ptr:
	DS 2
; 0812A
_d_0812A:
	DS 2
; 0812C
_d_0812C:
	DS 1
; 0812D
_ineq_mode:
	DS 1
; 0812E
_ratio_mode:
	DS 1
; 0812F
_d_0812F:
	DS 1
; 08130
_d_08130:
	DS 1
; 08131
_d_08131:
	DS 1
; 08132
_d_08132:
	DS 1
; 08133
_d_08133:
	DS 1
; 08134
_d_08134:
	DS 1
; 08135
_d_08135:
	DS 1
; 08136
_d_08136:
	DS 1
; 08137
_d_08137:
	DS 1
; 08138
_d_08138:
	DS 1
; 08139
_d_08139:
	DS 1
; 0813A
_d_0813A:
	DS 1
; 0813B
_d_0813B:
	DS 1
; 0813C
_d_0813C:
	DS 1
; 0813D
_d_0813D:
	DS 1
; 0813E
_d_0813E:
	DS 1
; 0813F
_d_0813F:
	DS 1
; 08140
_result:
	DS 20
; 08154
_input_area:
	DS 100
; 081B8
_cache_area:
	DS 100
; 0821C
_random_seed:
	DS 8
; 08224
_timer:
	DS 2
; 08226
_vars_start:
_var_m:
	DS 10
; 08230
_var_ans:
	DS 10
; 0823A
_var_a:
	DS 10
; 08244
_var_b:
	DS 10
; 0824E
_var_c:
	DS 10
; 08258
_var_d:
	DS 10
; 08262
_var_e:
	DS 10
; 0826C
_var_f:
	DS 10
; 08276
_var_x:
	DS 10
; 08280
_var_y:
	DS 10
; 0828A
_d_0828A:
	DS 10
; 08294
_d_08294:
	DS 10
; 0829E
_mode_ram:
	DS 880
; 0860E
_magic_string:
	DS 16
; 0861E
_d_0861E:
	DS 1
; 0861F
_d_0861F:
	DS 1
; 08620
_d_08620:
	DS 1
; 08621
_d_08621:
	DS 1
; 08622
_d_08622:
	DS 1
; 08623
_d_08623:
	DS 1
; 08624
_d_08624:
	DS 1
; 08625
_d_08625:
	DS 1
; 08626
_d_08626:
	DS 1
; 08627
_d_08627:
	DS 1
; 08628
_d_08628:
	DS 1
; 08629
_d_08629:
	DS 1
; 0862A
_d_0862A:
	DS 1
; 0862B
_d_0862B:
	DS 1
; 0862C
_d_0862C:
	DS 1
; 0862D
_d_0862D:
	DS 1
; 0862E
_d_0862E:
	DS 1
; 0862F
_d_0862F:
	DS 1
; 08630
_d_08630:
	DS 1
; 08631
_d_08631:
	DS 1
; 08632
_d_08632:
	DS 1
; 08633
_d_08633:
	DS 1
; 08634
_d_08634:
	DS 1
; 08635
_d_08635:
	DS 1
; 08636
_d_08636:
	DS 1
; 08637
_d_08637:
	DS 1
; 08638
_d_08638:
	DS 1
; 08639
_d_08639:
	DS 1
; 0863A
_d_0863A:
	DS 1
; 0863B
_d_0863B:
	DS 1
; 0863C
_d_0863C:
	DS 1
; 0863D
_d_0863D:
	DS 1
; 0863E
_d_0863E:
	DS 1
; 0863F
_d_0863F:
	DS 1
; 08640
_d_08640:
	DS 400
; 087D0
_screen_buffer:
	DS 384
; 08950
_d_08950:
	DS 1
; 08951
_d_08951:
	DS 1
; 08952
_d_08952:
	DS 1
; 08953
_d_08953:
	DS 1
; 08954
_d_08954:
	DS 1
; 08955
_d_08955:
	DS 1
; 08956
_d_08956:
	DS 1
; 08957
_d_08957:
	DS 1
; 08958
_d_08958:
	DS 1
; 08959
_d_08959:
	DS 1
; 0895A
_d_0895A:
	DS 1
; 0895B
_d_0895B:
	DS 1
; 0895C
_d_0895C:
	DS 1
; 0895D
_d_0895D:
	DS 1
; 0895E
_d_0895E:
	DS 1
; 0895F
_d_0895F:
	DS 1
; 08960
_d_08960:
	DS 1
; 08961
_d_08961:
	DS 1
; 08962
_d_08962:
	DS 1
; 08963
_d_08963:
	DS 1
; 08964
_d_08964:
	DS 1
; 08965
_d_08965:
	DS 1
; 08966
_d_08966:
	DS 1
; 08967
_d_08967:
	DS 1
; 08968
_d_08968:
	DS 1
; 08969
_d_08969:
	DS 1
; 0896A
_d_0896A:
	DS 1
; 0896B
_d_0896B:
	DS 1
; 0896C
_d_0896C:
	DS 1
; 0896D
_d_0896D:
	DS 1
; 0896E
_d_0896E:
	DS 1
; 0896F
_d_0896F:
	DS 1
; 08970
_d_08970:
	DS 1
; 08971
_d_08971:
	DS 1
; 08972
_d_08972:
	DS 1
; 08973
_d_08973:
	DS 1
; 08974
_d_08974:
	DS 1
; 08975
_d_08975:
	DS 1
; 08976
_d_08976:
	DS 1
; 08977
_d_08977:
	DS 1
; 08978
_d_08978:
	DS 1
; 08979
_d_08979:
	DS 1
; 0897A
_d_0897A:
	DS 1
; 0897B
_d_0897B:
	DS 1
; 0897C
_d_0897C:
	DS 1
; 0897D
_d_0897D:
	DS 1
; 0897E
_d_0897E:
	DS 1
; 0897F
_d_0897F:
	DS 1
; 08980
_d_08980:
	DS 1
; 08981
_d_08981:
	DS 1
; 08982
_d_08982:
	DS 1
; 08983
_d_08983:
	DS 1
; 08984
_d_08984:
	DS 1
; 08985
_d_08985:
	DS 1
; 08986
_d_08986:
	DS 1
; 08987
_d_08987:
	DS 1
; 08988
_d_08988:
	DS 1
; 08989
_d_08989:
	DS 1
; 0898A
_d_0898A:
	DS 1
; 0898B
_d_0898B:
	DS 1
; 0898C
_d_0898C:
	DS 1
; 0898D
_d_0898D:
	DS 1
; 0898E
_d_0898E:
	DS 1
; 0898F
_d_0898F:
	DS 1
; 08990
_d_08990:
	DS 1
; 08991
_d_08991:
	DS 1
; 08992
_d_08992:
	DS 1
; 08993
_d_08993:
	DS 1
; 08994
_d_08994:
	DS 1
; 08995
_d_08995:
	DS 1
; 08996
_d_08996:
	DS 1
; 08997
_d_08997:
	DS 1
; 08998
_d_08998:
	DS 1
; 08999
_d_08999:
	DS 1
; 0899A
_d_0899A:
	DS 1
; 0899B
_d_0899B:
	DS 1
; 0899C
_d_0899C:
	DS 1
; 0899D
_d_0899D:
	DS 1
; 0899E
_d_0899E:
	DS 1
; 0899F
_d_0899F:
	DS 1
; 089A0
_d_089A0:
	DS 1
; 089A1
_d_089A1:
	DS 1
; 089A2
_d_089A2:
	DS 1
; 089A3
_d_089A3:
	DS 1
; 089A4
_d_089A4:
	DS 1
; 089A5
_d_089A5:
	DS 1
; 089A6
_d_089A6:
	DS 1
; 089A7
_d_089A7:
	DS 1
; 089A8
_d_089A8:
	DS 1
; 089A9
_d_089A9:
	DS 1
; 089AA
_d_089AA:
	DS 1
; 089AB
_d_089AB:
	DS 1
; 089AC
_d_089AC:
	DS 1
; 089AD
_d_089AD:
	DS 1
; 089AE
_d_089AE:
	DS 1
; 089AF
_d_089AF:
	DS 1
; 089B0
_d_089B0:
	DS 1
; 089B1
_d_089B1:
	DS 1
; 089B2
_d_089B2:
	DS 1
; 089B3
_d_089B3:
	DS 1
; 089B4
_d_089B4:
	DS 1
; 089B5
_d_089B5:
	DS 1
; 089B6
_d_089B6:
	DS 1
; 089B7
_d_089B7:
	DS 1
; 089B8
_d_089B8:
	DS 1
; 089B9
_d_089B9:
	DS 1
; 089BA
_d_089BA:
	DS 1
; 089BB
_d_089BB:
	DS 1
; 089BC
_d_089BC:
	DS 1
; 089BD
_d_089BD:
	DS 1
; 089BE
_d_089BE:
	DS 1
; 089BF
_d_089BF:
	DS 1
; 089C0
_d_089C0:
	DS 1
; 089C1
_d_089C1:
	DS 1
; 089C2
_d_089C2:
	DS 1
; 089C3
_d_089C3:
	DS 1
; 089C4
_d_089C4:
	DS 1
; 089C5
_d_089C5:
	DS 1
; 089C6
_d_089C6:
	DS 1
; 089C7
_d_089C7:
	DS 1
; 089C8
_d_089C8:
	DS 1
; 089C9
_d_089C9:
	DS 1
; 089CA
_d_089CA:
	DS 1
; 089CB
_d_089CB:
	DS 1
; 089CC
_d_089CC:
	DS 1
; 089CD
_d_089CD:
	DS 1
; 089CE
_d_089CE:
	DS 1
; 089CF
_d_089CF:
	DS 1
; 089D0
_d_089D0:
	DS 1
; 089D1
_d_089D1:
	DS 1
; 089D2
_d_089D2:
	DS 1
; 089D3
_d_089D3:
	DS 1
; 089D4
_d_089D4:
	DS 1
; 089D5
_d_089D5:
	DS 1
; 089D6
_d_089D6:
	DS 1
; 089D7
_d_089D7:
	DS 1
; 089D8
_d_089D8:
	DS 1
; 089D9
_d_089D9:
	DS 1
; 089DA
_d_089DA:
	DS 1
; 089DB
_d_089DB:
	DS 1
; 089DC
_d_089DC:
	DS 1
; 089DD
_d_089DD:
	DS 1
; 089DE
_d_089DE:
	DS 1
; 089DF
_d_089DF:
	DS 1
; 089E0
_d_089E0:
	DS 1
; 089E1
_d_089E1:
	DS 1
; 089E2
_d_089E2:
	DS 1
; 089E3
_d_089E3:
	DS 1
; 089E4
_d_089E4:
	DS 1
; 089E5
_d_089E5:
	DS 1
; 089E6
_d_089E6:
	DS 1
; 089E7
_d_089E7:
	DS 1
; 089E8
_d_089E8:
	DS 1
; 089E9
_d_089E9:
	DS 1
; 089EA
_d_089EA:
	DS 1
; 089EB
_d_089EB:
	DS 1
; 089EC
_d_089EC:
	DS 1
; 089ED
_d_089ED:
	DS 1
; 089EE
_d_089EE:
	DS 1
; 089EF
_d_089EF:
	DS 1
; 089F0
_d_089F0:
	DS 1
; 089F1
_d_089F1:
	DS 1
; 089F2
_d_089F2:
	DS 1
; 089F3
_d_089F3:
	DS 1
; 089F4
_d_089F4:
	DS 1
; 089F5
_d_089F5:
	DS 1
; 089F6
_d_089F6:
	DS 1
; 089F7
_d_089F7:
	DS 1
; 089F8
_d_089F8:
	DS 1
; 089F9
_d_089F9:
	DS 1
; 089FA
_d_089FA:
	DS 1
; 089FB
_d_089FB:
	DS 1
; 089FC
_d_089FC:
	DS 1
; 089FD
_d_089FD:
	DS 1
; 089FE
_d_089FE:
	DS 1
; 089FF
_d_089FF:
	DS 1
; 08A00
_d_08A00:
	DS 1
; 08A01
_d_08A01:
	DS 1
; 08A02
_d_08A02:
	DS 1
; 08A03
_d_08A03:
	DS 1
; 08A04
_d_08A04:
	DS 1
; 08A05
_d_08A05:
	DS 1
; 08A06
_d_08A06:
	DS 1
; 08A07
_d_08A07:
	DS 1
; 08A08
_d_08A08:
	DS 1
; 08A09
_d_08A09:
	DS 1
; 08A0A
_d_08A0A:
	DS 1
; 08A0B
_d_08A0B:
	DS 1
; 08A0C
_d_08A0C:
	DS 1
; 08A0D
_d_08A0D:
	DS 1
; 08A0E
_d_08A0E:
	DS 1
; 08A0F
_d_08A0F:
	DS 1
; 08A10
_d_08A10:
	DS 1
; 08A11
_d_08A11:
	DS 1
; 08A12
_d_08A12:
	DS 1
; 08A13
_d_08A13:
	DS 1
; 08A14
_d_08A14:
	DS 1
; 08A15
_d_08A15:
	DS 1
; 08A16
_d_08A16:
	DS 1
; 08A17
_d_08A17:
	DS 1
_stack_start:

; This is used in draw_byte... Casio why???
_screen_buffer_y1_neg EQU -(OFFSET _screen_buffer + 12)
PUBLIC _screen_buffer_y1_neg

PUBLIC _ram_start
PUBLIC _d_08000
PUBLIC _d_08002
PUBLIC _d_08004
PUBLIC _d_08005
PUBLIC _d_08006
PUBLIC _d_08007
PUBLIC _d_08008
PUBLIC _d_08009            ; } same address
PUBLIC _mathi_enable_draw  ; }
PUBLIC _d_0800A
PUBLIC _d_0800B
PUBLIC _d_0800C
PUBLIC _d_0800D
PUBLIC _d_0800E
PUBLIC _d_0800F
PUBLIC _reg1
PUBLIC _d_0801A
PUBLIC _d_0801C
PUBLIC _d_0801D
PUBLIC _arith_op
PUBLIC _d_0801F
PUBLIC _reg2
PUBLIC _d_0802A
PUBLIC _d_0802B
PUBLIC _d_0802C
PUBLIC _d_0802D
PUBLIC _d_0802E
PUBLIC _d_0802F
PUBLIC _reg3
PUBLIC _d_0803A
PUBLIC _d_0803B
PUBLIC _reg4
PUBLIC _reg5
PUBLIC _reg6
PUBLIC _d_0805A
PUBLIC _d_0805B
PUBLIC _arg0_ref
PUBLIC _arg1_ref
PUBLIC _operator_stack
PUBLIC _number_stack
PUBLIC _d_080DC
PUBLIC _cursor_noflash
PUBLIC _d_080DE
PUBLIC _d_080DF
PUBLIC _matvct_dims
PUBLIC _last_key_scancode
PUBLIC _d_080F4
PUBLIC _last_key_keycode
PUBLIC _force_nochar
PUBLIC _modifiers
PUBLIC _mode
PUBLIC _submode
PUBLIC _screen_state
PUBLIC _table_mode
PUBLIC _d_080FD
PUBLIC _d_080FE
PUBLIC _result_template
PUBLIC _result_format
PUBLIC _d_08101
PUBLIC _setup_start
PUBLIC _setup_num_fmt
PUBLIC _setup_num_fmt_n
PUBLIC _setup_decimal_mark
PUBLIC _setup_angle_unit
PUBLIC _setup_mathi
PUBLIC _setup_frac_result
PUBLIC _setup_cmplx_result
PUBLIC _setup_stat_freq
PUBLIC _setup_rdec
PUBLIC _setup_simp
PUBLIC _setup_decimalo
PUBLIC _setup_contrast
PUBLIC _replay_idx
PUBLIC _cursor_pos_byte
PUBLIC _d_08111
PUBLIC _formula_x
PUBLIC _formula_y
PUBLIC _cursor_x
PUBLIC _cursor_y
PUBLIC _cursor_char
PUBLIC _d_08117
PUBLIC _table_viewport
PUBLIC _table_y
PUBLIC _table_x
PUBLIC _font_size
PUBLIC _draw_mode
PUBLIC _use_rambuf
PUBLIC _d_08120
PUBLIC _d_08121
PUBLIC _d_08122
PUBLIC _use_output_charset
PUBLIC _d_08124
PUBLIC _arrow_state
PUBLIC _d_08126
PUBLIC _input_area_ptr
PUBLIC _d_0812A
PUBLIC _d_0812C
PUBLIC _ineq_mode
PUBLIC _ratio_mode
PUBLIC _result
PUBLIC _input_area
PUBLIC _cache_area
PUBLIC _random_seed
PUBLIC _timer
PUBLIC _vars_start
PUBLIC _var_m
PUBLIC _var_ans
PUBLIC _var_x
PUBLIC _mode_ram
PUBLIC _magic_string
PUBLIC _d_08640
PUBLIC _screen_buffer
PUBLIC _stack_start
