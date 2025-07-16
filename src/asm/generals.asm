TYPE(ML610CASESplus)
MODEL LARGE
ROMWINDOW 0, 7FFFH

; NOTE:
; generals may be partially written in C (at least 1 C source and 1 ASM source), however full assembly will be used for now.
; generals may be made up of multiple ASM files. For now, these are combined into a single ASM file.

_d_080DC			EQU 80DCH
_cursor_noflash		EQU 80DDH
_d_080DE			EQU 80DEH
_d_080DF			EQU 80DFH
_last_key_scancode	EQU 80F2H
_d_080F4			EQU 80F4H
_last_key_keycode	EQU 80F5H
_d_080F7			EQU 80F7H
_modifiers			EQU 80F8H
_mode				EQU 80F9H
_submode			EQU 80FAH
_screen_state		EQU 80FBH
_table_mode			EQU 80FCH
_d_080FD			EQU 80FDH
_d_080FE			EQU 80FEH
_d_080FF			EQU 80FFH
_d_08100			EQU 8100H
_d_08101			EQU 8101H
_setup_num_fmt		EQU 8102H
_setup_decimal_mark	EQU 8104H
_setup_angle_unit	EQU 8105H
_setup_mathi		EQU 8106H
_setup_cmplx_result	EQU 8108H
_setup_stat_freq	EQU 8109H
_setup_contrast		EQU 810EH
_font_size			EQU 811BH
_draw_mode			EQU 811CH
_use_rambuf			EQU 811DH
_d_08121			EQU 8121H
_use_output_charset	EQU 8123H
_d_08125			EQU 8125H
_d_08126			EQU 8126H
_d_0812C			EQU 812CH
_result				EQU 8140H
_input_area			EQU 8154H
_vars_start			EQU 8226H
_var_m				EQU 8226H
_var_x				EQU 8276H
_vars_im_start		EQU 8408H
_var_m_im			EQU 8408H
_calc_history		EQU 829EH
_d_0850A			EQU 850AH
_d_08514			EQU 8514H
_d_0851E			EQU 851EH
_d_08528			EQU 8528H
_d_08532			EQU 8532H
_table_fx			EQU 8546H
_d_085AA			EQU 85AAH
_d_085B4			EQU 85B4H
_d_085C8			EQU 85C8H
_d_085CA			EQU 85CAH
_d_085CD			EQU 85CDH
_magic_string		EQU 860EH
_screen_buffer		EQU 87D0H
_screen_buffer_1	EQU _screen_buffer+1
_screen_buffer_2	EQU _screen_buffer+2
_screen_buffer_3	EQU _screen_buffer+3
_screen_buffer_4	EQU _screen_buffer+4
_screen_buffer_5	EQU _screen_buffer+5
_screen_buffer_6	EQU _screen_buffer+6
_screen_buffer_7	EQU _screen_buffer+7
_screen_buffer_8	EQU _screen_buffer+8
_screen_buffer_9	EQU _screen_buffer+9
_screen_buffer_10	EQU _screen_buffer+10
_screen_buffer_11	EQU _screen_buffer+11
_screen_buffer_end	EQU _screen_buffer+12*32-1
_stack_start		EQU 8A18H
_stop_type			EQU 8E00H

_pd_val_emu			EQU 0F050H
_real_screen		EQU 0F800H
_real_screen_end	EQU _real_screen+16*32-1

$$NCODgenerals SEGMENT CODE 2H ANY
$$NTABgenerals0 SEGMENT TABLE 2H #0
$$NTABgenerals1 SEGMENT TABLE 2H #0
$$NTABgenerals2 SEGMENT TABLE 2H #0
$$NTABgenerals3 SEGMENT TABLE 2H #0
$$NTABgenerals4 SEGMENT TABLE 2H #0

RSEG $$NTABgenerals0

; How the font works:
; Every 8 characters in a font is defined by 5 (4 for the "tiny" font) 8x9 1-bit bitmap images merged together.
; For example, for the big 5x9 font, for every image we take 8 bytes, convert to binary,
; place each pixel in one row, from left (MSB) to right (LSB) and stack them top to bottom.
; For example: We want to get the first 8 characters of the font. So we take the first 9*5 = 45 bytes.
; Converting to image, we get the following:
;Addr 22      2b      34      3d      46      
;Char 20   21   22   23   24   25   26   27   
; +0  .......#..##.##.#.#.................##..
; +1  .......#..##.##.#.#.................##..
; +2  .......#...#..#######...###.....#....#..
; +3  .......#..#..#..#.#..#.#.##..#......#...
; +4  .......#........#.#...#.....#.#####.....
; +5  .......#........#.#..#.#...#............
; +6  ...............######...#.#.....#.......
; +7  .......#........#.#......#..##..........
; +8  .......#........#.#.........##..........
; If the characters are hard to make out, no worries! Just cut it into 5x9 chunks:
;Char 20    21    22    23    24    25    26    27   
; 0   ..... ..#.. ##.## .#.#. ..... ..... ..... .##..
; 1   ..... ..#.. ##.## .#.#. ..... ..... ..... .##..
; 2   ..... ..#.. .#..# ##### #...# ##... ..#.. ..#..
; 3   ..... ..#.. #..#. .#.#. .#.#. ##..# ..... .#...
; 4   ..... ..#.. ..... .#.#. ..#.. ...#. ##### .....
; 5   ..... ..#.. ..... .#.#. .#.#. ..#.. ..... .....
; 6   ..... ..... ..... ##### #...# .#... ..#.. .....
; 7   ..... ..#.. ..... .#.#. ..... #..## ..... .....
; 8   ..... ..#.. ..... .#.#. ..... ...## ..... .....
; Rinse and repeat for the remaining characters.

; How the fonts are stored:
; The default character set for the 9px ("big") and 6px ("small") fonts contains a total of
; 176 (0xB0) characters, from 0x20 to 0xCF.
; The default character set for the 5px ("tiny") fonts contains a total of 45 (techincally 46,
; but 0xFD is unused) characters, from 0xD0 to 0xFC. These characters are only used in _modes with
; small input boxes, e.g. TABLE, EQN, MATRIX, VECTOR, etc.
; The "localization" character set can contain up to 31 (0x1F) chraracters, starting from index 0x01.
; However, in practice, most of these character slots are left completely unused.
; In GY builds (including this decomp), only 11 characters are actually used. 3 additional characters
; were filled up in LY builds.

; DATA: GY454XE  Re 00022
_font_big_0:
	DB	01H,	01H,	01H,	01H,	01H,	01H,	00H,	01H,	01H
	DB	36H,	36H,	13H,	24H,	00H,	00H,	01H,	00H,	00H
	DB	0A0H,	0A0H,	0F8H,	0A5H,	0A2H,	0A5H,	0F8H,	0A0H,	0A0H
	DB	00H,	00H,	0E0H,	64H,	0BH,	10H,	0A0H,	4CH,	0CH
	DB	0CH,	0CH,	84H,	08H,	0E0H,	00H,	80H,	00H,	00H
	DB	12H,	21H,	40H,	40H,	40H,	40H,	40H,	21H,	12H
	DB	00H,	00H,	80H,	80H,	99H,	98H,	80H,	00H,	00H
	DB	00H,	00H,	40H,	40H,	0F0H,	46H,	46H,	02H,	04H
	DB	00H,	00H,	00H,	00H,	7CH,	00H,	00H,	01H,	01H
	DB	00H,	02H,	02H,	04H,	04H,	08H,	08H,	90H,	90H
	DB	71H,	8BH,	89H,	89H,	89H,	89H,	89H,	89H,	73H
	DB	1CH,	23H,	23H,	02H,	04H,	08H,	11H,	21H,	0BEH
	DB	0E1H,	13H,	13H,	15H,	65H,	19H,	1FH,	11H,	0E1H
	DB	7CH,	41H,	42H,	7AH,	47H,	06H,	86H,	46H,	39H
	DB	0DFH,	11H,	01H,	02H,	0C2H,	22H,	24H,	24H,	0C4H
	DB	73H,	8CH,	8CH,	8CH,	73H,	88H,	88H,	88H,	73H
	DB	80H,	58H,	58H,	40H,	0C0H,	58H,	58H,	80H,	00H
	DB	00H,	0C1H,	0C2H,	04H,	08H,	0C4H,	0C2H,	41H,	80H
	DB	00H,	02H,	01H,	7CH,	00H,	7CH,	01H,	02H,	00H
	DB	0EH,	11H,	11H,	81H,	42H,	84H,	00H,	04H,	04H
	DB	01H,	72H,	8AH,	0BCH,	0ACH,	0AFH,	0BCH,	84H,	74H
	DB	3CH,	0A3H,	0A3H,	63H,	7DH,	0E3H,	63H,	63H,	7CH
	DB	0EEH,	19H,	08H,	08H,	08H,	08H,	08H,	19H,	0EEH
	DB	7FH,	42H,	0C2H,	0C2H,	0FBH,	0C2H,	0C2H,	42H,	7EH
	DB	0EEH,	11H,	10H,	10H,	0D7H,	11H,	11H,	13H,	0DH
	DB	8BH,	89H,	89H,	89H,	0F9H,	89H,	89H,	89H,	8BH
	DB	9FH,	05H,	05H,	05H,	05H,	05H,	25H,	25H,	99H
	DB	18H,	28H,	28H,	48H,	88H,	48H,	28H,	28H,	1FH
	DB	46H,	47H,	6FH,	6EH,	56H,	56H,	46H,	46H,	0C6H
	DB	2EH,	31H,	31H,	0B1H,	0B1H,	71H,	71H,	31H,	2EH
	DB	0F3H,	8CH,	8CH,	8CH,	0F4H,	84H,	85H,	86H,	83H
	DB	0BCH,	63H,	63H,	63H,	7CH,	68H,	65H,	0A5H,	0E2H
	DB	0EFH,	12H,	12H,	02H,	0E2H,	12H,	12H,	12H,	0E2H
	DB	0C6H,	46H,	46H,	46H,	45H,	45H,	45H,	44H,	38H
	DB	31H,	31H,	35H,	35H,	55H,	55H,	4AH,	8AH,	8AH
	DB	8CH,	8CH,	52H,	22H,	21H,	51H,	51H,	89H,	89H
	DB	7EH,	42H,	84H,	84H,	08H,	10H,	10H,	20H,	3EH
	DB	0EFH,	89H,	89H,	8FH,	80H,	80H,	80H,	80H,	0E0H
	DB	38H,	09H,	0AH,	08H,	08H,	08H,	08H,	08H,	38H
	DB	80H,	40H,	20H,	00H,	00H,	00H,	00H,	00H,	1FH
	DB	00H,	00H,	03H,	04H,	71H,	02H,	04H,	04H,	03H
	DB	20H,	20H,	0ACH,	73H,	0E3H,	63H,	63H,	0F3H,	6CH
	DB	00H,	00H,	0E6H,	19H,	08H,	08H,	08H,	19H,	0E6H
	DB	80H,	80H,	0B9H,	0C4H,	0C4H,	0FCH,	0C0H,	0C4H,	0B8H
	DB	60H,	80H,	0CFH,	91H,	91H,	91H,	8FH,	81H,	8EH
	DB	81H,	80H,	83H,	0B1H,	0C9H,	89H,	89H,	89H,	8BH
	DB	05H,	01H,	0DH,	05H,	05H,	05H,	05H,	25H,	99H
	DB	06H,	02H,	12H,	22H,	42H,	82H,	42H,	22H,	17H
	DB	00H,	00H,	6AH,	57H,	56H,	56H,	56H,	56H,	56H
	DB	00H,	00H,	0CEH,	31H,	31H,	31H,	31H,	31H,	2EH
	DB	00H,	00H,	0F3H,	8CH,	8CH,	8CH,	0CCH,	0B3H,	80H
	DB	00H,	00H,	0ECH,	73H,	61H,	60H,	0E0H,	61H,	60H
	DB	04H,	04H,	0EFH,	14H,	04H,	0E4H,	14H,	14H,	0E3H
	DB	00H,	00H,	46H,	46H,	46H,	45H,	45H,	0CCH,	34H
	DB	00H,	00H,	31H,	35H,	35H,	55H,	55H,	8AH,	8AH
	DB	00H,	00H,	8CH,	8CH,	52H,	22H,	51H,	89H,	8EH
	DB	00H,	00H,	7EH,	42H,	84H,	88H,	10H,	20H,	3EH
	DB	38H,	48H,	48H,	48H,	88H,	48H,	48H,	48H,	38H
	DB	60H,	10H,	10H,	11H,	0AH,	12H,	10H,	10H,	60H
	DB	00H,	00H,	00H,	00H,	0A0H,	0A0H,	40H,	00H,	00H
	DB	00H,	60H,	01H,	0E2H,	66H,	67H,	66H,	66H,	0F3H
	DB	00H,	00H,	81H,	63H,	95H,	09H,	15H,	63H,	81H
	DB	00H,	00H,	70H,	55H,	5AH,	5AH,	55H,	50H,	70H
	DB	32H,	4BH,	4AH,	4AH,	0B2H,	80H,	00H,	00H,	00H
	DB	0CEH,	12H,	12H,	0EH,	02H,	0CH,	00H,	00H,	00H
	DB	00H,	17H,	10H,	20H,	26H,	41H,	41H,	83H,	0FCH
	DB	00H,	0FEH,	01H,	00H,	63H,	92H,	0CH,	08H,	0F1H
	DB	42H,	0A5H,	18H,	00H,	98H,	64H,	43H,	0C2H,	3CH
	DB	00H,	00H,	90H,	08H,	0FCH,	89H,	13H,	01H,	00H
	DB	00H,	08H,	24H,	3EH,	0A1H,	3EH,	0E4H,	08H,	80H
	DB	00H,	00H,	00H,	00H,	8DH,	55H,	25H,	55H,	8DH
	DB	00H,	00H,	1EH,	00H,	0C0H,	6EH,	6AH,	6AH,	0EEH
	DB	01H,	02H,	04H,	08H,	14H,	12H,	11H,	10H,	0FFH
	DB	02H,	09H,	08H,	7CH,	10H,	7DH,	22H,	20H,	03H
	DB	00H,	00H,	80H,	40H,	80H,	01H,	03H,	07H,	0CFH
	DB	38H,	21H,	21H,	21H,	21H,	21H,	0A1H,	65H,	22H
	DB	80H,	40H,	00H,	1DH,	23H,	23H,	3FH,	23H,	23H
	DB	00H,	00H,	00H,	0E7H,	18H,	0E8H,	18H,	18H,	0E7H
	DB	00H,	01H,	01H,	59H,	0E5H,	45H,	45H,	0C5H,	44H
	DB	00H,	02H,	86H,	0CEH,	0FEH,	0CEH,	86H,	02H,	00H
	DB	71H,	8BH,	89H,	89H,	89H,	73H,	00H,	00H,	00H
	DB	1CH,	23H,	04H,	08H,	11H,	0BEH,	00H,	00H,	00H
	DB	0E1H,	13H,	65H,	19H,	1FH,	0E1H,	00H,	00H,	00H
	DB	7CH,	41H,	7BH,	06H,	0C6H,	39H,	00H,	00H,	00H
	DB	0DFH,	01H,	0C2H,	22H,	24H,	0C4H,	00H,	00H,	00H
	DB	73H,	8CH,	74H,	8BH,	88H,	73H,	00H,	00H,	00H
	DB	87H,	42H,	7AH,	0C2H,	83H,	02H,	00H,	00H,	00H
	DB	9BH,	6AH,	4AH,	0CAH,	3AH,	0BH,	00H,	00H,	00H
	DB	80H,	80H,	80H,	89H,	90H,	90H,	10H,	10H,	09H
	DB	00H,	00H,	04H,	04H,	9FH,	84H,	84H,	80H,	1FH
	DB	00H,	00H,	00H,	71H,	8BH,	89H,	89H,	89H,	73H
	DB	00H,	00H,	00H,	1CH,	22H,	05H,	08H,	10H,	0BEH
	DB	00H,	00H,	00H,	3FH,	18H,	0DFH,	18H,	18H,	18H
	DB	00H,	00H,	00H,	0C7H,	66H,	56H,	4FH,	46H,	46H
	DB	00H,	00H,	00H,	0C0H,	2AH,	2AH,	0CAH,	15H,	10H
	DB	77H,	0CEH,	0CEH,	0CEH,	0CFH,	0FEH,	0CEH,	0CEH,	0CFH
	DB	9DH,	73H,	71H,	71H,	0B1H,	71H,	71H,	73H,	9DH
	DB	0CFH,	0ACH,	9CH,	9CH,	9FH,	9CH,	9CH,	0ACH,	0CFH
	DB	0FFH,	63H,	63H,	63H,	7BH,	63H,	63H,	63H,	0E3H
	DB	0C0H,	28H,	2CH,	2AH,	0C9H,	0AH,	0CH,	08H,	00H
	DB	0F8H,	88H,	42H,	45H,	24H,	44H,	44H,	8DH,	0FAH
	DB	00H,	00H,	72H,	4CH,	88H,	88H,	91H,	91H,	50H
	DB	03H,	04H,	64H,	98H,	8FH,	0E8H,	09H,	19H,	0E6H
	DB	60H,	90H,	91H,	91H,	0A9H,	0A9H,	2AH,	46H,	46H
	DB	00H,	00H,	5FH,	4AH,	4AH,	4AH,	0AAH,	2BH,	12H
	DB	00H,	00H,	1BH,	65H,	95H,	95H,	93H,	92H,	62H
	DB	88H,	94H,	94H,	55H,	54H,	54H,	94H,	08H,	36H
	DB	8FH,	8FH,	0CFH,	8FH,	8FH,	0EFH,	9FH,	9FH,	9FH
	DB	0FCH,	0C4H,	0C4H,	0C5H,	0C6H,	0C4H,	0C4H,	0C6H,	0FDH
	DB	00H,	00H,	00H,	0C0H,	20H,	0C0H,	20H,	3FH,	0DFH

