import sys
import pyperclip

def gen(rom, addr):
	indices = rom[addr:addr+384]
	lines = [['\t;          ', '  ', '  ', '  ', '     ', '  ', '  ', '  ', f'\n\tDB {"0" if indices[i]>0x9f else " "}{indices[i]:02X}H, {indices[i+1]:08b}B, {indices[i+2]:08b}B\n'] for i in range(0, 384, 3)]
	lines2 = []

	for i in range(256):
		j = i * 1.5
		j_int = int(j)+1
		lidx = j_int // 3
		idx = (j_int % 3 - 1)*4 + (2 if j.is_integer() else 0)
		lines[lidx][idx] = lines[lidx][idx][:-2] + '--'
		lines[lidx][idx+1] = f'{i:02X}'

	pyperclip.copy(''.join(''.join(line) for line in lines))

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Invalid number of parameters')
		sys.exit()

	with open(sys.argv[1], 'rb') as f: gen(f.read(), int(sys.argv[2], 0))
