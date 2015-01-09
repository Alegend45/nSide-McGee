@echo off
if exist "out\nSide.exe" del "out\nSide.exe"
mingw32-make -j8
if not exist "out\nSide.exe" (pause)