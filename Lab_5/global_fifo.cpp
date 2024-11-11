#include<bits/stdc++.h>
using namespace std;

int pageFault=0;
class FrameStatus{
    public:
    
    int frameCount;
    bool* isFrameLocked;
    queue<pair<int, uint64_t>> f_queue;

    FrameStatus(int frameCount){
        this->frameCount=frameCount;
        isFrameLocked=new bool[frameCount]();
    }
    int getFrame(uint64_t p){
        for(int i=0;i<frameCount;i++){
            if(!isFrameLocked[i]){
                isFrameLocked[i]=true;
                f_queue.push({i, p});
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
    bool isPagePresent(uint64_t p){
        if(pageTable.find(p)==pageTable.end()){
            perprocess_pagefault++;
            return false;
        }
        return true;
    }
    bool insertPage(uint64_t p, FrameStatus& fstatus){
        int f=fstatus.getFrame(p);
        if(f==-1){
            return false;
        }
        pageTable[p]=f;
        return true;
    }            
};

void replacement_FIFO(FrameStatus& fstatus, PageTable& p0, PageTable& p1, PageTable& p2, PageTable& p3){
    pair<int, uint64_t> f_p = fstatus.f_queue.front();
    fstatus.f_queue.pop();
    int f=f_p.first;
    uint64_t p=f_p.second;
    fstatus.isFrameLocked[f]=false;
    if(p0.pageTable.find(p)!=p0.pageTable.end()){
        p0.pageTable.erase(p0.pageTable.find(p));
    }
    else if(p1.pageTable.find(p)!=p1.pageTable.end()){
        p1.pageTable.erase(p1.pageTable.find(p));
    }
    else if(p2.pageTable.find(p)!=p2.pageTable.end()){
        p2.pageTable.erase(p2.pageTable.find(p));
    }
    else if(p3.pageTable.find(p)!=p3.pageTable.end()){
        p3.pageTable.erase(p3.pageTable.find(p));
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
    FrameStatus fstatus = FrameStatus(numberOfMemFrames);
    for(auto &i : input){
        int d=(int)log2(pageSize);
        uint64_t p=i.second>>d;
        if(i.first==0){
            if(!ptable_0.isPagePresent(p)){
                pageFault++;
                if(!ptable_0.insertPage(p, fstatus)){
                    replacement_FIFO(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_0.insertPage(p, fstatus);
                }
            }
        }else if(i.first==1){
            if(!ptable_1.isPagePresent( p)){
                pageFault++;
                if(!ptable_1.insertPage(p, fstatus)){
                    replacement_FIFO(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_1.insertPage(p, fstatus);
                }
            }
        }else if(i.first==2){
            if(!ptable_2.isPagePresent(p)){
                pageFault++;
                if(!ptable_2.insertPage(p, fstatus)){
                    replacement_FIFO(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_2.insertPage(p, fstatus);
                }
            }
        }else{
            if(!ptable_3.isPagePresent(p)){
                pageFault++;
                if(!ptable_3.insertPage(p, fstatus)){
                    replacement_FIFO(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_3.insertPage(p, fstatus);
                }
            }
        }
    }
    cout<<"Total Page Fault : "<<pageFault<<"\nP0 : ";
    cout<<ptable_0.perprocess_pagefault<<"\nP1 : ";
    cout<<ptable_1.perprocess_pagefault<<"\nP2 : ";
    cout<<ptable_2.perprocess_pagefault<<"\nP3 : ";
    cout<<ptable_3.perprocess_pagefault<<"\n";

    return 0;
}