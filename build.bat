@echo off
REM IF cl.exe not found, execute vcvarsall.bat first
WHERE cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
ECHO cl.exe NOT found, execute vcvarsall.bat
vcvarsall.bat
ECHO Run this script again
)

REM IF build diretory not exist, make it
IF NOT EXIST build mkdir build

ECHO ===========================
ECHO  Generate NMake Makefiles
ECHO ===========================
cd build && cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release & cd ..

ECHO ===========================
ECHO   Build project
ECHO ===========================
cmake --build "build"
