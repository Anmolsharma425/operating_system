
#include<bits/stdc++.h>
#include<fstream>
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

    // for output
    vector<string> output_cpu0;
    vector<string> output_cpu1;
    
    // initializing the cpu_time1 with first process arrival time
    int cpu_time1=wait_queue.top()[1];
    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    // initializing the cpu_time1 with second process arrival time
    // Will cpu2 always start with second process?
    int cpu_time2=wait_queue.top()[1];   
    

    while(!wait_queue.empty() || !ready_queue.empty()){


        if(!ready_queue.empty()){

            int index=2, time_quantum1=10, time_quantum2=10;

            vector<int> curr_process=ready_queue.front();
            ready_queue.pop();

            //moving our index to non-zero cpu burst time index
            while(index<curr_process.size() && curr_process[index]==0)    index+=2;

            //if no non-zero cpu burst is found
            if(index>=curr_process.size())     continue;

            if(cpu_time1<=cpu_time2){
                int start_time=cpu_time1;
                if(curr_process[index]>=time_quantum1){
                    curr_process[index]-=time_quantum1;
                    cpu_time1+=time_quantum1;
                }
                
                else{
                    cpu_time1 += curr_process[index];
                    curr_process[index] = 0;
                }
                output_cpu0.push_back("P"+to_string(curr_process[0])+","+to_string(index/2)+" "+to_string(start_time)+" "+to_string(cpu_time1));
            }
            else{
                int start_time=cpu_time2;
                if(curr_process[index]>=time_quantum2){
                    curr_process[index]-=time_quantum2;
                    cpu_time2+=time_quantum2;
                }
                
                else{
                    cpu_time2 += curr_process[index];
                    curr_process[index] = 0;
                }
                output_cpu1.push_back("P"+to_string(curr_process[0])+","+to_string(index/2)+" "+to_string(start_time)+" "+to_string(cpu_time2));
            }

            // inserting all processes whose (max(cpu_time1, cpu_time2) <= arrival time)
            // to ensure that 
            while(!wait_queue.empty() && max(cpu_time1, cpu_time2)>=wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }

            if(curr_process[index]>0)    ready_queue.push(curr_process);
            else{
                if(curr_process[index+1]!=-1 && curr_process[index+2]!=-1){
                    curr_process[1] = max(cpu_time1, cpu_time2) + curr_process[index+1];
                    wait_queue.push(curr_process);
                }
            }

            
        }
        else{
            cpu_time1++;
            cpu_time2++;
            while(!wait_queue.empty() && max(cpu_time1, cpu_time2)>=wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }
        }
    }

    // printing outputs
    cout<<"CPU0\n"<<endl;
    for(string s:output_cpu0) cout<<s<<endl;
    
    cout<<"\n\n\nCPU1\n"<<endl;
    for(string s:output_cpu1) cout<<s<<endl;
    return 0;
}
