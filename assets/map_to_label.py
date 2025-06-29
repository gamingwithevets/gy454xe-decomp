import sys
import re
import pyu8disasx.labeltool.labeltool as labeltool


def map_to_label(mapfile, outfile):
	symbol_lines = []
	module_lines = []
	with open(mapfile, encoding = 'utf-8') as f:
		lines = f.readlines()

	# --- Extract Public Symbols Reference lines ---
	i = 0
	while i < len(lines) and lines[i].strip() != 'Public Symbols Reference':
		i += 1
	if i < len(lines):
		i += 4  # Skip header and separator lines
		while i < len(lines) and lines[i].strip():
			symbol_lines.append(lines[i].rstrip())
			i += 1

	# --- Extract Module listing lines ---
	i = 0
	while i < len(lines):
		if re.match(r'Module\s+Value\s+Type\s+Symbol', lines[i]):
			i += 2  # Skip header and separator
			while i < len(lines) and lines[i].strip():
				module_line = lines[i]
				if re.search(r'\d{2}:\w{4}', module_line):  # Only value+symbol lines
					module_lines.append(module_line.strip())
				i += 1
		else:
			i += 1

	labels = {}
	data_labels = {}

	# --- Process Public Symbols ---
	for line in symbol_lines:
		m = re.fullmatch(r'(.{30})0(.):(.{4})    (.{5})   .+', line)
		if not m:
			continue
		name = m.group(1).strip()
		if name == '_$$SP':
			continue
		if name.endswith('lw'):
			name = name[:-2]
		elif not ((name.endswith('_n') and not name.endswith('_base_n')) or name.endswith('_nn') or name[0] == '$'):
			name = name[1:]
		addr = (int(m.group(2), 16) << 16) + int(m.group(3), 16)
		if addr < 0x22:
			continue
		type_ = m.group(4).strip()
		if type_ in ('DATA', 'TABLE'):
			data_labels.setdefault(addr, name)
		else:
			labels.setdefault(addr, [name, True])  # Always True

	# --- Process Module Listings ---
	for line in module_lines:
		m = re.fullmatch(r'\s*(\d{2}):([0-9A-Fa-f]{4})\s+(Pub|Loc)\s+(\w+)\s+(\S+)', line)
		if not m:
			continue
		bank = int(m.group(1), 16)
		offset = int(m.group(2), 16)
		addr = (bank << 16) + offset
		type_ = m.group(4).strip()
		name = m.group(5).strip()

		if name.endswith('lw'):
			name = name[:-2]
		elif not ((name.endswith('_n') and not name.endswith('_base_n')) or name.endswith('_nn') or name[0] == '$'):
			name = name[1:]
		if addr < 0x22:
			continue
		if type_ in ('DATA', 'TABLE'):
			data_labels.setdefault(addr, name)
		else:
			labels.setdefault(addr, [name, True])  # Still always True, as per your decree

	with open(outfile, 'w', encoding = 'utf-8') as f: labeltool.save_labels(f, 0, labels, data_labels, {})

if __name__ == '__main__': map_to_label(sys.argv[1], sys.argv[2])
