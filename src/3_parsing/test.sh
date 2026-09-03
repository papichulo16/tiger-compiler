#!/bin/bash

TEST_DIR="../../tests/"

PARSER="./parse"

if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Directory $TEST_DIR does not exist."
    exit 1
fi

if [ ! -x "$PARSER" ]; then
    echo "Error: Parser executable '$PARSER' not found or not executable."
    exit 1
fi

echo "Scanning files for parsing failures..."
echo "-------------------------------------"

for file in "$TEST_DIR"*; do
    if [ -f "$file" ]; then
        if "$PARSER" "$file" 2>&1 | grep -q "Parsing failed"; then
            echo "Failed: $file"
        fi
    fi
done
