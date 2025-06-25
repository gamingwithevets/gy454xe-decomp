import sys
import re
import labeltool.labeltool as labeltool

def map_to_label(mapfile, outfile):
	symbol_lines = []
	with open(mapfile) as f:
		l = f.readline()
		while l != 'Public Symbols Reference\n': l = f.readline()
		for _ in range(3): f.readline()
		l = f.readline()
		while l[:-1]:
			symbol_lines.append(l[:-1])
			l = f.readline()

	labels = {}
	data_labels = {}
	for line in symbol_lines:
		m = re.fullmatch(r'(.{30})0(.):(.{4})    (.{5})   .+', line)
		name = m.group(1).strip()
		if name == '_$$SP': continue
		if name[-2:] == 'lw': name = name[:-2]
		elif not (name.endswith('_n') or name.endswith('_nn') or name[0] == '$'): name = name[1:]
		addr = (int(m.group(2)) << 16) + int(m.group(3), 16)
		if addr < 0x22: continue
		type = m.group(4).strip()
		if type == 'DATA' or type == 'TABLE':
			if addr not in data_labels: data_labels[addr] = name
		elif addr not in labels: labels[addr] = [name, True]

	with open(outfile, 'w') as f: labeltool.save_labels(f, 0, labels, data_labels, {})

if __name__ == '__main__': map_to_label(sys.argv[1], sys.argv[2])
