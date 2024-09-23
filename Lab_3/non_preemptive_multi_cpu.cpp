#include <bits/stdc++.h>
#include <fstream>
using namespace std;

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

struct BurstTimeComparator {
    bool operator()(const vector<int>& P1, const vector<int>& P2) {
        int index1 = 2;
        while (index1 < P1.size() && P1[index1] == 0) index1 += 2;

        int index2 = 2;
        while (index2 < P2.size() && P2[index2] == 0) index2 += 2;

        if (index1 >= P1.size() || index2 >= P2.size()) return false;

        return P1[index1] > P2[index2];
    }
};

struct ArrivalTimeComparator {
    bool operator()(const vector<int>& P1, const vector<int>& P2) {
        return P1[1] > P2[1];
    }
};

int main() {
    vector<vector<int>> process_table = read_from_file();
    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(
        process_table.begin(), process_table.end());
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;
    vector<string> output_cpu0, output_cpu1;

    // Initialize CPU times for processor 1 and processor 2
    int cpu_time1 = wait_queue.top()[1];  // First process arrives at this time
    int cpu_time2 = wait_queue.top()[1];
    int makespan_start = cpu_time1;

    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    vector<int> wait_time(process_table.size(), 0);
    vector<int> run_time(process_table.size(), 0);
    for(auto i:process_table){
        run_time.push_back(i[1]);
    }
    int total_waiting_time = 0;
    int total_running_time = 0;

    while (!ready_queue.empty() || !wait_queue.empty()) {
        int index = 2;

        // Select the next task for either CPU1 or CPU2
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            while (index < curr_process.size() && curr_process[index] == 0) index += 2;

            if (index >= curr_process.size()) continue;

            int burst = index / 2;
            int selected_cpu = 0;  // 1 for CPU1, 2 for CPU2
            stringstream output;

            // Assign the task to CPU1 if it's available or its time is less than or equal to CPU2's time
            if (cpu_time1 <= cpu_time2) {
                output << "P" << curr_process[0] << ",";
                selected_cpu = 1;
                output << burst << " ";
                output << cpu_time1 << " " << cpu_time1 + curr_process[index] << endl;

                // Update wait time and run time for this process
                wait_time[curr_process[0] - 1] += (cpu_time1 - curr_process[1]);
                // run_time[curr_process[0] - 1] += curr_process[index];

                cpu_time1 += curr_process[index] + 1;  // Adding burst time and 1 unit overhead
                output_cpu0.push_back(output.str());
            } else {  // Otherwise, assign the task to CPU2
                output << "P" << curr_process[0] << ",";
                selected_cpu = 2;
                output << burst << " ";
                output << cpu_time2 << " " << cpu_time2 + curr_process[index] << endl;

                // Update wait time and run time for this process
                wait_time[curr_process[0] - 1] += (cpu_time2 - curr_process[1]);
                // run_time[curr_process[0] - 1] += curr_process[index];

                cpu_time2 += curr_process[index] + 1;  // Adding burst time and 1 unit overhead
                output_cpu1.push_back(output.str());
            }

            curr_process[index] = 0;

            // If the process has more phases, add it back to the waiting queue
            if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = max(cpu_time1, cpu_time2) + curr_process[index + 1];  // Update the next start time
                wait_queue.push(curr_process);
            }
            else{
                if(selected_cpu==1)
                run_time[curr_process[0]-1]=cpu_time1-run_time[curr_process[0]-1];
                if(selected_cpu==2)
                run_time[curr_process[0]-1]=cpu_time2-run_time[curr_process[0]-1];
            }
        } else {
            // If no processes are ready, increment both CPUs' time to the next available process
            cpu_time1++;
            cpu_time2++;
        }
        while (!wait_queue.empty() && min(cpu_time1, cpu_time2) >= wait_queue.top()[1]) {
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }
    }

    // Calculating makespan, waiting time, and running time
    int makespan = max(cpu_time1, cpu_time2) - makespan_start;  // Time difference from first arrival to last process end
    int total_wt = accumulate(wait_time.begin(), wait_time.end(), 0);
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end());

    int total_rt = accumulate(run_time.begin(), run_time.end(), 0);
    int max_run_time = *max_element(run_time.begin(), run_time.end());

    // Output for processors
    cout << "CPU0\n" << endl;
    for (const string &out : output_cpu0) {
        cout << out;
    }
    cout << "\n\n\nCPU1\n" << endl;
    for (const string &out : output_cpu1) {
        cout << out;
    }

    // Printing required metrics
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << (double)total_wt / (double)process_table.size() << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Completion Time: " << (double)total_rt / (double)process_table.size() << endl;
    cout << "Maximum Completion Time: " << max_run_time << endl;

    return 0;
}
