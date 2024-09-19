#include<bits/stdc++.h>
#include<fstream>
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
int main(){
    vector<vector<int>> process_table=read_from_file();
    for(int i=0;i<process_table.size();i++){
        process_table[i].insert(process_table[i].begin(),i+1);
    }
    queue<vector<int>> ready_queue;
    ready_queue.push(process_table[0]);
    int cpu_time=process_table[0][1];
    int count=1;
    while(!ready_queue.empty()){
        vector<int> curr_process=ready_queue.front();
        ready_queue.pop();
        int index=2;
        //moving to current burst to be executing
        while(index<curr_process.size() && curr_process[index]==0){
            index+=2;
        }
        if(index>=curr_process.size())
            continue;
        cout<<"P"<<curr_process[0]<<",";
        int burst=index/2;
        cout<<burst<<" ";
        cout<<cpu_time<<" "<<cpu_time+curr_process[index]<<endl;
        cpu_time+=curr_process[index]+1;
        curr_process[index]=0;
        while(count<process_table.size()){
            if(process_table[count][1]<=cpu_time){
                ready_queue.push(process_table[count]),count++;
            }
            else
                break;
        }
        if(index+1<curr_process.size() && curr_process[index+1]!=-1)
            ready_queue.push(curr_process);
    }
    return 0;
}