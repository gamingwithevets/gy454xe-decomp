'''
Find all functions defined in the decomp source tree and pack them into a label file.
Any functions in the format f_XXXXX will only have the address listed.

Usage:
   gen_labelfile.py <target> <output file>

   For this decomp, <target> is in the format "<version string> <Re|Im>".
   e.g.  for     real fx-570ES PLUS VerE: "GY454XE  Re"
   e.g.  for emulator fx-991ES PLUS VerE: "GY455XE  Im"
'''

import re
import sys
import glob
from pyu8disasx.labeltool import labeltool

def process_c_file(path, name):
	labels = {}

	finding_func = False
	finding_func_addr = None
	with open(path, encoding = 'utf-8') as f:
		for line in f.readlines():
			match = re.match(r'^// (?:FUNCTION|STUB): (.+) ([0-9A-F]{5})$', line[:-1])
			match2 = re.match(r'^\s*(?:static\s+)?(?:[a-zA-Z_][a-zA-Z0-9_]*\s+|\*\s*)*([a-zA-Z_][a-zA-Z0-9_]*)\s*\([^;{]*\)\s*\{', line[:-1])
			if match:
				if match.group(1) != name: continue
				finding_func = True
				finding_func_addr = int(match.group(2), 16)
			elif match2:
				if finding_func:
					n = match2.group(1)
					if n == f'f_{finding_func_addr:05X}': n = ''
					labels[finding_func_addr] = [n, True]
				finding_func = False
			else: finding_func = False

	return labels

def process_asm_file(path, name):
	labels = {}

	finding_func = False
	finding_func_addr = None
	with open(path, encoding = 'utf-8') as f:
		for line in f.readlines():
			match = re.match(r'^; (?:FUNCTION|STUB): (.+) ([0-9A-F]{5})$', line[:-1])
			match2 = re.match(r'^_([a-zA-Z_][a-zA-Z0-9_]*):', line[:-1])
			if match:
				if match.group(1) != name: continue
				finding_func = True
				finding_func_addr = int(match.group(2), 16)
			elif match2:
				if finding_func:
					n = match2.group(1)
					if n == f'f_{finding_func_addr:05X}': n = ''
					labels[finding_func_addr] = [n, True]
				finding_func = False
			else: finding_func = False

	return labels

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Incorrect number of arguments')
		sys.exit()

	name = sys.argv[1]
	outfile = sys.argv[2]

	labels = {}
	for file in glob.glob('..\\src\\c\\*.c'): labels.update(process_c_file(file, name))
	for file in glob.glob('..\\src\\asm\\*.asm'): labels.update(process_asm_file(file, name))

	with open(outfile, 'w', encoding = 'utf-8') as f: labeltool.save_labels(f, 0, dict(sorted(labels.items())), {}, {})
