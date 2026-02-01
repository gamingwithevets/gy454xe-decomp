import os
import re
import sys
import sympy
import msvcrt
import pyperclip

cls = lambda: os.system('cls')

class PrettyFloat(sympy.Float):
	def __new__(cls, value, **kwargs):
		# Force it to create a SymPy Float, like the parent does
		return super().__new__(cls, float(value), **kwargs)

	def __str__(self):
		val = float(self)
		return str(int(val)) if val == int(val) else str(val)

	__repr__ = __str__  # Make sure it prints the same in lists etc.

def num_conv(numb):
	if len(numb) != 10: raise ValueError('number data must be exactly 10 bytes')
	if len(numb) == 14: raise ValueError('14 byte numbers are not supported yet, sorry!')
	if numb == b'\0'*10: return sympy.S.Zero
	numh = numb.hex()
	if re.search(r'[bcdef]', numh[1:]): return sympy.nan
	area1 = int(numh[0], 16)
	area2 = numh[1:-4]
	if 'a' in area2 and area1 != 2: return sympy.nan
	area3 = int(numh[-4:-2])
	area4 = int(numh[-2:])
	match area1:
		# Floating point format / DMS
		case 0 | 4:
			num_tmp = f'{area2[0]}.{area2[1:]}e'
			match area4:
				case 0: return PrettyFloat(f'{num_tmp}-{100-area3}')
				case 1: return PrettyFloat(f'{num_tmp}{area3}')
				case 5: return PrettyFloat(f'-{num_tmp}-{100-area3}')
				case 6: return PrettyFloat(f'-{num_tmp}{area3}')
				case _: return sympy.nan

		# Fraction format
		case 2:
			neg = '-' if area4 == 1 else ''
			if not neg and area4 != 6: return sympy.nan
			frac = area2[:area3].split('a')
			match len(frac):
				case 2: return sympy.Rational(f'{neg}{frac[0]}/{frac[1]}')
				case 3: return sympy.Integer(f'{neg}{frac[0]}') + sympy.Rational(f'{neg}{frac[1]}/{frac[2]}')
				case _: return sympy.nan

		# Radical format
		case 8:
			if area4 == 0: area4 = area3
			neg1 = -1 if area4 == 1 else 1
			if neg1 == 1 and area4 != 6: return sympy.nan
			neg2 = -1 if area3 == 1 else 1
			if neg2 == 1 and area3 != 6: return sympy.nan
			if area2[7] != '0': return sympy.nan
			return sympy.factor(neg1 * sympy.sqrt(int(area2[:3])) * sympy.Rational(int(area2[3:5]), int(area2[5:7])) \
						+ neg2 * sympy.sqrt(int(area2[8:11])) * sympy.Rational(int(area2[11:13]), int(area2[13:15])))

		# Error
		case 0xf:
			match area2[0]:
				case 1: return 'AC Break'
				case 2: return 'Syntax ERROR'
				case 3: return 'Math ERROR'
				case 4: return 'Insufficient MEM'
				case 7: return 'Stack ERROR'
				case 8: return 'Argument ERROR'
				case 9: return 'Dimension ERROR'
				case 10: return "Can't Solve"
				case 11: return 'Time Out'
				case 12: return 'Variable ERROR'
				case 13: return 'NULL ERROR'
				case _: return '<invalid error>'

		# Invalid/Unsupported
		case _: return sympy.nan

def lookup(rom, addr):
	while True:
		cls()
		string = ''
		numb = rom[addr:addr+10]
		for byte in numb:
			string += ',' if string else ''
			string += '0' if byte > 0x9f else ' '
			string += f'{byte:02X}H'
		num = num_conv(numb)
		if type(num) == PrettyFloat: num = f'{num} | {sympy.nsimplify(num)}'
		print(f'Address: {addr:05X}\n  {string}\n\n  -> {num}\n[-] Previous  [=] Next\n[C] Copy      [E] Copy2\n[Q] Quit')
		char = msvcrt.getwch().lower()
		match char:
			case '-': addr -= 10
			case '=': addr += 10
			case 'c':
				pyperclip.copy(f'; DATA: GY454XE  Re {addr:05X}\n_unk_{addr:05x}:\n\tDB{string}')
			case 'e':
				pyperclip.copy(f'DB{string}  ; {num}')
			case 'q':
				cls()
				break

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Invalid number of parameters')
		sys.exit()

	with open(sys.argv[1], 'rb') as f: lookup(f.read(), int(sys.argv[2], 0))
