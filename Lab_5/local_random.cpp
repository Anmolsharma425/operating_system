#include<bits/stdc++.h>
using namespace std;

int pageFault=0;
class FrameStatus{
    public:
    
    int frameCount;
    bool* isFrameLocked;
    vector<pair<long long, uint64_t>> lastUsed;

    FrameStatus(int frameCount){
        this->frameCount=frameCount;
        isFrameLocked=new bool[frameCount]();
        for(int i=0;i<frameCount;i++){
            lastUsed.push_back({0, 0});
        }
    }
    int getFrame(uint64_t p){
        for(int i=0;i<frameCount;i++){
            if(!isFrameLocked[i]){
                isFrameLocked[i]=true;
                return i;
            }
        }
        return -1;
    }
};

class PageTable{
    public:
    int perprocess_pagefault=0;
    unordered_map<uint64_t, int> pageTable;
    bool isPagePresent(uint64_t p, long long index, FrameStatus& fstatus){
        if(pageTable.find(p)==pageTable.end()){
            perprocess_pagefault++;
            return false;
        }
        fstatus.lastUsed[pageTable[p]]={index, p};
        return true;
    }
    bool insertPage(uint64_t p, FrameStatus& fstatus, long long index){
        int f=fstatus.getFrame(p);
        if(f==-1){
            return false;
        }
        pageTable[p]=f;
        fstatus.lastUsed[pageTable[p]]={index, p};
        return true;
    }            
};

void replacement_random(FrameStatus& fstatus, PageTable& pt){
    int f = rand()%(fstatus.frameCount);
    uint64_t p = fstatus.lastUsed[f].second;
    fstatus.isFrameLocked[f]=false;

    if(pt.pageTable.find(p)!=pt.pageTable.end()){
        pt.pageTable.erase(pt.pageTable.find(p));
    }
}


int main(int argc, char** argv){
    if(argc!=6){
        cout<<"usage: ./a.out <page-size> <number-of-memory-frames> <replacement-policy> <allocation-policy> <path-to-memory-trace-file>\n";
        exit(0);
    }
    int pageSize=stoi(argv[1]);
    int numberOfMemFrames=stoi(argv[2]);
    string replacementPolicy=argv[3];
    string allocationPolicy=argv[4];
    string tracePath=argv[5];

    ifstream file(tracePath);
    string line;
    vector<pair<int, uint64_t>> input;
    if(file.is_open()){
        while(getline(file, line)){
            uint64_t value;
            istringstream iss(line.substr(2));
            iss >> value;
            input.push_back({line[0]-'0', value});
        }
    }
    PageTable ptable_0 = PageTable();
    PageTable ptable_1 = PageTable();
    PageTable ptable_2 = PageTable();
    PageTable ptable_3 = PageTable();
    FrameStatus fstatus_0 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_1 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_2 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_3 = FrameStatus(numberOfMemFrames/4);
    long long index=0;
    for(auto &i : input){
        int d=(int)log2(pageSize);
        uint64_t p=i.second>>d;
        if(i.first==0){
            if(!ptable_0.isPagePresent(p, index, fstatus_0)){
                pageFault++;
                if(!ptable_0.insertPage(p, fstatus_0, index)){
                    replacement_random(fstatus_0, ptable_0);
                    ptable_0.insertPage(p, fstatus_0, index);
                }
            }
        }else if(i.first==1){
            if(!ptable_1.isPagePresent( p, index, fstatus_1)){
                pageFault++;
                if(!ptable_1.insertPage(p, fstatus_1, index)){
                    replacement_random(fstatus_1, ptable_1);
                    ptable_1.insertPage(p, fstatus_1, index);
                }
            }
        }else if(i.first==2){
            if(!ptable_2.isPagePresent(p, index, fstatus_2)){
                pageFault++;
                if(!ptable_2.insertPage(p, fstatus_2, index)){
                    replacement_random(fstatus_2, ptable_2);
                    ptable_2.insertPage(p, fstatus_2, index);
                }
            }
        }else{
            if(!ptable_3.isPagePresent(p, index, fstatus_3)){
                pageFault++;
                if(!ptable_3.insertPage(p, fstatus_3, index)){
                    replacement_random(fstatus_3, ptable_3);
                    ptable_3.insertPage(p, fstatus_3, index);
                }
            }
        }
        index++;
    }
    cout<<"Total Page Fault : "<<pageFault<<"\nP0 : ";
    cout<<ptable_0.perprocess_pagefault<<"\nP1 : ";
    cout<<ptable_1.perprocess_pagefault<<"\nP2 : ";
    cout<<ptable_2.perprocess_pagefault<<"\nP3 : ";
    cout<<ptable_3.perprocess_pagefault<<"\n";

    return 0;
}