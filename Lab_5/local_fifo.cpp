#include<bits/stdc++.h>
using namespace std;

int pageFault=0;
class FrameStatus{
    public:
    
    int frameCount;
    bool* isFrameLocked;
    // bool* isFrameLocked_1;
    // bool* isFrameLocked_2;
    // bool* isFrameLocked_3;
    queue<pair<int, long long>> f_queue;
    // queue<pair<int, long long>> f1_queue;
    // queue<pair<int, long long>> f2_queue;
    // queue<pair<int, long long>> f3_queue;

    FrameStatus(int frameCount){
        this->frameCount=frameCount;
        isFrameLocked=new bool[frameCount]();
        // isFrameLocked_1=new bool[frameCount/4]();
        // isFrameLocked_2=new bool[frameCount/4]();
        // isFrameLocked_3=new bool[frameCount/4]();
    }
    int getFrame(int pid, long long p){
        // if(pid==0){
        for(int i=0;i<frameCount;i++){
            if(!isFrameLocked[i]){
                isFrameLocked[i]=true;
                f_queue.push({i, p});
                return i;
            }
        }
        // }
        // if(pid==1){
        //     for(int i=0;i<frameCount/4;i++){
        //         if(!isFrameLocked_1[i]){
        //             isFrameLocked_1[i]=true;
        //             f1_queue.push({i, p});
        //             return i;
        //         }
        //     }
        // }
        // if(pid==2){
        //     for(int i=0;i<frameCount/4;i++){
        //         if(!isFrameLocked_2[i]){
        //             isFrameLocked_2[i]=true;
        //             f2_queue.push({i, p});
        //             return i;
        //         }
        //     }
        // }
        // if(pid==3){
        //     for(int i=0;i<frameCount/4;i++){
        //         if(!isFrameLocked_3[i]){
        //             isFrameLocked_3[i]=true;
        //             f3_queue.push({i, p});
        //             return i;
        //         }
        //     }
        // }
        return -1;
    }
};

class PageTable{
    public:
    int pid;
    int pageSize;
    int perprocess_pagefault=0;
    unordered_map<long long, int> pageTable;
    queue<long long> pageQueue;
    PageTable(int pageSize, int pid){
        this->pageSize=pageSize;
        this->pid=pid;
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
        int f=fstatus.getFrame(pid, p);
        if(f==-1){
            return false;
        }
        pageTable[p]=f;
        cout<<p<<" "<<pageTable[p]<<"\n";
        return true;
    }            
};

void replacement_func(FrameStatus& fstatus, PageTable& pt){
    pair<int, long long> f_p;
    f_p = fstatus.f_queue.front();
    fstatus.f_queue.pop();
    fstatus.isFrameLocked[f_p.first]=false;
    // if(pid==1){
    //     f_p = fstatus.f1_queue.front();
    //     fstatus.f1_queue.pop();
    //     fstatus.isFrameLocked_1[f_p.first]=false;
    // }
    // if(pid==2){
    //     f_p = fstatus.f2_queue.front();
    //     fstatus.f2_queue.pop();
    //     fstatus.isFrameLocked_2[f_p.first]=false;
    // }
    // if(pid==3){
    //     f_p = fstatus.f3_queue.front();
    //     fstatus.f3_queue.pop();
    //     fstatus.isFrameLocked_3[f_p.first]=false;
    // }
    int f=f_p.first;
    long long p=f_p.second;
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
    if(file.is_open()){
        while(getline(file, line)){
            input.push_back({line[0]-'0', stoll(line.substr(2))});
        }
    }
    PageTable ptable_0 = PageTable(pageSize, 0);
    PageTable ptable_1 = PageTable(pageSize, 1);
    PageTable ptable_2 = PageTable(pageSize, 2);
    PageTable ptable_3 = PageTable(pageSize, 3);
    FrameStatus fstatus_0 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_1 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_2 = FrameStatus(numberOfMemFrames/4);
    FrameStatus fstatus_3 = FrameStatus(numberOfMemFrames/4);
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
                if(!ptable_0.insertPage(p, fstatus_0)){
                    replacement_func(fstatus_0, ptable_0);
                    ptable_0.insertPage(p, fstatus_0);
                }
            }
        }else if(i.first==1){
            if(!ptable_1.isPagePresent( p)){
                pageFault++;
                if(!ptable_1.insertPage(p, fstatus_1)){
                    replacement_func(fstatus_1, ptable_1);
                    ptable_1.insertPage(p, fstatus_1);
                }
            }
        }else if(i.first==2){
            if(!ptable_2.isPagePresent(p)){
                pageFault++;
                if(!ptable_2.insertPage(p, fstatus_2)){
                    replacement_func(fstatus_2, ptable_2);
                    ptable_2.insertPage(p, fstatus_2);
                }
            }
        }else{
            if(!ptable_3.isPagePresent(p)){
                pageFault++;
                if(!ptable_3.insertPage(p, fstatus_3)){
                    replacement_func(fstatus_3, ptable_3);
                    ptable_3.insertPage(p, fstatus_3);
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