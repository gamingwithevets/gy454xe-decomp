import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import menu
import re
import enum

# Keycodes (last_key_keycode 80F5H)
class keycode(enum.IntEnum):
    K_SD        = 0x01  # STAT/DIST mode menu
    K_CMPLX     = 0x02  # CMPLX mode menu
    K_BASE      = 0x03  # BASE-N mode menu
    K_MATRIX    = 0x04  # MATRIX mode menu
    K_VECTOR    = 0x05  # VECTOR mode menu
    K_CONST     = 0x06  # Constant input
    K_CONV      = 0x07  # Conversion input
    K_CLR       = 0x08  # Data clear menu
    K_HYP       = 0x09  # Hyperbolic function select
    K_DRG       = 0x0A  # Angle unit select
    K_VERIFY    = 0x0B  # VERIF mode menu
    K_ATOMIC    = 0x0C  # Atomic weight menu
    K_RCL_A     = 0x0E  # Variable recall: A
    K_RCL_B     = 0x0F  # Variable recall: B
    K_RCL_C     = 0x10  # Variable recall: C
    K_RCL_D     = 0x11  # Variable recall: D
    K_RCL_E     = 0x12  # Variable recall: E
    K_RCL_F     = 0x13  # Variable recall: F
    K_RCL_X     = 0x14  # Variable recall: X
    K_RCL_Y     = 0x15  # Variable recall: Y
    K_RCL_M     = 0x16  # Variable recall: M
    K_STO_A     = 0x17  # Variable store: A
    K_STO_B     = 0x18  # Variable store: B
    K_STO_C     = 0x19  # Variable store: C
    K_STO_D     = 0x1A  # Variable store: D
    K_STO_E     = 0x1B  # Variable store: E
    K_STO_F     = 0x1C  # Variable store: F
    K_STO_X     = 0x1D  # Variable store: X
    K_STO_Y     = 0x1E  # Variable store: Y
    K_STO_M     = 0x1F  # Variable store: M
    K_PERCENT   = 0x25  # Percent sign (%)
    K_PAREN_L   = 0x28  # Opening parenthesis (
    K_PAREN_R   = 0x29  # Closing parenthesis )
    K_PLUS      = 0x2B  # Plus sign (+)
    K_COMMA     = 0x2C  # Function argument seperator (comma/semicolon depending on model)
    K_MINUS     = 0x2D  # Minus sign (not to be confused with the negative sign)
    K_PERIOD    = 0x2E  # Decimal point (dot/comma depending on model)
    K_0         = 0x30  # Number 0
    K_1         = 0x31  # Number 1
    K_2         = 0x32  # Number 2
    K_3         = 0x33  # Number 3
    K_4         = 0x34  # Number 4
    K_5         = 0x35  # Number 5
    K_6         = 0x36  # Number 6
    K_7         = 0x37  # Number 7
    K_8         = 0x38  # Number 8
    K_9         = 0x39  # Number 9
    K_COLON     = 0x3A  # Multi-statement colon
    K_EQUALS    = 0x3D  # Equals sign (=) used in equations
    K_VAR_A     = 0x41  # Variable A
    K_VAR_B     = 0x42  # Variable B
    K_VAR_C     = 0x43  # Variable C
    K_VAR_D     = 0x44  # Variable D
    K_VAR_E     = 0x45  # Variable E
    K_VAR_F     = 0x46  # Variable F
    K_MUL       = 0x4E  # Multiply sign (×)
    K_DIV       = 0x4F  # Division sign (÷)
    K_VAR_M     = 0x54  # Variable M
    K_FCTRIAL   = 0x57  # Factorial (x!)
    K_VAR_X     = 0x58  # Variable X
    K_VAR_Y     = 0x59  # Variable Y
    K_NEGATIVE  = 0x60  # Negative sign (-)
    K_LOG       = 0x68  # log₁₀ n
    K_POL       = 0x6C  # Pol( function
    K_REC       = 0x6D  # Rec( function
    K_EXP       = 0x74  # ×10 character
    K_EULER     = 0x81  # Euler's number (e)
    K_PI        = 0x82  # Pi
    K_ANS       = 0x8B  # Ans variable
    K_RAN       = 0x8C  # Ran#
    K_SIN       = 0xA0  # Sine function
    K_COS       = 0xA1  # Cosine function
    K_TAN       = 0xA2  # Tangent function
    K_LN        = 0xA3  # Natural logarithm function
    K_CONV_N    = 0xA5  # >Conv n (exclusive to Japanese models)
    K_ARCSIN    = 0xB0  # sin⁻¹ / Asn / Arcsin
    K_ARCCOS    = 0xB1  # cos⁻¹ / Acs / Arccos
    K_ARCTAN    = 0xB2  # tan⁻¹ / Atn / Arctan
    K_RND       = 0xB3  # Rnd( function
    K_HEX_A     = 0xB8  # Hexadecimal A
    K_HEX_B     = 0xB9  # Hexadecimal B
    K_HEX_C     = 0xBA  # Hexadecimal C
    K_HEX_D     = 0xBB  # Hexadecimal D
    K_HEX_E     = 0xBC  # Hexadecimal E
    K_HEX_F     = 0xBD  # Hexadecimal F
    K_PERMU     = 0xBE  # nPr
    K_COMBI     = 0xBF  # nCr
    K_RANINT    = 0xC2  # RanInt#( function
    K_FRAC      = 0xD0  # Fraction
    K_FRAC_ABC  = 0xD1  # Mixed fraction
    K_POW       = 0xD2  # xⁿ
    K_SQRT      = 0xD3  # Square root √
    K_POW_M1    = 0xD4  # x⁻¹
    K_POW_2     = 0xD5  # x²
    K_POW_3     = 0xD6  # x³
    K_CBRT      = 0xD7  # Cube root ∛
    K_NTH_RT    = 0xD8  # nth root ⁿ√
    K_E_POW     = 0xD9  # eⁿ
    K_10_POW    = 0xDA  # 10ⁿ
    K_LOGAB     = 0xDB  # logₐ b
    K_ABS       = 0xDC  # Absolute value
    K_INTEGRAL  = 0xDD  # Integral function (∫)
    K_DDX       = 0xDE  # Derivative function (d/dx)
    K_SUM       = 0xDF  # Summation function (∑)
    K_UP        = 0xE0  # Cursor Key Up
    K_DOWN      = 0xE1  # Cursor Key Down
    K_RIGHT     = 0xE2  # Cursor Key Right
    K_LEFT      = 0xE3  # Cursor Key Left
    K_MODE      = 0xE4  # MODE menu
    K_SETUP     = 0xE5  # SETUP menu
    K_AC        = 0xE6  # AC key
    K_OFF       = 0xE7  # Shutdown
    K_ALPHA     = 0xE8  # ALPHA modifier toggle
    K_SHIFT     = 0xE9  # SHIFT modifier toggle
    K_RCL       = 0xEA  # RCL modifier toggle
    K_STO       = 0xEB  # STO modifier toggle
    K_INS       = 0xEC  # Insert mode toggle (LineI only)
    K_APPROX    = 0xED  # Force decimal result *
    K_M_PLUS    = 0xEE  # M+
    K_M_MINUS   = 0xEF  # M-
    K_EXECUTE   = 0xF0  # Execute key **
    K_FACT      = 0xF1  # Prime factor display
    K_BASE_BIN  = 0xF2  # BASE-N: Binary
    K_BASE_OCT  = 0xF3  # BASE-N: Octal
    K_BASE_DEC  = 0xF4  # BASE-N: Decimal
    K_BASE_HEX  = 0xF5  # BASE-N: Hexadecimal
    K_DMS       = 0xF6  # Degrees Minutes Seconds
    K_DMS_R     = 0xF7  # Ditto but reverse (pretty much useless),
    K_ENG       = 0xF8  # Engineering notation. Divides the result by 10^-3 on each press (max 10^-9)
    K_ENG_R     = 0xF9  # Ditto but reverse. Multiples the result by 10^3 on each press (max 10^9),
    K_FMT_DEC   = 0xFA  # Standard/decimal format toggle [S⇔D]
    K_FMT_FRAC  = 0xFB  # Improper fraction/Mixed fraction toggle
    K_CALC      = 0xFC  # CALC
    K_SOLVE     = 0xFD  # SOLVE
    K_DEL       = 0xFE  # DEL

