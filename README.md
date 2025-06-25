This is a **WIP** decompilation of the Casio fx-570/991ES PLUS (`GY454X VerE`, `SUM 8929`) real calculator ROM.

## Why this ROM?
Casio fx-570/991ES PLUS VerE real ROM was chosen because it has been very well documented.
After this ROM is fully decompiled, the codebase may be expanded to include features in other GY models that are not in the fx-570/991ES PLUS (e.g. `VERIF` mode), and potentially expand the code to other Casio model families.

## General information
The fx-570/991ES PLUS uses the nX-U8/100 architecture by ROHM Co., Ltd.
The code is a mix of hand-written U8 assembly code, ANSI-C code compiled to U8 assembly with the **CCU8** compiler from the U8/U16 Development Tools, U8 implementations for some C library functions and the MCU's entry/startup code.

## About the code
In order to create an accurate decompilation, the code will need to be separated into 4 categories:
1. **Code written by Casio in ANSI-C** (`main`, `diagnostic`, `reset_all`, etc.): will be decompiled into **ANSI-C** code that can be compiled with CCU8 to produce matching assembly.
2. **Code written by Casio in ASM**: will be disassembled, reverse engineered and made to work with RASU8.
3. **Code from the SDK .lib files** (`memset`, etc): will not need to be decompiled. SDK code can be linked using the `/CC` option and the `libraries` field of RLU8.
4. **Code from the SDK MCU-specific files** (startup code, etc): will have to be recreated using existing files for other U8 MCUs, as the files for the specific MCU Casio used is not in the development tools.

The code was compiled with the LARGE memory model and NEAR data model options.

### Compiler
As (technically) corporate tools, CCU8 and the MACU8 Assembler Package (which, in combination, allows for compiling C code as a U8/U16 binary file) do not leave any traces or metadata in the final product. As such, the exact CCU8 version Casio used, as well as the exact compiler flags, cannot be determined, and will have to be guessed.

#### Compiler entropy
A term coined by [the LEGO Island modding/decompiling community](https://youtu.be/gthm-0Av93Q?t=542), "compiler entropy" is a quirk with compilers where changing minor elements in the source code that shouldn't affect anything causes the compiler to generate different results, usually for the same function. This usually results in functions randomly dropping in accuracy as more functions are implemented in a decomp. As of now, there is no real workaround for this phenomenon, nor is it known what exactly influences it.

This may not be related to this decomp at all, but it is worth mentioning.

### Naming scheme
As of now, we have not decided on a real naming scheme for the decomp. All files are named as `n_<name>`, where `n` is the order of the file's code in the final binary, and `<name>` is the actual filename.

## Building
As the U8/U16 SDK only officially supports Windows, these instructions below assume a Windows environment.
1. Install the U8/U16 Development Tools / LAPIS Development Tools LEXIDE-Ω, or simply the  It may be difficult to find a copy legally.
2. Assuming you have the SDK installed at `C:\LAPIS\LEXIDE`, copy `assets/ML610CASESplus.DCL` to `C:\LAPIS\LEXIDE\Dcl`.
3. Run `build.bat` in the root directory. Note that a successful build is NOT guaranteed.
4. Convert the outputted Intel Hex file to binary. This can be done in various ways, however I recommend using the [bincopy](https://pypi.org/project/bincopy/) Python library.
