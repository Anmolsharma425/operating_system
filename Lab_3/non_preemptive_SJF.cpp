#include<bits/stdc++.h>
#include<fstream>
using namespace std;

// Function to read process details from the file
vector<vector<int>> read_from_file() {
    ifstream file("process1.dat");
    vector<vector<int>> data;
    string line;
    int process_index = 1;
    while (getline(file, line)) {
        istringstream lineStream(line);
        vector<int> numbers;
        int number;

        while (lineStream >> number) {
            numbers.push_back(number);
        }

        if (!numbers.empty()) {
            numbers.insert(numbers.begin(), process_index);
            process_index++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

// Comparator for selecting process with shortest burst time
struct BurstTimeComparator {
    bool operator()(const vector<int>& P1, const vector<int>& P2) {
        int index1 = 2;
        while (index1 < P1.size() && P1[index1] == 0) index1 += 2;

        int index2 = 2;
        while (index2 < P2.size() && P2[index2] == 0) index2 += 2;

        if (index1 >= P1.size() || index2 >= P2.size()) return false;

        return P1[index1] > P2[index2]; // Shortest job comes first
    }
};

// Comparator for selecting process based on arrival time
struct ArrivalTimeComparator {
    bool operator()(const vector<int>& P1, const vector<int>& P2) {
        return P1[1] > P2[1];  // Higher arrival time comes later
    }
};

int main() {
    vector<vector<int>> process_table = read_from_file();

    // Priority queues: One for processes waiting to arrive, another for ready-to-execute processes
    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(process_table.begin(), process_table.end());
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;

    // Variables to store various outputs and metrics
    int makespan = wait_queue.top()[1]; // Keeps track of the time until all processes finish
    vector<int> wait_time(process_table.size(), 0);   // Wait time for each process
    vector<int> completion_time(process_table.size(), 0); // Completion time for each process
    vector<string> output; // Stores the sequence of process executions

    // Start the simulation from the time of the first arriving process
    int cpu_time = wait_queue.top()[1];

    while (!ready_queue.empty() || !wait_queue.empty()) {
        // Move all processes whose arrival time is <= current CPU time to the ready queue
        while (!wait_queue.empty() && cpu_time >= wait_queue.top()[1]) {
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }

        int index = 2;
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            // Move index to the first non-zero burst time
            while (index < curr_process.size() && curr_process[index] == 0) index += 2;

            // If no valid burst found, continue
            if (index >= curr_process.size()) continue;

            // Update wait time (the time the process waited in the queue)
            wait_time[curr_process[0] - 1] += (cpu_time - curr_process[1]);

            // Store output (process execution details)
            output.push_back("P" + to_string(curr_process[0]) + "," + to_string(index / 2) + " " + to_string(cpu_time) + " " + to_string(cpu_time + curr_process[index]));

            // Update CPU time (current process finishes its burst)
            cpu_time += curr_process[index];
            curr_process[index] = 0;  // Mark current burst as completed

            // Record the process completion time
            completion_time[curr_process[0] - 1] = cpu_time;

            // If there are more bursts remaining, add the next burst (if applicable)
            if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = cpu_time + curr_process[index + 1]; // Set the arrival time for next burst after I/O
                wait_queue.push(curr_process);
            }
        }
        else {
            cpu_time++;  // If no process is ready, move to the next time unit
        }
    }

    // Calculate and print the final metrics
    makespan = cpu_time - makespan;  // Total time to complete all processes
    int total_wait_time = accumulate(wait_time.begin(), wait_time.end(), 0);  // Total waiting time
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end());     // Maximum waiting time
    int total_completion_time = accumulate(completion_time.begin(), completion_time.end(), 0);  // Total completion time
    int max_completion_time = *max_element(completion_time.begin(), completion_time.end());  // Maximum completion time

    // Output the process execution details
    for (string s : output) {
        cout << s << endl;
    }

    // Output the metrics
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << (double)total_wait_time / (double)process_table.size() << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Completion Time: " << (double)total_completion_time / (double)process_table.size() << endl;
    cout << "Maximum Completion Time: " << max_completion_time << endl;

    return 0;
}
