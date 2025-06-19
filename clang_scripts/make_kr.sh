#!/bin/bash
# make_kr.sh
# Apply K&R style formatting using clang-format

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(realpath "$SCRIPT_DIR/..")"
cd "$PROJECT_ROOT" || exit 1
cat > "$PROJECT_ROOT/.clang-format" <<EOF
BasedOnStyle: LLVM

IndentWidth: 4
UseTab: Never
TabWidth: 4
IndentCaseLabels: true

BreakBeforeBraces: Attach
AllowShortIfStatementsOnASingleLine: true
AllowShortLoopsOnASingleLine: true
AllowShortFunctionsOnASingleLine: Empty

BinPackArguments: false
BinPackParameters: false
AlignAfterOpenBracket: DontAlign
PenaltyBreakBeforeFirstCallParameter: 10000

Cpp11BracedListStyle: true
AllowAllConstructorInitializersOnNextLine: false
BreakConstructorInitializers: AfterColon

ColumnLimit: 100
AlignOperands: false
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false

SpaceBeforeParens: ControlStatements
SpacesInContainerLiterals: false
EOF

echo ".clang-format updated with K&R style."

for dir in src include bindings; do
    if [ -d "$dir" ]; then
        echo "Formatting $dir/..."
        find "$dir" -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
    fi
done

echo "All files formatted to traditional K&R style."
