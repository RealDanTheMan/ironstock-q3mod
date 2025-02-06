@echo off
setlocal

rem Path to your MSYS installation (adjust if needed)
set MSYS_PATH=C:\msys64
cd ..\..\

rem Run commands inside mingw32.exe using bash -c
"%MSYS_PATH%\mingw32.exe" /bin/bash -c "./shell/win/run-mod.sh"

endlocal
