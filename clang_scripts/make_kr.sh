#!/bin/bash
# make_kr.sh

# Ensure we're in the root of the project (where .clang-format lives)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Write K&R style .clang-format config (optional)
cat > .clang-format <<EOF
BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
BreakBeforeBraces: Attach
AccessModifierOffset: -2

AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: true
AllowShortLoopsOnASingleLine: true

IndentCaseLabels: true
ColumnLimit: 100
EOF

# Format all relevant files in src/
find src -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
find include -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i

echo "Files have now been converted to K&R style."

