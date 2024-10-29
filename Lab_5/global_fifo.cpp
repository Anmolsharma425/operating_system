#include <bits/stdc++.h>
#include <queue>
using namespace std;

long long pageSize;
long long numberOfMemoryFrames;
long long pageFault=0;
long long framesAllocated=0;
queue<pair<int,pair<long long,long long>>> page;

class Process{
    public:
        unordered_map<long long , long long> pageTable;

        void allocateSpaceWithFifo(long long virtualId, int processId){
            vector<int> bit(64,0);
            int numberOfOffsetBits=0;x
            long long t=pageSize;
            while(t>0){
                numberOfOffsetBits++;
                t=t>>1;
            }
            int i=0;
            while(virtualId>0){
                int t2=virtualId & 1;
                bit[i]=t2;
                i++;
                virtualId= virtualId >> 1;
            }
            long long pageNo=0;
            long long offset=0;
            for(int i=0;i<numberOfOffsetBits;i++){
                offset+=(bit[i]*pow(2,i));
            }
            for(int i=numberOfOffsetBits;i<64;i++){
                pageNo+=(bit[i]*pow(2,i-numberOfOffsetBits));
            }
            if(!pageTable.count(pageNo)){
                pageFault++;
                if(framesAllocated < numberOfMemoryFrames){
                    pageTable[pageNo]=framesAllocated;
                    framesAllocated++;
                    page.push({processId,{pageNo,framesAllocated}});
                }
                else{
                    if(!page.empty()){
                        pair<int,pair<long long,long long>> it=page.front();
                        page.pop();
                        page.push({processId,{pageNo,it.second.second}});
                        pageTable[pageNo]=it.second.second;
                    }
                }
            }
        }
};
int main(int argc,char* argv[]){
    if(argc<5){
        cout<<"Enter all arguments"<<endl;
        exit(0);
    }
    pageSize = stoll(argv[1]);
    numberOfMemoryFrames=stoll(argv[2]);
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
    file.close();
    Process P0;
    Process P1;
    Process P2;
    Process P3;
    for(int i=0;i<demandPage.size();i++){
        int processId=demandPage[i].first;
        if(processId==0)
            P0.allocateSpaceWithFifo(demandPage[i].second,processId);
        else if(processId==1)
            P1.allocateSpaceWithFifo(demandPage[i].second,processId);
        else if(processId==2)
            P2.allocateSpaceWithFifo(demandPage[i].second,processId);
        else if(processId==3)
            P3.allocateSpaceWithFifo(demandPage[i].second,processId);
    }
    cout<<"Total number of Page faults are ";
    cout<<pageFault<<endl; 
    return 0;
}