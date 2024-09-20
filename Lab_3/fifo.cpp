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
int main(){
    vector<vector<int>> process_table=read_from_file();
    queue<vector<int>> ready_queue;

    // for storing the output of simulator
    vector<string> output;
    int makespan=process_table[0][1];
    vector<int> wait_time(process_table.size(), 0);
    vector<int> run_time(process_table.size(), 0);

    ready_queue.push(process_table[0]);
    int cpu_time=ready_queue.front()[1];
    int count=1;
    while(!ready_queue.empty()){
        vector<int> curr_process=ready_queue.front();
        ready_queue.pop();
        int index=2;
        while(index<curr_process.size() && curr_process[index]==0)  index+=2;
        if(index>=curr_process.size())  continue;

        wait_time[curr_process[0]-1]+=(cpu_time-curr_process[1]);
        // storing output
        output.push_back("P"+to_string(curr_process[0])+","+to_string(index/2)+
                            " "+to_string(cpu_time)+" "+to_string(cpu_time+curr_process[index]));

        run_time[curr_process[0]-1]+=curr_process[index];
        cpu_time+=curr_process[index];
        curr_process[index]=0;
        curr_process[1]=cpu_time;
        while(count<process_table.size()){
            if(process_table[count][1]<=cpu_time){
                ready_queue.push(process_table[count]),count++;
            }
            else    break;
        }
        if(index+1<curr_process.size() && curr_process[index+1]!=-1)
            ready_queue.push(curr_process);
    }

    makespan=cpu_time-makespan;
    int total_wt=0;
    for(int i:wait_time) total_wt+=i;
    int max_wait_time=*max_element(wait_time.begin(), wait_time.end());
    int total_rt=0;
    for(int i:run_time) total_rt+=i;
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