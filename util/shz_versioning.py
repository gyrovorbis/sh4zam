#!/usr/bin/env python3

## \file
#  \brief Sync SH4ZAM version across shz_version.h, CMakeLists.txt, and doc/Doxyfile.
#
#  Usage:
#       shz_versioning.py                    # read version from shz_version.h, sync others
#       shz_versioning.py major.minor.patch  # write version to all four files
#
#   \author    2026 Falco Girgis
#   \copyright MIT License

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent

VERSION_H    = ROOT / "include" / "sh4zam" / "shz_version.h"
CMAKE_LISTS  = ROOT / "CMakeLists.txt"
DOXYFILE     = ROOT / "doc" / "Doxyfile"


def read_version(header: Path) -> tuple[int, int, int]:
    pattern = re.compile(r"#define\s+SHZ_VERSION_(MAJOR|MINOR|PATCH)\s+(\d+)")
    fields: dict[str, int] = {}
    for line in header.read_text().splitlines():
        m = pattern.match(line)
        if m:
            fields[m.group(1)] = int(m.group(2))
    for key in ("MAJOR", "MINOR", "PATCH"):
        if key not in fields:
            raise ValueError(f"SHZ_VERSION_{key} not found in {header}")
    return fields["MAJOR"], fields["MINOR"], fields["PATCH"]


def parse_version_arg(version_str: str) -> tuple[int, int, int]:
    parts = version_str.strip().split(".")
    if len(parts) != 3 or not all(p.isdigit() for p in parts):
        raise ValueError(f"Version must be in major.minor.patch format, got: {version_str!r}")
    return int(parts[0]), int(parts[1]), int(parts[2])


def update_version_h(path: Path, major: int, minor: int, patch: int) -> None:
    replacements = {"MAJOR": major, "MINOR": minor, "PATCH": patch}
    pattern = re.compile(r"^(#define\s+SHZ_VERSION_(MAJOR|MINOR|PATCH)\s+)\d+(.*)$")
    lines = path.read_text().splitlines(keepends=True)
    updated = 0
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if m:
            component = m.group(2)
            lines[i] = f"{m.group(1)}{replacements[component]}{m.group(3)}\n"
            updated += 1
    if updated != 3:
        raise ValueError(f"Expected 3 version #define lines, found {updated} in {path}")
    path.write_text("".join(lines))


def update_cmake(path: Path, major: int, minor: int, patch: int) -> None:
    lines = path.read_text().splitlines(keepends=True)
    replacements = {
        "MAJOR": major,
        "MINOR": minor,
        "PATCH": patch,
    }
    pattern = re.compile(r"^(set\(SHZ_VERSION_(MAJOR|MINOR|PATCH)\s+)\d+(\))")
    updated = 0
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if m:
            component = m.group(2)
            lines[i] = f"{m.group(1)}{replacements[component]}{m.group(3)}\n"
            updated += 1
    if updated != 3:
        raise ValueError(f"Expected 3 cmake version lines, found {updated} in {path}")
    path.write_text("".join(lines))


def update_doxyfile(path: Path, major: int, minor: int, patch: int) -> None:
    version_str = f"{major}.{minor}.{patch}"
    pattern = re.compile(r"^(PROJECT_NUMBER\s*=\s*).*$")
    lines = path.read_text().splitlines(keepends=True)
    updated = 0
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if m:
            lines[i] = f"{m.group(1)}{version_str}\n"
            updated += 1
    if updated != 1:
        raise ValueError(f"Expected 1 PROJECT_NUMBER line, found {updated} in {path}")
    path.write_text("".join(lines))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", nargs="?", metavar="major.minor.patch",
                        help="Version string to write to all files (default: read from shz_version.h)")
    args = parser.parse_args()

    if args.version:
        major, minor, patch = parse_version_arg(args.version)
        update_version_h(VERSION_H, major, minor, patch)
        print(f"Updated {VERSION_H.name}")
    else:
        major, minor, patch = read_version(VERSION_H)
        print(f"Version read from {VERSION_H.name}: {major}.{minor}.{patch}")

    update_cmake(CMAKE_LISTS, major, minor, patch)
    print(f"Updated {CMAKE_LISTS.name}")

    update_doxyfile(DOXYFILE, major, minor, patch)
    print(f"Updated {DOXYFILE.name}")


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
