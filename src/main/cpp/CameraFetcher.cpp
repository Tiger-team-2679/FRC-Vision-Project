#include "CameraFetcher.h"

CameraFetcher::CameraFetcher(cv::VideoCapture * cap) {
    this->_cap = cap;
    this->_isRunning = false;
}
void CameraFetcher::start() {
    this->_isRunning = true;
    this->_thread = std::thread(&CameraFetcher::on_update, this);
}

void CameraFetcher::stop() {
    this->_isRunning = false;
    if(this->_thread.joinable()){
        this->_thread.join();
    }
}

void CameraFetcher::on_update() {
    Frame * frame;
    while(this->_isRunning){
        refresh_frame(&frame);
        _cap->read(frame->mat);
    }
}

void CameraFetcher::refresh_frame(Frame ** lastFrame) {
    for (auto &_set : _sets) {
        if(*lastFrame == &_set) {
            (*lastFrame)->lastFetcher = std::this_thread::get_id();
            _set.mutex.unlock();
            break;
        }
    }
    for (auto &_set : _sets) {
        if(*lastFrame != &_set) {
            if(_set.mutex.try_lock()){
                *lastFrame = &_set;
                break;
            }
        }
    }
}

