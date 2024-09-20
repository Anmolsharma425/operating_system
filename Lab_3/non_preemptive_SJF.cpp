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

struct BurstTimeComparator{
    bool operator()(const vector<int>& P1, const vector<int>& P2){
        int index1=2;
        while(index1<P1.size() && P1[index1]==0) index1+=2;
 
        int index2=2;
        while(index2<P2.size() && P2[index2]==0) index2+=2;

        if(index1>P1.size() || index2>P2.size()) return false;

        return P1[index1]>P2[index2];
    }
};

struct ArrivalTimeComparator{
    bool operator()(const vector<int>& P1, const vector<int>& P2){
        return P1[1]>P2[1];
    }
};

//curr_process goes into the wait queue with updated arrival time
//wait_queue and ready_queue both should be empty
int main(){
    vector<vector<int>> process_table=read_from_file();
    //priority queue based on arrival time and current cpu burst time
    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(
        process_table.begin(), process_table.end());
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;
    
    

    // vector to store the output of simulator
    int makespan=wait_queue.top()[1];
    vector<int> wait_time(process_table.size(), 0);
    vector<int> run_time(process_table.size(), 0);
    vector<string> output;

    //initializing the cpu_time
    int cpu_time=wait_queue.top()[1];

    while(!ready_queue.empty() || !wait_queue.empty()){
        
        // inserting all processes whose arrival time is less than cpu time
        while(!wait_queue.empty() && cpu_time>=wait_queue.top()[1]){
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }
        int index=2;
        if(!ready_queue.empty()){

            vector<int> curr_process=ready_queue.top();
            ready_queue.pop();

            //moving our index to non-zero cpu burst time index
            while(index<curr_process.size() && curr_process[index]==0)   index+=2;

            //if no non-zero cpu burst is found
            if(index>=curr_process.size())    continue;


            //storing outputs
            wait_time[curr_process[0]-1]+=(cpu_time-curr_process[1]);
            output.push_back("P"+to_string(curr_process[0])+","+to_string(index/2)+" "+to_string(cpu_time)+" "+to_string(cpu_time+curr_process[index]));
            run_time[curr_process[0]-1]+=curr_process[index];
            //update the cpu_time and update the current process' cpu burst(current) time
            cpu_time+=curr_process[index];
            curr_process[index]=0;

            if(curr_process[index+1]!=-1  && curr_process[index+2]!=-1){
                curr_process[1]=cpu_time+curr_process[index+1];
                wait_queue.push(curr_process);
            }
        }
        else cpu_time++;
    }

    makespan=cpu_time-makespan;
    int total_wt=0;
    for(int i:wait_time)    total_wt+=i;
    int max_wait_time=*max_element(wait_time.begin(), wait_time.end());
    int total_rt=0;
    for(int i:run_time)     total_rt+=i;
    int max_run_time=*max_element(run_time.begin(), run_time.end());

    //printing outputs
    for(string s:output)  cout<<s<<endl;
    cout<<"\nMakespan: "<<makespan<<endl;
    cout<<"Average Waiting Time: "<<(double)total_wt/(double)process_table.size()<<endl;
    cout<<"Maximum Waiting Time: "<<max_wait_time<<endl;
    cout<<"Average Running Time: "<<(double)total_rt/(double)process_table.size()<<endl;
    cout<<"Maximum Running Time: "<<max_run_time<<endl;

    return 0;
}

 