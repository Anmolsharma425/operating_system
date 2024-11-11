# Default target
all-demo: fifo lru random optimal

# Compile the C++ program
main.out: main.cpp
	g++ -g -o main.out main.cpp

# Build target to explicitly compile the code
build: main.out

fifo: main.out
	./main.out 4096 128 fifo GLOBAL combined.trace
	./main.out 4096 128 fifo LOCAL combined.trace

lru: main.out
	./main.out 4096 128 lru GLOBAL combined.trace
	./main.out 4096 128 lru LOCAL combined.trace

random: main.out
	./main.out 4096 128 random GLOBAL combined.trace
	./main.out 4096 128 random LOCAL combined.trace

optimal: main.out
	./main.out 4096 128 optimal GLOBAL combined.trace
	./main.out 4096 128 optimal LOCAL combined.trace





# List of inputs
FRAME-NO := 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768

# Run all simulations
all-simulate: simulate-fifo-global simulate-fifo-local simulate-lru-global simulate-lru-local simulate-random-global simulate-random-local simulate-optimal-global simulate-optimal-local

# Target to run the FIFO strategy with each input
simulate-fifo-global: main.out
	@echo "Running tests and saving output to fifo_global.txt"
	@> fifo_global.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input fifo GLOBAL combined.trace" | tee -a fifo_global.txt; \
		./main.out 4096 $$input fifo GLOBAL combined.trace | tee -a fifo_global.txt; \
		echo "----------------------------------------" | tee -a fifo_global.txt; \
	done

simulate-fifo-local: main.out
	@echo "Running tests and saving output to fifo_local.txt"
	@> fifo_local.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input fifo LOCAL combined.trace" | tee -a fifo_local.txt; \
		./main.out 4096 $$input fifo LOCAL combined.trace | tee -a fifo_local.txt; \
		echo "----------------------------------------" | tee -a fifo_local.txt; \
	done

# Target to run the LRU strategy with each input
simulate-lru-global: main.out
	@echo "Running tests and saving output to lru_global.txt"
	@> lru_global.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input lru GLOBAL combined.trace" | tee -a lru_global.txt; \
		./main.out 4096 $$input lru GLOBAL combined.trace | tee -a lru_global.txt; \
		echo "----------------------------------------" | tee -a lru_global.txt; \
	done

simulate-lru-local: main.out
	@echo "Running tests and saving output to lru_local.txt"
	@> lru_local.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input lru LOCAL combined.trace" | tee -a lru_local.txt; \
		./main.out 4096 $$input lru LOCAL combined.trace | tee -a lru_local.txt; \
		echo "----------------------------------------" | tee -a lru_local.txt; \
	done

# Target to run the Random strategy with each input
simulate-random-global: main.out
	@echo "Running tests and saving output to random_global.txt"
	@> random_global.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input random GLOBAL combined.trace" | tee -a random_global.txt; \
		./main.out 4096 $$input random GLOBAL combined.trace | tee -a random_global.txt; \
		echo "----------------------------------------" | tee -a random_global.txt; \
	done

simulate-random-local: main.out
	@echo "Running tests and saving output to random_local.txt"
	@> random_local.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input random LOCAL combined.trace" | tee -a random_local.txt; \
		./main.out 4096 $$input random LOCAL combined.trace | tee -a random_local.txt; \
		echo "----------------------------------------" | tee -a random_local.txt; \
	done

# Target to run the Optimal strategy with each input
simulate-optimal-global: main.out
	@echo "Running tests and saving output to optimal_global.txt"
	@> optimal_global.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input optimal GLOBAL combined.trace" | tee -a optimal_global.txt; \
		./main.out 4096 $$input optimal GLOBAL combined.trace | tee -a optimal_global.txt; \
		echo "----------------------------------------" | tee -a optimal_global.txt; \
	done

simulate-optimal-local: main.out
	@echo "Running tests and saving output to optimal_local.txt"
	@> optimal_local.txt  # Clear output file before appending
	@for input in $(FRAME-NO); do \
		echo "./main.out 4096 $$input optimal LOCAL combined.trace" | tee -a optimal_local.txt; \
		./main.out 4096 $$input optimal LOCAL combined.trace | tee -a optimal_local.txt; \
		echo "----------------------------------------" | tee -a optimal_local.txt; \
	done

# Clean up generated files
clean:
	rm -f a.out main.out fifo_global.txt fifo_local.txt lru_global.txt lru_local.txt random_global.txt random_local.txt optimal_global.txt optimal_local.txt
