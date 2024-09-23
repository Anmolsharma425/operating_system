# For FIFO (single CPU)
build-fifo: fifo.out

fifo.out: fifo.cpp
	g++ -g fifo.cpp -o fifo.out
run-fifo: fifo.out
	./fifo.out process1.dat

# For FIFO (multi CPU)
build-fifo_multi_cpu: fifo_multi_cpu.out

fifo_multi_cpu.out: fifo_multi_cpu.cpp
	g++ -g fifo_multi_cpu.cpp -o fifo_multi_cpu.out

run-fifo_multi_cpu: fifo_multi_cpu.out
	./fifo_multi_cpu.out process1.dat

# For Non-Preemptive SJF (single CPU)
build-non_preemptive_SJF: non_preemptive_SJF.out

non_preemptive_SJF.out: non_preemptive_SJF.cpp
	g++ -g non_preemptive_SJF.cpp -o non_preemptive_SJF.out

run-non_preemptive_SJF: non_preemptive_SJF.out
	./non_preemptive_SJF.out process1.dat

# For Non-Preemptive SJF (multi CPU)
build-non_preemptive_multi_cpu: non_preemptive_multi_cpu.out

non_preemptive_multi_cpu.out: non_preemptive_multi_cpu.cpp
	g++ -g non_preemptive_multi_cpu.cpp -o non_preemptive_multi_cpu.out

run-non_preemptive_multi_cpu: non_preemptive_multi_cpu.out
	./non_preemptive_multi_cpu.out process1.dat

# For Preemptive SJF (single CPU)
build-preemptive_SJF: preemptive_SJF.out

preemptive_SJF.out: preemptive_SJF.cpp
	g++ -g preemptive_SJF.cpp -o preemptive_SJF.out

run-preemptive_SJF: preemptive_SJF.out
	./preemptive_SJF.out process1.dat

# For Preemptive SJF (multi CPU)
build-preemptive_SJF_multi_cpu: preemptive_SJF_multi_cpu.out

preemptive_SJF_multi_cpu.out: preemptive_SJF_multi_cpu.cpp
	g++ -g preemptive_SJF_multi_cpu.cpp -o preemptive_SJF_multi_cpu.out

run-preemptive_SJF_multi_cpu: preemptive_SJF_multi_cpu.out
	./preemptive_SJF_multi_cpu.out process1.dat

# For Round Robin (single CPU)
build-round_robin: round_robin.out

round_robin.out: round_robin.cpp
	g++ -g round_robin.cpp -o round_robin.out

run-round_robin: round_robin.out
	./round_robin.out process1.dat

# For Round Robin (multi CPU)
build-round_robin_multi_cpu: round_robin_multi_cpu.out

round_robin_multi_cpu.out: round_robin_multi_cpu.cpp
	g++ -g round_robin_multi_cpu.cpp -o round_robin_multi_cpu.out

run-round_robin_multi_cpu: round_robin_multi_cpu.out
	./round_robin_multi_cpu.out process1.dat

# Clean all outputs
clean:
	rm -f *.out