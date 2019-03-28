#include <iostream>
#include "processing/ProcessingThread.h"

#define PROCESS_ARGUMENT "-p"

void procMon(int argc, char *argv[]) {
    std::string program_name = argv[0]; // get exe
    std::string command = program_name + " " + PROCESS_ARGUMENT;
    system(command.c_str());
    // TODO expand abilities, check internet connection and if all processes are running
}

int main (int argc, char *argv[]) {
    std::string arg;
    if(argc > 1) {
        arg = argv[1]; // get arguments
    }
    if (arg.empty()) { // run the proc mon
        std::cout << "Info: starting the proc-mon" << std::endl;
        procMon(argc, argv);
    }
    else if(arg == PROCESS_ARGUMENT){ // run the processing code
        std::cout << "Info: starting the processing" << std::endl;
        ProcessingThread processingThread;
        processingThread.start();

    } else{ // illegal value should quit
        std::cerr << "Fatal Error :an invalid argument was given, aborting.";
    }
}