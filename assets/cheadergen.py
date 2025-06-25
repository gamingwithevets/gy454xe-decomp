import re
import os
import argparse

CONTROL_KEYWORDS = {"if", "else", "for", "while", "switch", "case", "return", "do"}

def extract_functions_and_addresses(c_code):
    # Match optional address comment followed by a function definition
    pattern = re.compile(
        r'(?:\/\/\s*([0-9A-Fa-f]{5})(?:\s*-\s*STUB)?\s*\n)?'  # optional "// 1A2B3 - STUB"
        r'^\s*([a-zA-Z_][a-zA-Z0-9_ \t\*]*?)\s+'               # return type
        r'([a-zA-Z_][a-zA-Z0-9_]*)\s*'                         # function name
        r'\(([^)]*)\)\s*\{',                                   # parameters and opening brace
        re.MULTILINE
    )

    declarations = []

    for match in pattern.finditer(c_code):
        hex_addr, return_type, name, params = match.groups()

        if name in CONTROL_KEYWORDS:
            continue

        addr_line = f"// {hex_addr.upper():0>5}" if hex_addr else ""
        decl_line = f"{return_type.strip()} {name.strip()}({params.strip()});"

        if addr_line:
            declarations.append(addr_line)
        declarations.append(decl_line)

    return declarations

def generate_header_file(c_file_path, h_file_path):
    if not os.path.exists(c_file_path):
        print(f"🛑 File not found: {c_file_path}")
        return

    with open(c_file_path, 'r') as f:
        c_code = f.read()

    declarations = extract_functions_and_addresses(c_code)
    guard_macro = os.path.basename(h_file_path).replace('.', '_').upper()

    with open(h_file_path, 'w') as f:
        f.write(f"#ifndef {guard_macro}\n")
        f.write(f"#define {guard_macro}\n\n")

        for line in declarations:
            f.write(line + '\n')

        f.write(f"\n#endif // {guard_macro}\n")

    print(f"🧩 Header file '{h_file_path}' generated with {len(declarations)//2} functions (with address comments).")

def main():
    parser = argparse.ArgumentParser(
        description="🔍 Generate a decompiler-style C header from source with address comments above functions."
    )
    parser.add_argument("source", help="Path to the C source file (.c)")
    parser.add_argument(
        "-o", "--output",
        help="Path to the output header file (.h). Defaults to source name with .h extension."
    )

    args = parser.parse_args()
    source_path = args.source
    header_path = args.output if args.output else os.path.splitext(source_path)[0] + ".h"

    generate_header_file(source_path, header_path)

if __name__ == "__main__":
    main()
