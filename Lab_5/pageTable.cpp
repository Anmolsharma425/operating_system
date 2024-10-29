#include<bits/stdc++.h>
using namespace std;

class PageTable{
    private:
        int numFrames;  // total number of frames in physical memory
        int pageFaultCount;
        unordered_map<int, int> Table;

    public:
        PageTable(int numFrames, int pageFaultCount = 0){
            this->numFrames = numFrames;
            this->pageFaultCount = pageFaultCount;
        }

        //get the physical address from the page number
        void getPhysicalAddress(int pageNumber){
            if(Table.find(pageNumber) != Table.end()){
                int frameNumber = Table[pageNumber];
                cout << "page : " << pageNumber << "in the frame " << frameNumber << endl;
            }
            else{
                handlePageFault(pageNumber);
            }
        }

        //handle Page Fault
        void handlePageFault(int pageNumber){
            cout << "page Fault for the page : " << pageNumber << endl;
            pageFaultCount++;

            // ------------------------replacement policy ----------------------------------
        }

        int get_pageFaultCount() return pageFaultCount;
        int get_numFrame() return numFrames;
};

int main(){

}