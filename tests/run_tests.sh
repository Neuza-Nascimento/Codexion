#!/usr/bin/env bash
set -u
PASS=0
FAIL=0

cd "$(dirname "$0")/.." || exit 1
make >/tmp/build.log 2>&1 || { echo "BUILD FAIL"; cat /tmp/build.log; exit 1; }

run_case() {
    local name="$1"; local tmo="$2"; local exp="$3"; shift 3
    local out="/tmp/test_${name}.txt"
    timeout "$tmo" ./codexion "$@" >"$out" 2>&1
    local rc=$?
    local ok=0
    case "$exp" in
        success)  [ $rc -eq 0 ] && ! grep -q "burned out" "$out" && ok=1 ;;
        burnout)  [ $rc -eq 0 ] && grep -q "burned out" "$out" && ok=1 ;;
        invalid)  [ $rc -ne 0 ] && ok=1 ;;
    esac
    if [ $ok -eq 1 ]; then echo "[PASS] $name"; PASS=$((PASS+1));
    else echo "[FAIL] $name (rc=$rc)"; tail -3 "$out"; FAIL=$((FAIL+1)); fi
}

echo "=== Basic functional ==="
run_case basic_fifo 10 success 4 800 200 200 200 2 10 fifo
run_case basic_edf 10 success 4 800 200 200 200 2 10 edf
run_case one_compile 10 success 2 800 200 200 200 1 10 fifo
run_case cooldown 10 success 3 800 150 100 100 2 50 fifo
run_case high_load 30 success 50 1000 100 50 50 3 20 fifo
run_case high_load_edf 30 success 50 1000 100 50 50 3 20 edf
run_case max200 30 success 200 1000 100 50 50 2 20 fifo
run_case max200_edf 30 success 200 1000 100 50 50 2 20 edf

echo "=== Burnout ==="
run_case burnout_fifo 10 burnout 4 300 200 100 100 5 10 fifo
run_case burnout_edf 10 burnout 4 300 200 100 100 5 10 edf
run_case burnout_fast 10 burnout 8 200 150 50 50 10 5 fifo

echo "=== Invalid args ==="
run_case invalid_count 5 invalid 4 800 200 200 200 2 10
run_case invalid_negative 5 invalid 4 -1 200 200 200 2 10 fifo
run_case invalid_scheduler 5 invalid 4 800 200 200 200 2 10 foo
run_case invalid_nonint 5 invalid 4 800 200 200 200 2 10.5 fifo
run_case invalid_zero_coders 5 invalid 0 800 200 200 200 2 10 fifo

echo "=== Memory leak (valgrind) ==="
valgrind --leak-check=full --error-exitcode=42 ./codexion 10 800 200 200 200 2 10 fifo >/tmp/vg.txt 2>&1
if grep -q "All heap blocks were freed" /tmp/vg.txt && grep -q "ERROR SUMMARY: 0" /tmp/vg.txt; then
    echo "[PASS] valgrind_leak"; PASS=$((PASS+1))
else
    echo "[FAIL] valgrind_leak"; tail -8 /tmp/vg.txt; FAIL=$((FAIL+1))
fi

echo "=== Helgrind ==="
valgrind --tool=helgrind --error-exitcode=42 ./codexion 10 800 200 200 200 2 10 fifo >/tmp/hg.txt 2>&1
if grep -q "ERROR SUMMARY: 0" /tmp/hg.txt; then
    echo "[PASS] helgrind"; PASS=$((PASS+1))
else
    echo "[FAIL] helgrind"; grep -i "ERROR SUMMARY" /tmp/hg.txt; FAIL=$((FAIL+1))
fi

echo "=== Stress (repeated) ==="
for i in $(seq 1 5); do
    timeout 30 ./codexion 100 500 100 50 50 3 5 fifo >/tmp/stress_$i.txt 2>&1
    rc=$?
    if [ $rc -eq 0 ] && ! grep -q "burned out" /tmp/stress_$i.txt; then
        echo "[PASS] stress_fifo_$i"; PASS=$((PASS+1))
    else
        echo "[FAIL] stress_fifo_$i (rc=$rc)"; FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"
[ $FAIL -eq 0 ]
