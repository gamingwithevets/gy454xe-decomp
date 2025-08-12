font = '''\
 ¿àáéíóöüú¡Ó¿▒▒▒\
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\
 !"#×%÷'()*+,-./\
0123456789:;<=>?\
@ABCDEFGHIJKLMNO\
PQRSTUVWXYZ[]^_\
−abcdefghijklmno\
pqrstuvwxyz{|}~ \
𝐢𝐞×⏨∞°ʳᵍ∠→⇒\
⌟≤≠≥√∫ᴀʙᴄₙ▶◀\
⁰¹²³⁴⁵⁶⁷⁸⁹₍₎±\
₀₁₂ꜰɴᴘµ▷\
∑α𝛾𝜀𝜃λμπσ𝜙ℓℏ■□₃▂\
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\
'''

def read_string(rom, addr):
	b = 0xff
	string = ''
	i = 0
	while b:
		b = rom[addr+i]
		string += font[b]
		i += 1
	return string, i

def read_string_4line(rom, addr):
	strings = []
	i = 0
	for _ in range(4):
		string, length = read_string(rom, addr+i)
		strings.append(string)
		i += length
	return strings

class Menu:
	def __init__(self, string, ret_mode, char_mode, values, prev_page, next_page, parent, keycode):
		self.string = string
		self.ret_mode = ret_mode
		self.char_mode = char_mode
		self.values = values
		self.prev_page = prev_page
		self.next_page = next_page
		self.parent = parent
		self.keycode = keycode

	@staticmethod
	def get_id_str(val):
		if val & 0x80: return f'Value 0x{val:02x}'
		else: return f'ID {val:7}'

	def get_val_str(self, val_idx):
		val = self.values[val_idx]
		match self.get_val_type(val_idx):
			case 0: return '--        '
			case 1: return self.get_id_str(val)
			case 2: return f'Token 0x{val:02x}'
			case 3: return f'Value {val:4}'

	def get_val_type(self, val_idx):
		ret = self.ret_mode & (1 << (7 - val_idx))
		char = self.char_mode & (1 << (7 - val_idx))
		val = self.values[val_idx]
		if ret:
			if char: return 2 if val else 0
			else: return 3 if val else 0
		else: return 1

	def get_prev_page_str(self):
		if self.prev_page: return self.get_id_str(self.prev_page)
		else: return '--        '

	def get_next_page_str(self):
		if self.next_page: return self.get_id_str(self.next_page)
		else: return '--        '

	def get_parent_str(self):
		if self.parent: return self.get_id_str(self.parent)
		else: return '--        '

	@classmethod
	def from_rom(cls, rom, addr):
		string = read_string_4line(rom, (rom[addr+1] << 8) | rom[addr])
		ret_mode = rom[addr+2]
		char_mode = rom[addr+3]
		values = rom[addr+4:addr+12]
		prev_page = rom[addr+12]
		next_page = rom[addr+13]
		parent = rom[addr+14]
		keycode = rom[addr+15]
		return cls(string, ret_mode, char_mode, values, prev_page, next_page, parent, keycode)
