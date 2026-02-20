#!/usr/bin/env python3
"""
DEBUG/RELEASE DIRECTORIES CLEANUP TOOL
@Author: Valery Stanchyk (improved by AI)
@Version: 1.0 - Production ready cleanup
"""

import os
import shutil
import sys
import logging
from pathlib import Path

# CONSTANTS
RELEASE_DIR_NAME = "release"
DEBUG_DIR_NAME = "debug"
LEVELS_UP_TO_BUILD_DIRS = "../"

class UTF8StreamHandler(logging.StreamHandler):
    """Windows UTF-8 console handler."""
    def __init__(self, stream=None):
        super().__init__(stream or sys.stdout)
        if sys.platform.startswith('win'):
            try:
                self.stream.reconfigure(encoding='utf-8')
            except AttributeError:
                pass

def setup_logging(verbose: bool = False):
    """Configure logging."""
    for handler in logging.root.handlers[:]:
        logging.root.removeHandler(handler)
    
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(levelname)s - %(message)s',
        handlers=[UTF8StreamHandler(sys.stdout)],
        force=True
    )
    return logging.getLogger(__name__)

class DirectoryCleaner:
    """Clean release/debug directories."""
    
    def __init__(self, verbose: bool = False):
        self.logger = setup_logging(verbose)
        self.build_dirs_list = []
    
    def scan_build_dirs(self):
        """Scan for build directories with release/debug names."""
        self.logger.info("Scanning for build directories...")
        
        try:
            build_dirs = os.listdir(LEVELS_UP_TO_BUILD_DIRS)
        except FileNotFoundError:
            self.logger.error(f"Build root not found: {LEVELS_UP_TO_BUILD_DIRS}")
            return False
        
        for dir_name in build_dirs:
            dir_path = os.path.join(LEVELS_UP_TO_BUILD_DIRS, dir_name)
            
            # Skip if no release/debug in name
            if (RELEASE_DIR_NAME.lower() not in dir_name.lower() and 
                DEBUG_DIR_NAME.lower() not in dir_name.lower()):
                self.logger.debug(f"Ignore (no release/debug): {dir_name}")
                continue
            
            # Skip files
            if os.path.isfile(dir_path):
                self.logger.debug(f"Ignore (file): {dir_name}")
                continue
            
            self.logger.info(f"Found build dir: {dir_name}")
            self.build_dirs_list.append(dir_name)
        
        self.logger.info(f"Found {len(self.build_dirs_list)} build directories")
        return bool(self.build_dirs_list)
    
    def clean_build_dirs(self):
        """Remove all contents from release/debug subdirectories."""
        total_files = 0
        total_dirs = 0
        
        for build_dir in self.build_dirs_list:
            build_path = os.path.join(LEVELS_UP_TO_BUILD_DIRS, build_dir)
            
            try:
                contents = os.listdir(build_path)
            except FileNotFoundError:
                self.logger.warning(f"Build dir missing: {build_path}")
                continue
            
            self.logger.info(f"Cleaning: {build_dir} ({len(contents)} items)")
            
            for item in contents:
                item_path = os.path.join(build_path, item)
                
                try:
                    if os.path.isfile(item_path):
                        os.remove(item_path)
                        self.logger.debug(f"Removed file: {item}")
                        total_files += 1
                    elif os.path.isdir(item_path):
                        shutil.rmtree(item_path)
                        self.logger.debug(f"Removed dir: {item}")
                        total_dirs += 1
                except Exception as e:
                    self.logger.error(f"Failed to remove {item_path}: {e}")
        
        self.logger.info(f"Cleanup complete: {total_files} files, {total_dirs} dirs removed")
        return True
    
    def run(self) -> int:
        """Execute cleanup workflow."""
        try:
            if not self.scan_build_dirs():
                self.logger.error("No build directories found!")
                return 1
            
            self.clean_build_dirs()
            self.logger.info("SUCCESS: All release/debug directories cleaned!")
            return 0
            
        except Exception as e:
            self.logger.error(f"FATAL: {e}")
            return 1

def main():
    """CLI entrypoint."""
    import argparse
    parser = argparse.ArgumentParser(description="Clean release/debug directories")
    parser.add_argument('--verbose', '-v', action='store_true', help="Verbose logging")
    args = parser.parse_args()
    
    cleaner = DirectoryCleaner(verbose=args.verbose)
    sys.exit(cleaner.run())

if __name__ == "__main__":
    main()
