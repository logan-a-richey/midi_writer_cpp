#!/bin/bash
# make_allman.sh
# Format all C++ source/header files using Allman style

# Get absolute path to the root of the project
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(realpath "$SCRIPT_DIR/..")"
cd "$PROJECT_ROOT" || exit 1

# Create .clang-format config if it doesn't exist
if [ ! -f "$PROJECT_ROOT/.clang-format" ]; then
cat > "$PROJECT_ROOT/.clang-format" <<EOF
BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
BreakBeforeBraces: Allman
AccessModifierOffset: -2

AllowShortFunctionsOnASingleLine: false
AllowShortIfStatementsOnASingleLine: false
AllowShortLoopsOnASingleLine: false

IndentCaseLabels: true
ColumnLimit: 100

BinPackArguments: false
BinPackParameters: false
AllowAllConstructorInitializersOnNextLine: false
PenaltyBreakContainer: 0
AlignAfterOpenBracket: Align
EOF
fi

# Format .cpp/.h/.hpp files recursively under src/, include/, bindings/
for dir in src include bindings; do
    if [ -d "$dir" ]; then
        find "$dir" -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
    fi
done

echo "Formatted all source files to Allman style under src/, include/, and bindings/"

