#include<bits/stdc++.h>
#include<fstream>
using namespace std;

// Function to read the process details from the file
vector<vector<int>> read_from_file(string filename) {
    ifstream file(filename);
    vector<vector<int>> data;
    string line;
    int process_index = 1;
    while (getline(file, line)) {
        istringstream lineStream(line);
        vector<int> numbers;
        int number;

        // Reading numbers from the line
        while (lineStream >> number) {
            numbers.push_back(number);
        }

        // Insert the process index as the first element in each process entry
        if (!numbers.empty()) {
            numbers.insert(numbers.begin(), process_index);
            process_index++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);
    queue<vector<int>> ready_queue;

    // Variables to store simulation output and metrics
    vector<string> output;
    int makespan = process_table[0][1];  // The total time to complete all processes
    vector<int> wait_time(process_table.size(), 0);   // Wait time for each process
    vector<int> completion_time(process_table.size(), 0); // Completion time for each process

    ready_queue.push(process_table[0]);  // Push the first process into the ready queue
    int cpu_time = ready_queue.front()[1];  // Initialize CPU time with the arrival time of the first process
    int count = 1;

    while (!ready_queue.empty()) {
        vector<int> curr_process = ready_queue.front();
        ready_queue.pop();
        int index = 2;

        // Move index to the first non-zero burst time
        while (index < curr_process.size() && curr_process[index] == 0) index += 2;
        if (index >= curr_process.size()) continue;  // If all bursts are 0, skip the process

        // Calculate waiting time
        wait_time[curr_process[0] - 1] += (cpu_time - curr_process[1]);

        // Store output
        output.push_back("P" + to_string(curr_process[0]) + "," + to_string(index / 2) + 
                        " " + to_string(cpu_time) + " " + to_string(cpu_time + curr_process[index]));

        // Update CPU time and mark the current burst as completed
        cpu_time += curr_process[index];
        curr_process[index] = 0;
        curr_process[1] = cpu_time;  // Update arrival time for next burst

        // Record the completion time for this process
        completion_time[curr_process[0] - 1] = cpu_time;

        // Check if any other processes can now enter the ready queue based on their arrival time
        while (count < process_table.size()) {
            if (process_table[count][1] <= cpu_time) {
                ready_queue.push(process_table[count]);
                count++;
            } else {
                break;
            }
        }

        // If the process has another burst (i.e., I/O time is present), reinsert it into the queue
        if (index + 1 < curr_process.size() && curr_process[index + 1] != -1) {
            ready_queue.push(curr_process);
        }
    }

    // Calculate the makespan (total time to complete all processes)
    makespan = cpu_time - makespan;

    // Calculate metrics: total wait time, max wait time, total and max completion time
    int total_wait_time = accumulate(wait_time.begin(), wait_time.end(), 0);
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end());
    int total_completion_time = accumulate(completion_time.begin(), completion_time.end(), 0);
    int max_completion_time = *max_element(completion_time.begin(), completion_time.end());

    // Print output and final metrics
    for (string s : output) {
        cout << s << endl;
    }

    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << (double)total_wait_time / (double)process_table.size() << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Completion Time: " << (double)total_completion_time / (double)process_table.size() << endl;
    cout << "Maximum Completion Time: " << max_completion_time << endl;

    return 0;
}

