@echo off

if not exist "build\vs2022-windows-arm64" mkdir build\vs2022-windows-arm64
cd build\vs2022-windows-arm64

cmake -G "Visual Studio 17 2022" -DCMAKE_GENERATOR_PLATFORM=arm64 -DAMBEO_SYSTEM_PROCESSOR=arm64 ..\..\..
if ERRORLEVEL 1 exit /b %ERRORLEVEL%

cd ..
cd ..
