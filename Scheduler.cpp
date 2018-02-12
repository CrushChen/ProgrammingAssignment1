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

Scheduler::Scheduler(std::string file_name_, int block_duration, int time_slice) {
    BLOCK_DURATION = block_duration;
    TIME_SLICE = time_slice;
    global_time = 0;
    ParseFile(file_name_);
}

std::vector<Scheduler::Process> Scheduler::ParseFile(std::string file_name_) {
    vector<Scheduler::Process> processes;
    /****
     * TODO:
     * - Read input file and extract data
     * - Create std::vector of processes contained all processes that
     *   need to be run (all process variables having been filled with data 
     *   from input file
     *****/
    string line; //line we will send to Execute()
    Process temp;
    inputFileStream.open(file_name);

    if (inputFileStream.fail()) {

        cerr << "ERROR: file not found: " << file_name << "\n";

        exit(2);

    }
    //While our file has another line, execute that line
    while (getline(inputFileStream, line)) {
        vector <string> tokens;
        // stringstream class check1
        stringstream check1(line);
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
                temp.termination_time = 0; //need to update with time goes by
                processes.push_back(temp);   
    }

    // If terminated for reason other than end of file
    if (!text_file.eof()) {
        std::cerr << "ERROR: failure while reading file: " << file_name_ << "\n";
        exit(2);
    }

    text_file.close();
    return processes;
}

/****
 * TODO:
 * - Implement round robin scheduling algorithm
 *****/
void Scheduler::RoundRobin(std::vector<Scheduler::Process> processes) {

}

/****
 * TODO:
 * - Implement shortest process next scheduling algorithm
 *****/
void Scheduler::ShortestProcessNext(std::vector<Scheduler::Process> processes) {

}

/****
 * TODO:
 * - Compute average turn around time of processes
 *****/
float Scheduler::AverageTurnaroundTime(std::vector<Scheduler::Process> processes) {
    int average = 0 ;
    for (unsigned i=0; i<processes.size(); i++)
    {
        average += processes.at(i).termination_time;
    }
    average = average/processes.size() ;
    return 0;
}
