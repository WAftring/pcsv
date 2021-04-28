@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
mkdir build
cd build
rem cl /Zi /FC /EHsc /I ..\src /MTd ..\src\*.cpp /link kernel32.lib user32.lib advapi32.lib /out:"pcsv.exe"
cl /Zi /FC /EHsc /I ..\src /MT ..\src\*.cpp /link kernel32.lib user32.lib advapi32.lib /out:"pcsv.exe"
cd ..
