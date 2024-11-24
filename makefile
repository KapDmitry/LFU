CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
TARGET = bin/program
SRC = driver.cpp
TEST_DIR = tests
TESTS = $(wildcard $(TEST_DIR)/*.dat)

build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TARGET)
	@echo "Starting tests..."
	@passed=0; failed=0; total=0; \
	for test in $(TESTS); do \
		num=$$(basename $$test .dat); \
		echo "Running test $$num..."; \
		./$(TARGET) < $$test > result_$$num.out; \
		diff -q result_$$num.out $(TEST_DIR)/$$num.ans > /dev/null; \
		if [ $$? -eq 0 ]; then \
			echo "Test $$num PASSED"; \
			passed=$$((passed+1)); \
		else \
			echo "Test $$num FAILED"; \
			failed=$$((failed+1)); \
		fi; \
		total=$$((total+1)); \
	done; \
	echo "Total tests: $$total"; \
	echo "Passed: $$passed"; \
	echo "Failed: $$failed"; \
	if [ $$failed -eq 0 ]; then \
		echo "PASSED!"; \
	else \
		echo "FAILED!"; \
	fi;

clean:
	rm -rf bin result_*.out

compare: $(SRC)
	@mkdir -p $(dir $(TARGET))
	$(CXX) -DMODE=2 $(CXXFLAGS) -o $(TARGET) $^


