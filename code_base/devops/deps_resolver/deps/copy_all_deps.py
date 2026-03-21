#!/usr/bin/env python3
"""
SCRIPT FOR RESOLVING ALL DEPENDENCIES
@Author: Valery Stanchyk (improved by AI)
@Date: 2026/02/20
@Version: 3.2 - Production ready + .gitkeep ignore EVERYWHERE
@Description: Copies/unpacks dependencies from categorized folders to release/debug build directories
"""

import os
import shutil
import sys
import logging
import argparse
from pathlib import Path
from typing import List, Optional

try:
    import py7zr
    PY7ZR_AVAILABLE = True
except ImportError:
    PY7ZR_AVAILABLE = False

# CONSTANTS
DEPS_EXE_FILE = "deps_exe"
DEPS_ONE_LEVEL_ABOVE_EXE_FILE = "deps_above_exe"
DEPS_ONLY_FOR_DEBUG = "deps_only_for_debug"
DEPS_ONLY_FOR_RELEASE = "deps_only_for_release"
RELEASE_DIR_NAME = "release"
DEBUG_DIR_NAME = "debug"
LEVELS_UP_TO_BUILD_DIRS = "../"
ARCHIVE_EXTENSIONS = {".7z"}

class UTF8StreamHandler(logging.StreamHandler):
    """Windows UTF-8 console handler."""
    def __init__(self, stream=None):
        super().__init__(stream or sys.stdout)
        if sys.platform.startswith('win'):
            try:
                self.stream.reconfigure(encoding='utf-8')
            except AttributeError:
                pass

class UTF8FileHandler(logging.FileHandler):
    """UTF-8 file handler."""
    def __init__(self, filename, mode='a', encoding='utf-8', **kwargs):
        super().__init__(filename, mode, encoding=encoding, **kwargs)

def setup_logging(verbose: bool = False):
    """Configure production logging."""
    for handler in logging.root.handlers[:]:
        logging.root.removeHandler(handler)
    
    level = logging.DEBUG if verbose else logging.INFO
    
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(levelname)s - %(message)s',
        handlers=[
            UTF8StreamHandler(sys.stdout),
            UTF8FileHandler('deps_copy.log', encoding='utf-8')
        ],
        force=True
    )
    return logging.getLogger(__name__)

