TYPE(ML610CASESplus)
MODEL LARGE

; ROM configuration and version information. This is always at address 1:FFD4H in all ES PLUS ROMs.
CSEG #1 AT 0FFD4H

; == Default Settings ==
_setup_default:
; Values are in order as follows:
; #0  -> 8102H: Number format. 8 = Fix, 9 = Sci, Norm1 = 0, Norm2 = 1
; #1  -> 8103H: FixN/SciN. Unused for Norm1 and Norm2
; #2  -> 8104H: Decimal mark. 0 = Comma, 1 = Dot
; #3  -> 8105H: Angle Unit. 4 = Degree, 5 = Radian, 6 = Gradian
; #4  -> 8106H: Math input toggle.
; #5  -> 8107H: Fraction result. 0 = Improper fraction (d/c), 1 = Mixed fraction (ab/c)
; #6  -> 8108H: Complex result. 0: Polar coordinates (r∠θ), 1: Rectangular coordinates (a+bi)
; #7  -> 8109H: STAT: Frequency toggle.
; #8  -> 810AH: Recurring decimal result toggle. Only relevant on models with this feature.
; #9  -> 810BH: Manual simplification toggle. Only relevant on models with this feature.
; #10 -> 810CH: Decimal output toggle.
; #11         : Terminator byte for alignment. Not used.
	DB 0, 0, 1, 4, 1, 0, 1, 0, 0, 0, 0, 0

; Unknown.
DB 0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15

CSEG #1 AT 0FFF4H

; == Version Info ==

; Version string. Must be exactly 6 characters.
_version:
	DB "GY454X"
; Revision string. Must be exactly 2 characters.
	DB "E "
; ROM checksum (see _diag_get_checksum for checksum algorithm).
; Must be manually filled in.
_checksum:
DW 1234H

; Filler bytes. They don't do anything.
DB 1, 1

PUBLIC _setup_default
PUBLIC _version
PUBLIC _checksum

