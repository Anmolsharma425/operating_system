#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<vector<int>> read_from_file() {
    ifstream file("process1.dat");
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
            numbers.push_back(process_count);
            process_count++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

int main() {
    vector<vector<int>> process_table = read_from_file();
    for (int i = 0; i < process_table.size(); i++) {
        process_table[i].insert(process_table[i].begin(), i + 1);
    }
    queue<vector<int>> ready_queue;
    ready_queue.push(process_table[0]);
    int cpu_time1 = process_table[0][1];  
    int cpu_time2 = process_table[0][1];
    int count = 1;
    bool cpu1_available = true, cpu2_available = true;
    vector<string> processor1_output, processor2_output;

    while (!ready_queue.empty() || count < process_table.size()) {
        if (ready_queue.empty() && count < process_table.size()) {
            ready_queue.push(process_table[count]);
            count++;
        }
        if (!ready_queue.empty()) {
            vector<int> curr_process = ready_queue.front();
            ready_queue.pop();
            int index = 2;
            while (index < curr_process.size() && curr_process[index] == 0) {
                index += 2;
            }
            if(index >= curr_process.size()) 
                continue;
            int burst = index / 2;
            int selected_cpu = 0; 
            stringstream output;
            if (cpu1_available && cpu_time1 <= cpu_time2) {
                output << "P" << curr_process[0] << ",";
                selected_cpu = 1;
                output << burst << " " << cpu_time1 << " " << cpu_time1 + curr_process[index];
                cpu_time1 += curr_process[index];
                cpu1_available = false;
                processor1_output.push_back(output.str());
            } 
            else if (cpu2_available) {
                output << "P" << curr_process[0] << ",";
                selected_cpu = 2;
                output << burst << " " << cpu_time2 << " " << cpu_time2 + curr_process[index];
                cpu_time2 += curr_process[index];
                cpu2_available = false;
                processor2_output.push_back(output.str());
            }
            curr_process[index] = 0;
            while (count < process_table.size()) {
                if (process_table[count][1] <= min(cpu_time1, cpu_time2)) {
                    ready_queue.push(process_table[count]);
                    count++;
                } 
                else {
                    break;
                }
            }
            if (index + 1 < curr_process.size() && curr_process[index + 1] != -1) {
                ready_queue.push(curr_process);
            }
            if (selected_cpu == 1) cpu1_available = true;
            if (selected_cpu == 2) cpu2_available = true;
        }
    }
    cout << "CPU0\n" << endl;
    for (const string &out : processor1_output) {
        cout << out << endl;
    }
    cout << "\n\n\nCPU1\n" << endl;
    for (const string &out : processor2_output) {
        cout << out << endl;
    }
    return 0;
}