class DependencyResolver:
    """Production dependency resolver for build automation."""
    
    def __init__(self, verbose: bool = False):
        self.logger = setup_logging(verbose)
        self.out_dir_list: List[str] = []
    
    def is_gitkeep(self, filename: str) -> bool:
        """Check if file is .gitkeep."""
        return filename == '.gitkeep'
    
    def is_archive_file(self, file_path: str) -> bool:
        """Check if file is .7z archive."""
        return Path(file_path).suffix.lower() in ARCHIVE_EXTENSIONS
    
    def unpack_archive(self, file_path: str, destination: str) -> bool:
        """Unpack .7z archive."""
        if not PY7ZR_AVAILABLE:
            self.logger.warning(f"py7zr not available: {os.path.basename(file_path)}")
            return False
        
        try:
            with py7zr.SevenZipFile(file_path, mode='r') as archive:
                archive.extractall(destination)
            self.logger.info(f"UNPACKED: {os.path.basename(file_path)} -> {destination}")
            return True
        except Exception as e:
            self.logger.error(f"Unpack failed {file_path}: {e}")
            return False
    
    def safe_copytree(self, src: str, dst: str):
        """Safely copy directory tree, skip existing items and .gitkeep."""
        src_path = Path(src)
        dst_path = Path(dst)
        
        for item in src_path.iterdir():
            s = str(item)
            d = dst_path / item.name
            
            # 🚫 SKIP .gitkeep files - только для Git
            if self.is_gitkeep(item.name):
                self.logger.debug(f"Skip .gitkeep: {item}")
                continue
                
            if item.is_dir():
                if d.exists():
                    self.logger.debug(f"Skip existing dir: {d}")
                    continue
                shutil.copytree(s, str(d))
                self.logger.info(f"Copied DIR: {item.name}")
            else:
                if d.exists():
                    self.logger.debug(f"Skip existing file: {d}")
                    continue
                if self.is_archive_file(s):
                    self.unpack_archive(s, str(dst_path))
                    continue
                shutil.copy2(s, str(d))
                self.logger.info(f"Copied FILE: {item.name}")
    
    def get_build_config_path(self, out_dir: str) -> str:
        """Determine release/debug subpath."""
        out_lower = out_dir.lower()
        if RELEASE_DIR_NAME in out_lower:
            return RELEASE_DIR_NAME
        if DEBUG_DIR_NAME in out_lower:
            return DEBUG_DIR_NAME
        return ""
    
    def resolve_deps(self, files_list: List[str], deps_folder: str, is_exe_level: bool):
        """Copy dependencies to target build directories - NO .gitkeep!"""
        self.logger.info(f"Processing deps: {deps_folder} ({len(files_list)} files)")
        
        for filename in files_list:
            # 🚫 SKIP .gitkeep В КОРЕ deps ПАПОК
            if self.is_gitkeep(filename):
                self.logger.debug(f"Skip .gitkeep in root: {deps_folder}/{filename}")
                continue
                
            src_path = os.path.join(deps_folder, filename)
            
            if not os.path.exists(src_path):
                self.logger.debug(f"Source missing: {src_path}")
                continue
            
            for out_dir in self.out_dir_list:
                # Determine target subdir
                if is_exe_level:
                    subdir = self.get_build_config_path(out_dir)
                    if not subdir:
                        continue
                    
                    # Skip config-specific deps
                    if deps_folder == DEPS_ONLY_FOR_RELEASE and subdir == DEBUG_DIR_NAME:
                        continue
                    if deps_folder == DEPS_ONLY_FOR_DEBUG and subdir == RELEASE_DIR_NAME:
                        continue
                else:
                    subdir = ""
                
                dest_path = os.path.join(LEVELS_UP_TO_BUILD_DIRS, out_dir, subdir)
                os.makedirs(dest_path, exist_ok=True)
                
                try:
                    if os.path.isfile(src_path):
                        if self.is_archive_file(src_path):
                            self.unpack_archive(src_path, dest_path)
                        else:
                            shutil.copy2(src_path, dest_path)
                            self.logger.info(f"Copied: {filename} -> {out_dir}/{subdir}")
                    elif os.path.isdir(src_path):
                        target_dir = os.path.join(dest_path, os.path.basename(src_path))
                        os.makedirs(target_dir, exist_ok=True)
                        self.safe_copytree(src_path, target_dir)
                        self.logger.info(f"Copied DIR: {filename} -> {out_dir}/{subdir}")
                except Exception as e:
                    self.logger.error(f"Copy failed {src_path} -> {dest_path}: {e}")
    
    def prepare_build_dirs(self):
        """Scan and prepare build directories."""
        self.logger.info("Scanning dependency folders...")
        
        # Get dep files exactly like original script
        dep_files = {
            DEPS_EXE_FILE: [],
            DEPS_ONE_LEVEL_ABOVE_EXE_FILE: [],
            DEPS_ONLY_FOR_DEBUG: [],
            DEPS_ONLY_FOR_RELEASE: []
        }
        
        for folder in dep_files:
            try:
                all_files = os.listdir(folder)
                # Filter out .gitkeep from listing
                dep_files[folder] = [f for f in all_files if not self.is_gitkeep(f)]
                self.logger.info(f"{folder}: {len(dep_files[folder])} items (excluding .gitkeep)")
            except FileNotFoundError:
                self.logger.debug(f"{folder} missing")
        
        self.logger.info("Scanning build directories...")
        build_dirs_list = os.listdir(LEVELS_UP_TO_BUILD_DIRS)
        
        for dir_name in build_dirs_list:
            dir_path = os.path.join(LEVELS_UP_TO_BUILD_DIRS, dir_name)
            
            if (RELEASE_DIR_NAME.lower() not in dir_name.lower() and 
                DEBUG_DIR_NAME.lower() not in dir_name.lower()):
                continue
                
            if not os.path.isdir(dir_path):
                continue
            
            self.logger.info(f"Found build dir: {dir_name}")
            
            # Create release/debug subdirs
            for subdir in [RELEASE_DIR_NAME, DEBUG_DIR_NAME]:
                full_path = os.path.join(LEVELS_UP_TO_BUILD_DIRS, dir_name, subdir)
                os.makedirs(full_path, exist_ok=True)
            
            self.out_dir_list.append(dir_name)
        
        self.logger.info(f"Found {len(self.out_dir_list)} build directories")
    
    def run(self) -> int:
        """Execute full dependency resolution workflow."""
        try:
            self.prepare_build_dirs()
            
            if not self.out_dir_list:
                self.logger.error("No build directories found! Looking for 'release'/'debug' in ../ folder names")
                return 1
            
            # Execute in original order
            self.resolve_deps(
                [f for f in os.listdir(DEPS_EXE_FILE) if not self.is_gitkeep(f)] if os.path.exists(DEPS_EXE_FILE) else [],
                DEPS_EXE_FILE, True
            )
            
            self.resolve_deps(
                [f for f in os.listdir(DEPS_ONE_LEVEL_ABOVE_EXE_FILE) if not self.is_gitkeep(f)] if os.path.exists(DEPS_ONE_LEVEL_ABOVE_EXE_FILE) else [],
                DEPS_ONE_LEVEL_ABOVE_EXE_FILE, False
            )
            
            self.resolve_deps(
                [f for f in os.listdir(DEPS_ONLY_FOR_DEBUG) if not self.is_gitkeep(f)] if os.path.exists(DEPS_ONLY_FOR_DEBUG) else [],
                DEPS_ONLY_FOR_DEBUG, True
            )
            
            self.resolve_deps(
                [f for f in os.listdir(DEPS_ONLY_FOR_RELEASE) if not self.is_gitkeep(f)] if os.path.exists(DEPS_ONLY_FOR_RELEASE) else [],
                DEPS_ONLY_FOR_RELEASE, True
            )
            
            self.logger.info("SUCCESS: All dependencies resolved! (.gitkeep excluded)")
            return 0
            
        except Exception as e:
            self.logger.error(f"FATAL: {e}")
            return 1

def main():
    """CLI entrypoint with --verbose option."""
    parser = argparse.ArgumentParser(description="Resolve build dependencies")
    parser.add_argument('--verbose', '-v', action='store_true', help="Verbose logging")
    args = parser.parse_args()
    
    resolver = DependencyResolver(verbose=args.verbose)
    sys.exit(resolver.run())

if __name__ == "__main__":
    main()
