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

struct BurstTimeComparator{
    bool operator()(vector<int> &P1, vector<int> &P2){
        int index1=2;
        for(;index1<P1.size();index1+=2){
            if(P1[index1]!=0)
                break;
        }

        int index2=2;
        for(;index2<P2.size();index2+=2){
            if(P2[index2]!=0)
                break;
        }
        if(index1>P1.size() || index2>P2.size())
            return false;
        return P1[index1]>P2[index2];
    }
};

struct ArrivalTimeComparator{
    bool operator()(vector<int> &P1, vector<int> &P2){
        return P1[1]>P2[1];
    }
};

//curr_process goes into the wait queue with updated arrival time
//wait_queue and ready_queue both should be empty
int main(){
    vector<vector<int>> process_table=read_from_file();

    priority_queue<vector<int>, vector<vector<int>>, ArrivalTimeComparator> wait_queue(process_table.begin(), process_table.end());

    //priority queue based on current cpu burst time
    priority_queue<vector<int>, vector<vector<int>>, BurstTimeComparator> ready_queue;

    //initializing the cpu_time
    int cpu_time=wait_queue.top()[1];

    //first process in the ready queue
    ready_queue.push(wait_queue.top());
    wait_queue.pop();

    int count=1;
    while(!ready_queue.empty() || !wait_queue.empty()){
        int index=2;
        if(!ready_queue.empty()){

            vector<int> curr_process=ready_queue.top();
            ready_queue.pop();

            //moving our index to non-zero cpu burst time index
            while(index<curr_process.size() && curr_process[index]==0){
                index+=2;
            }

            //if no non-zero cpu burst is found
            if(index>=curr_process.size()){
                continue;
            }

            int burst=index/2;   //current burst

            //output
            cout<<"P"<<curr_process[0]<<",";
            cout<<burst<<" ";
            cout<<cpu_time<<" "<<cpu_time+curr_process[index]<<endl;

            //update the cpu_time and update the current process' cpu burst(current) time
            cpu_time+=curr_process[index]+1;
            curr_process[index]=0;

            if(curr_process[index+1]!=-1  && curr_process[index+2]!=-1){
                curr_process[1]=cpu_time+curr_process[index+1];
                wait_queue.push(curr_process);
            }
        }
        else cpu_time++;

        // inserting all processes whose arrival time is less than cpu time
        while(!wait_queue.empty() && cpu_time>=wait_queue.top()[1]){
            ready_queue.push(wait_queue.top());
            wait_queue.pop();
        }
    }
    return 0;
}

 