@echo off
chcp 65001 >nul
echo Installing py7zr for 7z unpacking support...
pip install py7zr
if %ERRORLEVEL% neq 0 (
    echo WARNING: py7zr installation failed - .7z unpacking will be disabled
    echo Continuing without py7zr...
)

echo.
echo Changing to script directory: %~dp0
cd /D "%~dp0"
echo Current directory: %CD%

echo.
echo ========================================
echo run copy_all_deps.py...
echo ========================================
python copy_all_deps.py

if %ERRORLEVEL% equ 0 (
    echo.
    echo ========================================
    echo SUCCESS: Dependencies resolved! Check deps_copy.log
    echo ========================================
) else (
    echo.
    echo ========================================
    echo ERROR: Script failed! Code: %ERRORLEVEL%
    echo Check deps_copy.log for details
    echo ========================================
)

echo.
echo Press any key to exit...
pause >nul
