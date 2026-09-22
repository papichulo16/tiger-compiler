#!/usr/bin/env bash
#
# run_tests.sh - build sem_anal and run it against every .tig file in
#                tests/, checking that error programs report a
#                compilation error and that normal programs don't.
#
# Usage: ./run_tests.sh [-v]
#   -v   also print the compiler output for each test

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$SCRIPT_DIR/../../tests"
BIN="$SCRIPT_DIR/sem_anal"

VERBOSE=0
[ "${1:-}" = "-v" ] && VERBOSE=1

# tests/*.tig files (the classic Appel-style suite) that are expected to
# report a compilation error. Anything else directly under tests/ is
# expected to compile clean. Files under tests/parser_errors/ and
# tests/semantic_errors/ are always expected to error.
EXPECTED_ERROR_TESTS=(
  test9 test10 test11 test13 test14 test15 test16 test17 test18 test19
  test20 test21 test22 test23 test24 test25 test26 test28 test29 test31
  test32 test33 test34 test35 test36 test38 test39 test40 test43 test45
  test49
)

is_expected_error() {
  local name="$1"
  local e
  for e in "${EXPECTED_ERROR_TESTS[@]}"; do
    [ "$e" = "$name" ] && return 0
  done
  return 1
}

echo "Building sem_anal..."
make -C "$SCRIPT_DIR" sem_anal >/tmp/sem_anal_build.log 2>&1
if [ ! -x "$BIN" ]; then
  echo "Build failed, see /tmp/sem_anal_build.log"
  cat /tmp/sem_anal_build.log
  exit 1
fi

pass=0
fail=0
declare -a failures=()

run_one() {
  local file="$1"
  local expect_error="$2"
  local rel="${file#"$TESTS_DIR"/}"

  local output
  output="$("$BIN" "$file" 2>&1)"
  local code=$?

  local got_error=0
  [ $code -ne 0 ] && got_error=1

  if [ "$VERBOSE" -eq 1 ]; then
    echo "--- $rel ---"
    echo "$output"
  fi

  if [ "$expect_error" -eq "$got_error" ]; then
    pass=$((pass + 1))
    printf "PASS  %s\n" "$rel"
  else
    fail=$((fail + 1))
    failures+=("$rel")
    if [ "$expect_error" -eq 1 ]; then
      printf "FAIL  %s (expected a compilation error, got none)\n" "$rel"
    else
      printf "FAIL  %s (expected no error, but compilation failed)\n" "$rel"
    fi
  fi
}

echo
echo "Running standard test suite (tests/*.tig)..."
for file in "$TESTS_DIR"/*.tig; do
  [ -e "$file" ] || continue
  name="$(basename "$file" .tig)"
  if is_expected_error "$name"; then
    run_one "$file" 1
  else
    run_one "$file" 0
  fi
done

echo
echo "Running parser error tests (tests/parser_errors/*.tig)..."
for file in "$TESTS_DIR"/parser_errors/*.tig; do
  [ -e "$file" ] || continue
  run_one "$file" 1
done

echo
echo "Running semantic error tests (tests/semantic_errors/*.tig)..."
for file in "$TESTS_DIR"/semantic_errors/*.tig; do
  [ -e "$file" ] || continue
  run_one "$file" 1
done

echo
echo "============================================"
echo "  $pass passed, $fail failed"
echo "============================================"

if [ "$fail" -gt 0 ]; then
  echo "Failing tests:"
  for f in "${failures[@]}"; do
    echo "  - $f"
  done
  exit 1
fi

exit 0
