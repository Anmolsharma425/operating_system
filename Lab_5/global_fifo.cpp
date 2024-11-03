#include<bits/stdc++.h>
using namespace std;

int pageFault=0;
class FrameStatus{
    public:
    
    int frameCount;
    bool* isFrameLocked;
    queue<pair<int, long long>> f_queue;

    FrameStatus(int frameCount){
        this->frameCount=frameCount;
        isFrameLocked=new bool[frameCount]();
    }
    int getFrame(long long p){
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
    int pageSize;
    int perprocess_pagefault=0;
    unordered_map<long long, int> pageTable;
    queue<long long> pageQueue;
    PageTable(int pageSize){
        this->pageSize=pageSize;
    }
    bool isPagePresent(long long p){
        if(pageTable.find(p)==pageTable.end()){
            perprocess_pagefault++;
            return false;
        }
        cout<<p<<"-"<<pageTable[p]<<"\n";
        return true;
    }
    bool insertPage(long long p, FrameStatus& fstatus){
        int f=fstatus.getFrame(p);
        if(f==-1){
            return false;
        }
        pageTable[p]=f;
        cout<<p<<" "<<pageTable[p]<<"\n";
        return true;
    }            
};

void replacement_func(FrameStatus& fstatus, PageTable& p0, PageTable& p1, PageTable& p2, PageTable& p3){
    pair<int, long long> f_p = fstatus.f_queue.front();
    int f=f_p.first;
    long long p=f_p.second;
    fstatus.f_queue.pop();
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
    vector<pair<int, long long>> input;
    if(file.is_open()){
        while(getline(file, line)){
            input.push_back({line[0]-'0', stoll(line.substr(2))});
        }
    }
    PageTable ptable_0 = PageTable(pageSize);
    PageTable ptable_1 = PageTable(pageSize);
    PageTable ptable_2 = PageTable(pageSize);
    PageTable ptable_3 = PageTable(pageSize);
    FrameStatus fstatus = FrameStatus(numberOfMemFrames);
    for(auto &i : input){
        int d=0;
        int t=pageSize;
        while(t>0){
            d++;
            t=t>>1;
        }
        d--;
        long long p=i.second>>d;
        if(i.first==0){
            if(!ptable_0.isPagePresent(p)){
                pageFault++;
                if(!ptable_0.insertPage(p, fstatus)){
                    replacement_func(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_0.insertPage(p, fstatus);
                }
            }
        }else if(i.first==1){
            if(!ptable_1.isPagePresent( p)){
                pageFault++;
                if(!ptable_1.insertPage(p, fstatus)){
                    replacement_func(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_1.insertPage(p, fstatus);
                }
            }
        }else if(i.first==2){
            if(!ptable_2.isPagePresent(p)){
                pageFault++;
                if(!ptable_2.insertPage(p, fstatus)){
                    replacement_func(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_2.insertPage(p, fstatus);
                }
            }
        }else{
            if(!ptable_3.isPagePresent(p)){
                pageFault++;
                if(!ptable_3.insertPage(p, fstatus)){
                    replacement_func(fstatus, ptable_0, ptable_1, ptable_2, ptable_3);
                    ptable_3.insertPage(p, fstatus);
                }
            }
        }
    }
    cout<<pageFault<<"\nP0 : ";
    cout<<ptable_0.perprocess_pagefault<<"\nP1 : ";
    cout<<ptable_1.perprocess_pagefault<<"\nP2 : ";
    cout<<ptable_2.perprocess_pagefault<<"\nP3 : ";
    cout<<ptable_3.perprocess_pagefault<<"\n";

    return 0;
}