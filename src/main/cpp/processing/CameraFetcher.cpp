#include "CameraFetcher.h"

/**
 * Initializes a new camera fetcher with a given capture device
 * @param cap the capture device to fetch from
 */
CameraFetcher::CameraFetcher(cv::VideoCapture * cap) {
    this->_cap = cap;
    this->_running = false;
}

/**
 * starts the camera fetcher in a new thread
 * doesn't do nothing if the thread is already running
 */
void CameraFetcher::start() {
    if(!this->_running) {
        this->_running = true;
        this->_thread = std::thread(&CameraFetcher::on_update, this);
    }
}

/**
 * tells the thread to stop after waiting for the iteration to finish
 */
void CameraFetcher::stop() {
    this->_running = false;
    if(this->_thread.joinable()){
        this->_thread.join();
    }
}

/**
 * updates a frame in one of the sets from the capture device
 */
void CameraFetcher::on_update() {
    Frame * frame;
    while(this->_running){
        refresh_frame(&frame);
        _cap->read(frame->mat);
    }
}