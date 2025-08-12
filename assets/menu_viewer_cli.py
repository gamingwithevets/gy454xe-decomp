import os
import sys
import menu
import msvcrt
import tkinter

clear = lambda: os.system('cls')

def main(rom, menus_addr, count):
	menus = [menu.Menu.from_rom(rom, menus_addr + i * 16) for i in range(count)]

	exit = False
	idx = 1
	while not exit:
		clear()
		print(f'[ID {idx}]\n\n')
		print('\n'.join(menus[idx].string))
		print()
		for i in range(0, 8, 2): print(f'[{i+1}] {menus[idx].get_val_str(i)}  [{i+2}] {menus[idx].get_val_str(i+1)}')
		print(f'[↑] {menus[idx].get_prev_page_str()}  [↓] {menus[idx].get_next_page_str()}\n[←] {menus[idx].get_parent_str()}  Keycode   0x{menus[idx].keycode:02x}')
		print('\n[J] Previous    [L] Next\n[Q] Quit')
		match msvcrt.getwch().lower():
			case 'à':
				match ord(msvcrt.getwch()):
					case 0x48:
						val = menus[idx].prev_page
						if val and val & 0x80 == 0: idx = val
					case 0x4b:
						val = menus[idx].parent
						if val and val & 0x80 == 0: idx = val
					case 0x50:
						val = menus[idx].next_page
						if val and val & 0x80 == 0: idx = val
			case j if '1' <= j <= '8':
				val_idx = ord(j) - 0x31
				if menus[idx].get_val_type(val_idx) == 1: idx = menus[idx].values[val_idx]
			case 'j':
				if idx > 0: idx -= 1
				else: idx = len(menus) - 1
			case 'l':
				if idx < len(menus) - 1: idx += 1
				else: idx = 0
			case 'q': exit = True
	clear()

if __name__ == '__main__':
	if len(sys.argv) != 4:
		print('Invalid number of parameters')
		sys.exit()

	with open(sys.argv[1], 'rb') as f: main(f.read(), int(sys.argv[2], 0), int(sys.argv[3], 0))
