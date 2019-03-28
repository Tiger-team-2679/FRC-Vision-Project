#pragma once

#include <iostream>
#include "thread"
#include "CameraFetcher.h"
#include "Command.h"
#include "Processor.h"

#define SERVER_PORT 2679

class ProcessingThread {
public:
    explicit ProcessingThread();
    ~ProcessingThread();
    void start();
private:
    void processingHandler();
    void updateCommand(Command & command);
    void startThread();
    void stopThread();
    bool _running;
    Command _command;
    std::thread _thread;
    Processor _processor;
};