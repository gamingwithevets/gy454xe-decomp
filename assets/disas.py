# Note: This script may be integrated into PyU8disasX.

import math
import pyu8disasx.disas as disas
import labeltool.labeltool as labeltool

def process_ins_param(param):
	if type(param) == list: return ', '.join(param)
	elif type(param) == disas.Address:
		if param.seg is None:
			addr = param.addr.value
			if addr in dis.data_labels: return dis.data_labels[addr]
		else:
			addr = (param.seg.value << 16) | param.addr.value
			if addr in dis.labels: return dis.labels[addr][1]
	elif type(param) == disas.DSRPrefix:
		if type(param.dsr) == disas.Num and type(param.item) == disas.Num:
			addr = (param.dsr.value << 16) | param.item.value
			if addr in dis.data_labels: return dis.data_labels[addr]
	elif type(param) == disas.BitOffset and type(param.item) == disas.Address:
		addr = param.item.addr.value
		if addr in dis.data_labels: return f'{dis.data_labels[addr]}.{param.bit}'

	return str(param)

import sys, os
if len(sys.argv) > 2:
	filename = os.path.join(os.getcwd(), sys.argv[1])
	out = os.path.join(os.getcwd(), sys.argv[2])
else: sys.exit()
os.chdir(os.path.dirname(os.path.abspath(__file__)))

with open(filename, 'rb') as f: dis = disas.Disassembly(f.read())
dis.disassemble()
with open(os.path.join(os.path.dirname(filename), 'labels')) as f: labels, data_labels, _ = labeltool.load_labels(f, 0)
for k, v in labels.items():
	if v[1]: dis.labels[k] = [disas.labeltype.FUN, ('' if v[0].endswith('u8') or v[0].endswith('_n') or v[0].endswith('_nn') else '_')+v[0].replace('.', '_')]
	else: dis.labels[k] = [disas.labeltype.LAB, f'_${labels[v[2]][0]}_{v[0][1:]}']
for k, v in data_labels.items(): dis.data_labels[k] = '_' + v.replace('.', '_')
for k in labels.keys(): dis.queue_add(k)
dis.disassemble()

with open(out, 'w') as f:
	f.write('TYPE(ML610CASESplus)\nMODEL LARGE\n\n')

	l = math.ceil(max(len(v) for v in dis.data_labels.values()) / 4) * 4
	equs = ''
	extrns = ''
	dis.data_labels = dict(sorted(dis.data_labels.items()))
	for k, v in dis.data_labels.items():
		tabs = '\t'*math.ceil((l - len(v)) / 4)
		if k >= 0x8000: equs += f'{v}{tabs}EQU {"" if hex(k)[2].isnumeric() else "0"}{k:04X}H\n'
		else: extrns += f'EXTRN DATA\t: {v}{tabs}; {k:05X}\n'

	f.write(f'{equs}\n{extrns}\n')

	for addr, ins in dis.code.items():
		if addr in dis.labels:
			if dis.labels[addr][0] == disas.labeltype.FUN: f.write(f'\n; {addr:05X}\n')
			f.write(f'{dis.labels[addr][1]}:\n')
		instrl = ins[0]
		instr = ins[1]
		tab = '\t'
		#string = f'{addr >> 16:X}:{addr & 0xfffe:04X}H\t\t{"".join([format(a, "04X") for a in instrl])}{tab*(3-len(instrl))}\t{instr[0]}'
		string = f'\t{instr[0]}'
		if len(instr) >= 2: string += ' ' + process_ins_param(instr[1])
		if len(instr) == 3: string += ', ' + process_ins_param(instr[2])
		f.write(string + '\n')

	f.write('\n')
	for k, v in dict(sorted(dis.labels.items())).items():
		if v[0] == disas.labeltype.FUN: f.write(f'PUBLIC {v[1]}  ; {k:05X}\n')

	f.write('\nEND\n')
	f.close()
