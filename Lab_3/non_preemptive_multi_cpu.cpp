#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<vector<int>> read_from_file() {
    ifstream file("process3.dat");
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

struct mycmp {
    bool operator()(vector<int> &P1, vector<int> &P2) {
        int index1 = 2;
        for (; index1 < P1.size(); index1 += 2) {
            if (P1[index1] != 0)
                break;
        }
        int index2 = 2;
        for (; index2 < P2.size(); index2 += 2) {
            if (P2[index2] != 0)
                break;
        }
        if (index1 > P1.size() || index2 > P2.size())
            return false;
        return P1[index1] > P2[index2];
    }
};

struct mycmp2 {
    bool operator()(vector<int> &P1, vector<int> &P2) {
        return P1[1] > P2[1]; // Sort by arrival time
    }
};

int main() {
    vector<vector<int>> process_table = read_from_file();
    priority_queue<vector<int>, vector<vector<int>>, mycmp2> wait_queue(process_table.begin(), process_table.end());
    priority_queue<vector<int>, vector<vector<int>>, mycmp> ready_queue;

    // Initialize CPU times for processor 1 and processor 2
    int cpu_time1 = wait_queue.top()[1];  // First process arrives at this time
    int cpu_time2 = wait_queue.top()[1];

    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    while (!ready_queue.empty() || !wait_queue.empty()) {
        int index = 2;

        // Select the next task for either CPU1 or CPU2
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            while (index < curr_process.size() && curr_process[index] == 0) {
                index += 2;
            }
            if (index >= curr_process.size()) {
                continue;
            }

            int burst = index / 2;
            int selected_cpu = 0;  // 1 for CPU1, 2 for CPU2

            // Assign the task to CPU1 if it's available or its time is less than or equal to CPU2's time
            if (cpu_time1 <= cpu_time2) {
                cout << "P" << curr_process[0] << ", ";
                cout << "CPU1, ";
                selected_cpu = 1;
                cout << burst << " ";
                cout << cpu_time1 << " " << cpu_time1 + curr_process[index] << endl;
                cpu_time1 += curr_process[index] + 1; // Adding burst time and 1 unit overhead
            } else {  // Otherwise, assign the task to CPU2
                cout << "P" << curr_process[0] << ", ";
                cout << "CPU2, ";
                selected_cpu = 2;
                cout << burst << " ";
                cout << cpu_time2 << " " << cpu_time2 + curr_process[index] << endl;
                cpu_time2 += curr_process[index] + 1; // Adding burst time and 1 unit overhead
            }

            curr_process[index] = 0;

            // If the process has more phases, add it back to the waiting queue
            if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = max(cpu_time1, cpu_time2) + curr_process[index + 1];  // Update the next start time
                wait_queue.push(curr_process);
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

    return 0;
}
