#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<vector<int>> read_from_file() {
    ifstream file("process2.dat");
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
    bool operator()(vector<int> &P1, vector<int> &P2) {
        int index1 = 2;
        while (index1 < P1.size() && P1[index1] == 0) index1 += 2;

        int index2 = 2;
        while (index2 < P2.size() && P2[index2] == 0) index2 += 2;

        if (index1 > P1.size() || index2 > P2.size()) return false;

        return P1[index1] > P2[index2];
    }
};

struct ArrivalTimeComparator {
    bool operator()(vector<int> &P1, vector<int> &P2) {
        return P1[1] > P2[1]; // Sort by arrival time
    }
};

int main() {
    vector<vector<int>> process_table = read_from_file();

    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(process_table.begin(), process_table.end());

    // Priority queue based on current CPU burst time (for preemptive SJF)
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;

    // Initialize CPU times for both processors
    int cpu_time1 = wait_queue.top()[1];
    int cpu_time2 = wait_queue.top()[1];

    int start_time1 = cpu_time1;
    int start_time2 = cpu_time2;

    int process_index1 = -1;
    int process_index2 = -1;
    int burst1 = 0;
    int burst2 = 0;

    while (!ready_queue.empty() || !wait_queue.empty()) {
        // Insert all processes that have arrived by cpu_time1 or cpu_time2 into the ready queue
        while (!wait_queue.empty() && (cpu_time1 >= wait_queue.top()[1] || cpu_time2 >= wait_queue.top()[1])) {
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }

        int index = 2;

        // CPU1 picks the next shortest job
        if (!ready_queue.empty() && (cpu_time1 <= cpu_time2 || process_index2 == -1)) {
            vector<int> curr_process = ready_queue.top();
            ready_queue.pop();

            while (index < curr_process.size() && curr_process[index] == 0) {
                index += 2;
            }

            if (index >= curr_process.size()) {
                continue;
            }

            if (process_index1 != curr_process[0] || burst1 != index / 2) {
                if (process_index1 != -1) {
                    // Output the previous process handled by CPU1
                    cout << "P" << process_index1 << ", CPU1, " << burst1 << " " << start_time1 << " " << cpu_time1 << endl;
                }
                start_time1 = cpu_time1;
                process_index1 = curr_process[0];
                burst1 = index / 2;
            }

            curr_process[index]--;
            cpu_time1++;

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

            while (index < curr_process.size() && curr_process[index] == 0) {
                index += 2;
            }

            if (index >= curr_process.size()) {
                continue;
            }

            if (process_index2 != curr_process[0] || burst2 != index / 2) {
                if (process_index2 != -1) {
                    // Output the previous process handled by CPU2
                    cout << "P" << process_index2 << ", CPU2, " << burst2 << " " << start_time2 << " " << cpu_time2 << endl;
                }
                start_time2 = cpu_time2;
                process_index2 = curr_process[0];
                burst2 = index / 2;
            }

            curr_process[index]--;
            cpu_time2++;

            if (curr_process[index] > 0) {
                curr_process[1] = cpu_time2;
                ready_queue.push(curr_process);
            } else if (curr_process[index + 1] != -1 && curr_process[index + 2] != -1) {
                curr_process[1] = cpu_time2 + curr_process[index + 1];
                wait_queue.push(curr_process);
            }
        }

        // In case both CPUs are idle and no task is available
        if (ready_queue.empty()) {
            if (!wait_queue.empty()) {
                int next_time = wait_queue.top()[1];
                cpu_time1 = max(cpu_time1, next_time);
                cpu_time2 = max(cpu_time2, next_time);
            }
        }
    }

    // Output the remaining tasks processed by each CPU
    if (process_index1 != -1) {
        cout << "P" << process_index1 << ", CPU1, " << burst1 << " " << start_time1 << " " << cpu_time1 << endl;
    }
    if (process_index2 != -1) {
        cout << "P" << process_index2 << ", CPU2, " << burst2 << " " << start_time2 << " " << cpu_time2 << endl;
    }

    return 0;
}
