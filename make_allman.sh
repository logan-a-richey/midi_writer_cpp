#!/bin/bash
# make_allman.sh

# Ensure we're in the root of the project (where .clang-format should live)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Write Allman-style .clang-format config
cat > .clang-format <<EOF
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
EOF

# Format all relevant files in src/
find src -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
find include -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i

echo "Files have now been converted to Allman style."

