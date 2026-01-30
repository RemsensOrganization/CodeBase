#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Скрипт для рекурсивного исправления header guard в стиле Chromium.
# Основан на логике из build/write_buildflag_header.py

import os
import re

def chromium_style_guard(path: str) -> str:
    """Генерация header guard в стиле Chromium."""
    name = os.path.basename(path).upper()
    if name[0].isdigit():
        name = "_" + name
    name = re.sub(r'\W', '_', name)
    return name + "_"

def fix_header_guard(path: str):
    """Исправляет header guard в одном файле."""
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    guard = chromium_style_guard(path)

    new_lines = []
    replaced = False
    for line in lines:
        if line.startswith("#ifndef") or line.startswith("#define"):
            if not replaced:
                new_lines.append(f"#ifndef {guard}\n")
                new_lines.append(f"#define {guard}\n")
                replaced = True
            # пропускаем старые строки
            continue
        new_lines.append(line)

    # Добавляем #endif в конце, если его нет
    if not any(l.strip().startswith("#endif") for l in lines):
        new_lines.append(f"#endif  // {guard}\n")

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
    # Укажи путь к корню проекта
    process_project("code_base")
