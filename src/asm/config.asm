/*
== ES PLUS ROM CONFIGURATION FILE ==
This file is used for configuring the ROM settings located at the end of the ROM,
which includes default settings and version information.

For feature customization, see src/features/features.inc.
*/

TYPE(ML610CASESplus)
MODEL LARGE

TSEG #1 AT 0FFD4H

; == Default Settings ==
; Number format. 8 = Fix, 9 = Sci, Norm1 = 0, Norm2 = 1
	DB 0
; FixN/SciN. (Unused for Norm1 and Norm2)
	DB 0
; Decimal mark. 0 = Comma, 1 = Dot
	DB 1
; Angle Unit. 4 = Degree, 5 = Radian, 6 = Gradian
	DB 4
; Math input toggle.
	DB 1
; Fraction result. 0 = Improper fraction (d/c), 1 = Mixed fraction (ab/c)
	DB 0
; Complex result. 0: Polar coordinates (r∠θ), 1: Rectangular coordinates (a+bi)
	DB 1
; STAT: Frequency toggle.
	DB 0
; Recurring decimal result toggle. Only relevant on models with this feature.
	DB 0
; Manual simplification toggle. Only relevant on models with this feature.
	DB 0
; Decimal output toggle.
	DB 0
; Unused
	DB 0

; == Display Settings ==
; Unknown.
	DB 0
; Disable radical format.
	DB 0
; Recurring decimal display.
;   0 = Dot above first and last repeating digit. Used in JP and UK models.
;   1 = Overline. Used in American models.
;   2 = Underline. Unused.
;   3 = Parentheses. Used in VN models.
	DB 0
; Unknown.
	DB 0

; Unknown
DB 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15

TSEG #1 AT 0FFF4H

; == Version Info ==

; Version string. Must be exactly 6 characters.
	DB "GY45DX"
; Revision string. Must be exactly 2 characters.
	DB "--"
; ROM checksum (see diag_get_checksum for checksum algorithm).
; Must be manually filled in. (Seriously though, does Casio use an auto checksum calculator? I doubt it)
	DW 1BBAH

; Filler bytes. They don't do anything.
DB 1, 1
