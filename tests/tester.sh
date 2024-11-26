#!/bin/bash
rm -rf tests
mkdir tests
cd tests


echo "Starting tests..."

passed=0
failed=0
total=0

echo "Current directory: $(pwd)"

for test in ./../../tests/*.dat; do
    num=$(basename "$test" .dat)  
    echo "Running test $num..."
    
    ./../test < "$test" > "result_$num.out"
    diff -q "result_$num.out" "./../../tests/$num.ans" > /dev/null
    
    if [ $? -eq 0 ]; then
        echo "Test $num PASSED"
        passed=$((passed + 1))
    else
        echo "Test $num FAILED"
        failed=$((failed + 1))
    fi
    
    total=$((total + 1))
done

echo "Total tests: $total"
echo "Passed: $passed"
echo "Failed: $failed"

if [ $failed -eq 0 ]; then
    echo "PASSED!"
else
    echo "FAILED!"
fi

cd ..
rm -rf tests