#include <bits/stdc++.h>
#include <queue>
using namespace std;

long long pageSize;
long long numberOfMemoryFrames;
long long pageFault=0;
long long framesAllocated=0;
int numberOfOffsetBits;
queue<pair<int,pair<long long,long long>>> page;

void erasePage(int processId, long long pageNo);

class Process{
    public:
        unordered_map<long long , long long> pageTable;
        
        void allocateSpaceWithFifo(long long virtualId, int processId){
            long long pageNo = virtualId >> numberOfOffsetBits;
            if(!pageTable.count(pageNo)){
                pageFault++;
                if(framesAllocated < numberOfMemoryFrames){
                    pageTable[pageNo]=framesAllocated;
                    page.push({processId,{pageNo,framesAllocated}});
                    framesAllocated++;
                }
                else{
                    if(!page.empty()){
                        auto it=page.front();
                        page.pop();
                        erasePage(it.first,it.second.first);
                        pageTable[pageNo]=it.second.second;
                        page.push({processId,{pageNo,it.second.second}});
                    }
                    else{
                        cerr<<"page table is empty"<<endl;
                        return;
                    }
                }
            }
        }
};

Process P0, P1, P2, P3;
void erasePage(int processId, long long pageNo){
    if(processId==0)
        P0.pageTable.erase(pageNo);
    else if(processId==1)
        P1.pageTable.erase(pageNo);
    else if(processId==2)
        P2.pageTable.erase(pageNo);
    else
        P3.pageTable.erase(pageNo);
}

int main(int argc,char* argv[]){
    if(argc<6){
        cout<<"Enter all arguments"<<endl;
        exit(0);
    }
    pageSize = stoi(argv[1]);
    numberOfMemoryFrames = stoi(argv[2]);
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
    numberOfOffsetBits=log2(pageSize);
    // just go through the demandTable and compute the number of pageFault
    for(int i=0;i<demandPage.size();i++){
        int processId=demandPage[i].first;
        long long virtualAddress= demandPage[i].second;
        if(processId==0)
            P0.allocateSpaceWithFifo(virtualAddress,processId);
        else if(processId==1)
            P1.allocateSpaceWithFifo(virtualAddress,processId);
        else if(processId==2)
            P2.allocateSpaceWithFifo(virtualAddress,processId);
        else if(processId==3)
            P3.allocateSpaceWithFifo(virtualAddress,processId);
    }
    cout<<"Total number of Page faults are ";
    cout<<pageFault<<endl;
    return 0;
}