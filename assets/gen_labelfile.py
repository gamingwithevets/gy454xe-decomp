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
	data_labels = {}

	finding_func = False
	finding_func_addr = None
	finding_data = False
	finding_data_addr = None
	with open(path, encoding = 'utf-8') as f:
		for i, line in enumerate(f.readlines()):
			match = re.match(r'^// (?:FUNCTION|STUB): (.+) ([0-9A-F]{5})$', line[:-1])
			match2 = re.match(r'^\s*(?:static\s+)?(?:[a-zA-Z_][a-zA-Z0-9_]*\s+|\*\s*)*([a-zA-Z_][a-zA-Z0-9_]*)\s*\([^;{]*\)\s*\{', line[:-1])
			match3 = re.match(r'^// DATA: (.+) ([0-9A-F]{5})$', line[:-1])
			match4 = re.match(r'^^\s*(?:static\s+)?(?:const\s+)?(?:volatile\s+)?(?:unsigned\s+|signed\s+)?(?:short\s+|long\s+|long\s+long\s+)?[a-zA-Z_][a-zA-Z0-9_]*\s+(?:\*+\s*)*([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*[^;]+)?\s*;', line[:-1])
			if match:
				finding_data = False
				if match.group(1) != name: continue
				finding_func = True
				finding_func_addr = int(match.group(2), 16)
			elif match2:
				if finding_func:
					n = match2.group(1)
					if n == f'f_{finding_func_addr:05X}': n = ''
					labels[finding_func_addr] = [n, True]
				finding_func = False
				finding_data = False
			elif match3:
				finding_func = False
				if match3.group(1) != name: continue
				finding_data = True
				finding_data_addr = int(match3.group(2), 16)
			elif match4:
				if finding_data:
					n = match4.group(1)
					if n == f'd_{finding_data_addr:05X}' or n == f'unk_{finding_data_addr:05x}': n = ''
					data_labels[finding_data_addr] = n
				finding_func = False
				finding_data = False
			else:
				finding_func = False
				finding_data = False

	return labels, data_labels

def process_asm_file(path, name):
	labels = {}
	data_labels = {}

	finding_func = False
	finding_func_addr = None
	finding_data = False
	finding_data_addr = None
	with open(path, encoding = 'utf-8') as f:
		for line in f.readlines():
			match = re.match(r'^; (?:FUNCTION|STUB): (.+) ([0-9A-F]{5})$', line[:-1])
			match2 = re.match(r'^_([a-zA-Z_][a-zA-Z0-9_]*):', line[:-1])
			match3 = re.match(r'^; DATA: (.+) ([0-9A-F]{5})$', line[:-1])
			if match:
				finding_data = False
				if match.group(1) != name: continue
				finding_func = True
				finding_func_addr = int(match.group(2), 16)
			elif match2:
				if finding_func:
					n = match2.group(1)
					if n == f'f_{finding_func_addr:05X}': n = ''
					labels[finding_func_addr] = [n, True]
				elif finding_data:
					n = match2.group(1)
					if n == f'd_{finding_data_addr:05X}' or n == f'unk_{finding_data_addr:05x}': n = ''
					data_labels[finding_data_addr] = n
				finding_func = False
				finding_data = False
			elif match3:
				finding_func = False
				if match3.group(1) != name: continue
				finding_data = True
				finding_data_addr = int(match3.group(2), 16)
			else:
				finding_func = False
				finding_data = False

	return labels, data_labels

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Incorrect number of arguments')
		sys.exit()

	name = sys.argv[1]
	outfile = sys.argv[2]

	labels = {}
	data_labels = {}
	for file in glob.glob('..\\src\\c\\**\\*.c', recursive = True):
		print(f'Processing: {file}')
		l, dl = process_c_file(file, name)
		labels.update(l)
		data_labels.update(dl)
	for file in glob.glob('..\\src\\asm\\*.asm'):
		print(f'Processing: {file}')
		l, dl = process_asm_file(file, name)
		labels.update(l)
		data_labels.update(dl)

	with open(outfile, 'w', encoding = 'utf-8') as f: labeltool.save_labels(f, 0, dict(sorted(labels.items())), dict(sorted(data_labels.items())), {})
	print(f'Done! Generated {len(labels)} function labels, {len(data_labels)} data labels')
