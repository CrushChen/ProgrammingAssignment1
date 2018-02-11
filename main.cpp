/* 
 * File:   main.cpp
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:23 PM
 */

/*
 * The program should accept three arguments:
 * 1) input file name
 * 2) block_duration: the decimal integer time length that a process
 *                    is unavailable to fun after it blocks
 * 3) time_slice: the decimal integer of the time slice for the 
 *                Round-Robin scheduler
 * -Arguments are passed in the order shown above
 * 
 */

#include "Scheduler.h"

#include <cstdlib>
#include <iostream>


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "usage: Assignment1 input_file\n";
        exit(1);
    }
    
    /* TODO:
     *  -pass file name and both scheduler parameters to Scheduler class
     */

    return 0;
}

