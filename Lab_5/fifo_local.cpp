#include <bits/stdc++.h>
#include <queue>
using namespace std;

long long pageSize;
long long numberOfMemoryFrames;
long long pageFault=0;
int numberOfOffsetBits;

class Process{
    public:
        unordered_map<long long , long long> pageTable;
        queue<pair<long long,long long>> page;
        long long framesAllocated=0;
        void allocateSpaceWithFifo(long long virtualId){
            long long pageNo = virtualId >> numberOfOffsetBits;
            if(!pageTable.count(pageNo)){
                pageFault++;
                if(framesAllocated < numberOfMemoryFrames){
                    pageTable[pageNo]=framesAllocated;
                    page.push({pageNo,framesAllocated});
                    framesAllocated++;
                }
                else{
                    pair<long long,long long> it=page.front();
                    page.pop();
                    page.push({pageNo,it.second});
                    pageTable.erase(it.first);
                    pageTable[pageNo]=it.second;
                }
            }
        }
};

int main(int argc,char* argv[]){
    if(argc<6){
        cout<<"Enter all arguments"<<endl;
        exit(0);
    }
    pageSize = stoi(argv[1]);
    // divide them equally among all processes
    numberOfMemoryFrames = stoi(argv[2]);
    cout<<numberOfMemoryFrames<<endl;
    if(numberOfMemoryFrames < 4){
        cerr<<" at lest 4 frames are needed for equal partition"<<endl;
        return 1;
    }
    numberOfMemoryFrames/=4;
    cout<<numberOfMemoryFrames<<endl;
    string replacementPolicy=argv[3];
    string allocationPolicy=argv[4];
    vector<pair<int,long long>> demandPage;
    ifstream file(argv[5]);
    string line;
    if(file.is_open()){
        while(getline(file,line)){
            int n=line.size();
            int index=-1;
            int process=line[0]-'0';
            string t="";
            for(int i=2;i<line.size();i++){
                t+=line[i];
            }
            long long page=stoll(t);
            demandPage.push_back({process,page});
        }
    }
    else{
        cerr<<"cannot open the file"<<endl;
        return 1;
    }
    file.close();
    // now lets compute number of offset bit
    numberOfOffsetBits=static_cast<int> (log2(pageSize));
    Process P0, P1, P2, P3;
    // simply virtual to physical memory mapping is happening
    for(int i=0;i<demandPage.size();i++){
        int processId=demandPage[i].first;
        long long virtualAddress= demandPage[i].second;
        if(processId==0)
            P0.allocateSpaceWithFifo(virtualAddress);
        else if(processId==1)
            P1.allocateSpaceWithFifo(virtualAddress);
        else if(processId==2)
            P2.allocateSpaceWithFifo(virtualAddress);
        else if(processId==3)
            P3.allocateSpaceWithFifo(virtualAddress);
    }
    cout<<"Total number of Page faults are ";
    cout<<pageFault<<endl; 
    return 0;
}