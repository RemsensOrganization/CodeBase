@echo off
REM Запуск fix_guards.py для папки code_base (относительный путь)

set PYTHON_EXE=python
set SCRIPT_PATH=fix_guards.py
set PROJECT_PATH=code_base

echo Run fix_guards.py %SCRIPT_PATH% for project %PROJECT_PATH%
%PYTHON_EXE% "%SCRIPT_PATH%" "%PROJECT_PATH%"

pause
