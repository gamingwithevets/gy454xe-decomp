import sys

parser_names = (
	' calculus function',
	' regular function',
	' operator',
	' postfix',
	' number literal',
	' variable',
	' constant',
	' matrix/vector',
	' variable store',
	' seperator',
	'n end of input/execute',
	'n INVALID',
	'n INVALID',
	'n INVALID',
	'n INVALID',
	' Syntax ERROR',
)

def get(rom, addr):
	indices = rom[addr:addr+384]

	for i in range(256):
		shift_amt = 0 if i % 2 == 0 else 4
		idx = i * 3 // 2
		merge = (int.from_bytes(indices[idx:idx+2], 'little') >> shift_amt) & 0xffff
		print(f'Index 0x{i:02x} is a{parser_names[(merge >> 8) & 0xf]} token ({(merge >> 8) & 0xf}), argument value {merge & 0xff}')

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Invalid number of parameters')
		sys.exit()

	with open(sys.argv[1], 'rb') as f: get(f.read(), int(sys.argv[2], 0))
