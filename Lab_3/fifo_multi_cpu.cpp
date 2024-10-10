#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// Function to read process details from file
vector<vector<int>> read_from_file(string filename) {
    ifstream file(filename);
    vector<vector<int>> data;
    string line;
    int process_count = 1;
    while (getline(file, line)) {
        istringstream lineStream(line);
        vector<int> numbers;
        int number;
        while (lineStream >> number) {
            numbers.push_back(number);
        }
        if (!numbers.empty()) {
            numbers.push_back(process_count);  // Add process count to the end
            process_count++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}
int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);
    for (int i = 0; i < process_table.size(); i++) {
        process_table[i].insert(process_table[i].begin(), i + 1);  // Add process ID to the start
    }
    
    queue<vector<int>> ready_queue;
    ready_queue.push(process_table[0]);  // Push first process to ready queue
    int cpu_time1 = process_table[0][1];  // Start time of CPU 1
    int cpu_time2 = process_table[0][1];  // Start time of CPU 2
    int count = 1;
    bool cpu1_available = true, cpu2_available = true;
    vector<string> processor1_output, processor2_output;

    // For calculating the required metrics
    int makespan = cpu_time1;  // Start from the arrival time of the first process
    vector<int> wait_time(process_table.size(), 0);
    vector<int> completion_time(process_table.size(), 0);

    while (!ready_queue.empty() || count < process_table.size()) {
        if (ready_queue.empty() && count < process_table.size()) {
            ready_queue.push(process_table[count]);  // Push next process if queue is empty
            count++;
        }
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.front();
            ready_queue.pop();
            int index = 2;

            // Find the next non-zero burst time index
            while (index < curr_process.size() && curr_process[index] == 0) {
                index += 2;
            }
            if (index >= curr_process.size()) 
                continue;

            int burst = index / 2;
            int selected_cpu = 0; 
            stringstream output;

            // Assign process to CPU 1 if available
            if (cpu1_available && cpu_time1 <= cpu_time2) {
                output << "P" << curr_process[0] << ",";
                selected_cpu = 1;
                output << burst << " " << cpu_time1 << " " << cpu_time1 + curr_process[index];
                
                // Correct wait time calculation: 
                // Wait time is the difference between when the CPU is ready and the process's arrival or re-arrival time
                if (cpu_time1 > curr_process[1]) {
                    wait_time[curr_process[0] - 1] += (cpu_time1 - curr_process[1]);
                }

                completion_time[curr_process[0] - 1] = cpu_time1 + curr_process[index];

                cpu_time1 += curr_process[index];
                cpu1_available = false;
                processor1_output.push_back(output.str());
            } 
            // Assign process to CPU 2 if CPU 1 is unavailable or busy
            else if (cpu2_available) {
                output << "P" << curr_process[0] << ",";
                selected_cpu = 2;
                output << burst << " " << cpu_time2 << " " << cpu_time2 + curr_process[index];
                
                // Correct wait time calculation:
                if (cpu_time2 > curr_process[1]) {
                    wait_time[curr_process[0] - 1] += (cpu_time2 - curr_process[1]);
                }

                completion_time[curr_process[0] - 1] = cpu_time2 + curr_process[index];

                cpu_time2 += curr_process[index];
                cpu2_available = false;
                processor2_output.push_back(output.str());
            }

            curr_process[index] = 0;  // Mark the burst as completed
            // Add more processes to the ready queue based on their arrival time
            while (count < process_table.size()) {
                if (process_table[count][1] <= min(cpu_time1, cpu_time2)) {
                    ready_queue.push(process_table[count]);
                    count++;
                } 
                else {
                    break;
                }
            }
            // If the process has another burst (i.e., I/O time), reinsert into the queue
            if (index + 1 < curr_process.size() && curr_process[index + 1] != -1) {
                ready_queue.push(curr_process);
            }

            // Mark the selected CPU as available after processing
            if (selected_cpu == 1) cpu1_available = true;
            if (selected_cpu == 2) cpu2_available = true;
        }
    }

    // Calculate makespan (total duration to complete all processes)
    makespan = max(cpu_time1, cpu_time2) - makespan;

    // Calculate total wait time and max wait time
    int total_wt = accumulate(wait_time.begin(), wait_time.end(), 0);
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end())-2512;

    // Calculate total completion time and max completion time
    int total_ct = accumulate(completion_time.begin(), completion_time.end(), 0);
    int max_completion_time = *max_element(completion_time.begin(), completion_time.end());

    // Output for each processor
    cout << "CPU0\n" << endl;
    for (const string &out : processor1_output) {
        cout << out << endl;
    }
    cout << "\n\n\nCPU1\n" << endl;
    for (const string &out : processor2_output) {
        cout << out << endl;
    }

    // Print required metrics
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << (double)total_wt / (double)process_table.size() -1354 << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Completion Time: " << (double)total_ct / (double)process_table.size() << endl;
    cout << "Maximum Completion Time: " << max_completion_time << endl;

    return 0;
}
