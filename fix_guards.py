#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Скрипт для рекурсивного исправления header guard в стиле Chromium.
# Основан на логике из build/write_buildflag_header.py

import os
import re

PROJECT_ROOT = "code_base"

def chromium_style_guard(path: str) -> str:
    """Генерация header guard в стиле Chromium (с путём от корня проекта)."""
    rel_path = os.path.relpath(path, PROJECT_ROOT)   # относительный путь
    name = rel_path.upper()
    if name[0].isdigit():
        name = "_" + name
    name = re.sub(r'\W', '_', name)  # заменяем всё кроме букв/цифр на _
    return name + "_"

def fix_header_guard(path: str):
    """Проверяет и исправляет header guard в одном файле."""
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    guard = chromium_style_guard(path)

    new_lines = []
    replaced = False

    for line in lines:
        if line.startswith("#ifndef"):
            new_lines.append(f"#ifndef {guard}\n")
            replaced = True
            continue
        elif line.startswith("#define"):
            new_lines.append(f"#define {guard}\n")
            continue
        elif line.strip().startswith("#endif"):
            new_lines.append(f"#endif  // {guard}\n")
            continue
        else:
            new_lines.append(line)

    if not replaced:
        header = [f"#ifndef {guard}\n", f"#define {guard}\n"]
        footer = [f"#endif  // {guard}\n"]
        new_lines = header + new_lines + footer

    with open(path, "w", encoding="utf-8") as f:
        f.writelines(new_lines)

def process_project(root: str):
    """Рекурсивно обходит проект и исправляет все заголовочные файлы."""
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            if filename.endswith((".h", ".hpp")):
                filepath = os.path.join(dirpath, filename)
                print(f"Fixing guard in {filepath}")
                fix_header_guard(filepath)

if __name__ == "__main__":
    process_project(PROJECT_ROOT)