def process_str(string): return re.sub(r' +', ' ', string.strip())

class ParamInput(tk.Toplevel):
    def __init__(self, master, on_start):
        super().__init__(master)
        self.title("Menu Viewer by ChatGPT")
        self.geometry("400x220")
        self.on_start = on_start

        self.columnconfigure(0, weight=1)

        ttk.Label(self, text="ROM file").grid(row=0, column=0, sticky='w', padx=10, pady=(10, 0))
        self.rom_path_var = tk.StringVar()
        rom_frame = ttk.Frame(self)
        rom_frame.grid(row=1, column=0, sticky='ew', padx=10)
        rom_frame.columnconfigure(0, weight=1)
        self.rom_entry = ttk.Entry(rom_frame, textvariable=self.rom_path_var)
        self.rom_entry.grid(row=0, column=0, sticky='ew')
        ttk.Button(rom_frame, text="Browse", command=self.browse_file).grid(row=0, column=1, padx=5)

        ttk.Label(self, text="Menu array address").grid(row=2, column=0, sticky='w', padx=10, pady=(10, 0))
        self.addr_var = tk.StringVar(value="0x23c6")
        ttk.Entry(self, textvariable=self.addr_var).grid(row=3, column=0, sticky='ew', padx=10)

        ttk.Label(self, text="Menu count").grid(row=4, column=0, sticky='w', padx=10, pady=(10, 0))
        self.count_var = tk.StringVar(value="43")
        ttk.Entry(self, textvariable=self.count_var).grid(row=5, column=0, sticky='ew', padx=10)

        ttk.Button(self, text="Load", command=self.validate_and_start).grid(row=6, column=0, pady=15)

    def browse_file(self):
        file_path = filedialog.askopenfilename(title="Select ROM")
        if file_path:
            self.rom_path_var.set(file_path)

    def validate_and_start(self):
        path = self.rom_path_var.get()
        try:
            menus_addr = int(self.addr_var.get(), 0)
            count = int(self.count_var.get(), 0)
        except ValueError:
            messagebox.showerror("Invalid input", "Menu address or count is invalid.")
            return
        if not path:
            messagebox.showerror("Missing ROM", "ROM file not selected.")
            return

        try:
            with open(path, 'rb') as f:
                rom = f.read()
        except Exception as e:
            messagebox.showerror("File error", f"Could not read ROM file:\n{e}")
            return

        self.destroy()
        self.on_start(rom, menus_addr, count)


