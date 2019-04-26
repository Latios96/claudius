mkdir build
cd build
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
C:\cmake\bin\cmake -G "Visual Studio 14 2015 Win64" ..
REM C:\cmake\bin\cmake -G "Visual Studio 15 2017 Win64" ..
C:\cmake\bin\cmake --build . --config Release