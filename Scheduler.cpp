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
#include <algorithm>

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
    Execute(ParseFile(file_name_));
}

Scheduler::~Scheduler() {
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
        temp.termination_time = -1; //indicating the process has not terminated, needs to be updated when process completes
        temp.is_blocked = false;
        temp.time_blocked = temp.block_interval;
        temp.time_until_blocked = temp.block_interval;
        processes.push_back(temp);
    }

    // If terminated for reason other than end of file
    if (!inputFileStream.eof()) {
        std::cerr << "ERROR: failure while reading file: " << file_name_ << "\n";
        exit(2);
    }

    inputFileStream.close();

    /* Confirms file sets process list correctly
    for(int i = 0; i < processes.size(); ++i){
        cout << "Process " << processes.at(i).name << " has arrival time " << processes.at(i).arrival_time;
        cout << " blocking time " << processes.at(i).block_interval << " total time " << processes.at(i).total_time << std::endl;
    }
     */
    return processes;
}

void Scheduler::Execute(std::vector<Scheduler::Process> processes) {
    RoundRobin(processes);
    //process objects are reset by the AverageTurnaroundTime function
    ShortestProcessNext(processes);
}

void Scheduler::RoundRobin(std::vector<Scheduler::Process> processes) {
    int time = 0;
    int currentIndex = 0;
    int numProcesses = processes.size();
    int currentIntervalTime = 0;
    bool switched = false;
    bool complete = false;
    int numBlocked = 0;
    int count;
    int activeProcesses = 0;
    bool wasIdle = false;
    Scheduler::Process* temp;
    Process* currentProcess;

    cout << "RR " << BLOCK_DURATION << " " << TIME_SLICE << std::endl;

    //wait until a process arrives 
    bool running = false;
    while (!running) {
        for (int i = 0; i < numProcesses; ++i) {
            if (processes.at(i).arrival_time == time) {
                currentProcess = &processes.at(i);
                running = true;
                currentIndex = i;
                ++activeProcesses;
            }
        }
        ++time;
        ++currentIntervalTime;
    }

    while (!complete) {
        //Update blocked Processes
        if (numBlocked != 0) {
            for (int i = 0; i < numProcesses; ++i) {
                temp = &processes.at(i);
                if (temp->is_blocked) {
                    temp->time_blocked -= 1;
                    if (temp->time_blocked <= 0) {
                        temp->is_blocked = false;
                        temp->time_blocked = temp->block_interval;
                        if (numBlocked == activeProcesses) { //system was idle
                            cout << " " << (time - currentIntervalTime) << "\t<idle>\t" << +currentIntervalTime << "\tI" << std::endl;
                            currentIntervalTime = 0;
                            wasIdle = true;
                        }
                        --numBlocked;
                    }
                }
            }
        }

        if (numBlocked != activeProcesses && !wasIdle) { //system is not idle
            //check for current process termination
            //switch processes and put currentProcess on completed list
            if ((currentProcess->remaining_time == currentIntervalTime)) { //process is terminated
                currentProcess->termination_time = time;
                cout << " " << (time - currentIntervalTime) << "\t" << currentProcess->name << "\t" << currentProcess->remaining_time << "\tT" << std::endl;
                currentProcess->remaining_time = 0;
                currentIntervalTime = 0;
                --activeProcesses;
                switched = true;
            } else if (currentIntervalTime == currentProcess->time_blocked) { //process is blocking
                currentProcess->is_blocked = true;
                currentProcess->remaining_time -= currentIntervalTime;
                currentProcess->time_blocked = BLOCK_DURATION;
                cout << " " << (time - currentIntervalTime) << "\t" << currentProcess->name << "\t" << +currentIntervalTime << "\tB" << std::endl;
                currentIntervalTime = 0;
                ++numBlocked;
                switched = true;
            } else if (currentIntervalTime == TIME_SLICE) { //time slice occurs
                currentProcess->remaining_time -= TIME_SLICE;
                currentProcess->time_blocked -= TIME_SLICE;
                cout << " " << (time - currentIntervalTime) << "\t" << currentProcess->name << "\t" << +currentIntervalTime << "\tS" << std::endl;
                currentIntervalTime = 0;
                switched = true;
            }

            //process switching
            if (switched && activeProcesses != 0) {
                count = 0;
                getNextIndex(currentIndex, numProcesses);
                currentProcess = &processes.at(currentIndex);
                //Search through all processes to find one that is not blocked, is past it's arrival time, and has not terminated
                for (int i = 0; i < processes.size(); ++i) {
                    if (currentProcess->termination_time == -1) { //process hasn't finished
                        if (!currentProcess->is_blocked) { //process isn't blocked
                            if (currentProcess->arrival_time < time) { //process has arrived
                                break; //successfully found a runnable process
                            }
                        }
                    }
                    getNextIndex(currentIndex, numProcesses);
                    currentProcess = &processes.at(currentIndex);
                }

                //new process became available
                if (currentProcess->remaining_time == currentProcess->total_time && currentProcess->arrival_time < time) {
                    ++activeProcesses;
                }
                switched = false;
            }
        }
        if (activeProcesses == 0) { //all processes have terminated
            complete = true;
            for (int i = 0; i < processes.size(); ++i) {//confirm no available processes
                if (processes.at(i).termination_time == -1) {
                    currentProcess = &processes.at(i);
                    complete = false;
                }
            }
            if (complete) {
                cout << " " << +time << "\t<done>\t" << AverageTurnaroundTime(processes) << std::endl;
            }
        }
        if (wasIdle) {
            wasIdle = false;
        }
        ++currentIntervalTime;
        ++time;
    }
}

