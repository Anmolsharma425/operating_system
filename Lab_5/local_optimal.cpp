#include<bits/stdc++.h>
using namespace std;

int pageFault=0;
class FrameStatus{
    public:
    
    int frameCount;
    bool* isFrameLocked;
    queue<pair<int, long long>> f_queue;
    vector<pair<long long, long long>> lastUsed;    //{index, p}

    FrameStatus(int frameCount){
        this->frameCount=frameCount;
        isFrameLocked=new bool[frameCount]();
        for(int i=0;i<frameCount;i++){
            lastUsed.push_back({0, 0});
        }
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
    bool isPagePresent(long long p, long long index, FrameStatus& fstatus){
        if(pageTable.find(p)==pageTable.end()){
            perprocess_pagefault++;
            return false;
        }
        cout<<p<<"-"<<pageTable[p]<<"\n";
        fstatus.lastUsed[pageTable[p]]={index, p};
        return true;
    }
    bool insertPage(long long p, FrameStatus& fstatus, long long index){
        int f=fstatus.getFrame(p);
        if(f==-1){
            return false;
        }
        pageTable[p]=f;
        cout<<p<<" "<<pageTable[p]<<"\n";
        fstatus.lastUsed[pageTable[p]]={index, p};
        return true;
    }            
};

void replacement_FIFO(FrameStatus& fstatus, PageTable& p0, PageTable& p1, PageTable& p2, PageTable& p3){
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

void replacement_LRU(FrameStatus& fstatus, PageTable& p0, PageTable& p1, PageTable& p2, PageTable& p3){
    int f, min = INT_MAX;
    long long p;
    for(int i=0;i<fstatus.lastUsed.size();i++){
        if(min>fstatus.lastUsed[i].first){
            min=fstatus.lastUsed[i].first;
            f=i;
            p=fstatus.lastUsed[i].second;
        }
    }
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

void replacement_random(FrameStatus& fstatus, PageTable& p0, PageTable& p1, PageTable& p2, PageTable& p3){
    // pair<int, long long> f_p = fstatus.f_queue.front();
    // int f=f_p.first;
    // long long p=f_p.second;
    // fstatus.f_queue.pop();
    // fstatus.isFrameLocked[f]=false;
    int f = rand()%(fstatus.frameCount);
    long long p = fstatus.lastUsed[f].second;
    fstatus.isFrameLocked[f]=false;
    cout<<p<<"\n";

    if(p0.pageTable.find(p)!=p0.pageTable.end()){
        p0.pageTable.erase(p0.pageTable.find(p));
    }
    if(p1.pageTable.find(p)!=p1.pageTable.end()){
        p1.pageTable.erase(p1.pageTable.find(p));
    }
    if(p2.pageTable.find(p)!=p2.pageTable.end()){
        p2.pageTable.erase(p2.pageTable.find(p));
    }
    if(p3.pageTable.find(p)!=p3.pageTable.end()){
        p3.pageTable.erase(p3.pageTable.find(p));
    }
}

int getFarthestFrame(FrameStatus& fstatus, vector<long long> pageNumberRecord, int index){
    int farthestFrame=0, farthestPageIndex=-1;
    for(int i=0;i<fstatus.lastUsed.size();i++){
        for(int j=index;j<pageNumberRecord.size();j++){
            if(fstatus.lastUsed[i].second==pageNumberRecord[j]){
                if(farthestPageIndex<j){
                    farthestPageIndex=j;
                    farthestFrame=i;
                }
                break;
            }
            if(j+1==pageNumberRecord.size()){
                cout<<i<<" gFF\n";
                return i;
            }
        }
    }
    cout<<farthestFrame<<" gFF\n";
    return farthestFrame;
}

void replacement_optimal(FrameStatus& fstatus, PageTable& pt, vector<long long> pageNumberRecord, int index){
    // pair<int, long long> f_p = fstatus.f_queue.front();
    // int f=f_p.first;
    // long long p=f_p.second;
    // fstatus.f_queue.pop();
    // fstatus.isFrameLocked[f]=false;
    int f = getFarthestFrame(fstatus, pageNumberRecord, index);
    long long p = fstatus.lastUsed[f].second;
    fstatus.isFrameLocked[f]=false;
    // cout<<p<<"\n";

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
    vector<pair<int, long long>> input;
    vector<long long> pageNumberRecord_0;
    vector<long long> pageNumberRecord_1;
    vector<long long> pageNumberRecord_2;
    vector<long long> pageNumberRecord_3;
    if(file.is_open()){
        while(getline(file, line)){
            input.push_back({line[0]-'0', stoll(line.substr(2))});
        }
    }
    for(auto &i : input){
        int d=0;
        int t=pageSize;
        while(t>0){
            d++;
            t=t>>1;
        }
        long long p=i.second>>d;
        cout<<p<<"\n";
        if(i.first==0)  pageNumberRecord_0.push_back(p);
        if(i.first==1)  pageNumberRecord_1.push_back(p);
        if(i.first==2)  pageNumberRecord_2.push_back(p);
        if(i.first==3)  pageNumberRecord_3.push_back(p);
    }
    PageTable ptable_0 = PageTable(pageSize);
    PageTable ptable_1 = PageTable(pageSize);
    PageTable ptable_2 = PageTable(pageSize);
    PageTable ptable_3 = PageTable(pageSize);
    FrameStatus fstatus_0 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_1 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_2 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_3 = FrameStatus(numberOfMemFrames/4);
    long long index_0=0, index_1=0, index_2=0, index_3=0;
    for(auto &i : input){
        int d=0;
        int t=pageSize;
        while(t>0){
            d++;
            t=t>>1;
        }
        long long p=i.second>>d;
        if(i.first==0){
            if(!ptable_0.isPagePresent(p, index_0, fstatus_0)){
                pageFault++;
                if(!ptable_0.insertPage(p, fstatus_0, index_0)){
                    replacement_optimal(fstatus_0, ptable_0, pageNumberRecord_0, index_0);
                    ptable_0.insertPage(p, fstatus_0, index_0);
                }
            }
            index_0++;
        }else if(i.first==1){
            if(!ptable_1.isPagePresent( p, index_1, fstatus_1)){
                pageFault++;
                if(!ptable_1.insertPage(p, fstatus_1, index_1)){
                    replacement_optimal(fstatus_1, ptable_1, pageNumberRecord_1, index_1);
                    ptable_1.insertPage(p, fstatus_1, index_1);
                }
            }
            index_1++;
        }else if(i.first==2){
            if(!ptable_2.isPagePresent(p, index_2, fstatus_2)){
                pageFault++;
                if(!ptable_2.insertPage(p, fstatus_2, index_2)){
                    replacement_optimal(fstatus_2, ptable_2, pageNumberRecord_2, index_2);
                    ptable_2.insertPage(p, fstatus_2, index_2);
                }
            }
            index_2++;
        }else{
            if(!ptable_3.isPagePresent(p, index_3, fstatus_3)){
                pageFault++;
                if(!ptable_3.insertPage(p, fstatus_3, index_3)){
                    replacement_optimal(fstatus_3, ptable_3, pageNumberRecord_3, index_3);
                    ptable_3.insertPage(p, fstatus_3, index_3);
                }
            }
            index_3++;
        }
    }
    cout<<pageFault<<"\nP0 : ";
    cout<<ptable_0.perprocess_pagefault<<"\nP1 : ";
    cout<<ptable_1.perprocess_pagefault<<"\nP2 : ";
    cout<<ptable_2.perprocess_pagefault<<"\nP3 : ";
    cout<<ptable_3.perprocess_pagefault<<"\n";

    return 0;
}