#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// Function to read input from the file and parse process information
vector<vector<int>> read_from_file(string filename) {
    ifstream file(filename);
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

// Comparator for priority queue: Selects process with the shortest remaining burst time
struct BurstTimeComparator {
    bool operator()(vector<int>& P1, vector<int>& P2) {
        int index1 = 2;
        while (index1 < P1.size() && P1[index1] == 0) index1 += 2;
 
        int index2 = 2;
        while (index2 < P2.size() && P2[index2] == 0) index2 += 2;

        if (index1 >= P1.size() || index2 >= P2.size()) return false;

        return P1[index1] > P2[index2];  // Sort by remaining burst time
    }
};

// Comparator for arrival time: Helps to organize processes based on their arrival times
struct ArrivalTimeComparator {
    bool operator()(vector<int>& P1, vector<int>& P2) {
        return P1[1] > P2[1];  // Higher arrival time comes later
    }
};

int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);

    // Queue for processes yet to arrive, ordered by arrival time
    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(process_table.begin(), process_table.end());

    // Queue for processes ready to execute, ordered by burst time (preemptive SJF)
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;

    // Variables to store output and metrics
    int makespan = wait_queue.top()[1];
    vector<int> wait_time(process_table.size(), 0);    // Store wait time per process
    vector<int> completion_time(process_table.size(), 0);  // Store completion time per process
    for (auto i : process_table) {
        completion_time.push_back(i[1]);  // Initialize with arrival times
    }
    vector<string> output;

    // Initializing the CPU time based on the first process arrival time
    int cpu_time = wait_queue.top()[1];
    int start_time = cpu_time;
    int process_index = wait_queue.top()[0];
    int burst = 1;

    // Simulate the preemptive SJF
    while (!ready_queue.empty() || !wait_queue.empty()) {
        // Insert all processes whose (arrival time <= current cpu time)
        while (!wait_queue.empty() && cpu_time >= wait_queue.top()[1]) {
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }

        int index = 2;
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            // Move to the non-zero burst time index
            while (index < curr_process.size() && curr_process[index] == 0) index += 2;

            // If no non-zero CPU burst is found, continue
            if (index >= curr_process.size()) continue;

            // Update wait time: Time spent in the queue since last ready
            wait_time[curr_process[0] - 1] += (cpu_time - curr_process[1]);

            // Store output when switching between processes or bursts
            if (process_index != curr_process[0] || burst != index / 2) {
                output.push_back("P" + to_string(process_index) + "," + to_string(burst) + " " + to_string(start_time) + " " + to_string(cpu_time));
                start_time = cpu_time;
                process_index = curr_process[0];
                burst = index / 2;  // Update to the current burst index
            }

            // Decrement the current burst
            curr_process[index]--;

            // If burst still remains, requeue the process in the ready queue
            if (curr_process[index] > 0) {
                curr_process[1] = cpu_time;  // Update the last queue entry time
                ready_queue.push(curr_process);
            } 
            // If the burst is complete but there are more I/O and CPU bursts left
            else if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = cpu_time + curr_process[index + 1];  // Update arrival time after I/O burst
                wait_queue.push(curr_process);  // Requeue based on new arrival time
            } 
            // If the process is finished, update its completion time
            else {
                completion_time[curr_process[0] - 1] = cpu_time - completion_time[curr_process[0]-1];
            }
        }
        cpu_time++;
    }

    // Store final output for the last process
    output.push_back("P" + to_string(process_index) + "," + to_string(burst) + " " + to_string(start_time) + " " + to_string(cpu_time));

    // Calculate makespan and other metrics
    makespan = cpu_time - makespan;
    int total_wait_time = accumulate(wait_time.begin(), wait_time.end(), 0);
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end());
    int total_completion_time = accumulate(completion_time.begin(), completion_time.end(), 0);
    int max_completion_time = *max_element(completion_time.begin(), completion_time.end());

    // Print outputs
    for (string s : output) cout << s << endl;
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << (double)total_wait_time / (double)process_table.size() << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Completion Time: " << (double)total_completion_time / (double)process_table.size() << endl;
    cout << "Maximum Completion Time: " << max_completion_time << endl;

    return 0;
}
