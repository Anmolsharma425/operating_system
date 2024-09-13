#include<bits/stdc++.h>
#include <fstream>  // For file handling
using namespace std;

int main() {
    ifstream file("process2.dat");
    queue<vector<int>> data;
    string line;
    int process_count =1;

    while (getline(file, line)) {
        istringstream lineStream(line);
        vector<int> numbers;
        int number;

        while (lineStream >> number) {
            numbers.push_back(number);
        }

        if (!numbers.empty()) {
             numbers.push_back(process_count);
             process_count++;
            data.push(numbers);
        }
    }

    // Close the file
    file.close();

    // Output the result
    while(!data.empty())
    {
        auto temp=data.front();
        data.pop();
        for(auto it:temp)
        {
            cout<<it<<" ";
        }
        cout<<endl;
    }

    return 0;
}