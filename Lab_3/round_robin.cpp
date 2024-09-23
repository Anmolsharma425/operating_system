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
int main(int argc, char *argv[]) {
    
    string filename = argv[1];
    vector<vector<int>> process_table = read_from_file(filename);
    priority_queue<vector<int>, vector<vector<int>>, arrival_time_comparator> wait_queue(
        process_table.begin(), process_table.end());
    queue<vector<int>> ready_queue;
    
    // for storing the output of simulator
    vector<string> output;
    int makespan=0;
    vector<int> wait_time(process_table.size()+1, 0);
    vector<int> completion_time(process_table.size()+1, 0);
    for(auto i:process_table){
        completion_time.push_back(i[1]);
    }
    
    //initializing the cpu_time
    int cpu_time=wait_queue.top()[1];
    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    while(!wait_queue.empty() || !ready_queue.empty()){


        if(!ready_queue.empty()){
            
            
            int index=2;                      //for calculating non-zero cpu burst index
            int time_quantum=10;              //time quantum

            vector<int> curr_process=ready_queue.front();     
            ready_queue.pop();
            
            

            //moving our index to non-zero cpu burst time index
            while(index<curr_process.size() && curr_process[index]==0)    index+=2;

            //if no non-zero cpu burst is found
            if(index>=curr_process.size())  continue;

            // storing waiting time for each process
            wait_time[curr_process[0]-1]+=(cpu_time-curr_process[1]);

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

            if(curr_process[index]>0){
                curr_process[1]=cpu_time;
                ready_queue.push(curr_process);
            }    
            else{
                if(curr_process[index+1]!=-1 && curr_process[index+2]!=-1){
                    curr_process[1] = cpu_time + curr_process[index+1];
                    wait_queue.push(curr_process);
                }
                else completion_time[curr_process[0]-1]=cpu_time-completion_time[curr_process[0]-1];
            }

            //storing outputs
            output.push_back("P"+to_string(curr_process[0])+","+to_string(index/2)
            +"    "+to_string(start_time)+"   "+to_string(cpu_time));
        }
        else{
            cpu_time++;
            while(!wait_queue.empty() && cpu_time>=wait_queue.top()[1]){
                ready_queue.push(wait_queue.top());
                wait_queue.pop();
            }
        }
    }

    makespan=cpu_time-makespan;
    int total_wt=0;
    for(int i:wait_time)    total_wt+=i;
    int max_wait_time = *max_element(wait_time.begin(), wait_time.end());
    int total_ct=0;
    for(int i:completion_time)     total_ct+=i;
    int max_compl_time = *max_element(completion_time.begin(), completion_time.end());

    //printing outputs
    for(string s:output)  cout<<s<<endl;
    cout<<"\n"<<"Makespan: "<<makespan<<endl;
    cout<<"Average Waiting Time: "<<(double)total_wt/(double)process_table.size()<<endl;
    cout<<"Maximum Waiting Time: "<<max_wait_time<<endl;
    cout<<"Average Completion Time: "<<(double)total_ct/(double)process_table.size()<<endl;
    cout<<"Maximum Completion Time: "<<max_compl_time<<endl;
    return 0;
}
