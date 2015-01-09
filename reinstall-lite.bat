@echo off
rem Balanced profile
del /q "out\nSide-balanced.exe"
copy "Makefile-balanced" "Makefile"
call cc.bat
rename "out\nSide.exe" "nSide-balanced.exe"
@echo on