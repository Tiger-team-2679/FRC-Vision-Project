#include "ProcessingThread.h"

ProcessingThread::ProcessingThread() {
    this->_running = false;
}

ProcessingThread::~ProcessingThread() {
    this->stopThread();
}

void ProcessingThread::start(){
    Server server(SERVER_PORT);
    server.start();
    while (true) {
        Command command = server.recvMessage();
        if(command.is_valid) {
            this->updateCommand(command);
        }
    }
}

void ProcessingThread::startThread() {
    this->_running = true;
    this->_thread = std::thread(&ProcessingThread::processingHandler, this);
}

void ProcessingThread::stopThread() {
    this->_running = false;
    if(this->_thread.joinable()){
        this->_thread.join();
    }
}

void ProcessingThread::processingHandler() {
    while(this->_running){
        if(_command.method == INFINITE_RETURN_METHOD){
            if(_command.target == CARGO_TARGET_CODE){
                this->_processor.processCargo();
            }
            // TODO send data
        }
        else if(_command.method == SINGLE_RETURN_METHOD){
            if(_command.target == CARGO_TARGET_CODE){
                while(!this->_processor.processCargo()){};
            }
            // TODO send data
            this->_running= false;
            break;
        } else{
            std::cerr << "Processing Error: invalid process method, doing nothing." << std::endl;
            this->_running = false;
            break;
        }
    }
}

void ProcessingThread::updateCommand(Command & command) {
    if(command.is_valid) {
        this->_command = command;
        if (!this->_running) {
            this->stopThread();
            this->startThread();
        }
    }
}
