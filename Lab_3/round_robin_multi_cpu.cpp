#include<bits/stdc++.h>
#include<fstream>
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
            numbers.insert(numbers.begin(), process_index);  // Process ID as the first element
            process_index++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

struct arrival_time_comparator{
    bool operator()(const vector<int>& P1, const vector<int>& P2){
        return P1[1] > P2[1];  // Compare based on arrival time
    }
};

int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);

    priority_queue<vector<int>, vector<vector<int>>, arrival_time_comparator> wait_queue(process_table.begin(), process_table.end());
    queue<vector<int>> ready_queue;

    // for output
    vector<string> output_cpu0;
    vector<string> output_cpu1;
    
    // initializing cpu_time1 and cpu_time2 with first process arrival time
    int cpu_time1 = wait_queue.top()[1];
    ready_queue.push(wait_queue.top());
    wait_queue.pop();
    
    int cpu_time2 = wait_queue.top()[1];   
    
    // Variables to track waiting and running times
    vector<int> waiting_time(process_table.size(), 0);
    vector<int> running_time(process_table.size(), 0);
    vector<int> start_time(process_table.size(), -1);  // Process start times
    int total_waiting_time = 0;
    int total_running_time = 0;

    while(!wait_queue.empty() || !ready_queue.empty()){

        if(!ready_queue.empty()){

            int index = 2, time_quantum1 = 10, time_quantum2 = 10;

            vector<int> curr_process = ready_queue.front();
            ready_queue.pop();

            // moving index to the non-zero CPU burst time
            while(index < curr_process.size() && curr_process[index] == 0) index += 2;

            // if no non-zero CPU burst is found, skip
            if(index >= curr_process.size()) continue;

            if(cpu_time1 <= cpu_time2){
                int start_time_cpu1 = cpu_time1;

                if(start_time[curr_process[0] - 1] == -1) {
                    start_time[curr_process[0] - 1] = cpu_time1;
                    waiting_time[curr_process[0] - 1] = start_time_cpu1 - curr_process[1];  // Waiting time = start_time - arrival_time
                }

                if(curr_process[index] >= time_quantum1){
                    curr_process[index] -= time_quantum1;
                    cpu_time1 += time_quantum1;
                } else {
                    cpu_time1 += curr_process[index];
                    curr_process[index] = 0;
                }
                running_time[curr_process[0] - 1] += time_quantum1;  // Increment running time for the process

                output_cpu0.push_back("P" + to_string(curr_process[0]) + "," + to_string(index/2) + " " + to_string(start_time_cpu1) + " " + to_string(cpu_time1));
            } else {
                int start_time_cpu2 = cpu_time2;

                if(start_time[curr_process[0] - 1] == -1) {
                    start_time[curr_process[0] - 1] = cpu_time2;
                    waiting_time[curr_process[0] - 1] = start_time_cpu2 - curr_process[1];
                }

                if(curr_process[index] >= time_quantum2){
                    curr_process[index] -= time_quantum2;
                    cpu_time2 += time_quantum2;
                } else {
                    cpu_time2 += curr_process[index];
                    curr_process[index] = 0;
                }
                running_time[curr_process[0] - 1] += time_quantum2;

                output_cpu1.push_back("P" + to_string(curr_process[0]) + "," + to_string(index/2) + " " + to_string(start_time_cpu2) + " " + to_string(cpu_time2));
            }

            // Insert new processes into the ready queue if they have arrived
            while(!wait_queue.empty() && max(cpu_time1, cpu_time2) >= wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }

            if(curr_process[index] > 0) ready_queue.push(curr_process);
            else {
                // If the process has IO bursts, adjust its arrival time for the next burst
                if(curr_process[index + 1] != -1 && curr_process[index + 2] != -1){
                    curr_process[1] = max(cpu_time1, cpu_time2) + curr_process[index + 1];
                    wait_queue.push(curr_process);
                }
            }

        } else {
            // If ready queue is empty, increment CPU times to the next available process
            cpu_time1++;
            cpu_time2++;
            while(!wait_queue.empty() && max(cpu_time1, cpu_time2) >= wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }
        }
    }

    // Calculating metrics
    int makespan = max(cpu_time1, cpu_time2);  // Total time to finish all processes
    int max_wait_time = *max_element(waiting_time.begin(), waiting_time.end());
    int max_run_time = *max_element(running_time.begin(), running_time.end());
    double avg_wait_time = accumulate(waiting_time.begin(), waiting_time.end(), 0.0) / waiting_time.size();
    double avg_run_time = accumulate(running_time.begin(), running_time.end(), 0.0) / running_time.size();

    // Printing CPU outputs
    cout << "CPU0\n" << endl;
    for(const string& s : output_cpu0) cout << s << endl;
    
    cout << "\n\n\nCPU1\n" << endl;
    for(const string& s : output_cpu1) cout << s << endl;
    
    // Printing metrics
    cout << "\nMakespan: " << makespan << endl;
    cout << "Average Waiting Time: " << avg_wait_time << endl;
    cout << "Maximum Waiting Time: " << max_wait_time << endl;
    cout << "Average Running Time: " << avg_run_time << endl;
    cout << "Maximum Running Time: " << max_run_time << endl;

    return 0;
}
