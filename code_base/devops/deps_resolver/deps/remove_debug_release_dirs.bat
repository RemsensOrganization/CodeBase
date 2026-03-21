@echo off
chcp 65001 >nul
title Dependency Cleanup Tool

echo ========================================
echo DEPENDENCY CLEANUP TOOL v1.0
echo ========================================
echo.

cd /D "%~dp0"
echo Current directory: %CD%
echo.

if exist "remove_debug_release_dirs.py" (
    echo Starting cleanup...
    echo ========================================
    python remove_debug_release_dirs.py
    
    if %ERRORLEVEL% equ 0 (
        echo.
        echo SUCCESS: Cleanup completed!
    ) else (
        echo.
        echo ERROR: Cleanup failed! Code: %ERRORLEVEL%
    )
) else (
    echo ERROR: remove_debug_release_dirs.py not found!
)

echo.
pause
