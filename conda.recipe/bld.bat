REM Build datatypes.

REM Set up destinations dirs for install
@REM mkdir "%LIBRARY_PREFIX%\tests\swift"
mkdir build

REM test runner script
@REM copy "%RECIPE_DIR%\datatypes_run_tests.bat" "%LIBRARY_PREFIX%\tests\swift\"

REM build everything
cd build

set PATH="%PREFIX%\bin";%PATH%

cmake -G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ..\
if %errorlevel% neq 0 exit 1
msbuild /p:Configuration=Release /v:q /clp:/v:q "INSTALL.vcxproj"
if %errorlevel% neq 0 exit 1
xcopy /S /Y install\lib\*.* %LIBRARY_LIB%\
xcopy /S /Y install\include\*.* %LIBRARY_INC%\
if %errorlevel% neq 0 exit 1
del *.*

@REM cmake -G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ..\tests
@REM if %errorlevel% neq 0 exit 1
@REM msbuild /p:Configuration=Release /v:q /clp:/v:q "INSTALL.vcxproj"
@REM if %errorlevel% neq 0 exit 1

REM Install the build stuff
@REM move install\bin\datatypes_tests.exe %LIBRARY_PREFIX%\tests\swift\
