@echo off
if exist build del build\*
if not exist build mkdir build
pushd build
cmake .. -G "Visual Studio 17 2022" -A x64 > nul
cmake --build . --config Release 
popd
echo PROGRAM OUTPUT : 
build\Release\PhoneHelper.exe