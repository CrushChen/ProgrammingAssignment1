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
    return 0;
}