; DATA: GY454XE  Re 00400
_font_small_0:
	DB	01H,	01H,	01H,	01H,	00H,	01H
	DB	36H,	12H,	25H,	00H,	01H,	00H
	DB	00H,	0A8H,	0F5H,	0A2H,	0F5H,	0A8H
	DB	00H,	0E4H,	68H,	13H,	2CH,	0CCH
	DB	0CH,	84H,	08H,	0E0H,	00H,	80H
	DB	12H,	21H,	21H,	21H,	21H,	12H
	DB	00H,	00H,	18H,	19H,	00H,	00H
	DB	00H,	40H,	40H,	0F6H,	42H,	44H
	DB	00H,	00H,	00H,	7CH,	01H,	01H
	DB	00H,	01H,	02H,	04H,	88H,	90H
	DB	71H,	8BH,	89H,	89H,	89H,	73H
	DB	1CH,	23H,	04H,	08H,	11H,	0BEH
	DB	0E1H,	13H,	65H,	19H,	1FH,	0E1H
	DB	7CH,	41H,	7BH,	06H,	0C6H,	39H
	DB	0DFH,	01H,	0C2H,	22H,	24H,	0C4H
	DB	73H,	8CH,	74H,	8BH,	88H,	73H
	DB	80H,	58H,	58H,	0C0H,	98H,	18H
	DB	0C0H,	0C1H,	02H,	0C4H,	42H,	81H
	DB	00H,	01H,	7CH,	00H,	7CH,	01H
	DB	0EH,	11H,	82H,	44H,	80H,	04H
	DB	71H,	8AH,	0BCH,	0AFH,	9CH,	64H
	DB	3CH,	0A3H,	7DH,	0E3H,	63H,	7CH
	DB	0EEH,	19H,	08H,	08H,	19H,	0EEH
	DB	7FH,	42H,	0FBH,	0C2H,	42H,	7EH
	DB	0EEH,	11H,	0D0H,	17H,	11H,	0FH
	DB	8BH,	89H,	0F9H,	89H,	89H,	8BH
	DB	8FH,	05H,	05H,	05H,	25H,	99H
	DB	18H,	28H,	48H,	0C8H,	28H,	1FH
	DB	46H,	6FH,	56H,	56H,	46H,	0C6H
	DB	2EH,	31H,	0B1H,	71H,	31H,	2EH
	DB	0F3H,	8CH,	8CH,	0F5H,	84H,	83H
	DB	0BCH,	63H,	62H,	7CH,	0A5H,	62H
	DB	0EFH,	12H,	0C2H,	22H,	12H,	0E2H
	DB	0C6H,	46H,	45H,	45H,	44H,	38H
	DB	31H,	35H,	55H,	55H,	8AH,	8AH
	DB	8CH,	54H,	22H,	51H,	89H,	89H
	DB	7EH,	42H,	84H,	08H,	10H,	3EH
	DB	0EEH,	8AH,	8EH,	80H,	80H,	0E0H
	DB	38H,	09H,	0AH,	08H,	08H,	38H
	DB	80H,	40H,	20H,	00H,	00H,	1FH
	DB	00H,	03H,	00H,	73H,	04H,	03H
	DB	20H,	0ACH,	73H,	0E3H,	63H,	0FCH
	DB	00H,	0E6H,	09H,	08H,	18H,	0E7H
	DB	80H,	0B8H,	0C5H,	0FCH,	0C0H,	0B8H
	DB	60H,	8FH,	0D1H,	8FH,	81H,	8EH
	DB	81H,	80H,	0B3H,	0C9H,	89H,	8BH
	DB	04H,	00H,	04H,	04H,	24H,	98H
	DB	86H,	92H,	0A2H,	0C2H,	0A2H,	97H
	DB	00H,	6AH,	57H,	56H,	56H,	56H
	DB	00H,	0CEH,	31H,	31H,	31H,	2EH
	DB	00H,	0F3H,	8CH,	0CCH,	0B3H,	80H
	DB	00H,	0ECH,	73H,	0E0H,	60H,	61H
	DB	04H,	0FFH,	04H,	0E4H,	14H,	0E3H
	DB	00H,	46H,	46H,	45H,	0CDH,	34H
	DB	00H,	31H,	35H,	55H,	55H,	8AH
	DB	00H,	0CCH,	32H,	21H,	61H,	9EH
	DB	00H,	7EH,	44H,	88H,	10H,	3EH
	DB	38H,	48H,	88H,	48H,	48H,	38H
	DB	60H,	10H,	09H,	12H,	10H,	60H
	DB	00H,	00H,	00H,	0A0H,	40H,	00H
	DB	61H,	02H,	0E6H,	67H,	66H,	0F3H
	DB	80H,	63H,	95H,	09H,	55H,	0A3H
	DB	00H,	75H,	5AH,	5AH,	55H,	70H
	DB	32H,	4BH,	0CAH,	0B2H,	00H,	00H
	DB	9CH,	14H,	1CH,	04H,	18H,	00H
	DB	0FH,	10H,	26H,	41H,	83H,	0FCH
	DB	0FEH,	00H,	63H,	94H,	08H,	0F1H
	DB	42H,	0A5H,	98H,	65H,	0C2H,	3CH
	DB	00H,	10H,	89H,	7FH,	09H,	10H
	DB	20H,	0A4H,	3EH,	0E1H,	1EH,	84H
	DB	00H,	00H,	00H,	0AFH,	4DH,	0AFH
	DB	00H,	0EH,	00H,	38H,	28H,	38H
	DB	01H,	02H,	04H,	2FH,	20H,	0EFH
	DB	02H,	09H,	7CH,	13H,	7CH,	23H
	DB	00H,	00H,	80H,	0C2H,	06H,	0CEH
	DB	38H,	21H,	21H,	0A1H,	61H,	26H
	DB	0C0H,	1DH,	23H,	3FH,	23H,	23H
	DB	00H,	0E7H,	18H,	0E8H,	18H,	0E7H
	DB	00H,	01H,	59H,	65H,	0C5H,	45H
	DB	00H,	86H,	0CEH,	0FEH,	0CEH,	86H
	DB	61H,	93H,	91H,	91H,	63H,	00H
	DB	19H,	24H,	08H,	10H,	0BDH,	00H
	DB	0C2H,	26H,	0CAH,	2FH,	0C2H,	00H
	DB	79H,	42H,	73H,	0AH,	71H,	00H
	DB	9EH,	02H,	84H,	48H,	88H,	00H
	DB	63H,	94H,	63H,	90H,	63H,	00H
	DB	07H,	82H,	0BAH,	83H,	02H,	00H
	DB	9BH,	6AH,	0CAH,	3BH,	00H,	00H
	DB	80H,	80H,	89H,	90H,	10H,	09H
	DB	04H,	04H,	1FH,	84H,	84H,	1FH
	DB	00H,	61H,	93H,	91H,	91H,	63H
	DB	00H,	18H,	24H,	09H,	10H,	0BCH
	DB	00H,	3FH,	18H,	0DEH,	18H,	18H
	DB	00H,	4BH,	6AH,	5BH,	4AH,	4AH
	DB	00H,	80H,	4AH,	8AH,	15H,	10H
	DB	77H,	0CEH,	0CFH,	0FEH,	0CEH,	0CFH
	DB	9DH,	73H,	0B1H,	71H,	73H,	9DH
	DB	0EFH,	9CH,	9FH,	9CH,	9CH,	0EFH
	DB	0FFH,	63H,	7BH,	63H,	63H,	0E3H
	DB	0C0H,	2CH,	2AH,	0C9H,	0AH,	0CH
	DB	0F8H,	4BH,	24H,	24H,	4CH,	0FBH
	DB	00H,	72H,	8CH,	88H,	91H,	50H
	DB	03H,	74H,	8FH,	0E8H,	09H,	0E6H
	DB	20H,	91H,	91H,	0A9H,	4AH,	46H
	DB	00H,	5FH,	4AH,	4AH,	0ABH,	12H
	DB	18H,	63H,	95H,	95H,	93H,	62H
	DB	88H,	94H,	55H,	54H,	88H,	36H
	DB	8FH,	0CFH,	8FH,	0EFH,	9FH,	9FH
	DB	0FCH,	0C7H,	0C4H,	0C5H,	0C4H,	0FFH
	DB	00H,	80H,	40H,	80H,	5FH,	9FH

; The 5px "tiny" font actually has a width of 4, meaning 8 chr/4 img.
; Charmap for reference:
;    x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
; Dx 0  1  2  3  4  5  6  7  8  9  +  -  .  *1 0  0⁻
; Ex A  B  C  D  E  F  M  O  Q  R  X  Y     ,  ^  v
; Fx a  b  c  d  n  s  (  )  ´  ∑  i  ∠  >
; DATA: GY454XE  Re 00694
_font_tiny:
	DB	72H,	52H,	52H,	52H,	72H
	DB	77H,	11H,	77H,	41H,	77H
	DB	57H,	54H,	57H,	71H,	17H
	DB	77H,	45H,	71H,	51H,	71H
	DB	77H,	55H,	77H,	51H,	77H
	DB	00H,	20H,	77H,	20H,	00H
	DB	00H,	00H,	05H,	61H,	61H
	DB	03H,	00H,	77H,	55H,	77H
	DB	26H,	55H,	76H,	55H,	56H
	DB	36H,	45H,	45H,	45H,	36H
	DB	77H,	44H,	77H,	44H,	74H
	DB	52H,	75H,	75H,	55H,	52H
	DB	76H,	55H,	56H,	65H,	35H
	DB	55H,	55H,	22H,	52H,	52H
	DB	00H,	00H,	06H,	02H,	04H
	DB	20H,	70H,	77H,	07H,	02H
	DB	24H,	14H,	37H,	55H,	36H
	DB	01H,	31H,	47H,	45H,	33H
	DB	03H,	64H,	52H,	51H,	56H
	DB	14H,	22H,	22H,	22H,	14H
	DB	27H,	44H,	02H,	04H,	07H
	DB	60H,	01H,	62H,	64H,	67H
	DB	40H,	20H,	10H,	20H,	40H

; DATA: GY454XE  Re 00707
_font_big_1:
	DB	22H,	21H,	00H,	23H,	40H,	83H,	8CH,	8CH,	73H
	DB	04H,	08H,	00H,	9CH,	43H,	0DFH,	63H,	0E7H,	5AH
	DB	21H,	42H,	00H,	0E6H,	12H,	0F2H,	02H,	12H,	0E7H
	DB	08H,	11H,	00H,	39H,	46H,	46H,	46H,	46H,	39H
	DB	00H,	4AH,	00H,	0D1H,	31H,	31H,	31H,	33H,	0CDH
	DB	11H,	21H,	00H,	89H,	89H,	89H,	89H,	99H,	69H
	DB	04H,	08H,	1CH,	22H,	22H,	22H,	22H,	22H,	1CH

