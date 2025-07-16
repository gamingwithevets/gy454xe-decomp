TYPE(ML610CASESplus)
MODEL LARGE

; unk0 was originally decompiled to C, however _num_unk_2__ uses a weird long copying algorithm that
; Casio would probably not do in C (casts would have to be used).

$$NCODunk0 SEGMENT CODE 2H ANY

RSEG $$NCODunk0

; FUNCTION: GY454XE  Re 04D64
_num_unk_1__:
	PUSH LR
	PUSH FP
	MOV FP, SP
	ADD SP, #-0EH
	PUSH XR8
	PUSH XR4
	MOV ER8, ER0
	MOV ER0, #0H
	ST ER0, -0EH[FP]
	ST ER0, -0CH[FP]
	MOV R10, #0H
_$j_04d7a:
	CMP R10, #18H
	BC GE, _$j_04d94
	MOV R2, #BYTE1 _num_256
	MOV R3, #BYTE2 _num_256
	MOV ER0, FP
	ADD ER0, #-0AH
	BL _num_cpy
	MOV ER2, FP
	ADD ER2, #-0AH
	MOV ER0, ER8
	BL _num_unk_1_1__
_$j_04d94:
	MOV ER0, ER8
	BL _num_to_byte
	MOV R1, #0H
	MOV ER2, #0H
	MOV R4, R10
	BC EQ, _$j_04dae
_$j_04da2:
	SLLC R3, #1
	SLLC R2, #1
	SLLC R1, #1
	SLL R0, #1
	ADD R4, #-1H
	BC NE, _$j_04da2
_$j_04dae:
	MOV ER4, ER0
	MOV ER6, ER2
	L ER0, -0EH[FP]
	L ER2, -0CH[FP]
	OR R0, R4
	OR R1, R5
	OR R2, R6
	OR R3, R7
	ST ER0, -0EH[FP]
	ST ER2, -0CH[FP]
	ADD R10, #8H
	MOV ER2, FP
	ADD ER2, #-0AH
	MOV ER0, ER8
	BL _num_cpy
	CMP R10, #18H
	BC LE, _$j_04d7a
	L ER0, -0EH[FP]
	L ER2, -0CH[FP]
	POP XR4
	POP XR8
	MOV SP, FP
	POP FP
	POP PC

; FUNCTION: GY454XE  Re 04DE0
_num_unk_2__:
	PUSH LR
	PUSH FP
	MOV FP, SP
	ADD SP, #-0EH
	PUSH BP
	PUSH ER8
	PUSH ER4
	MOV ER8, ER0
	MOV R4, #0H
_$j_04df2:
	MOV R12, R4
	MOV R13, #0H
	ADD BP, FP
	L R0, 6H[FP]
	ST R0, -4H[BP]
	L R1, 8H[FP]
	L R0, 7H[FP]
	ST ER0, 6H[FP]
	L R0, 9H[FP]
	MOV R1, #0H
	ST ER0, 8H[FP]
	ADD R4, #1H
	CMP R4, #4H
	BC LT, _$j_04df2
	MOV R2, #0H
	MOV ER0, ER8
	BL _num_fromdigit
	MOV R4, #3H
_$j_04e18:
	MOV R12, R4
	MOV R13, #0H
	ADD BP, FP
	L R2, -4H[BP]
	MOV ER0, FP
	ADD ER0, #-0EH
	BL _num_frombyte
	MOV ER2, FP
	ADD ER2, #-0EH
	MOV ER0, ER8
	BL _f_1A460
	CMP R4, #0H
	BC EQ, _$j_04e44
	MOV R2, #BYTE1 _num_256
	MOV R3, #BYTE2 _num_256
	MOV ER0, ER8
	BL _f_1A438
	ADD R4, #-1H
	BC AL, _$j_04e18
_$j_04e44:
	POP ER4
	POP ER8
	POP BP
	MOV SP, FP
	POP FP
	POP PC

EXTRN TABLE	: _num_256
EXTRN CODE	: _num_cpy
EXTRN CODE	: _num_unk_1_1__
EXTRN CODE	: _num_to_byte
EXTRN CODE	: _num_fromdigit
EXTRN CODE	: _num_frombyte
EXTRN CODE	: _f_1A460
EXTRN CODE	: _f_1A438

PUBLIC _num_unk_1__
PUBLIC _num_unk_2__
