# Default target
all: part1 part2_1 part2_2 part2_3


# Part 1: Single-process version
part1: image_sharpener.cpp libppm.cpp
	g++ -g -o part1 image_sharpener.cpp libppm.cpp
	./part1 1.ppm output_part1.ppm

# Part 2.1: Multi-process, pipe version
part2_1: part2_1.cpp libppm.cpp
	g++ -Wall -O2 -o part2_1 part2_1.cpp libppm.cpp
	./part2_1 1.ppm output_part2_1.ppm

# Part 2.2: Multi-process, atomic operation version
part2_2: shared_mem.cpp libppm.cpp
	g++ -o part2_2 shared_mem.cpp libppm.cpp
	./part2_2 1.ppm output_part2_2.ppm

# Part 2.3: Multi-thread, semaphore version
part2_3: image_thread.cpp libppm.cpp
	g++ -lpthread -o part2_3 image_thread.cpp libppm.cpp
	./part2_3 1.ppm output_part2_3.ppm

# Clean up generated files
clean:
	rm -f a.out part1 part2_1 part2_2 part2_3 output_part1.ppm output_part2_1.ppm output_part2_2.ppm output_part2_3.ppm
