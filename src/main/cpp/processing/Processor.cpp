#include "Processor.h"

#define FPS 30
#define HEIGHT 640
#define WIDTH 480

#define CARGO_H_LOW 0
#define CARGO_S_LOW 0
#define CARGO_V_LOW 0
#define CARGO_H_HIGH 70
#define CARGO_S_HIGH 255
#define CARGO_V_HIGH 255

Processor::Processor() {
    this->cap = new cv::VideoCapture(0); //getGstreamerPipe(FPS, WIDTH, HEIGHT));
    if(!cap->isOpened()) {
        std::cerr << "Gstreamer Error: Can't create gstreamer reader." << std::endl;
        return;
    }
    this->_cameraFetcher = new CameraFetcher(this->cap);
    _cameraFetcher->start();
}

Processor::~Processor() {
    this->_cameraFetcher->stop();
    this->cap->release();
    delete this->_cameraFetcher;
}

std::string Processor::getGstreamerPipe(int fps, int width, int height) {
    return "nvcamerasrc fpsRange='" + std::to_string(fps) + " " + std::to_string(fps) + "' ! video/x-raw(memory:NVMM)" \
            ", width=(int)" + std::to_string(width) + \
            ", height=(int)" + std::to_string(height) + ",format=(string)I420, framerate=(fraction)FPS/1 !" \
            " nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw," \
            " format=(string)BGR ! appsink ";
}