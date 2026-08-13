#!/usr/bin/env python3
"""
Progress tracker for LSD decomp project
Just counts stuff and prints it out
"""

import os
import re
from pathlib import Path

class ProgressTracker:
    def __init__(self, root_dir=None):
        self.root = Path(root_dir) if root_dir else Path(__file__).parent.parent
        self.src_dir = self.root / "src" / "lsdde"
        self.asm_dir = self.root / "asm" / "lsdde"

    def count_stubs(self):
        stub_file = self.asm_dir / "missing_stubs.s"
        if stub_file.exists():
            try:
                content = stub_file.read_text(encoding='utf-8', errors='ignore')
                return len(re.findall(r'^\.globl', content, re.MULTILINE))
            except Exception as e:
                print(f"Warning: Could not read {stub_file}: {e}")
                return 0
        return 0

    def count_raw_ghidra_vars(self):
        count = 0
        pattern = re.compile(r'iVar|uVar')
        
        for c_file in self.src_dir.glob("*.c"):
            try:
                content = c_file.read_text(encoding='utf-8', errors='ignore')
                count += len(pattern.findall(content))
            except Exception as e:
                print(f"Warning: Could not read {c_file}: {e}")
        
        return count

    def count_assembly_files(self):
        all_asm = list(self.asm_dir.glob("*.s"))
        stub_files = [
            "auto_stubs_combined.s", "auto_func_stubs.s", "auto_data_stubs.s",
            "missing_stubs.s", "stub_fns.s", "override.s"
        ]
        
        real_stubs = sum(1 for f in stub_files if (self.asm_dir / f).exists())
        
        return {
            "total": len(all_asm),
            "stubs": real_stubs
        }

    def count_c_files(self):
        return len(list(self.src_dir.glob("*.c")))

    def check_linker_hacks(self):
        build_script = self.root / "scripts" / "build_ps1.ps1"
        if not build_script.exists():
            return {"allow_multiple": False, "ignore_unresolved": False}
        
        try:
            content = build_script.read_text(encoding='utf-8', errors='ignore')
        except Exception as e:
            print(f"Warning: Could not read {build_script}: {e}")
            return {"allow_multiple": False, "ignore_unresolved": False}
        
        return {
            "allow_multiple": "--allow-multiple-definition" in content,
            "ignore_unresolved": "--unresolved-symbols=ignore-all" in content
        }

    def estimate_boot_progress(self):
        notes_file = self.root / "Notes.txt"
        if not notes_file.exists():
            return "Unknown"
        
        try:
            content = notes_file.read_text(encoding='utf-8', errors='ignore')
        except Exception as e:
            print(f"Warning: Could not read {notes_file}: {e}")
            return "Unknown"
        
        if "hangs on the licensing screen" in content:
            return "Licensing Screen (VSync/Timer issue)"
        elif "hangs on the loading screen" in content:
            return "Loading Screen (CD-ROM issue)"
        elif "boots past" in content:
            return "Partial Boot"
        else:
            return "Unknown"

    def calculate_overall_progress(self, stubs, raw_vars, linker_hacks):
        # Progress estimates (heuristic)
        stub_progress = max(0, 100 - (stubs / 10))  # Assume ~629 stubs to clear
        var_progress = max(0, 100 - (raw_vars / 100))  # Assume ~8648 vars to clean
        linker_progress = 100 if not (linker_hacks["allow_multiple"] or linker_hacks["ignore_unresolved"]) else 0
        
        # Weighted average
        overall = (stub_progress * 0.4) + (var_progress * 0.3) + (linker_progress * 0.3)
        
        return {
            "overall": round(overall, 1),
            "stub_progress": round(stub_progress, 1),
            "var_progress": round(var_progress, 1),
            "linker_progress": linker_progress
        }

    def run(self):
        print("=== LSD decomp progress ===")
        
        stubs = self.count_stubs()
        print(f"Stubs: {stubs}")
        
        raw_vars = self.count_raw_ghidra_vars()
        print(f"Raw Ghidra vars: {raw_vars}")
        
        asm_info = self.count_assembly_files()
        print(f"Assembly files: {asm_info['total']}")
        
        c_files = self.count_c_files()
        print(f"C files: {c_files}")
        
        linker_hacks = self.check_linker_hacks()
        print(f"Linker hacks: allow_multiple={linker_hacks['allow_multiple']}, ignore_unresolved={linker_hacks['ignore_unresolved']}")
        
        boot_status = self.estimate_boot_progress()
        print(f"Boot status: {boot_status}")
        
        progress = self.calculate_overall_progress(stubs, raw_vars, linker_hacks)
        print(f"Overall: {progress['overall']}%")
        print(f"Stub progress: {progress['stub_progress']}%")
        print(f"Var progress: {progress['var_progress']}%")
        print(f"Linker progress: {progress['linker_progress']}%")

if __name__ == "__main__":
    tracker = ProgressTracker()
    tracker.run()