; Note: the small font's localization character set is actually 7px tall!
; DATA: GY454XE  Re 00746
_font_small_1:
	DB	02H,	21H,	03H,	20H,	43H,	8CH,	73H
	DB	04H,	08H,	9CH,	43H,	0DFH,	63H,	0DEH
	DB	21H,	42H,	0E0H,	16H,	0F2H,	02H,	0E7H
	DB	09H,	10H,	00H,	39H,	46H,	46H,	39H
	DB	4AH,	00H,	00H,	0D1H,	31H,	33H,	0CDH
	DB	10H,	21H,	00H,	89H,	89H,	99H,	69H
	DB	04H,	08H,	1CH,	22H,	22H,	22H,	1CH

RSEG $$NTABgenerals1

; DATA: GY454XE  Re 00840
_base_n_submodes:
	DB 1
	DB 7
	DB 9
	DB 15

; DATA: GY454XE  Re 00844
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
_blacklist:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 6CH	; Pol(
	DB 6DH	; Rec(
	DB 3AH	; Multi-statement colon
	DB 5FH	; Remainder (÷R)
	DB 0A4H	; Recurring decimal
	DB 0

; DATA: GY454XE  Re 00856
_blacklist_stat_mat_vct:
	DB 6AH	; Integral function (∫)
	DB 6BH	; Derivative function (d/dx)
	DB 3AH	; Multi-statement colon
	DB 0A4H	; Recurring decimal
	DB 0

; DATA: GY454XE  Re 0085B
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

; DATA: GY454XE  Re 00875
_s_err_acbreak:
	DB "AC Break", 0

; DATA: GY454XE  Re 0087E
_s_err_syntax:
	DB "Syntax ERROR", 0

; DATA: GY454XE  Re 0088B
_s_err_math:
	DB "Math ERROR", 0

; DATA: GY454XE  Re 00896
_s_err_outofmem:
	DB "Insufficient MEM", 0

; DATA: GY454XE  Re 008A7
_s_err_stack:
	DB "Stack ERROR", 0

; DATA: GY454XE  Re 008B3
_s_err_argument:
	DB "Argument ERROR", 0

; DATA: GY454XE  Re 008C2
_s_err_dimension:
	DB "Dimension ERROR", 0

; DATA: GY454XE  Re 008D2
_s_err_solve:
	DB "Can't Solve", 0

; DATA: GY454XE  Re 008DE
_s_err_timeout:
	DB "Time Out", 0

; DATA: GY454XE  Re 008E7
_s_err_variable:
	DB "Variable ERROR", 0

; DATA: GY454XE  Re 008F6
_s_err_null:
	DB "NULL ERROR", 0
	DB 0

; DATA: GY454XE  Re 00902
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
_s_prompt_goto:
	DB "[\x9f][\x9e]:Goto", 0

; FUNCTION: GY454XE  Re 00928
_unk_00928:
	;  "  X"
	DB "\xec\xec\xea", 0

; FUNCTION: GY454XE  Re 0092C
_unk_0092c:
	;  "  Y"
	DB "\xec\xec\xeb", 0
	
; FUNCTION: GY454XE  Re 00930
_unk_00930:
	;  " FREQ"
	DB "\xec\xe5\xe9\xe4\xe8", 0
	
	
; FUNCTION: GY454XE  Re 00936
_unk_00936:
	;  " Ans"
	DB "\xec\xe0\xf4\xf5", 0

; FUNCTION: GY454XE  Re 0093B
_unk_0093b:
	;  " F(X)"
	DB "\xec\xe5\xf6\xea\xf7", 0

; FUNCTION: GY454XE  Re 00941
_unk_00941:
	;  "  a"
	DB "\xec\xec\xf0", 0

; FUNCTION: GY454XE  Re 00945
_unk_00945:
	;  "1"
	DB "\xd1", 0

; DATA: GY454XE  Re 00947
_s_blank_line:
	DB "                ", 0

; DATA: GY454XE  Re 00958
_unk_00958:
	DB "Fix 0~9?", 0
	
; DATA: GY454XE  Re 00961
_unk_00961:
	DB "Sci 0~9?", 0

; DATA: GY454XE  Re 0096A
_unk_0096a:
	DB "Norm 1~2?", 0

; DATA: GY454XE  Re 00974
_contrast_screen:
	DB "CONTRAST", 0
	DB "", 0
	DB "LIGHT       DARK", 0
	DB " [\x9f]        [\x9e]", 0

; DATA: GY454XE  Re 0099F
_const_input_template:
	DB "[__]", 0
	
; DATA: GY454XE  Re 009A4
_const_screen:
	DB "CONSTANT", 0
	DB "Number 01~40?", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 009BE
_conv_screen:
	DB "CONVERSION", 0
	DB "Number 01~40?", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 009DA
_menu_clr:
	DB "Clear?", 0
	DB "1:Setup 2:Memory", 0
	DB "3:All", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 009FA
_s_clr_setup:
	DB "Clear Setup?", 0

; DATA: GY454XE  Re 00A07
_s_clr_mem:
	DB "Clear Memory?", 0

; DATA: GY454XE  Re 00A15
_s_reset_all:
	DB "Reset All?", 0

; DATA: GY454XE  Re 00A20
_s_clr_done_all:
	DB "   Reset All", 0

; DATA: GY454XE  Re 00A2D
_s_prompt_yes:
	DB "[=]   :Yes", 0

; DATA: GY454XE  Re 00A38
_s_prompt_cancel:
	DB "[AC]  :Cancel", 0

; DATA: GY454XE  Re 00A46
_s_clr_done:
	DB "   Complete!", 0
	
; DATA: GY454XE  Re 00A53
_s_clr_prompt_ac:
	DB " Press [AC] key", 0
	
; DATA: GY454XE  Re 00A63
_menu_matrix_table:
	DB "1:Dim   2:Data", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00A76
_menu_matrix:
	DB "1:Dim   2:Data", 0
	DB "3:MatA  4:MatB", 0
	DB "5:MatC  6:MatAns", 0
	DB "7:det   8:Trn", 0
	DB 0
	
; DATA: GY454XE  Re 00AB4
_menu_matrix_data:
	DB "1:MatA  2:MatB", 0
	DB "3:MatC", 0
	DB 0
	DB 0

; DATA: GY454XE  Re 00ACC
_menu_matrix_dim0:
	DB "1:3$3   2:3$2", 0
	DB "3:3$1   4:2$3", 0
	DB "5:2$2   6:2$1", 0

; DATA: GY454XE  Re 00AF6
_menu_matrix_dim1:
	DB "1:1$3   2:1$2", 0
	DB "3:1$1", 0
	DB "", 0

; DATA: GY454XE  Re 00B0C
_unk_00b0c:
	DB "Matrix?", 0

; DATA: GY454XE  Re 00B14
_unk_00b14:
	DB "MatA(m$n) m$n?", 0

; DATA: GY454XE  Re 00B23
_unk_00b23:
	DB "MatB(m$n) m$n?", 0

; DATA: GY454XE  Re 00B32
_unk_00b32:
	DB "MatC(m$n) m$n?", 0

; DATA: GY454XE  Re 00B41
_menu_vector_table:
	DB "1:Dim   2:Data", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00B54
_menu_vector:
	DB "1:Dim   2:Data", 0
	DB "3:VctA  4:VctB", 0
	DB "5:VctC  6:VctAns", 0
	DB "7:Dot", 0
	DB 0
	
; DATA: GY454XE  Re 00B8A
_menu_vector_data:
	DB "1:VctA  2:VctB", 0
	DB "3:VctC", 0
	DB 0
	DB 0

; DATA: GY454XE  Re 00BA2
_menu_vector_dim:
	DB "1:3     2:2", 0
	DB "", 0
	DB "", 0

; DATA: GY454XE  Re 00BB1
_unk_00bb1:
	DB "Vector?", 0

; DATA: GY454XE  Re 00BB9
_unk_00bb9:
	DB "VctA(m) m?", 0

; DATA: GY454XE  Re 00BC4
_unk_00bc4:
	DB "VctB(m) m?", 0

; DATA: GY454XE  Re 00BCF
_unk_00bcf:
	DB "VctC(m) m?", 0

; DATA: GY454XE  Re 00BDA
_menu_cmplx:
	DB "1:arg   2:Conjg", 0
	DB "3:\x9er\x88\xc4  4:\x9ea+b\x80", 0
	DB "", 0
	DB "", 0

; DATA: GY454XE  Re 00BFC
_menu_base_n_0:
	DB "1:and   2:or", 0
	DB "3:xor   4:xnor", 0
	DB "5:Not   6:Neg", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C28
_menu_base_n_1:
	DB "1:d     2:h", 0
	DB "3:b     4:o", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C43
_menu_drg:
	DB "1:\x85     2:\x86", 0
	DB "3:\x87", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C56
_menu_hyp:
	DB "1:sinh  2:cosh", 0
	DB "3:tanh  4:sinh\xaa", 0
	DB "5:cosh\xaa 6:tanh\xaa", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00C87
_menu_stat_data_edit:
	DB "1:Ins   2:Del-A", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0
	
; DATA: GY454XE  Re 00C9B
_menu_stat_1var_sum:
	DB "1:\xc0x\xa2   2:\xc0x\xa", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00CAC
_menu_stat_2var_sum:
	DB "1:\xc0x\xa2   2:\xc0x", 0
	DB "3:\xc0y\xa2   4:\xc0y", 0
	DB "5:\xc0xy   6:\xc0x\xa3", 0
	DB "7:\xc0x\xa2y  8:\xc0x\xa4", 0

; DATA: GY454XE  Re 00CE2
_menu_stat_1var_var:
	DB "1:n     2:\x89", 0
	DB "3:\xc8x    4:sx", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00CFE
_menu_stat_2var_var:
	DB "1:n     2:\x89", 0
	DB "3:\xc8x    4:sx", 0
	DB "5:\x8a     6:\xc8y", 0
	DB "7:sy", 0
	DB 0
	
; DATA: GY454XE  Re 00D29
_menu_stat_1var_minmax:
	DB "1:minX  2:maxX", 0
	DB "", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D3C
_menu_stat_2var_minmax:
	DB "1:minX  2:maxX", 0
	DB "3:minY  4:maxY", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D5D
_menu_stat_distr:
	DB "1:P(    2:Q(", 0
	DB "3:R(    4:\x9et", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D7A
_menu_stat_reg:
	DB "1:\x9a     2:\x9b", 0
	DB "3:r     4:\x8b", 0
	DB "5:\x8c", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00D98
_menu_stat_reg_quad:
	DB "1:\x9a     2:\x9b", 0
	DB "3:\x9c     4:\x8b\xb1", 0
	DB "5:\x8b\xb2    6:\x8c", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DBF
_menu_ratio:
	DB "1:a:b=X:d", 0
	DB "2:a:b=c:X", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DD6
_menu_verify:
	DB "1:=     2:\x95", 0
	DB "3:>     4:<", 0
	DB "5:\x96     6:\x94", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00DFC
_menu_ineq:
	DB "1:aX\xa2+bX+c", 0
	DB "2:aX\xa3+bX\xa2+cX+d", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00E19
_menu_ineq_poly2:
	DB "1:aX\xa2+bX+c>0", 0
	DB "2:aX\xa2+bX+c<0", 0
	DB "3:aX\xa2+bX+c\x960", 0
	DB "4:aX\xa2+bX+c\x940", 0
	DB 0

; DATA: GY454XE  Re 00E4E
_menu_ineq_poly3:
	DB "1:aX\xa3+bX\xa2+cX+d>0", 0
	DB "2:aX\xa3+bX\xa2+cX+d<0", 0
	DB "3:aX\xa3+bX\xa2+cX+d\x960", 0
	DB "4:aX\xa3+bX\xa2+cX+d\x940", 0
	DB 0

; DATA: GY454XE  Re 00E93
_menu_setup_stat_freq:
	DB "Frequency?", 0
	DB "1:ON    2:OFF", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00EAF
_menu_setup_rdec:
	DB "Recur Decimal?", 0
	DB "1:ON    2:OFF", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00ECF
_menu_setup_cmplx_result:
	DB "Complex Result?", 0
	DB "1:a+b\x80  2:r\x88\xc4", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00EF0
_menu_setup_decimal_mark:
	DB "Decimal Point?", 0
	DB "1:Dot   2:Comma", 0
	DB "", 0
	DB "", 0
	DB 0

; DATA: GY454XE  Re 00F12
_menu_setup_decimalo:
	DB "Result Format?", 0
	DB "1:MathO 2:LineO", 0
	DB "", 0
	DB "", 0
	DB 0

RSEG $$NTABgenerals2

; DATA: GY454XE  Re 010F2
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
_char_placeholder:
	DB "@"

; DATA: GY454XE  Re 013F3
_char_cs01:
	DB "m\xb6"

; DATA: GY454XE  Re 013F5
_char_cs02:
	DB "m\x9d"

; DATA: GY454XE  Re 013F7
_char_cs03:
	DB "me"

; DATA: GY454XE  Re 013F9
_char_cs04:
	DB "m\xb7"

; DATA: GY454XE  Re 013FB
_char_cs05:
	DB "a\xb0"

; FUNCTION: GY454XE  Re 013FD
_char_cs06:
_char_base_hex:
	DB "h"

; DATA: GY454XE  Re 013FE
_char_cs07:
	DB "\xc6\xb5"

; DATA: GY454XE  Re 01400
_char_cs08:
	DB "\xc6\x9b"

; DATA: GY454XE  Re 01402
_char_cs09:
	DB "\xcb"

; DATA: GY454XE  Re 01403
_char_cs10:
	DB "\xc1"

; FUNCTION: GY454XE  Re 01404
_char_cs11:
_char_stat_reg_r:
	DB "re"

; DATA: GY454XE  Re 01406
_char_cs12:
	DB "\xc5c"

; DATA: GY454XE  Re 01408
_char_cs13:
	DB "\xc2\xb6"

; DATA: GY454XE  Re 0140A
_char_cs14:
	DB "\xc5cp"

; DATA: GY454XE  Re 0140D
_char_cs15:
	DB "\xc5cn"

; FUNCTION: GY454XE  Re 01410
_char_sum_x2:
_char_sum_x:
_char_sum:
	DB "\xc0x\xa2"

; DATA: GY454XE  Re 01413
_char_n:
	DB "n"

; FUNCTION: GY454XE  Re 01414
_char_sum_y2:
_char_sum_y:
	DB "\xc0y\xa2"

; DATA: GY454XE  Re 01417
_char_sum_xy:
	DB "\xc0xy"

; DATA: GY454XE  Re 0141A
_char_sum_x3:
	DB "\xc0x\xa3"

; DATA: GY454XE  Re 0141D
_char_sum_x2y:
	DB "\xc0x\xa2y"

; DATA: GY454XE  Re 01421
_char_sum_x4:
	DB "\xc0x\xa4"

; DATA: GY454XE  Re 01424
_char_min_x:
	DB "minX"

; DATA: GY454XE  Re 01428
_char_max_x:
	DB "maxX"

; DATA: GY454XE  Re 0142C
_char_min_y:
	DB "minY"

; DATA: GY454XE  Re 01430
_char_max_y:
	DB "maxY"

; DATA: GY454XE  Re 01434
_char_cs16:
	DB "R\x84"

; DATA: GY454XE  Re 01436
_char_cs17:
	DB "u"

; DATA: GY454XE  Re 01437
_char_cs18:
	DB "\xc6\xb6"

; DATA: GY454XE  Re 01439
_char_0x20:
	DB " "

; DATA: GY454XE  Re 0143A
_char_box:
	DB "\xcd"

; DATA: GY454XE  Re 0143B
_char_cs19:
	DB "\xc6e"

; DATA: GY454XE  Re 0143D
_char_cs20:
	DB "\xc6\x9d"

; DATA: GY454XE  Re 0143F
_char_cs21:
	DB "\xc6\xb7"

; DATA: GY454XE  Re 01441
_char_percent:
	DB "%"

; DATA: GY454XE  Re 01442
_char_cs22:
	DB "F"

; DATA: GY454XE  Re 01443
_char_cs23:
	DB "e"

; DATA: GY454XE  Re 01444
_char_paren_l:
	DB "("

; DATA: GY454XE  Re 01445
_char_paren_r:
	DB ")"

; DATA: GY454XE  Re 01446
_char_cs24:
	DB "N\x9a"

; DATA: GY454XE  Re 01448
_char_plus:
	DB "+"

; DATA: GY454XE  Re 01449
_char_sep:
	DB ","

; DATA: GY454XE  Re 0144A
_char_minus:
	DB "-"

; DATA: GY454XE  Re 0144B
_char_decimal:
	DB "."

; DATA: GY454XE  Re 0144C
_char_0:
	DB "0"

; DATA: GY454XE  Re 0144D
_char_1:
	DB "1"

; DATA: GY454XE  Re 0144E
_char_2:
	DB "2"

; DATA: GY454XE  Re 0144F
_char_3:
	DB "3"

; DATA: GY454XE  Re 01450
_char_4:
	DB "4"

; DATA: GY454XE  Re 01451
_char_5:
	DB "5"

; DATA: GY454XE  Re 01452
_char_6:
	DB "6"

; DATA: GY454XE  Re 01453
_char_7:
	DB "7"

; DATA: GY454XE  Re 01454
_char_8:
	DB "8"

; DATA: GY454XE  Re 01455
_char_9:
	DB "9"

; DATA: GY454XE  Re 01456
_char_colon:
	DB ":"

; DATA: GY454XE  Re 01457
_char_equals:
	DB "="

; DATA: GY454XE  Re 01458
_char_cs26:
	DB "Vm"

; DATA: GY454XE  Re 0145A
_char_sto_a:
	DB "\x8dA"

; FUNCTION: GY454XE  Re 0145C
_char_var_b:
_char_sto_b:
	DB "\x8dB"

; DATA: GY454XE  Re 0145E
_char_sto_c:
	DB "\x8dC"

; FUNCTION: GY454XE  Re 01460
_char_var_d:
_char_sto_d:
	DB "\x8dD"

; DATA: GY454XE  Re 01462
_char_sto_m:
	DB "\x8dM"

; FUNCTION: GY454XE  Re 01464
_char_sto_x:
_char_var_x:
	DB "\x8dX"

; FUNCTION: GY454XE  Re 01466
_char_sto_y:
_char_var_y:
	DB "\x8dY"

; DATA: GY454XE  Re 01468
_char_mul:
	DB "$"

; DATA: GY454XE  Re 01469
_char_div:
	DB "&"

; DATA: GY454XE  Re 0146A
_char_base_bin:
	DB "b"

; DATA: GY454XE  Re 0146B
_char_cmplx_rec:
	DB "\x9ea+b\x80"

; DATA: GY454XE  Re 01470
_char_cmplx_pol:
	DB "\x9er\x88\xc4"

; DATA: GY454XE  Re 01474
_char_factorial:
	DB "!"

; DATA: GY454XE  Re 01475
_char_0x5b:
	DB "["

; DATA: GY454XE  Re 01476
_char_dms:
	DB "\\"

; DATA: GY454XE  Re 01477
_char_0x5d:
	DB "]"

; DATA: GY454XE  Re 01478
_char_pow:
	DB "^"

; DATA: GY454XE  Re 01479
_char_negative:
	DB "`"

; DATA: GY454XE  Re 0147A
_char_not:
	DB "Not"

; DATA: GY454XE  Re 0147D
_char_neg:
	DB "Neg"

; FUNCTION: GY454XE  Re 01480
_char_var_a:
_char_abs:
	DB "Abs"

; FUNCTION: GY454XE  Re 01483
_char_xhat1:
_char_xhat:
	DB "\x8b\xb1"

; DATA: GY454XE  Re 01485
_char_yhat:
	DB "\x8c"

; DATA: GY454XE  Re 01486
_char_xhat2:
	DB "\x8b\xb2"

; DATA: GY454XE  Re 01488
_char_log:
	DB "log"

; DATA: GY454XE  Re 0148B
_char_integral:
	DB "\x99"

; FUNCTION: GY454XE  Re 0148C
_char_base_dec:
_char_ddx:
	DB "d/dx"

; FUNCTION: GY454XE  Re 01490
_char_pol:
_char_stat_p:
	DB "Pol"

; FUNCTION: GY454XE  Re 01493
_char_rec:
_char_cs27:
_char_stat_r:
	DB "Rec"

; DATA: GY454XE  Re 01496
_char_and:
	DB "and"

; FUNCTION: GY454XE  Re 01499
_char_base_oct:
_char_or:
	DB "or"

; FUNCTION: GY454XE  Re 0149B
_char_epow:
_char_euler:
	DB "\x81^"

; DATA: GY454XE  Re 0149D
_char_exp:
	DB "\x82\x83"

; DATA: GY454XE  Re 0149F
_char_pow_2:
	DB "\xa2"

; DATA: GY454XE  Re 014A0
_char_pow_3:
	DB "\xa3"

; DATA: GY454XE  Re 014A1
_char_pow_m1:
	DB "\xaa"

; DATA: GY454XE  Re 014A2
_char_cs28:
	DB "c\xb0"

; DATA: GY454XE  Re 014A4
_char_cs29:
	DB "c\xb0"

; DATA: GY454XE  Re 014A6
_char_0x7b:
	DB "{"

; DATA: GY454XE  Re 014A7
_char_0x7d:
	DB "}"

; DATA: GY454XE  Re 014A8
_char_xor:
	DB "xor"

; DATA: GY454XE  Re 014AB
_char_xnor:
	DB "xnor"

; DATA: GY454XE  Re 014AF
_char_cmplx_i:
	DB "\x80"

; DATA: GY454XE  Re 014B0
_char_pi:
	DB "\xc7"

; DATA: GY454XE  Re 014B1
_char_d:
	DB "\x85"

; DATA: GY454XE  Re 014B2
_char_r:
	DB "\x86"

; DATA: GY454XE  Re 014B3
_char_g:
	DB "\x87"

; FUNCTION: GY454XE  Re 014B4
_char_var_c:
_char_conjg:
	DB "Conjg"

; DATA: GY454XE  Re 014B9
_char_xbar:
	DB "\x89"

; DATA: GY454XE  Re 014BA
_char_ybar:
	DB "\x8a"

; DATA: GY454XE  Re 014BB
_char_ans:
	DB "Ans"

; DATA: GY454XE  Re 014BE
_char_ran:
	DB "Ran#"

; DATA: GY454XE  Re 014C2
_char_10pow:
	DB "\x83^"

; DATA: GY454XE  Re 014C4
_char_sqrt:
	DB "\x98"

; FUNCTION: GY454XE  Re 014C5
_char_var_m:
_char_m_plus:
	DB "M+"

; DATA: GY454XE  Re 014C7
_char_stat_a:
	DB "\x9a"

; DATA: GY454XE  Re 014C8
_char_stat_b:
	DB "\x9b"

; DATA: GY454XE  Re 014C9
_char_stat_c:
	DB "\x9c"

; DATA: GY454XE  Re 014CA
_char_dot:
	DB "*"

; DATA: GY454XE  Re 014CB
_char_nth_rt:
	DB "\xab\x98"

; DATA: GY454XE  Re 014CD
_char_ln:
	DB "ln"

; DATA: GY454XE  Re 014CF
_char_conv:
	DB "\x9eConv "

; DATA: GY454XE  Re 014D5
_char_cbrt:
	DB "\xa3\x98"

; DATA: GY454XE  Re 014D7
_char_m_minus:
	DB "M-"

; DATA: GY454XE  Re 014D9
_char_stat_ox:
	DB "\xc8x"

; DATA: GY454XE  Re 014DB
_char_stat_sx:
	DB "sx"

; DATA: GY454XE  Re 014DD
_char_stat_oy:
	DB "\xc8y"

; DATA: GY454XE  Re 014DF
_char_stat_sy:
	DB "sy"

; DATA: GY454XE  Re 014E1
_char_frac:
	DB "\x93"

; DATA: GY454XE  Re 014E2
_char_angle:
	DB "\x88"

; DATA: GY454XE  Re 014E3
_char_cs38:
	DB "t"

; DATA: GY454XE  Re 014E4
_char_rnd:
	DB "Rnd"

; DATA: GY454XE  Re 014E7
_char_cs30:
	DB "c\xb2"

; DATA: GY454XE  Re 014E9
_char_cs31:
	DB "\xc8"

; DATA: GY454XE  Re 014EA
_char_cs32:
	DB "\xc3\xb0"

; DATA: GY454XE  Re 014EC
_char_cs33:
	DB "\xc6\xb0"

; DATA: GY454XE  Re 014EE
_char_hex_a:
	DB "\xb8"

; DATA: GY454XE  Re 014EF
_char_hex_b:
	DB "\xb9"

; FUNCTION: GY454XE  Re 014F0
_char_combi:
_char_hex_c:
	DB "\xba"

; DATA: GY454XE  Re 014F1
_char_hex_d:
	DB "\xbb"

; DATA: GY454XE  Re 014F2
_char_hex_e:
	DB "\xbc"

; DATA: GY454XE  Re 014F3
_char_hex_f:
	DB "\xbd"

; DATA: GY454XE  Re 014F4
_char_permu:
	DB "\xbe"

; DATA: GY454XE  Re 014F5
_char_det:
	DB "det"

; DATA: GY454XE  Re 014F8
_char_trn:
	DB "Trn"

; DATA: GY454XE  Re 014FB
_char_ranint:
	DB "RanInt#"

; DATA: GY454XE  Re 01502
_char_arg:
	DB "arg"

; DATA: GY454XE  Re 01505
_char_cs34:
	DB "\xc9\xb0"

; FUNCTION: GY454XE  Re 01507
_char_cs36:
_char_cs39:
	DB "G\xb0"

; DATA: GY454XE  Re 01509
_char_cs37:
	DB "Z\xb0"

; DATA: GY454XE  Re 0150B
_char_matb:
	DB "MatB"

; DATA: GY454XE  Re 0150F
_char_matc:
	DB "MatC"

; FUNCTION: GY454XE  Re 01513
_char_mata:
_char_matans:
	DB "MatAns"

; DATA: GY454XE  Re 01519
_char_vctb:
	DB "VctB"

; DATA: GY454XE  Re 0151D
_char_vctc:
	DB "VctC"

; FUNCTION: GY454XE  Re 01521
_char_vcta:
_char_vctans:
	DB "VctAns"

; DATA: GY454XE  Re 01527
_char_stat_q:
	DB "Q"

; DATA: GY454XE  Re 01528
_char_stat_tot:
	DB "\x9et"

; DATA: GY454XE  Re 0152A
_char_cs40:
	DB "atm"

; FUNCTION: GY454XE  Re 0152D
_char_cv01:
_char_cv02:
	DB "cm\x9ein\x9ecm"

; FUNCTION: GY454XE  Re 01535
_char_cv03:
_char_cv04:
	DB "m\x9eft\x9em"

; FUNCTION: GY454XE  Re 0153B
_char_cv05:
_char_cv06:
	DB "m\x9eyd\x9em"

; FUNCTION: GY454XE  Re 01541
_char_cs25:
_char_cv07:
_char_cv08:
	DB "k"
	DB "m\x9emile\x9ekm"

; FUNCTION: GY454XE  Re 0154B
_char_cv09:
_char_cv10:
	DB "m\x9en mile\x9em"

; FUNCTION: GY454XE  Re 01555
_char_cv11:
_char_cv12:
	DB "m\xa2\x9eacre\x9em\xa2"

; FUNCTION: GY454XE  Re 0155F
_char_cv13:
_char_cv14:
	DB "\xca\x9egal(US)\x9e\xca"

; FUNCTION: GY454XE  Re 0156A
_char_cv15:
_char_cv16:
	DB "\xca\x9egal(UK)\x9e\xca"

; FUNCTION: GY454XE  Re 01575
_char_cv17:
_char_cv18:
	DB "km\x9epc\x9ekm"

; FUNCTION: GY454XE  Re 0157D
_char_cv19:
_char_cv20:
	DB "m/s\x9ekm/h\x9em/s"

; FUNCTION: GY454XE  Re 01589
_char_cs35:
_char_cv21:
_char_cv22:
	DB "g\x9eoz\x9eg"

; FUNCTION: GY454XE  Re 0158F
_char_cv23:
_char_cv24:
	DB "kg\x9elb\x9ekg"

; FUNCTION: GY454XE  Re 01597
_char_cv25:
_char_cv26:
	DB "Pa\x9eatm\x9ePa"

; FUNCTION: GY454XE  Re 015A0
_char_cv27:
_char_cv28:
	DB "Pa\x9emmHg\x9ePa"

; FUNCTION: GY454XE  Re 015AA
_char_cv29:
_char_cv30:
	DB "kW\x9ehp\x9ekW"

; FUNCTION: GY454XE  Re 015B2
_char_cv31:
_char_cv32:
	DB "Pa\x9ekgf/cm\xa2\x9ePa"

; FUNCTION: GY454XE  Re 015BF
_char_cv33:
_char_cv34:
	DB "J\x9ekgf*m\x9eJ"

; FUNCTION: GY454XE  Re 015C8
_char_cv35:
_char_cv36:
	DB "kPa\x9elbf/in\xa2\x9ekPa"

; FUNCTION: GY454XE  Re 015D7
_char_cv37:
_char_cv38:
	DB "\x85C\x9e\x85F\x9e\x85C"

; FUNCTION: GY454XE  Re 015DF
_char_cv39:
_char_cv40:
	DB "cal\x9eJ\x9ecal"

; FUNCTION: GY454XE  Re 015E8
_char_var_e:
_char_sto_e:
	DB "\x8dE"

; FUNCTION: GY454XE  Re 015EA
_char_var_f:
_char_sto_f:
	DB "\x8dF"

; DATA: GY454XE  Re 015EC
_char_remainder:
	DB "&R"

; DATA: GY454XE  Re 015EE
_char_lt:
	DB "<"

; DATA: GY454XE  Re 015EF
_char_gt:
	DB ">"

; DATA: GY454XE  Re 015F0
_char_le:
	DB "\x94"

; DATA: GY454XE  Re 015F1
_char_ne:
	DB "\x95"

; DATA: GY454XE  Re 015F2
_char_ge:
	DB "\x96"

; FUNCTION: GY454XE  Re 015F3
_char_sinh:
_char_asinh:
	DB "sinh\xaa"

; FUNCTION: GY454XE  Re 015F8
_char_cosh:
_char_acosh:
	DB "cosh\xaa"

; FUNCTION: GY454XE  Re 015FD
_char_tanh:
_char_atanh:
	DB "tanh\xaa"

; FUNCTION: GY454XE  Re 01602
_char_sin:
_char_asin:
	DB "sin\xaa"

; FUNCTION: GY454XE  Re 01606
_char_cos:
_char_acos:
	DB "cos\xaa"

; FUNCTION: GY454XE  Re 0160A
_char_tan:
_char_atan:
	DB "tan\xaa"


RSEG $$NTABgenerals3

; X position bitmask. Indexed with x_pos & 7. Used in char_print.
; DATA: GY454XE  Re 01C64
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

; STUB: GY454XE  Re 01C80
_unk_01c80:


; STUB: GY454XE  Re 01C8A
_unk_01c8a:


; STUB: GY454XE  Re 01C94
_unk_01c94:


; STUB: GY454XE  Re 01CEE
_unk_01cee:

RSEG $$NTABgenerals4

; 96x13 bitmap image, 12 bytes per row. Need I say more?
; DATA: GY454XE  Re 01F10
_casio_logo:
	DB	00H,	01H,	0FFH,	0C0H,	3EH,	03H,	0FFH,	0C7H,	0FH,	0FEH,	00H,	00H
	DB	00H,	03H,	0FFH,	0E0H,	3EH,	07H,	0FFH,	0E7H,	1FH,	0FFH,	00H,	00H
	DB	00H,	07H,	0FFH,	0F0H,	7FH,	07H,	0FFH,	0E7H,	3FH,	0FFH,	80H,	00H
	DB	00H,	07H,	80H,	0F0H,	77H,	07H,	00H,	0E7H,	3CH,	07H,	80H,	00H
	DB	00H,	07H,	00H,	70H,	0F7H,	87H,	00H,	07H,	38H,	03H,	80H,	00H
	DB	00H,	07H,	00H,	00H,	0E3H,	87H,	0FFH,	0C7H,	38H,	03H,	8EH,	00H
	DB	00H,	07H,	00H,	01H,	0E3H,	0C7H,	0FFH,	0E7H,	38H,	03H,	91H,	00H
	DB	00H,	07H,	00H,	01H,	0C1H,	0C3H,	0FFH,	0E7H,	38H,	03H,	0ACH,	80H
	DB	00H,	07H,	00H,	73H,	0FFH,	0E0H,	00H,	0E7H,	38H,	03H,	0AAH,	80H
	DB	00H,	07H,	80H,	0F3H,	0FFH,	0E7H,	00H,	0E7H,	3CH,	07H,	0ACH,	80H
	DB	00H,	07H,	0FFH,	0F7H,	0FFH,	0F7H,	0FFH,	0E7H,	3FH,	0FFH,	0AAH,	80H
	DB	00H,	03H,	0FFH,	0E7H,	00H,	77H,	0FFH,	0E7H,	1FH,	0FFH,	11H,	00H
	DB	00H,	01H,	0FFH,	0C7H,	00H,	73H,	0FFH,	0C7H,	0FH,	0FEH,	0EH,	00H

RSEG $$NCODgenerals

; FUNCTION: GY454XE  Re 02676
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
_unk_0268e:
	DB	0BH,	0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H
	DB	0H,		0H,		0H,		0H,		98H,	0D0H,	0DCH,	0H,		0H,		0H,		0H,		0H,		0H,		0CH,	0CCH,	0CH
	DB	0H,		0H,		0H,		0C0H,	0CCH,	0CCH,	0CCH,	0DDH,	0H,		0H,		0C0H,	0CH,	0H,		0H,		0H,		0D1H
	DB	0ADH,	1AH,	0H,		0H,		11H,	11H,	0AAH,	0H,		0AAH,	1AH,	0H,		0H,		0H,		0H,		1H,		0H
	DB	0H,		0C0H,	0CH,	0H,		0AH,	0H,		0C0H,	0CCH,	0AAH,	1AH,	0CCH,	0CCH,	0C1H,	0H,		0H,		10H
	DB	0AAH,	0AAH,	1H,		0H,		0C0H,	0H,		0H,		0D1H,	0AAH,	0AAH,	0H,		0H,		32H,	65H,	7H,		0DDH
	DB	0AAH,	0AAH,	0H,		0H,		0H,		0H,		0H,		0H,		0AAH,	0AH,	0H,		0H,		0H,		0H,		0H,		0H
	DB	0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H,		0H

; FUNCTION: GY454XE  Re 0270E
_f_0270E:
	MOV R1, #5EH
	SUB R0, R1
	MOV R1, R0
	BEQ _$j_0274e
	MOV R0, #1H
	CMP R1, #5H
	BEQ _$j_0274e
	MOV R0, #6H
	CMP R1, #15H
	BEQ _$j_0274e
	MOV R0, #7H
	CMP R1, #1EH
	BEQ _$j_0274e
	MOV R0, #8H
	CMP R1, #35H
	BEQ _$j_0274e
	MOV R0, #9H
	CMP R1, #3AH
	BEQ _$j_0274e
	MOV R0, #0AH
	CMP R1, #41H
	BEQ _$j_0274e
	MOV R0, #0BH
	CMP R1, #50H
	BEQ _$j_0274e
	MOV R0, #0DH
	CMP R1, #0AH
	BLT _$j_0274e
	CMP R1, #0DH
	BGT _$j_0274e
	MOV R0, R1
	ADD R0, #-8H
_$j_0274e:
	RT

; FUNCTION: GY454XE  Re 02750
_smart_strlen:
	MOV ER0, ER0
	BEQ _$j_02758
	B _strlen_n
_$j_02758:
	RT

; FUNCTION: GY454XE  Re 0275A
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
_smart_strcat:
	PUSH LR
	MOV ER0, ER0
	BEQ _$j_02768
	MOV ER2, ER2
	BEQ _$j_0276a
	BL _strcat_nn
	POP PC

; FUNCTION: GY454XE  Re 0277E
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
_num_exp_to_str:
	PUSH LR
	PUSH QR8
	PUSH XR4
	LEA [ER2]
	MOV BP, ER0
	MOV R4, #90H   ; × in ×10
	L R9, _use_output_charset
	CMP R9, #1H
	BNE _$j_02812  ; If use_output_charset != 1...
	MOV R4, #0E0H  ; Use character 0xE0 (???)
	BAL _$j_0281a
_$j_02812:
	L R8, 16H[BP]  ; Otherwise, if unk_0x16 == 1, use character 0xDD (tiny)
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
	CMP R9, #1H    ; If use_output_charset != 1...
	BNE _$j_02840
	MOV R6, #0F0H  ; Start with character 0xF0 (???)
	BAL _$j_02846
_$j_02840:
	CMP R8, #1H    ; Otherwise, if unk_0x16 == 1, start with number 0 (0xD0 tiny)
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
	BL _f_14800
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
	ADD ER2, #1EH
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
	L ER0, 0EEH[FP]
	CMP R0, #0AH
	CMPC R1, #0H
	BGES _$j_02a28
	PUSH R2
	L R2, _d_080FF
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
	ADD ER2, #16H
	MOV ER0, FP
	ADD ER0, #1EH
	BL _num_exp_to_str
	MOV ER0, #0H
	ST ER0, -10H[FP]
	MOV ER0, FP
	ADD ER0, #16H
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
	ADD ER0, #1EH
	BL _f_0285C
	MOV ER2, FP
	ADD ER2, #16H
	MOV ER0, BP
	BL _smart_strcat
	BAL _$j_029a2
_$j_02a72:
	ST ER0, -10H[FP]
	L ER0, 0EEH[FP]
	BPS _$j_02a80
	L R0, 0EEH[FP]
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
	ADD ER0, #1EH
	BL _f_0285C
	B _$j_029a2
_$j_02aa2:
	CMP R0, #-3H
	CMPC R1, #-1H
	BLTS _$j_02a28
	BAL _$j_02a24

; FUNCTION: GY454XE  Re 02AAA
_f_02AAA:
	L R0, _d_08100
	SRL R0, #4
	RT

; FUNCTION: GY454XE  Re 02AB2
_f_02AB2:
	L R0, _d_08100
	AND R0, #0FH
	RT

; FUNCTION: GY454XE  Re 02ABA
_f_02ABA:
	L R1, _d_08100
	AND R1, #0FH
	SLL R0, #4
	OR R1, R0
	ST R1, _d_08100
	RT

; FUNCTION: GY454XE  Re 02ACA
_f_02ACA:
	L R1, _d_08100
	AND R1, #-10H
	OR R0, R1
	ST R0, _d_08100
	MOV R0, #0H
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 02ADE
_f_02ADE:
	ST R0, _d_08100
	MOV R0, #0H
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 02AEA
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
_base_strs:
	DB "Bin", 0
	DB "Oct", 0
	DB "Dec", 0
	DB "Hex", 0

; FUNCTION: GY454XE  Re 02B3E
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
_f_02BE8:
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
	BL _f_02ABA
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
_f_02C76:
	PUSH LR
	PUSH R8
	MOV R8, #1H
	MOV R0, #0H
	BL _f_0B7B6
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
	CMP R8, #6H
	BNE _$j_02cdc
	CMP R9, #1H
	BLT _$j_02cdc
	CMP R9, #3H
	BGT _$j_02cdc
	BAL _$j_02cc8
_$j_02cdc:
	MOV R0, #0H
	BAL _$j_02cca

; FUNCTION: GY454XE  Re 02CE0
_buffer_clear_lastnline:
	PUSH LR
	MOV R2, #80H
	MOV R3, #1H
	MOV R1, #0CH
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

; FUNCTION: GY454XE  Re 02D90
_f_02D90:
	PUSH LR
	PUSH XR8
	MOV ER8, ER0
	MOV ER10, ER2
	TB R11.7
	BEQ _$j_02da2
	ADD R0, #1H
	BL _f_02FC8
_$j_02da2:
	TB R11.5
	BEQ _$j_02dae
	MOV ER0, ER8
	ADD R0, #-1H
	BL _f_02FC8
_$j_02dae:
	MOV ER0, ER8
	BL _f_02FC8
	CMP R9, R10
	BEQ _$j_02dbc
	ADD R9, #1H
	BAL _$j_02dae
_$j_02dbc:
	TB R11.6
	BEQ _$j_02dc8
	MOV ER0, ER8
	ADD R0, #1H
	BL _f_02FC8
_$j_02dc8:
	TB R11.4
	BEQ _$j_02dd4
	MOV ER0, ER8
	ADD R0, #-1H
	BL _f_02FC8
_$j_02dd4:
	POP XR8
	POP PC

; FUNCTION: GY454XE  Re 02DD8
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
	BL _f_02FB8
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
	BL _f_02FB8
	ADD R8, #-1H
	BC AL, _$j_02e48
_$j_02e70:
	MOV SP, FP
	POP QR8
	POP XR4
	POP PC

; FUNCTION: GY454XE  Re 02E78
_line_print_col_0:
	MOV R0, #0H

; FUNCTION: GY454XE  Re 02E7A
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
_char_print:
	PUSH LR
	PUSH XR4
	PUSH QR8
	MOV FP, SP
	ADD SP, #-15H
	CMP R0, #5FH
	BGT _$j_02fac
	CMP R1, #20H
	BGES _$j_02fac
	MOV R3, #0CH
	MOV R4, #BYTE1 _screen_buffer_end
	MOV R5, #BYTE2 _screen_buffer_end
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
	BL _f_02FEC
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
_f_02FB8:
	CMP R0, #0H
	BLTS _$j_02fea
	CMP R0, #5FH
	BGTS _$j_02fea
	CMP R1, #0H
	BLTS _$j_02fea
	CMP R1, #1FH
	BGTS _$j_02fea

; FUNCTION: GY454XE  Re 02FC8
_f_02FC8:
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
_f_02FEC:
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
	MOV R4, #-40H
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
	INC [EA]
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
_draw_byte:
	PUSH FP
	MOV R14, #24H
	MOV R15, #78H
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
_setup_status_bar:
	PUSH LR
	MOV ER2, #12
	MOV R0, #BYTE1 _screen_buffer
	MOV R1, #BYTE2 _screen_buffer
	BL _memzero
	TB _modifiers.3
	BEQ _$j_031a8
	SB _screen_buffer.4  ; SHIFT
_$j_031a8:
	TB _modifiers.2
	BEQ _$j_031b2
	SB _screen_buffer.2  ; ALPHA
_$j_031b2:
	TB _modifiers.0
	BEQ _$j_031bc
	SB _screen_buffer_1.1  ; STO
_$j_031bc:
	TB _modifiers.1
	BEQ _$j_031c6
	SB _screen_buffer_2.6  ; RCL
_$j_031c6:
	MOV R0, #BYTE1 _var_m
	MOV R1, #BYTE2 _var_m
	BL _num_invalid__
	CMP R0, #1H
	BEQ _$j_031d6
	SB _screen_buffer_1.4  ; M
_$j_031d6:
	L R0, _mode
	CMP R0, #0C4H  ; CMPLX mode
	BNE _$j_031f2
	SB _screen_buffer_4.7  ; CMPLX
	MOV R0, #BYTE1 _var_m_im
	MOV R1, #BYTE2 _var_m_im
	BL _num_invalid__
	CMP R0, #1H
	BEQ _$j_031f2
	SB _screen_buffer_1.4  ; M
_$j_031f2:
	L R0, _mode
	CMP R0, #3H  ; STAT mode
	BNE _$j_031fe
	SB _screen_buffer_3.6  ; STAT
_$j_031fe:
	CMP R0, #6H  ; MATRIX mode
	BNE _$j_03206
	SB _screen_buffer_5.6  ; MAT
_$j_03206:
	CMP R0, #7H  ; VECTOR mode
	BNE _$j_0320e
	SB _screen_buffer_5.1  ; VCT
_$j_0320e:
	L R0, _mode
	CMP R0, #2H  ; Skip if in BASE-N mode
	BEQ _$j_03246
	L R0, _setup_angle_unit
	CMP R0, #4H  ; Degree
	BNE _$j_03222
	SB _screen_buffer_7.5  ; [D]
_$j_03222:
	CMP R0, #5H  ; Radian
	BNE _$j_0322a
	SB _screen_buffer_7.1  ; [R]
_$j_0322a:
	CMP R0, #6H  ; Gradian
	BNE _$j_03232
	SB _screen_buffer_8.4  ; [G]
_$j_03232:
	L R0, _setup_num_fmt
	CMP R0, #8H  ; Fix
	BNE _$j_0323e
	SB _screen_buffer_8.0  ; FIX
_$j_0323e:
	CMP R0, #9H  ; Sci
	BNE _$j_03246
	SB _screen_buffer_9.5  ; SCI
_$j_03246:
	L R0, _setup_mathi
	BEQ _$j_03258
	L R0, _mode
	AND R0, #11000000B  ; If current mode has MathIO enabled
	BEQ _$j_03258
	SB _screen_buffer_10.6  ; Math
_$j_03258:
	TB _d_080FE.6
	BEQ _$j_03262
	SB _screen_buffer_11.4  ; Disp
_$j_03262:
	TB _d_080FE.7
	BNE _$j_03290
	TB _d_08125.0
	BEQ _$j_03272
	SB _screen_buffer_11.7  ; ^
_$j_03272:
	TB _d_08125.1
	BEQ _$j_0327c
	SB _screen_buffer_10.3  ; v
_$j_0327c:
	TB _d_08125.2
	BEQ _$j_03286
	SB _screen_buffer.6  ; Unused
_$j_03286:
	TB _d_08125.3
	BEQ _$j_03290
	SB _screen_buffer_11.1  ; Unused
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
_f_032F0:
	PUSH LR
	PUSH XR12
	MOV FP, SP
	ADD SP, #-10H
	MOV BP, FP
	ADD BP, #-0AH
	PUSH ER2
	MOV R2, #-14H
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
	MOV R8, #-14H
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
_f_033AC:
	PUSH R4
	LEA [ER2]
	CMP R0, #0H
	BGES _$j_033ba
	NEG R0
	MOV R1, #-25H
	ST R1, [EA+]
_$j_033ba:
	MOV R4, #0H
	MOV R2, #64H
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
_f_0345E:
	PUSH LR
	MOV R0, #7H
	ST R0, _font_size
	MOV R0, #0H
	ST R0, _use_rambuf
	BL _f_0479C
	MOV R0, #0H
	MOV R1, #2H
	BL _fill_screen
	POP PC

; FUNCTION: GY454XE  Re 0347A
_set_up_arrow:
	SB 0F80BH.7
	RT

; FUNCTION: GY454XE  Re 03480
_set_down_arrow:
	SB 0F80AH.3
	RT

; FUNCTION: GY454XE  Re 03486
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
	BL _f_03518
	MOV R0, R15
	POP QR8
	POP PC
ELSE
	PUSH ER2
	MOV R3, #BYTE2 _pd_val_emu
	MOV R2, #BYTE1 _pd_val_emu
	L R0, [ER2]
	POP ER2
	RT
ENDIF

; FUNCTION: GY454XE  Re 03518
_f_03518:
	PUSH ER0
	MOV R0, #10000000B
	MOV R1, #11111111B
	ST R0, KOMASK0
	ST R1, KOMASK1
	POP ER0
	RT

; FUNCTION: GY454XE  Re 0352A
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
_f_03558:
	PUSH LR
	MOV ER0, #1H
	ST R0, _table_mode
	ST R1, _d_080F4
	MOV R2, #0E6H  ; AC key
	ST R2, _last_key_keycode
	MOV R3, #1H
	ST R3, _d_080F7
	ST R1, _d_08126
	ST R1, _d_08125
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
_is_char_keycode:
	MOV R1, R0
	BEQ _$j_035ce
	L R0, _d_080F7
	BEQ _$j_035ca
	CMP R1, #1FH
	BLE _$j_035ce
	CMP R1, #0DFH
	BGT _$j_035ce
_$j_035ca:
	MOV R0, #1H
	RT
_$j_035ce:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 035D2
_f_035D2:
	MOV R1, R0
	BEQ _$j_035ce
	L R0, _d_080F7
	BEQ _$j_035ce
	CMP R1, #-0FH
	BEQ _$j_035ca
	CMP R1, #-0AH
	BLT _$j_035e8
	CMP R1, #-5H
	BLE _$j_035ca
_$j_035e8:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 035EC
_f_035EC:
	PUSH LR
	MOV ER2, ER0
	MOV R1, #0H
	ST R1, [ER2]
	L R1, _mode
	CMP R1, #6H
	BEQ _$j_03622
	CMP R1, #7H
	BEQ _$j_03622
	CMP R1, #45H
	BEQ _$j_03648
	CMP R1, #4BH
	BEQ _$j_03648
	CMP R1, #4AH
	BEQ _$j_03648
	CMP R1, #88H
	BNE _$j_03618
	L R0, _table_mode
	CMP R0, #12H
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
_is_ac_key:
	CMP R0, #0E6H
	BEQ _$j_0365c
	BAL _f_03698
_$j_0365c:
	MOV R0, #1H
	RT

; FUNCTION: GY454XE  Re 03660
_f_03660:
	MOV R1, #3H
	BAL _$j_03666

; FUNCTION: GY454XE  Re 03664
_f_03664:
	MOV R1, #1H
_$j_03666:
	L R0, _d_080FE
	AND R0, #0FH
	CMP R0, R1
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 03672
_f_03672:
	L R1, _d_080F7
	BEQ _$j_03684
	CMP R0, #0E0H
	BLT _$j_03684
	CMP R0, #0E3H
	BGT _$j_03684
_$j_03680:
	MOV R0, #1H
	RT
_$j_03684:
	MOV R0, #0H
	RT

; FUNCTION: GY454XE  Re 03688
_f_03688:
	L R1, _d_080F7
	BEQ _$j_03684
	CMP R0, #0E0H
	BEQ _$j_03680
	CMP R0, #0E1H
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 03698
_f_03698:
	L R1, _d_080F7
	BEQ _$j_03684
	CMP R0, #0E3H
	BEQ _$j_03680
	CMP R0, #0E2H
	BEQ _$j_03680
	BAL _$j_03684

; FUNCTION: GY454XE  Re 036A8
_f_036A8:
	L R1, _d_080F7
	BEQ _$j_03684
	CMP R0, #17H
	BLT _$j_03684
	CMP R0, #19H
	BGT _$j_03684
	BAL _$j_03680

; FUNCTION: GY454XE  Re 036B8
_f_036B8:
	L R0, _mode
	CMP R0, #88H  ; TABLE mode
	BNE _$j_03684
	L R0, _table_mode
	CMP R0, #12H  ; TABLE_STAT_TABLE
	BNE _$j_03684
	BAL _$j_03680

; FUNCTION: GY454XE  Re 036CA
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
_filter_chars:
	LEA _blacklist
_$j_036ec:
	L R1, [EA+]
	BEQ _$j_036f6
	CMP R1, R0
	BNE _$j_036ec
	MOV R0, #0H
_$j_036f6:
	RT

; FUNCTION: GY454XE  Re 036F8
_filter_chars_table:
	L R1, _mode
	CMP R1, #88H  ; TABLE mode
	BNE _$j_036f6
	LEA _blacklist_table
	BAL _$j_036ec

; FUNCTION: GY454XE  Re 03706
_filter_chars_cmplx:
	L R1, _mode
	CMP R1, #0C4H  ; CMPLX mode
	BNE _$j_036f6
	LEA _blacklist_cmplx
	BAL _$j_036ec

; FUNCTION: GY454XE  Re 03714
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
_num_sum_1__:
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
	BL _f_154F2
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
	BL _f_14800
	MOV ER2, ER0
	MOV ER0, ER8
	ADD ER0, #0AH
	BL _f_14800
	BL _num_cmp
	CMP R0, #4H
	BNE _$j_0378e
_$j_0378a:
	MOV R0, #8H
	BAL _$j_0380c
_$j_0378e:
	BL _f_0479C
	MOV ER0, ER8
	ADD ER0, #14H
	BL _f_154F2
	MOV R0, #-42H
	MOV R1, #17H
	MOV ER2, ER8
	BL _f_1553C
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
	BL _f_15526
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

; FUNCTION: GY454XE  Re 03814
_f_03814:
	PUSH LR
	PUSH ER0
	MOV ER0, ER8
	MOV R2, #BYTE1 _d_085B4
	MOV R3, #BYTE2 _d_085B4
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _d_085AA
	MOV R3, #BYTE2 _d_085AA
	BL _f_1A438
	POP ER0
	MOV R2, #BYTE1 _d_085B4
	MOV R3, #BYTE2 _d_085B4
	BL _f_1A410
	ADD ER8, #0AH
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 0383C
_f_0383C:
	MOV FP, SP
	L FP, 2H[FP]
	PUSH LR
	PUSH FP
	MOV ER0, ER8
	MOV R2, #BYTE1 _d_085AA
	MOV R3, #BYTE2 _d_085AA
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _table_fx+60
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	MOV R2, #BYTE1 _d_085B4
	MOV R3, #BYTE2 _d_085B4
	BL _f_1553C
	MOV R2, #BYTE1 _table_fx+70
	MOV R3, #BYTE2 _table_fx
	BL _f_1A3FC
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	BL _num_negate
	MOV R2, #BYTE1 _d_085AA
	MOV R3, #BYTE2 _d_085AA
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
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	BL _f_154F2
	MOV R0, #BYTE1 _d_085B4
	MOV R1, #BYTE2 _d_085B4
	MOV R2, #BYTE1 _table_fx+70
	MOV R3, #BYTE2 _table_fx
	BL _f_1A410
	CMP R0, #0H
	BNE _$j_038c0
	MOV R2, #BYTE1 _d_085B4
	MOV R3, #BYTE2 _d_085B4
	BAL _$j_03878
_$j_038ac:
	RB R8.0
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	MOV ER2, BP
	BL _f_1A410
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	BL _f_1B238
_$j_038c0:
	ADD SP, #2H
	ADD ER8, #0AH
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 038C8
_f_038C8:
	POP FP
	PUSH FP
	PUSH LR
	PUSH FP
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+60
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _d_08528
	MOV R3, #BYTE2 _d_08528
	BL _f_1A3FC
	MOV R0, #BYTE1 _table_fx+60
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV R0, #BYTE1 _table_fx+60
	MOV R1, #BYTE2 _table_fx
	BL _num_invalid__
	CMP R0, #0F0H
	BEQ _$j_039ac
	CMP R0, #1H
	BEQ _$j_039ac
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+70
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _d_08528
	MOV R3, #BYTE2 _d_08528
	BL _f_1A410
	MOV R0, #BYTE1 _table_fx+70
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	CMP R0, #3H
	BEQ _$j_039ac
	MOV R0, #8H
	MOV R2, #BYTE1 _table_fx+70
	MOV R3, #BYTE2 _table_fx
	BL _st_var
	MOV R6, #1H
	BL _num_eval__
	BNE _$j_039ac
	MOV R0, #BYTE1 _unk_01c80
	MOV R1, #BYTE2 _unk_01c80
	MOV R2, #BYTE1 _table_fx+80
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV ER2, BP
	BL _f_1A438
	MOV R0, #BYTE1 _unk_01c8a
	MOV R1, #BYTE2 _unk_01c8a
	MOV R2, #BYTE1 _table_fx+90
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV ER2, BP
	BL _f_1A438
	PUSH ER8
	MOV R8, #BYTE1 _unk_01c94
	MOV R9, #BYTE2 _unk_01c94
_$j_03968:
	BL _check_ac
	CMP R0, #0H
	BNE _$j_039b0
	BL _f_0383C
	BNE _$j_039aa
	MOV R0, #BYTE1 _table_fx+90
	MOV R1, #BYTE2 _table_fx
	BL _f_03814
	BNE _$j_039aa
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	BL _f_03814
	BNE _$j_039aa
	MOV R0, #BYTE1 _unk_01cee
	MOV R1, #BYTE2 _unk_01cee
	CMP ER8, ER0
	BLT _$j_03968
_$j_03992:
	BL _check_ac
	CMP R0, #0H
	BNE _$j_039b0
	BL _f_0383C
	BNE _$j_039aa
	MOV R0, #BYTE1 _table_fx+90
	MOV R1, #BYTE2 _table_fx
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
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	BL _f_1B238
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	BL _f_14800
	MOV R2, #BYTE1 _table_fx+60
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	CMP R0, #0H
	BNE _$j_039ac
	MOV R0, #BYTE1 _table_fx+90
	MOV R1, #BYTE2 _table_fx
	BL _f_1B238
	MOV R0, #BYTE1 _table_fx+90
	MOV R1, #BYTE2 _table_fx
	BL _f_14800
	MOV R2, #BYTE1 _table_fx+60
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	CMP R0, #0H
	BNE _$j_039ac
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _table_fx+90
	MOV R3, #BYTE2 _table_fx
	BL _f_1A3FC
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	BL _num_abs
	MOV R0, #BYTE1 _table_fx+80
	MOV R1, #BYTE2 _table_fx
	BL _f_1B238
	MOV R0, #0H
_$j_03a14:
	ADD SP, #2H
	CMP R0, #0H
	POP PC

; FUNCTION: GY454XE  Re 03A1A
_f_03A1A:
	PUSH LR
	PUSH BP
	ADD SP, #-0AH
	MOV BP, SP
	MOV R0, #BYTE1 _d_0851E
	MOV R1, #BYTE2 _d_0851E
	MOV R2, #BYTE1 _table_fx+80
	MOV R3, #BYTE2 _table_fx
	PUSH ER2
	BL _f_15526
	MOV ER0, BP
	MOV R2, #BYTE1 _table_fx+90
	MOV R3, #BYTE2 _table_fx
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
	BL _f_1553C
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

; FUNCTION: GY454XE  Re 03A72
_f_03A72:
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
	BL _f_14800
	MOV R0, #BYTE1 _d_0850A
	MOV R1, #BYTE2 _d_0850A
	BL _f_154F2
	BL _num_eval__
	BGE _$j_03aba
	B _$j_03de0
_$j_03aba:
	MOV R2, R0
	MOV ER0, BP
	BL _f_14800
	MOV R0, #BYTE1 _d_08514
	MOV R1, #BYTE2 _d_08514
	BL _f_154F2
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
	BL _f_14800
	BL _num_invalid__
	CMP R0, #2H
	BNE _$j_03afe
	MOV R0, #8H
	B _$j_03de0
_$j_03af0:
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _f_15526
	MOV R0, #95H
	MOV R1, #0H
	ST ER0, 8H[BP]
_$j_03afe:
	MOV R0, #BYTE1 _d_0851E
	MOV R1, #BYTE2 _d_0851E
	BL _f_154F2
	ST FP, _d_085C8
	MOV R0, #BYTE1 _d_0850A
	MOV R1, #BYTE2 _d_0850A
	MOV R2, #BYTE1 _d_08514
	MOV R3, #BYTE2 _d_08514
	BL _f_1B0DC
	CMP R0, #1H
	BNE _$j_03b5a
	MOV R0, #8H
	MOV R2, #BYTE1 _d_08514
	MOV R3, #BYTE2 _d_08514
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b34
	B _$j_03dda
_$j_03b34:
	MOV R0, #8H
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
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
	BL _f_15526
	B _$j_03dc8
_$j_03b5a:
	MOV R0, #8H
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b74
	B _$j_03dda
_$j_03b74:
	MOV R0, #8H
	MOV R2, #BYTE1 _d_08514
	MOV R3, #BYTE2 _d_08514
	BL _st_var
	POP FP
	PUSH FP
	MOV R6, #1H
	BL _num_eval__
	BEQ _$j_03b8e
	B _$j_03dda
_$j_03b8e:
	MOV R0, #BYTE1 _d_0850A
	MOV R1, #BYTE2 _d_0850A
	MOV R2, #BYTE1 _d_08528
	MOV R3, #BYTE2 _d_08528
	BL _f_1553C
	ADD ER0, #0AH
	ADD ER2, #0AH
	BL _f_1553C
	MOV R0, #BYTE1 _num_0
	MOV R1, #BYTE2 _num_0
	MOV R2, #BYTE1 _table_fx
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	ADD ER0, #0AH
	ADD ER2, #14H
	BL _f_1553C
	ADD ER2, #0AH
	BL _f_1553C
	MOV R0, #BYTE1 _d_085CD
	MOV R1, #BYTE2 _d_085CD
	MOV R2, #2FH
	ST R2, [ER0]
	MOV R1, #1H
	MOV R0, #44H
	MOV R2, #BYTE1 _d_085CA
	MOV R3, #BYTE2 _d_085CA
	ST ER0, [ER2]
	MOV R0, #BYTE1 _d_08514
	MOV R1, #BYTE2 _d_08514
	MOV R2, #BYTE1 _table_fx+50
	MOV R3, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
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
	MOV R0, #BYTE1 _table_fx+90
	MOV R1, #BYTE2 _table_fx
	BL _f_15526
	B _$j_03dc8
_$j_03c0a:
	LEA _d_085CA
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
	MOV R0, #BYTE1 _table_fx+30
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A438
	LEA _d_085CD
	DEC [EA]
	BNE _$j_03c68
	MOV R0, #BYTE1 _table_fx
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _d_085AA
	MOV R3, #BYTE2 _d_085AA
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _table_fx+90
	MOV R3, #BYTE2 _table_fx
	BL _f_1A410
	MOV R0, #BYTE1 _d_085AA
	MOV R1, #BYTE2 _d_085AA
	MOV R2, #BYTE1 _table_fx
	MOV R3, #BYTE2 _table_fx
	BL _num_cmp
	CMP R0, #1H
	BEQ _$j_03cc0
_$j_03c62:
	MOV R0, #0BH
	B _$j_03de0
_$j_03c68:
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A438
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _d_08532
	MOV R3, #BYTE2 _d_08532
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+30
	MOV R3, #BYTE2 _table_fx
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+50
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
	BL _f_1A410
	CMP R0, #3H
	BNE _$j_03cbe
	B _$j_03dda
_$j_03cbe:
	BAL _$j_03c0a
_$j_03cc0:
	LEA _d_085CD
	INC [EA]
	MOV R0, #BYTE1 _table_fx+30
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
_$j_03cd2:
	MOV R0, #BYTE1 _table_fx
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _table_fx+90
	MOV R3, #BYTE2 _table_fx
	BL _f_1A410
	CMP R0, #3H
	BEQ _$j_03dda
_$j_03ce2:
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1B0DC
	CMP R0, #1H
	BNE _$j_03cfc
	MOV R0, #BYTE1 _table_fx
	MOV R1, #BYTE2 _table_fx
	BL _f_15526
	BAL _$j_03dc8
_$j_03cfc:
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	BL _f_1B4D0
	CMP R0, #0H
	BEQ _$j_03d6c
	LEA _d_085CD
	INC [EA]
	MOV R0, #BYTE1 _table_fx+30
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	ADD ER2, #0AH
	BL _f_1A44C
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _d_08528
	MOV R3, #BYTE2 _d_08528
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _table_fx+30
	MOV R3, #BYTE2 _table_fx
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 _d_08528
	MOV R1, #BYTE2 _d_08528
	MOV R2, #BYTE1 _table_fx+50
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	MOV R0, #BYTE1 _d_08528
	MOV R1, #BYTE2 _d_08528
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
	BL _f_1A410
	CMP R0, #3H
	BEQ _$j_03dda
	BAL _$j_03ce2
_$j_03d6c:
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _d_08528
	MOV R3, #BYTE2 _d_08528
	BL _f_1553C
	MOV ER2, ER0
	MOV R0, #BYTE1 _table_fx+20
	MOV R1, #BYTE2 _table_fx
	BL _f_1553C
	MOV ER0, ER2
	MOV R2, #BYTE1 _num_1
	MOV R3, #BYTE2 _num_1
	BL _f_1A410
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+30
	MOV R3, #BYTE2 _table_fx
	PUSH XR0
	BL _f_1A3FC
	POP XR0
	BL _f_1A44C
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _table_fx+50
	MOV R3, #BYTE2 _table_fx
	BL _f_1A438
	MOV R0, #BYTE1 _d_08532
	MOV R1, #BYTE2 _d_08532
	MOV R2, #BYTE1 _d_0850A
	MOV R3, #BYTE2 _d_0850A
	BL _f_1A410
	B _$j_03c0a
_$j_03dc8:
	MOV ER0, BP
	BL _f_1B238
	MOV R0, #0H
_$j_03dd0:
	L FP, _d_085C8
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

; FUNCTION: GY454XE  Re 03DE6
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
	BL _f_154F2
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
	BL _f_14800
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

; FUNCTION: GY454XE  Re 03E72
_f_03E72:
	PUSH LR
	CMP R6, #0H
	BEQ _$j_03e7c
	MOV R2, #2H
	POP PC
_$j_03e7c:
	PUSH ER8
	MOV R8, #BYTE1 _d_085B4
	MOV R9, #BYTE2 _d_085B4
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
	BL _f_14800
	MOV ER0, ER8
	ADD ER0, #-4H
	BL _f_154F2
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
	BL _f_14800
	MOV ER0, BP
	BL _num_invalid__
	CMP R0, #4H
	BEQ _$j_03eda
	MOV R0, #8H
	B _$j_04148
_$j_03eda:
	MOV ER0, ER8
	ADD ER0, #32H
	BL _f_154F2
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	MOV R2, #-1H
	ST R2, [ER0]
	BAL _$j_03f18
_$j_03eee:
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _f_15526
	MOV R0, #90H
	MOV R1, #0H
	ST ER0, 8H[BP]
	MOV ER0, ER8
	ADD ER0, #32H
	BL _f_154F2
	MOV R0, #93H
	ST R0, 8H[BP]
	MOV ER0, ER8
	ADD R0, #46H
	ADDC R1, #0H
	BL _f_154F2
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
	BL _f_15526
	MOV ER0, BP
	BL _num_invalid__
	MOV R2, #98H
	CMP R0, #1H
	BEQ _$j_03f82
	MOV R0, #BYTE1 _num_1
	MOV R1, #BYTE2 _num_1
	BL _f_15532
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
	BL _f_154F2
	B _$j_04150
_$j_03f94:
	MOV ER0, ER8
	ADD ER0, #-18H
	BL _num_abs
	MOV ER0, ER8
	ADD ER0, #-18H
	MOV ER2, ER8
	ADD ER2, #1EH
	BL _f_1553C
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
	BL _f_1553C
	MOV R2, #BYTE1 _num_2
	MOV R3, #BYTE2 _num_2
	BL _f_1A44C
	MOV ER0, ER8
	MOV ER2, ER8
	ADD ER2, #0AH
	BL _f_1553C
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
	BL _f_1553C
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
	BL _f_1553C
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
	BL _f_1553C
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
	BL _f_15526
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
	BL _f_14800
	MOV ER0, ER8
	ADD ER0, #0EH
	BL _f_154F2
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
	BL _f_154E0
	MOV ER0, ER2
	MOV ER2, ER8
	ADD ER2, #-18H
	BL _f_1A410
	CMP R0, #0H
	BNE _$j_042a0
	MOV ER0, ER8
	ADD ER0, #4H
	BL _f_1B288
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
	BL _f_14800
	MOV ER0, ER8
	ADD ER0, #4H
	BL _f_154F2
	MOV ER0, ER8
	ADD ER0, #4H
	BL _f_1B288
	CMP R0, #0H
	BEQ _$j_04224
	CMP R0, #3H
	BEQ _$j_0428e
	BAL _$j_042a0
_$j_04224:
	MOV ER0, ER8
	ADD ER0, #0EH
	BL _num_invalid__
	CMP R0, #1H
	BEQ _$j_042a6
	MOV ER0, ER8
	ADD ER0, #4H
	BL _num_invalid__
	CMP R0, #1H
	BEQ _$j_042a6
	MOV ER0, ER8
	ADD ER0, #4H
	BL _f_15526
	MOV ER2, ER8
	ADD ER2, #0EH
	MOV ER0, BP
	BL _f_1A44C
	MOV ER0, BP
	BL _num_invalid__
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
	BL _num_invalid__
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

; FUNCTION: GY454XE  Re 042AA
_f_042AA:
	PUSH LR
	PUSH QR8
	PUSH XR4
	PUSH ER0
	L FP, [ER0]
	ADD SP, #-3CH
	MOV BP, SP
	MOV R0, #BYTE1 _calc_history
	MOV R1, #BYTE2 _calc_history
	MOV R2, #BYTE1 _calc_history+10
	MOV R3, #BYTE2 _calc_history
	BL _f_1B0DC
	CMP R0, #0F0H
	BEQ _$j_043a4
	CMP R0, #4H
	BEQ _$j_043a4
	MOV R0, #BYTE1 _calc_history+20
	MOV R1, #BYTE2 _calc_history
	BL _num_invalid__
	CMP R0, #4H
	BNE _$j_043a4
	MOV R0, #BYTE1 _calc_history+10
	MOV R1, #BYTE2 _calc_history
	BL _f_15526
	MOV ER0, BP
	MOV R2, #BYTE1 _calc_history
	MOV R3, #BYTE2 _calc_history
	BL _f_1A3FC
	MOV ER0, BP
	MOV R2, #BYTE1 _calc_history+20
	MOV R3, #BYTE2 _calc_history
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
	MOV R0, #BYTE1 _calc_history
	MOV R1, #BYTE2 _calc_history
	MOV R2, #BYTE1 _var_x
	MOV R3, #BYTE2 _var_x
	BL _f_1553C
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
	BL _f_1B12C
_$j_0435a:
	MOV R0, #0H
	MOV R1, R9
	MOV R2, #BYTE1 _var_x
	MOV R3, #BYTE2 _var_x
	BL _f_0AF30
	MOV R0, #1H
	MOV R1, R9
	MOV ER2, BP
	BL _f_0AF30
	CMP R9, R8
	BEQ _$j_04394
	ADD R9, #1H
	BL _check_ac
	CMP R0, #0H
	BNE _$j_04396
	MOV R0, #BYTE1 _var_x
	MOV R1, #BYTE2 _var_x
	MOV R2, #BYTE1 _calc_history+20
	MOV R3, #BYTE2 _calc_history
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
_f_043AC:
	PUSH R0
	ADD R1, #-1H
	MOV R0, #3H
	MUL ER0, R1
	ADD R2, #-1H
	MOV R3, #0H
	ADD ER2, ER0
	POP R0
	MOV R1, #9H
	MUL ER0, R1
	ADD R2, R0
	MOV R0, #0AH
	MUL ER2, R0
	MOV R0, #BYTE1 _calc_history
	MOV R1, #BYTE2 _calc_history
	ADD ER0, ER2
	RT

; FUNCTION: GY454XE  Re 043CE
_f_043CE:
	PUSH LR
	PUSH XR8
	MOV ER8, ER2
	MOV ER10, ER0
	BL _f_0441A
	CMP R0, R11
	BLT _$j_04414
	MOV R0, #1H
	CMP R10, #3H
	BEQ _$j_043e8
	BL _f_04448
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
_f_0441A:
	PUSH QR8
	MOV ER10, ER2
	MOV R8, #BYTE1 _calc_history+80
	MOV R9, #BYTE2 _calc_history
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
	BL _f_04448
	MUL BP, R0
	MOV R0, #0AH
	MUL BP, R0
	ADD ER8, BP
	L R12, _d_080DF
	POP LR
	BAL _$j_0442a

; FUNCTION: GY454XE  Re 04448
_f_04448:
	MOV R0, #2H
	L R1, _mode
	CMP R1, #88H  ; TABLE mode
	BEQ _$j_04466
	MOV R0, #1H
	L R1, _submode
	CMP R1, #1H
	BEQ _$j_0445e
	ADD R0, #1H
_$j_0445e:
	L R1, _setup_stat_freq
	BEQ _$j_04466
	ADD R0, #1H
_$j_04466:
	RT

; FUNCTION: GY454XE  Re 04468
_f_04468:
	PUSH LR
	ST R0, _d_080DE
	BL _f_0AF0A
	LEA 258H
	PUSH EA
	MOV ER2, #0H
	ST R2, _d_080DF
	MOV R0, #BYTE1 _calc_history+80
	MOV R1, #BYTE2 _calc_history
	BL _memset_n
	POP ER0
	POP PC

; FUNCTION: GY454XE  Re 0448A
_f_0448A:
	PUSH LR
	ADD ER0, #0H
	BEQ _$j_04498
	ADD ER2, #0H
	BEQ _$j_04498
	BL _num_cpy
_$j_04498:
	POP PC

; FUNCTION: GY454XE  Re 0449A
_clear_result:
	PUSH LR
	MOV ER2, #20
	MOV R0, #BYTE1 _result
	MOV R1, #BYTE2 _result
	BL _memzero
	POP PC

; FUNCTION: GY454XE  Re 044A8
_memzero:
	PUSH LR
	PUSH ER2
	MOV ER2, #0H
	BL _memset_n
	POP ER2
	POP PC

; FUNCTION: GY454XE  Re 044B6
_f_044B6:
	MOV R0, #0H
	ST R0, _d_080FE
	ST R0, _d_080FF
_$j_044c0:
	ST R0, _d_08101
_$j_044c4:
	ST R0, _d_08100
	ST R0, _d_0812C
	RT

; FUNCTION: GY454XE  Re 044CE
_f_044CE:
	MOV R0, #0H
	BAL _$j_044c0

; FUNCTION: GY454XE  Re 044D2
_f_044D2:
	MOV R0, #0H
	BAL _$j_044c4

; FUNCTION: GY454XE  Re 044D6
_f_044D6:
	ST R0, _last_key_keycode
	MOV R0, #1H
	ST R0, _d_080F7
	RT

; FUNCTION: GY454XE  Re 044E2
_f_044E2:
	ST R0, _last_key_keycode
	MOV R0, #0H
	ST R0, _d_080F7
	RT

; FUNCTION: GY454XE  Re 044EE
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
	MOV R2, #BYTE1 _vars_im_start
	MOV R3, #BYTE2 _vars_im_start
	ADD ER2, ER8
	BL _memcpy_nn
	CMP R14, #1H
	BEQ _$j_0454c
	MOV ER0, ER10
	BL _f_15EE4
_$j_0454c:
	BAL _$j_04532

; FUNCTION: GY454XE  Re 0454E
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
	MOV R0, #BYTE1 _vars_im_start
	MOV R1, #BYTE2 _vars_im_start
	ADD ER0, ER8
	BL _memcpy_nn
	POP ER0
_$j_04584:
	POP QR8
	POP PC

; Unused since ES
; FUNCTION: GY454XE  Re 04588
_f_04588:
	MOV ER0, SP
	RT

; FUNCTION: GY454XE  Re 0458C
_get_remaining_stack_space:
	MOV R2, #BYTE1 _stack_start
	MOV R3, #BYTE2 _stack_start
	MOV ER0, SP
	SUB R0, R2
	SUBC R1, R3
	RT

; FUNCTION: GY454XE  Re 04598
_reset_magic_string:
	LEA _magic_string
	MOV R0, #0FH
_$rms_loop:
	ST R0, [EA+]
	ADD R0, #-1
	BNE _$rms_loop
	RT

; FUNCTION: GY454XE  Re 045A6
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
_shutdown:
	L R0, _cursor_noflash
	BNE _$stop_exit
	BL _buffer_clear
	MOV R0, #156
	MOV R1, #0
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
	MOV R0, #88H
	MOV R1, #13H
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
_get_IRQ0:
	LEA IRQ0
	L R0, [EA]
	RT
	
; FUNCTION: GY454XE  Re 046AE
_f_046AE:
	TB _d_080F4.7
	BNE _$j_046bc
	MOV R0, #88H
; FUNCTION: GY454XE  Re 046B6
_f_046B6:
	ST R0, _d_080F4
	RT
_$j_046bc:
	MOV R0, #1
	RT
	
; FUNCTION: GY454XE  Re 046C0
_f_046C0:
	MOV R0, #80H
	BAL _f_046B6
	
; FUNCTION: GY454XE  Re 046C4
_f_046C4:
	MOV R0, #0
	ST R0, _d_080F4
	RT

; FUNCTION: GY454XE  Re 046CC	
_f_046CC:
	TB _d_080F4.7
	BEQ _$j_046dc
	BAL _$j_046bc
	
; FUNCTION: GY454XE  Re 046D4
_f_046D4:
	TB _d_080F4.3
	BEQ _$j_046dc
	BAL _$j_046bc
_$j_046dc:
	MOV R0, #0
	RT

; FUNCTION: GY454XE  Re 046E0
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
	BL _f_03518
	BL _clr_all_kimask
	BL _clr_all_ko
	POP PC

; FUNCTION: GY454XE  Re 04728
_clr_port0:
	LEA P0D
	MOV ER0, #0H
	MOV ER2, #7H
	ST ER0, [EA+]
	ST ER2, [EA+]
	ST R2, [EA]
	RT

; FUNCTION: GY454XE  Re 04738
_f_04738:
	LEA IE0
	MOV ER0, #-1EH
	ST ER0, [EA]
	MOV R0, #3H
	ST R0, EXICON0
	RT

; FUNCTION: GY454XE  Re 04760
_f_04760:
	MOV ER0, #3H
	ST R0, 0F033H
	ST R1, 0F030H
	ST R1, 0F034H
	MOV R0, #7H
	ST R0, 0F031H
	MOV R0, #11H

; FUNCTION: GY454XE  Re 04776
_set_contrast_sfr:
	ST R0, 0F032H
	RT

; FUNCTION: GY454XE  Re 0478E
_set_scr_normal:
	MOV R0, #5H
	ST R0, 0F031H
	RT

; FUNCTION: GY454XE  Re 04796
_f_04796:
	TB _table_mode.4
	BNE _$j_047a2

; FUNCTION: GY454XE  Re 0479C
_f_0479C:
	MOV R0, #6H
	ST R0, 0F031H
_$j_047a2:
	RB 0F800H.4
	RB 0F800H.2
	RB 0F801H.1
	RB 0F802H.6
	RB 0F80BH.7
	RB 0F80AH.3
_$j_047ba:
	RB 0F80BH.4
	RT

; FUNCTION: GY454XE  Re 047C0
_set_disp_indicator:
	CMP R0, #0H
	BEQ _$j_047ba
	SB 0F80BH.4
	RT

; FUNCTION: GY454XE  Re 047CA
_set_contrast2_0:
	MOV R0, #0H
	ST R0, 0F033H
	RT

; FUNCTION: GY454XE  Re 047D2
_set_all_kimask:
	MOV R0, #11111111B
	ST R0, KIMASK
	RT

; FUNCTION: GY454XE  Re 047DA
_clr_all_kimask:
	MOV R0, #0H
	ST R0, KIMASK
	RT

; FUNCTION: GY454XE  Re 047E2
_set_all_ko:
	MOV R0, #01111111B
	ST R0, KOD0
	RT

; FUNCTION: GY454XE  Re 047EA
_clr_all_ko:
	MOV R2, #0H
	ST R2, KOD0
	RT

; FUNCTION: GY454XE  Re 047F2
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
	MOV R0, #9AH
	MOV R1, #12H
	BL _delay
	MOV ER0, #0H
	L R2, [ER4]
	BC EQ, _$j_04936
	MOV R0, #4H
	MOV R1, #10H
	ST ER0, _last_key_scancode
	MOV ER0, #1H
_$j_04936:
	MOV R2, #0H
	ST R2, [ER4]
	MOV R0, R0
	POP ER4
ENDIF
	POP PC

; FUNCTION: GY454XE  Re 04832
_interrupt_stub:
	RTI

PUBLIC _base_n_submodes
PUBLIC _s_blank_line
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
PUBLIC _menu_vector_table
PUBLIC _menu_vector
PUBLIC _menu_vector_data
PUBLIC _menu_vector_dim
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
PUBLIC _unk_01c74

PUBLIC _f_02676
PUBLIC _f_0270E
PUBLIC _smart_strlen
PUBLIC _smart_strcpy
PUBLIC _smart_strcat
PUBLIC _init_num_struct
PUBLIC _num_exp_to_str
PUBLIC _f_0285C
PUBLIC _f_02986
PUBLIC _f_02AAA
PUBLIC _f_02AB2
PUBLIC _f_02ABA
PUBLIC _f_02ACA
PUBLIC _f_02ADE
PUBLIC _result_str_print
PUBLIC _basen_base_print
PUBLIC _f_02B3E
PUBLIC _get_token
PUBLIC _get_token_length
PUBLIC _print_continue_prompt
PUBLIC _f_02BE8
PUBLIC _num_output_print
PUBLIC _f_02C76
PUBLIC _f_02CB6
PUBLIC _buffer_clear_lastnline
PUBLIC _fill_screen
PUBLIC _buffer_clear
PUBLIC _f_02D52
PUBLIC _f_02D90
PUBLIC _draw_line
PUBLIC _line_print_col_0
PUBLIC _line_print
PUBLIC _char_print
PUBLIC _render
PUBLIC _get_screen_addr
PUBLIC _setup_status_bar
PUBLIC _f_032A4
PUBLIC _f_032F0
PUBLIC _f_0336A
PUBLIC _f_033AC
PUBLIC _print_4lines_4str
PUBLIC _print_4lines_head
PUBLIC _print_4lines
PUBLIC _f_0345E
PUBLIC _set_up_arrow
PUBLIC _set_down_arrow
PUBLIC _pd_value
PUBLIC _f_03518
PUBLIC _print_error
PUBLIC _f_03558
PUBLIC _is_char_keycode
PUBLIC _f_035D2
PUBLIC _f_035EC
PUBLIC _is_ac_key
PUBLIC _f_03660
PUBLIC _f_03664
PUBLIC _f_03672
PUBLIC _f_03688
PUBLIC _f_03698
PUBLIC _f_036A8
PUBLIC _f_036B8
PUBLIC _filter_chars_stat_mat_vct
PUBLIC _filter_chars
PUBLIC _filter_chars_table
PUBLIC _filter_chars_cmplx
PUBLIC _f_03714
PUBLIC _num_sum_1__
PUBLIC _f_03814
PUBLIC _f_0383C
PUBLIC _f_038C8
PUBLIC _f_03A1A
PUBLIC _f_03A72
PUBLIC _f_03DE6
PUBLIC _f_03E72
PUBLIC _f_042AA
PUBLIC _f_043AC
PUBLIC _f_043CE
PUBLIC _f_0441A
PUBLIC _f_04448
PUBLIC _f_04468
PUBLIC _f_0448A
PUBLIC _clear_result
PUBLIC _memzero
PUBLIC _f_044B6
PUBLIC _f_044CE
PUBLIC _f_044D2
PUBLIC _f_044D6
PUBLIC _f_044E2
PUBLIC _l_var
PUBLIC _st_var
PUBLIC _get_remaining_stack_space
PUBLIC _reset_magic_string
PUBLIC _need_reset
PUBLIC _shutdown
PUBLIC _delay
PUBLIC _stop_enable
PUBLIC _waitkey
PUBLIC _get_IRQ0
PUBLIC _f_046AE
PUBLIC _f_046C0
PUBLIC _f_046C4
PUBLIC _f_046CC
PUBLIC _f_046D4
PUBLIC _f_046E0
PUBLIC _clr_port0
PUBLIC _f_04738
PUBLIC _f_04760
PUBLIC _set_contrast_sfr
PUBLIC _set_scr_normal
PUBLIC _f_04796
PUBLIC _f_0479C
PUBLIC _set_disp_indicator
PUBLIC _set_contrast2_0
PUBLIC _set_all_kimask
PUBLIC _clr_all_kimask
PUBLIC _set_all_ko
PUBLIC _clr_all_ko
PUBLIC _is_key_pressed
PUBLIC _check_ac

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
EXTRN CODE	: _f_0AF30
EXTRN CODE	: _f_0B05A
EXTRN CODE	: _f_0B7B6
EXTRN CODE	: _f_0B8B8
EXTRN CODE	: _f_14800
EXTRN CODE	: _f_154E0
EXTRN CODE	: _f_154F2
EXTRN CODE	: _f_15526
EXTRN CODE	: _f_15532
EXTRN CODE	: _f_1553C
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
EXTRN CODE	: _num_invalid__
EXTRN CODE	: _num_cmp
EXTRN CODE	: _f_1B0DC
EXTRN CODE	: _num_fromdigit
EXTRN CODE	: _f_1B12C
EXTRN CODE	: _num_negate
EXTRN CODE	: _f_1B238
EXTRN CODE	: _f_1B288
EXTRN CODE	: _num_mulxp__
EXTRN CODE	: _num_trunc__
EXTRN CODE	: _num_frombyte
EXTRN CODE	: _num_cpy
EXTRN CODE	: _f_1B4D0
EXTRN CODE	: _invalid_var

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
