This is a decompilation of the Casio ES PLUS **GYX** firmware, which includes code from the latest revisions of all calculators released before the fx-55 PLUS.

Feel free to contribute by forking this repo and making a pull request.

## General information
All ES PLUS models use the nX-U8/100 architecture by ROHM Co., Ltd.
The code is a mix of hand-written U8 assembly code, ANSI-C code compiled to U8 assembly with the **CCU8** compiler from the U8/U16 Development Tools, U8 implementations for some C library functions and the MCU's entry/startup code.

Due to different models having different features, a full decompilation of the firmware will need to have code sourced from multiple ROMs.

## About the code
In order to create an accurate decompilation, the code will need to be separated into 4 categories:
1. **Code written by Casio in ANSI-C** (`main`, `diagnostic`, `reset_all`, etc.): will be decompiled into **ANSI-C** code that can be compiled with CCU8 to produce matching assembly.
2. **Code written by Casio in ASM**: will be disassembled, reverse engineered and made to work with RASU8.
3. **Code from the SDK .lib files** (`memset`, etc): will not need to be decompiled. SDK code can be linked using the `/CC` option and the `libraries` field of RLU8.
4. **Code from the SDK MCU-specific files** (startup code, etc): will have to be recreated using existing files for other U8 MCUs, as the files for the specific MCU Casio used is not in the development tools.

The code was compiled with the LARGE memory model and NEAR data model options.

### Compiler
As (technically) corporate tools, CCU8 and the MACU8 Assembler Package (which, in combination, allows for compiling C code as a U8/U16 binary file) do not leave any traces or metadata in the final product. As such, the exact CCU8 version Casio used, as well as the exact compiler flags, cannot be determined, and will have to be guessed.

Additionally, the ClassWiz series is technically a fork of ES PLUS with the `/Zc` option disabled (which makes each constant its own relocatable segment).

#### Compiler entropy
A term coined by [the LEGO Island modding/decompiling community](https://youtu.be/gthm-0Av93Q?t=542), "compiler entropy" is a quirk with compilers where changing minor elements in the source code that shouldn't affect anything causes the compiler to generate different results, usually for the same function. This usually results in functions randomly dropping or rising in accuracy as more functions are implemented in a decomp. As of now, there is no real workaround for this phenomenon, nor is it known what exactly influences it.

This may not be related to this decomp at all, but it is worth mentioning.

### Naming scheme
As of now, we have not decided on a real naming scheme for the decomp. Scripts and headers that have not been given actual names are named `unk[n]`, where `[n]` is the order of the file's code in the final binary.

### Missing features
Currently, this decomp is missing the following features:
- Diagnostic mode present on fx-570/991ES PLUS `GY454X VerF`/`GY455X VerF`. This specific version of the diagnostic mode includes a [debug menu](https://gwe.42web.io/calcupedia/Calculator_self-test?i=1#Debug_menu) similar to that of the 2nd edition series.
- Japanese models' extra conversions, which includes an extra 160 conversions (80 conversion pairs). Unfortunately, no ROM of a Japanese ES PLUS GYX model has been dumped, nor has Casio distributed an emulator for the fx-510AZ. It is possible the code responsible is the exact same as the one used on the LY series.

### Building custom ROMs
To configure ROMs, edit `src/asm/features.inc`. Note that this is just for feature toggles; you will need to edit other scripts to modify menus, key layouts, etc. See the comment in the aforementioned file for more information.

As of currently, not all toggles have been implemented.

## Building
As the U8/U16 SDK only officially supports Windows, these instructions below assume a Windows environment.
1. Install the U8/U16 Development Tools / LAPIS Development Tools LEXIDE-Ω, or simply the necessary build tools (CCU8 + MACU8 Assembler Package). It may be difficult to find a copy legally.
2. Clone this repository with `git`: ```git clone https://github.com/gamingwithevets/gy454xe-decomp.git --recursive``` The `--recursive` flag is important as it will also set up all necessary submodules.
3. Assuming you have the SDK installed at `C:\LAPIS\LEXIDE`, copy `assets/ML610CASESplus.DCL` to `C:\LAPIS\LEXIDE\Dcl`.
4. Run `build.bat` in the root directory.
5. If needed, convert the outputted Intel Hex file to binary. There are many tools online, however this project uses the [Python Intel HEX library](https://pypi.org/project/intelhex/) for this.
