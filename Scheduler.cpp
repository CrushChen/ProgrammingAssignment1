/*
 * File:   Scheduler.cpp
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:36 PM
 */

#include "Scheduler.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;
using std::ifstream;

Scheduler::Scheduler(std::string file_name_, int block_duration, int time_slice) {
    BLOCK_DURATION = block_duration;
    TIME_SLICE = time_slice;
    ParseFile(file_name_);
}

std::vector<Scheduler::Process> Scheduler::ParseFile(std::string file_name_) {
    vector<Scheduler::Process> processes;
    ifstream inputFileStream; //input file stream
    /****
     * TODO:
     * - Read input file and extract data
     * - Create std::vector of processes contained all processes that
     *   need to be run (all process variables having been filled with data 
     *   from input file
     *****/
    string line; //line we will send to Execute()
    Process temp;
    inputFileStream.open(file_name_);

    if (inputFileStream.fail()) {

        cerr << "ERROR: file not found: " << file_name_ << "\n";

        exit(2);

    }
    //While our file has another line, execute that line
    while (getline(inputFileStream, line)) {
        vector <string> tokens;
        // stringstream class check1
        istringstream check1(line);
        string intermediate;
        // Tokenizing w.r.t. space ' '
        while (getline(check1, intermediate, ' ')) {
            tokens.push_back(intermediate);
        }
        // get each character and store it into process        
        temp.name = tokens[0];
        temp.arrival_time = stoi(tokens[1]);
        temp.total_time = stoi(tokens[2]);
        temp.remaining_time = stoi(tokens[2]);
        temp.block_interval = stoi(tokens[3]);
        temp.termination_time = -1; //need to update with time goes by
        processes.push_back(temp);
    }

    // If terminated for reason other than end of file
    if (!inputFileStream.eof()) {
        std::cerr << "ERROR: failure while reading file: " << file_name_ << "\n";
        exit(2);
    }

    inputFileStream.close();
    return processes;
}

/****
 * TODO:
 * - Implement round robin scheduling algorithm
 *****/
void Scheduler::RoundRobin(std::vector<Scheduler::Process> processes) {
     cout<< "RR " + block_duration +" " + time_slice ;
    bool done = false;
    int time =0;
    int i =0;
    std::vector<int> blocktimeremain(processes.size());
    for(int x = 0; x < processes.size(); ++x)
    { blocktimeremain[x] = 0;}
    while(done == false)
    {
        if((((processes.at(i).total_time)-(processes.at(i).remaining_time))+time_slice)%(processes.at(i).block_interval)==0)
        {
            time=time+time_slice;
        cout << time + " " + processes.at(i).name + " " + time_slice +" " + B +'\n';
        processes.at(i).remaining_time=processes.at(i).remaining_time-time_slice;
        processes.at(i).termination_time= time;
        blocktimeremain.at(i)=block_duration;
        }
        else 
        {
            if(((((processes.at(i).total_time)-(processes.at(i).remaining_time))+time_slice)%(processes.at(i).block_interval))>time_slice) 
            {
                cout << time + " " + processes.at(i).name + " " + time_slice +" " + S +'\n';
                processes.at(i).remaining_time=processes.at(i).remaining_time-time_slice;
                processes.at(i).termination_time= time;
                for(int x = 0; x < processes.size(); ++x)
                    { if (blocktimeremain[x] != 0)
                        {
                        blocktimeremain[x]=blocktimeremain[x]-time_slice;
                    
                        }
                    }
            else
            {
                
            }
        }
    }
}

/****
 * TODO:
 * - Implement shortest process next scheduling algorithm
 *****/
void Scheduler::ShortestProcessNext(std::vector<Scheduler::Process> processes) {
    //Documentation for std::priority_queue:
    //http://en.cppreference.com/w/cpp/container/priority_queue
    std::priority_queue<Process> blocked__list; //maintains the blocked process list
    std::priority_queue<Process> ready_list; //maintains the ready list for SPN;


}

/**
 * Computes average turn around time of processes
 * @param processes
 * @return 
 */
float Scheduler::AverageTurnaroundTime(std::vector<Scheduler::Process> processes) {
    float sum = 0;
    for (unsigned i = 0; i < processes.size(); i++) {
        sum += processes.at(i).termination_time - processes.at(i).arrival_time;
    }
    return sum / static_cast<float> (processes.size());
}
