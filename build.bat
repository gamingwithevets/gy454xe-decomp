@echo off
where python >nul 2>&1
set /a nopython=%errorlevel%

setlocal EnableDelayedExpansion
set "libs="
set "cdefs="
set "asmdefs="
set "pad=00"

if exist obj\ rd /s /q obj
md obj

for /f "usebackq tokens=* delims=" %%A in (src\features\features.inc) do (
	set "line=%%A"
	for /f "tokens=* delims= " %%B in ("!line!") do set "line=%%B"
	echo !line! | findstr /C:"REAL" >nul
	if !errorlevel! == 0 (
		set "pad=FF"
	)
	if not "!line!"=="" (
		echo !line! | findstr /b /c:"//" >nul
		if errorlevel 1 (
			set "cdefs=!cdefs! /D!line!"
			set "asmdefs=!asmdefs! /DEF!line!"
		)
	)
)

:: In older versions of the SDK, arguments had to be supplied to setenv.bat.
:: Uncomment/comment the appropriate lines depending on your U8/U16 SDK environment.
set l=C:\LAPIS\LEXIDE
set p=%l%\BuildTools\R1_10_0a
call %p%\setenv.bat "%l%\Bin;%p%\Bin" "%p%\Inc;%l%\Inc" "%l%\Dcl" "%p%\Lib"
::call C:\LAPIS\LEXIDE\BuildTools\Ver.20231124\setVer20231124.bat

del /f /s /q ~*.c 2>nul

echo C definitions: !cdefs!
echo ASM definitions: !asmdefs!

echo Compiling...
for /r %%a in (*.c) do (
::	ccu8 /J /SS 980 /SD /TML610CASESplus /Om /Orpn /Zc /LP /Lv /ML %%a | findstr /L /C:"Error :"
	ccu8 !cdefs! /J /SS 980 /SD /TML610CASESplus /Om /Orpn /Zc /Faobj\%%~na.asm /Lv /ML %%a | findstr /L /C:"Error :"
	if !errorlevel! equ 0 goto exit
	echo Compiled %%~nxa.
)
cd obj
xcopy ..\src\asm\*.asm . /y >nul
echo Assembling...
for %%a in (*.asm) do (
	rasu8 %%~nxa !asmdefs! /G /SD /NWES /NPL /T4 | findstr /L /C:"Error " | findstr /V "File"
	if !errorlevel! equ 0 goto exit
	set libs=!libs! %%~na
	echo Assembled %%~nxa.
)
echo Object files:!libs!
echo Linking...
rlu8 /CC /D /S /SD!libs!,rom,,LONGU8.lib LU8100LW.lib; | findstr /L /C:"Error E"
if !errorlevel! equ 0 goto exit
echo.
echo Converting to Intel Hex...
ohu8 rom.abs; | findstr /L C:"Error "
if !errorlevel! equ 0 goto exit
endlocal & set "pad=%pad%"

echo Copying to `build` directory...
if exist build\ rd /s /q build
md build
move obj\rom.hex build\rom.hex >nul
move obj\rom.map build\rom.map >nul
cd build
if %nopython% equ 1 (
	echo Skipped binary file conversion, label file and disassembly generation (no Python^)
	echo Done!
	goto exit
)
pip show intelhex >nul 2>&1
if %errorlevel% equ 1 (
	echo Installing Intel HEX module first.
	pip install ..\assets\intelhex
)
echo Converting to binary file...
python ..\assets\intelhex\intelhex\scripts\hex2bin.py -p %pad% rom.hex rom.bin
echo Generating label file...
python ..\assets\map_to_label.py rom.map labels
echo Disassembling...
python ..\assets\disas.py rom.bin rom.asm
:done
echo Done!
goto exit

:exit
cd..
endlocal
::move *.i obj\ >nul
echo Press any key to exit.
pause >nul
exit /b
