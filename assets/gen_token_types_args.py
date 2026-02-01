import sys
import pyperclip

def gen(rom, addr):
	indices = rom[addr:addr+384]
	lines = [[
			'\t;  ', '    ', '    ', '   ', '    ', '    ', '   ', '    ', '    ',
			f'\n\tDB {indices[i]:08b}B, {indices[i+1]:08b}B, {indices[i+2]:08b}B\n'
			] for i in range(0, 384, 3)]
	lines2 = []

	for i in range(256):
		j = i * 1.5
		j_int = int(j)
		j_int_p1 = j_int+1
		tylidx = j_int_p1 // 3
		tyidx = 5 + (j_int_p1 % 3 - 1)*2
		arlidx = j_int // 3
		alidx = 2 + (j_int % 3)*2
		ahidx = alidx - 1 if alidx == 2 else tyidx + 1
		lines[tylidx][tyidx] = f'TY{i:02X}'
		lines[arlidx][alidx] = f'AL{i:02X}'
		lines[arlidx][ahidx] = f'AH{i:02X}'

	pyperclip.copy(''.join(''.join(line) for line in lines))

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Invalid number of parameters')
		sys.exit()

	with open(sys.argv[1], 'rb') as f: gen(f.read(), int(sys.argv[2], 0))
