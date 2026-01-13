@echo off
set CONFIG=Release
set ARCHITECTURE=x64
@REM if exist build del build\*
if not exist build mkdir build
pushd build
cmake .. -G "Visual Studio 17 2022" -A %ARCHITECTURE% > nul
cmake --build . --config %CONFIG% 
popd
echo PROGRAM OUTPUT : 
bin\%CONFIG%\PhoneHelper.exe