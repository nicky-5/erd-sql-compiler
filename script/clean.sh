#! /bin/bash

ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT"

echo "Cleaning generated files..."
rm ../src/frontend/lexical-analysis/flex-scanner.c
rm ../src/frontend/syntactic-analysis/bison-parser.c
rm ../src/frontend/syntactic-analysis/bison-parser.h

echo "All done."