void Scheduler::getNextIndex(int& currentIndex, int& numProcesses) {
    if (currentIndex + 1 == numProcesses) {
        currentIndex = 0;
    } else {
        ++currentIndex;
    }
}

/****
 * TODO:
 * - Implement shortest process next scheduling algorithm
 *****/
void Scheduler::ShortestProcessNext(std::vector<Scheduler::Process> processes) {
    //Documentation for std::priority_queue:
    //http://en.cppreference.com/w/cpp/container/priority_queue
    std::priority_queue<Process> blocked_list; //maintains the blocked process list
    std::priority_queue<Process> ready_list; //maintains the ready list for SPN;
    cout << "SPN " << BLOCK_DURATION << " " << TIME_SLICE << "\n";
    int time = 0;
    int processesRemaining = processes.size();
    bool done = false;

    /* Outer loop verifying that we have finished every process */
    while (processesRemaining != 0) {
        int timeLeftInSlice = TIME_SLICE;
        while (timeLeftInSlice > 0) {
            /* Add all processes that have arrived which are not completed or blocked */
            for (Process p : processes) {
                if (p.arrival_time <= time && p.remaining_time > 0 && !p.is_blocked) {
                    ready_list.push(p);
                }
            }

            /* If we are idle */
            if (ready_list.empty()) {
                timeLeftInSlice--;
                time++;
            } else if(!ready_list.empty()){
                cout << " " << time << "\t" << "<idle>" << "\t" << 
                        timeLeftInSlice << "\t" << "I" << endl;
            }
            /* Update our blocked_list */
            if (!blocked_list.empty()) {
                /* Move all processes to ready list which are no longer blocked */
                while (blocked_list.top().time_blocked + BLOCK_DURATION >= time) {
                    ready_list.push(blocked_list.top());
                }
            }

            while (!ready_list.empty() && timeLeftInSlice > 0) {
                Process p = ready_list.top();
                ready_list.pop();

                /* We may be able to complete process before time slice is over */
                if (p.remaining_time <= timeLeftInSlice) {

                    /* If we can complete the process before it gets blocked */
                    if (p.remaining_time <= p.time_until_blocked) {
                        cout << " " << time << "\t" << p.name << "\t" << 
                                timeLeftInSlice << "\t" << "T" << endl;
                        /* Update time */
                        time += p.remaining_time;
                        timeLeftInSlice -= p.remaining_time;
                        /* Update process */
                        p.remaining_time = 0;
                        p.termination_time = time;
                        processesRemaining--;

                        /* If there are more ready processes */
                        if (ready_list.size() != 0) {
                            p = ready_list.top();
                            ready_list.pop();
                        }
                    }/* If we cannot complete our process before blocking */
                    else if (p.remaining_time > p.time_until_blocked) {

                        /* If we will block before the current time slice ends */
                        if (p.time_until_blocked <= timeLeftInSlice) {
                            cout << " " << time << "\t" << p.name << "\t" << 
                                timeLeftInSlice << "\t" << "B" << endl;                            
                            p.remaining_time -= p.time_until_blocked;
                            timeLeftInSlice -= p.time_until_blocked;
                            time += p.time_until_blocked;
                            p.time_until_blocked = p.block_interval;
                            p.time_blocked = time;
                            p.is_blocked = true;
                            blocked_list.push(p);
                            
                            if (ready_list.size() != 0) {
                                p = ready_list.top();
                                ready_list.pop();
                            }
                        }

                        /* If we won't block before the current time slice ends */
                        if (p.time_until_blocked > timeLeftInSlice) {
                            cout << " " << time << "\t" << p.name << "\t" << 
                                    timeLeftInSlice << "\t" << "S" << endl;
                            time += timeLeftInSlice;
                            p.remaining_time -= timeLeftInSlice;
                            p.time_until_blocked -= timeLeftInSlice;
                            /* Put the process back on the ready list since it isn't blocking */
                            ready_list.push(p);
                            timeLeftInSlice = 0;
                        }
                    }
                    /* If we won't complete our process before time slice ends */
                } else if (p.remaining_time > timeLeftInSlice) {

                    /* If we will block before finishing time slice */
                    if (p.time_until_blocked <= timeLeftInSlice) {
                        cout << " " << time << "\t" << p.name << "\t" << 
                                timeLeftInSlice << "\t" << "B" << endl;
                        p.remaining_time -= p.time_until_blocked;
                        timeLeftInSlice -= p.time_until_blocked;
                        time += p.time_until_blocked;
                        p.time_until_blocked = p.block_interval;
                        p.time_blocked = time;
                        p.is_blocked = true;
                        blocked_list.push(p);                       
                        
                        if(ready_list.size() != 0) {
                            p = ready_list.top();
                            ready_list.pop();
                        }
                        /* If we won't block before we run out of time in this slice */
                    } else if (p.time_until_blocked > timeLeftInSlice) {
                        cout << " " << time << "\t" << p.name << "\t" << 
                                timeLeftInSlice << "\t" << "S" << endl;
                        time += timeLeftInSlice;
                        p.remaining_time -= timeLeftInSlice;
                        p.time_until_blocked -= timeLeftInSlice;
                        ready_list.push(p);
                        timeLeftInSlice = 0;
                    }
                }
            }
        }
        timeLeftInSlice = TIME_SLICE;
        //cout << time << "\t" << p.name << "\t" << TIME_SLICE << endl;
    }
}

/**
 * Computes average turn around time of processes 
 * Resets variables that have been changed by scheduling algorithms
 * 
 * @param processes
 * @return 
 */
float Scheduler::AverageTurnaroundTime(std::vector<Scheduler::Process> processes) {
    float sum = 0;
    for (int i = 0; i < processes.size(); ++i) {
        sum += (processes.at(i).termination_time - processes.at(i).arrival_time);
        //reset process data
        processes.at(i).remaining_time = processes.at(i).total_time;
        processes.at(i).termination_time = -1;
        processes.at(i).is_blocked = false;
        processes.at(i).time_blocked = processes.at(i).block_interval;
    }
    return sum / static_cast<float> (processes.size());
}