class MenuViewer(tk.Tk):
    def __init__(self, rom, menus_addr, count):
        super().__init__()
        self.title("Menu Viewer by ChatGPT")
        self.geometry("600x400")
        self.rom = rom
        self.menus_addr = menus_addr
        self.count = count
        self.menus = [menu.Menu.from_rom(rom, menus_addr + i * 16) for i in range(count)]

        self.idx = 1  # Current menu index
        self.create_widgets()
        self.bind_keys()
        self.update_menu_display()

    def create_widgets(self):
        self.columnconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)

        # Header label with ID
        self.label_id = ttk.Label(self, text="")
        self.label_id.grid(row=0, column=0, pady=5, sticky='n')

        # Menu string display
        self.menu_text = tk.Text(self, width=60, height=10, font=("ES PLUS Display", 18), wrap='none')
        self.menu_text.grid(row=1, column=0, pady=5, sticky='nsew')
        self.menu_text.config(state=tk.DISABLED)

        # Buttons for values [1]-[8]
        val_frame = ttk.Frame(self)
        val_frame.grid(row=2, column=0, pady=5)

        self.val_buttons = []
        for i in range(8):
            btn = ttk.Button(val_frame, text="", width=20, command=lambda i=i: self.select_value(i))
            btn.grid(row=i // 4, column=i % 4, padx=5, pady=5)
            self.val_buttons.append(btn)

        # Navigation buttons with exact CLI style labels
        nav_button_frame = ttk.Frame(self)
        nav_button_frame.grid(row=3, column=0, pady=10)

        self.btn_up = ttk.Button(nav_button_frame, text="", width=20, command=self.go_prev_page)
        self.btn_up.grid(row=0, column=0, padx=10, pady=2)

        self.btn_down = ttk.Button(nav_button_frame, text="", width=20, command=self.go_next_page)
        self.btn_down.grid(row=0, column=1, padx=10, pady=2)

        self.btn_left = ttk.Button(nav_button_frame, text="", width=20, command=self.go_parent)
        self.btn_left.grid(row=0, column=2, padx=10, pady=2)

        # Keycode label below navigation buttons
        self.nav_label = ttk.Label(self, text="", justify='center')
        self.nav_label.grid(row=4, column=0, pady=5)

        # Controls buttons frame
        ctrl_frame = ttk.Frame(self)
        ctrl_frame.grid(row=5, column=0, pady=10)

        self.btn_prev = ttk.Button(ctrl_frame, text="Previous (J)", command=self.prev_menu)
        self.btn_prev.grid(row=0, column=0, padx=10)

        self.btn_next = ttk.Button(ctrl_frame, text="Next (L)", command=self.next_menu)
        self.btn_next.grid(row=0, column=1, padx=10)

        self.btn_quit = ttk.Button(ctrl_frame, text="Quit (Q)", command=self.quit)
        self.btn_quit.grid(row=0, column=2, padx=10)

    def bind_keys(self):
        for i in range(1, 9): self.bind(str(i), lambda e: self.select_value(ord(e.char) - 0x31))
        self.bind('<Left>', lambda e: self.go_parent())
        self.bind('<Right>', lambda e: self.go_next_page())
        self.bind('<Up>', lambda e: self.go_prev_page())
        self.bind('<Down>', lambda e: self.go_next_page())
        self.bind('j', lambda e: self.prev_menu())
        self.bind('l', lambda e: self.next_menu())
        self.bind('q', lambda e: self.quit())

    def update_menu_display(self):
        m = self.menus[self.idx]

        # Update header
        self.label_id.config(text=f"[ID {self.idx}]")

        # Update menu text area
        self.menu_text.config(state=tk.NORMAL)
        self.menu_text.delete('1.0', tk.END)
        self.menu_text.insert(tk.END, "\n".join(m.string))
        self.menu_text.config(state=tk.DISABLED)

        # Update buttons text and state
        for i, btn in enumerate(self.val_buttons):
            val_str = process_str(m.get_val_str(i))
            if val_str == '--': btn.config(text = '--')
            else: btn.config(text=f"[{i + 1}] {val_str}")
            if m.get_val_type(i) == 1:
                btn.state(['!disabled'])
            else:
                btn.state(['disabled'])

        btn_up_str = process_str(m.get_prev_page_str())
        self.btn_up.config(text=f'{"[↑] " if btn_up_str != "--" else ""}{btn_up_str}')
        self.btn_up.state([f'{"!" if m.prev_page and not m.prev_page & 0x80 else ""}disabled'])
        btn_down_str = process_str(m.get_next_page_str())
        self.btn_down.config(text=f'{"[↓] "if btn_down_str != "--" else ""}{btn_down_str}')
        self.btn_down.state([f'{"!" if m.next_page and not m.next_page & 0x80 else ""}disabled'])
        btn_left_str = process_str(m.get_parent_str())
        self.btn_left.config(text=f'{"[←] "if btn_left_str != "--" else ""}{btn_left_str}')
        self.btn_left.state([f'{"!" if m.parent and not m.parent & 0x80 else ""}disabled'])

        # Update nav info label with keycode only (since the rest is on buttons)
        if m.keycode in keycode: self.nav_label.config(text=f"Keycode: {keycode(m.keycode).name} (0x{m.keycode:02x})")
        else: self.nav_label.config(text=f"Keycode: 0x{m.keycode:02x}")

    def select_value(self, val_idx):
        m = self.menus[self.idx]
        if m.get_val_type(val_idx) == 1:
            new_idx = m.values[val_idx]
            if new_idx and new_idx & 0x80 == 0:
                self.idx = new_idx
                self.update_menu_display()

    def go_prev_page(self):
        val = self.menus[self.idx].prev_page
        if val and val & 0x80 == 0:
            self.idx = val
            self.update_menu_display()

    def go_next_page(self):
        val = self.menus[self.idx].next_page
        if val and val & 0x80 == 0:
            self.idx = val
            self.update_menu_display()

    def go_parent(self):
        val = self.menus[self.idx].parent
        if val and val & 0x80 == 0:
            self.idx = val
            self.update_menu_display()

    def prev_menu(self):
        self.idx = (self.idx - 1) if self.idx > 0 else (self.count - 1)
        self.update_menu_display()

    def next_menu(self):
        self.idx = (self.idx + 1) if self.idx < self.count - 1 else 0
        self.update_menu_display()

    def quit(self):
        if messagebox.askyesno("Quit", "Are you sure you want to quit?"):
            self.destroy()


def main():
    root = tk.Tk()
    root.withdraw()  # Hide main window while param input is open

    def start_viewer(rom, addr, count):
        root.destroy()  # Close the hidden root
        app = MenuViewer(rom, addr, count)
        app.mainloop()

    ParamInput(root, start_viewer)
    root.mainloop()


if __name__ == '__main__':
    main()
