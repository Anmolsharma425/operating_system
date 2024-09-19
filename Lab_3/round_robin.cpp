#include<bits/stdc++.h>
#include<fstream>
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
            numbers.insert(numbers.begin(),process_index);
            process_index++;
            data.push_back(numbers);
        }
    }
    file.close();
    return data;
}

struct arrival_time_comparator{
    bool operator()(const vector<int>& P1, const vector<int>& P2){
        return P1[1]>P2[1];
    }
};
//curr_process goes into the wait queue with updated arrival time
//wait_queue and ready_queue both should be empty
int main(){
    vector<vector<int>> process_table=read_from_file();

    priority_queue<vector<int>, vector<vector<int>>, arrival_time_comparator> wait_queue(process_table.begin(), process_table.end());

    queue<vector<int>> ready_queue;
    
    //initializing the cpu_time
    int cpu_time=wait_queue.top()[1];
    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    while(!wait_queue.empty() || !ready_queue.empty()){


        if(!ready_queue.empty()){

            int index=2, time_quantum=10;

            vector<int> curr_process=ready_queue.front();
            ready_queue.pop();

            //moving our index to non-zero cpu burst time index
            while(index<curr_process.size() && curr_process[index]==0)    index+=2;

            //if no non-zero cpu burst is found
            if(index>=curr_process.size())     continue;

            int start_time=cpu_time;
            if(curr_process[index]>=time_quantum){
                curr_process[index]-=time_quantum;
                cpu_time+=time_quantum;
            }
            
            else{
                cpu_time += curr_process[index];
                curr_process[index] = 0;
            }

            // inserting all processes whose (cpu time <= arrival time)
            while(!wait_queue.empty() && cpu_time>=wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }

            if(curr_process[index]>0)    ready_queue.push(curr_process);
            else{
                if(curr_process[index+1]!=-1 && curr_process[index+2]!=-1){
                    curr_process[1] = cpu_time + curr_process[index+1];
                    wait_queue.push(curr_process);
                }
            }

            cout<<"P"<<curr_process[0]<<","<<index/2<<" "<<start_time<<" "<<cpu_time<<endl;
        }
        else{
            cpu_time++;
            while(!wait_queue.empty() && cpu_time>=wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }
        }
    }
    return 0;
}

//cpu_time wait_queue
/*
curr_process=ready_queue.top();
ready_queue.pop();
if(process_index!=curr_process[0])
    output based on start, cpu_time-1, burst, process_index
    start=cpu_time;
    process_index=curr_process[0];
cpu_time++;
*/

/*
 */