#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        // cout<<"hello";
        cout << "usage: ./a.out <page-size> <number-of-memory-frames> <replacement-policy> <allocation-policy> <path-to-memory-trace-file>\n";
        exit(1);
    }

    int pageSize = stoi(argv[1]);
    int numberOfMemFrames = stoi(argv[2]);
    string replacementPolicy = argv[3];
    string allocationPolicy = argv[4];
    string tracePath = argv[5];

    string sourceFile;

    // Determine source file based on policies
    if (allocationPolicy[0] == 'g' || allocationPolicy[0] == 'G')
    {
        if (replacementPolicy[0] == 'f' || replacementPolicy[0] == 'F')
            sourceFile = "global_fifo.cpp";
        else if (replacementPolicy[0] == 'l' || replacementPolicy[0] == 'L')
            sourceFile = "global_lru.cpp";
        else if (replacementPolicy[0] == 'r' || replacementPolicy[0] == 'R')
            sourceFile = "global_random.cpp";
        else if (replacementPolicy[0] == 'o' || replacementPolicy[0] == 'O')
            sourceFile = "global_optimal.cpp";
    }
    else if (allocationPolicy[0] == 'l' || allocationPolicy[0] == 'L')
    {
        if (replacementPolicy[0] == 'f' || replacementPolicy[0] == 'F')
            sourceFile = "local_fifo.cpp";
        else if (replacementPolicy[0] == 'l' || replacementPolicy[0] == 'L')
            sourceFile = "local_lru.cpp";
        else if (replacementPolicy[0] == 'r' || replacementPolicy[0] == 'R')
            sourceFile = "local_random.cpp";
        else if (replacementPolicy[0] == 'o' || replacementPolicy[0] == 'O')
            sourceFile = "local_optimal.cpp";
    }

    // Check if sourceFile was set properly
    if (sourceFile.empty())
    {
        cerr << "Invalid allocation or replacement policy.\n";
        exit(1);
    }

    // Compile the chosen source file
    string compileCommand = "g++ " + sourceFile + " -o compiled_program";
    if (system(compileCommand.c_str()) != 0)
    {
        cerr << "Compilation failed.\n";
        exit(1);
    }
    char *new_argv[] = {(char *)"./compiled_program", argv[1],
                        argv[2], argv[3], argv[4], argv[5], NULL};

    if (execv("./compiled_program", new_argv) == -1)
    {
        perror("execv failed for child");
        exit(1);
    }
    return 0;
}
