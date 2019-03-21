#pragma once

#include <mutex>
#include <thread>
#include <opencv2/videoio/videoio.hpp>

#define BUFFER_SIZE 3

struct Frame{
    cv::Mat mat;
    std::mutex mutex;
    std::thread::id lastFetcher;
};

class CameraFetcher {
public:
    explicit CameraFetcher(cv::VideoCapture * cap);
    void on_update();
    void start();
    void stop();
    void refresh_frame(Frame ** lastFrame);
private:
    cv::VideoCapture * _cap;
    std::thread _thread;

    Frame _sets[BUFFER_SIZE];

    bool _isRunning;
};