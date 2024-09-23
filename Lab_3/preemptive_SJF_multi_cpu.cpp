#include <bits/stdc++.h>
#include <fstream>
using namespace std;

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
            numbers.insert(numbers.begin(), process_index); // Insert process ID
            process_index++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

struct BurstTimeComparator {
    bool operator()(vector<int> &P1, vector<int> &P2) {
        int index1 = 2;
        while (index1 < P1.size() && P1[index1] == 0) index1 += 2;

        int index2 = 2;
        while (index2 < P2.size() && P2[index2] == 0) index2 += 2;

        if (index1 >= P1.size() || index2 >= P2.size()) return false;

        return P1[index1] > P2[index2];
    }
};

struct ArrivalTimeComparator {
    bool operator()(vector<int> &P1, vector<int> &P2) {
        return P1[1] > P2[1]; // Sort by arrival time
    }
};

int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);
    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(process_table.begin(), process_table.end());
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;

    vector<string> output_cpu0, output_cpu1;

    // Initialize CPU times for both processors
    int cpu_time1 = wait_queue.top()[1];
    int cpu_time2 = wait_queue.top()[1];

    int start_time1 = cpu_time1;
    int start_time2 = cpu_time2;

    int process_index1 = -1, process_index2 = -1;
    int burst1 = 0, burst2 = 0;

    // To track waiting and running times
    vector<int> waiting_time(process_table.size(), 0);
    vector<int> running_time(process_table.size(), 0);
    vector<int> start_time(process_table.size(), -1); // To store when a process starts execution
    int total_wait_time = 0, total_run_time = 0;
    
    while (!ready_queue.empty() || !wait_queue.empty()) {
        // Insert all processes that have arrived by cpu_time1 or cpu_time2 into the ready queue
        while (!wait_queue.empty() && (cpu_time1 >= wait_queue.top()[1] || cpu_time2 >= wait_queue.top()[1])) {
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }

        int index = 2;
        stringstream output;
        
        // CPU1 picks the next shortest job
        if (!ready_queue.empty() && (cpu_time1 <= cpu_time2 || process_index2 == -1)) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            while (index < curr_process.size() && curr_process[index] == 0) index += 2;
            if (index >= curr_process.size()) continue;

            if (process_index1 != curr_process[0] || burst1 != index / 2) {
                if (process_index1 != -1) {
                    output << "P" << process_index1 << "," << burst1 << " " << start_time1 << " " << cpu_time1 << endl;
                    output_cpu0.push_back(output.str());
                }

                start_time1 = cpu_time1;
                process_index1 = curr_process[0];
                burst1 = index / 2;
            }

            // First execution start, calculate waiting time
            if (start_time[process_index1 - 1] == -1) {
                start_time[process_index1 - 1] = cpu_time1;
                waiting_time[process_index1 - 1] = start_time1 - curr_process[1];
            }

            curr_process[index]--;
            cpu_time1++;
            running_time[process_index1 - 1]++;

            if (curr_process[index] > 0) {
                curr_process[1] = cpu_time1;
                ready_queue.push(curr_process);
            } else if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = cpu_time1 + curr_process[index + 1];
                wait_queue.push(curr_process);
            }
        }

        // CPU2 picks the next shortest job
        if (!ready_queue.empty() && (cpu_time2 < cpu_time1 || process_index1 == -1)) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            while (index < curr_process.size() && curr_process[index] == 0) index += 2;
            if (index >= curr_process.size()) continue;

            if (process_index2 != curr_process[0] || burst2 != index / 2) {
                if (process_index2 != -1) {
                    output << "P" << process_index2 << "," << burst2 << " " << start_time2 << " " << cpu_time2 << endl;
                    output_cpu1.push_back(output.str());
                }

                start_time2 = cpu_time2;
                process_index2 = curr_process[0];
                burst2 = index / 2;
            }

            // First execution start, calculate waiting time
            if (start_time[process_index2 - 1] == -1) {
                start_time[process_index2 - 1] = cpu_time2;
                waiting_time[process_index2 - 1] = start_time2 - curr_process[1];
            }

            curr_process[index]--;
            cpu_time2++;
            running_time[process_index2 - 1]++;

            if (curr_process[index] > 0) {
                curr_process[1] = cpu_time2;
                ready_queue.push(curr_process);
            } else if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = cpu_time2 + curr_process[index + 1];
                wait_queue.push(curr_process);
            }
        }

        // If both CPUs are idle and no task is available
        if (ready_queue.empty()) {
            if (!wait_queue.empty()) {
                int next_time = wait_queue.top()[1];
                cpu_time1 = max(cpu_time1, next_time);
                cpu_time2 = max(cpu_time2, next_time);
            }
        }
    }

    // Output remaining processes handled by each CPU
    if (process_index1 != -1) {
        stringstream output;
        output << "P" << process_index1 << "," << burst1 << " " << start_time1 << " " << cpu_time1 << endl;
        output_cpu0.push_back(output.str());
    }
    if (process_index2 != -1) {
        stringstream output;
        output << "P" << process_index2 << "," << burst2 << " " << start_time2 << " " << cpu_time2 << endl;
        output_cpu1.push_back(output.str());
    }

    // Calculate makespan
    int makespan = max(cpu_time1, cpu_time2);

    // Calculate average and maximum waiting/running times
    int max_wait_time = *max_element(waiting_time.begin(), waiting_time.end());
    int max_run_time = *max_element(running_time.begin(), running_time.end());
    double avg_wait_time = accumulate(waiting_time.begin(), waiting_time.end(), 0.0) / waiting_time.size();
    double avg_run_time = accumulate(running_time.begin(), running_time.end(), 0.0) / running_time.size();

    // Print outputs for each CPU
    cout << "CPU0\n" << endl;
    for (const string &out : output_cpu0) cout << out;
    cout << "\n\n\nCPU1\n" << endl;
    for (const string &out : output_cpu1) cout << out;

    // Print metrics
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << avg_wait_time << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Running Time: " << avg_run_time << endl;
    cout << "Maximum Running Time: " << max_run_time << endl;

    return 0;
}
