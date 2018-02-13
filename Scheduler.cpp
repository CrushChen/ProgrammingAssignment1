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
    Execute(ParseFile(file_name_));
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
        temp.began_blocking = -1;
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

void Scheduler::Execute(std::vector<Scheduler::Process> processes) {
    RoundRobin(processes);

    /* Reset the member variables for the processes
     * after round robin algorithm has completed
     */
    for (int i = 0; i < processes.size(); ++i) {
        processes.at(i).remaining_time = processes.at(i).total_time;
        processes.at(i).termination_time = -1;
    }
    ShortestProcessNext(processes);
}

/****
 * TODO:
 * - Implement round robin scheduling algorithm
 *****/
void Scheduler::RoundRobin(std::vector<Scheduler::Process> processes) {
    cout << "RR " << BLOCK_DURATION << " " << TIME_SLICE;
   

    int time = 0;
    int i = 0;


    std::vector<int> blocktimeremain(processes.size());
    for (int x = 0; x < processes.size(); ++x) {

        blocktimeremain.at(i) = 0;
    }
   while (blocktimeremain.size() != 0) {

        
        while (processes.at(i).remaining_time = 0) {
            blocktimeremain.erase(i);
            i = i + 1;
            if(i>blocktimeremain.size())
            {
                i=i-blocktimeremain.size();
            }
                
        }


        if (checkisallblocked(blocktimeremain) == true) {
            int timesliceforblock = std::min_element(blocktimeremain.begin(), blocktimeremain.end());
            int i = findindexofelement(blocktimeremain, timesliceforblock);
            time = time + timesliceforblock;
            cout << time + " " + "<idle>" + " " + timesliceforblock + " " + I + '\n';
            processes.at(i).remaining_time = processes.at(i).remaining_time - timesliceforblock;
            processes.at(i).termination_time = time;
            blocktimeremain.at(i) = block_duration;
            for (int x = 0; x < processes.size(); ++x) {
                if (blocktimeremain.at(i) != 0) {
                    blocktimeremain.at(i) = blocktimeremain.at(i) - timesliceforblock;
                    if (blocktimeremain.at(i) < 0) {
                        blocktimeremain.at(i) = 0;
                    }

                }
            }
        } else {
            while (blocktimeremain[i] != 0) {
                i = i + 1;
                if(i>blocktimeremain.size())
            {
                i=i-blocktimeremain.size();
            }
            }
            if ((((processes.at(i).total_time)-(processes.at(i).remaining_time)) + TIME_SLICE) % (processes.at(i).block_interval) == 0) {
                time = time + TIME_SLICE;
                cout << time + " " + processes.at(i).name + " " + TIME_SLICE + " " + B + '\n';
                processes.at(i).remaining_time = processes.at(i).remaining_time - TIME_SLICE;
                processes.at(i).termination_time = time;
                blocktimeremain.at(i) = block_duration;
                for (int x = 0; x < processes.size(); ++x) {
                    if (blocktimeremain.at(i) != 0) {
                        blocktimeremain.at(i) = blocktimeremain.at(i) - TIME_SLICE;
                        if (blocktimeremain.at(i) < 0) {
                            blocktimeremain.at(i) = 0;
                        }

                    }
                }
            } else {
                if (((((processes.at(i).total_time)-(processes.at(i).remaining_time)) + TIME_SLICE) % (processes.at(i).block_interval)) > time_slice) {
                    time = time + TIME_SLICE;
                    cout << time + " " + processes.at(i).name + " " + TIME_SLICE + " " + S + '\n';
                    processes.at(i).remaining_time = processes.at(i).remaining_time - TIME_SLICE;
                    processes.at(i).termination_time = time;
                    for (int x = 0; x < processes.size(); ++x) {
                        if (blocktimeremain.at(i) != 0) {
                            blocktimeremain.at(i) = blocktimeremain.at(i) - TIME_SLICE;
                            if (blocktimeremain.at(i) < 0) {
                                blocktimeremain.at(i) = 0;
                            }
                        }

                    }

                } else {
                    int quotient = ((((processes.at(i).total_time)-(processes.at(i).remaining_time)) + TIME_SLICE) / (processes.at(i).block_interval)) + 1;
                    int newtimeslice = (quotient * (processes.at(i).block_interval)) - ((processes.at(i).total_time)-(processes.at(i).remaining_time))
                            cout << time + " " + processes.at(i).name + " " + newtimeslice + " " + B + '\n';
                    processes.at(i).remaining_time = processes.at(i).remaining_time - newtimeslice;
                    time = time + newtimeslice;

                    processes.at(i).termination_time = time;
                    for (int x = 0; x < processes.size(); ++x) {
                        if (blocktimeremain.at(i) != 0) {
                            blocktimeremain.at(i) = blocktimeremain.at(i) - newtimeslice;
                            if (blocktimeremain.at(i) < 0) {
                                blocktimeremain.at(i) = 0;
                            }

                        }
                    }
                    blocktimeremain.at(i) = block_duration;

                }
            }
        }
        i = i + 1;
       if(i>blocktimeremain.size())
            {
                i=i-blocktimeremain.size();
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
    
    std::priority_queue<Process> blocked_list;//maintains the blocked process list
    std::priority_queue<Process> ready_list; 
    //Sorts Process(es) based on remaining_time or block_interval time, whichever is shortest
    
    cout << "SPN " << BLOCK_DURATION << " " << TIME_SLICE << "\n";
    int time = 0;
    int numRemainingProcesses = processes.size();
    bool done = false;
    
    for(Process p : processes){// at time zero, get all processes that arrive
        if(p.arrival_time == 0){
            ready_list.push(p);
        }
    }
    cout << ready_list.size() << endl;
    /* Outer loop verifying that we have finished every process */
    while(numRemainingProcesses > 0 && time < 5000){ //TIME < 5000 BECAUSE CODE IS BROKEN. TO AVOID EXCESSIVE LOOPAGE
        for(Process p : processes){
            if(p.arrival_time <= time && p.remaining_time > 0){
                ready_list.push(p);
            }
        }
        
        if(ready_list.empty() && !blocked_list.empty()){
            Process p = blocked_list.top();
            if(p.began_blocking >= time+BLOCK_DURATION){
                p.began_blocking = -1;
                ready_list.push(p);
                blocked_list.pop();
            }
            cout << "UPDATING BLOCKED LIST\n";
            time += BLOCK_DURATION;
            cout << "LOOP RESTART" << endl;
            continue; //restart the loop
        }
        
        Process p = ready_list.top();
        ready_list.pop();
        /* If we have no processes ready but all are blocking */

        /* If our process will finish before blocking */
        printf("name: %s, arrival_time: %i, total_time: %i, remaining_time: %i, "
                "block_interval: %i,\ntermination_time: %i, began_blocking: %i, "
                "Current Time: %i\n",
                p.name, p.arrival_time, p.total_time, p.remaining_time, p.block_interval,
                p.termination_time, p.began_blocking, time);
        if (p.remaining_time - TIME_SLICE < p.block_interval) {
            cout << " " << time << "\t" << p.name << "\t" << 
                    TIME_SLICE - p.remaining_time << "T" << endl;
            p.remaining_time = 0;
            p.termination_time = time;
            numRemainingProcesses--;
            time += TIME_SLICE;
            cout << "PROCESS_FINISH_BEFORE_BLOCK\n";
            
        } 
        /* If our process will block before finishing */
        if (p.remaining_time - TIME_SLICE > p.block_interval) {
            p.remaining_time -= TIME_SLICE;
            p.began_blocking = time;
            time += TIME_SLICE;
            blocked_list.push(p);
            cout << "PROCESS_BLOCK_BEFORE_FINISHING\n";
        /* IDLING (nothing ready and nothing is blocked, then we are idle. */
        } else if (ready_list.empty() && blocked_list.empty()) {
            cout << " " << time << "\t" << "<idle>" << "\t" << "" << "I" << endl;
            time += TIME_SLICE;
            cout << "IDLING\n";            
        /* BLOCKING (some proceses are ready, but are being blocked) */
        } else if (!ready_list.empty() && !blocked_list.empty()){
            cout << " " << time << "\t" << p.name << "\t" << "" << "B" << endl;
            time += BLOCK_DURATION;
            cout << "BLOCKING\n";            
        }

    }
    cout << " " << time << "\t" << "<done>" << AverageTurnaroundTime(processes);
}

bool Scheduler::checkifallblocked(std::vector<int> a) {
                    for (int i = 0; i < a.size(); ++i) {
                        if (a.at(i) == 0) {
                            return false;
                        }
                    }
                    return true;

                }

                int Scheduler::findindexofelement(std::vector<int> a, int b) {
                    for (int i = 0; i < a.size(); ++i) {
                        if (a.at(i) == b) {
                            return i;
                        }
                    }
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
