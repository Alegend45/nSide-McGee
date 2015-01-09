@echo off
rem Balanced profile
del /q "obj\sfc-*.o"
if exist "out\nSide-balanced.exe" del /q "out\nSide-balanced.exe"
copy "Makefile-balanced" "Makefile"
call cc.bat
rename "out\nSide.exe" "nSide-balanced.exe"

rem Accuracy profile
del /q "obj\sfc-*.o"
copy "Makefile-accuracy" "Makefile"
call cc.bat
@echo on