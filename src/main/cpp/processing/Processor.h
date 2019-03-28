#pragma once

#include <iostream>
#include "../networking/Server.h"
#include "CameraFetcher.h"

class Processor {
public:
    Processor();
    ~Processor();
    int processCargo();
private:
    cv::VideoCapture * cap;
    CameraFetcher * _cameraFetcher;
    Frame * _frame;
    inline std::string getGstreamerPipe(int fps, int width, int height);
};

inline int Processor::processCargo() {
    this->_cameraFetcher->refresh_frame(&this->_frame);
    if(this->_frame->mat.empty() || this->_frame->lastFetcher == std::this_thread::get_id()){
        return 0;
    }
    // TODO implement processing code here
    return  1;
}