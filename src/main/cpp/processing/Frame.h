#pragma once

#include <opencv2/videoio/videoio.hpp>
#include <mutex>
#include <thread>

struct Frame{
    cv::Mat mat;
    std::mutex mutex;
    std::thread::id lastFetcher;
};