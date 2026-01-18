@echo off
setlocal
set CONFIG=Release
set ARCHITECTURE=x64
@REM if exist build del build\*
if not exist build mkdir build

where cmake >nul 2>nul
if errorlevel 1 (
    echo "CMake not found."
    exit /b 1
)

pushd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=%CONFIG% -A %ARCHITECTURE% >nul 2>nul
if errorlevel 1 (
    echo "Failed to generate build files."
    exit /b 1
)

cmake --build . --config %CONFIG% --parallel >nul 2>nul
if errorlevel 1 (
    echo "Failed to build program."
    exit /b 1
)

popd
echo PROGRAM OUTPUT : 
bin\%CONFIG%\PhoneHelper.exe
endlocal