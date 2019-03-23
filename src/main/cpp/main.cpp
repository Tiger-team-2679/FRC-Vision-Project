#include <iostream>
#include <string>
#include <vector>
#include "networking/Server.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "CameraFetcher.h"

#define FPS 30
#define HEIGHT 640
#define WIDTH 480

#define CARGO_H_LOW 0
#define CARGO_S_LOW 0
#define CARGO_V_LOW 0
#define CARGO_H_HIGH 70
#define CARGO_S_HIGH 255
#define CARGO_V_HIGH 255

std::string getGstreamerPipe(int fps, int width, int height){
    return "nvcamerasrc fpsRange='" + std::to_string(fps) + " " + std::to_string(fps) + "' ! video/x-raw(memory:NVMM)" \
            ", width=(int)" + std::to_string(width) + \
            ", height=(int)" + std::to_string(height) + ",format=(string)I420, framerate=(fraction)FPS/1 !" \
            " nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw," \
            " format=(string)BGR ! appsink ";
}

void filterCargo(cv::Mat * frame){
    cv::cvtColor(*frame, *frame, cv::COLOR_BGR2HSV,0);
    cv::inRange(*frame,cv::Scalar(CARGO_H_LOW,CARGO_S_LOW,CARGO_V_LOW),
                cv::Scalar(CARGO_H_HIGH,CARGO_S_HIGH,CARGO_V_HIGH), *frame);
}

std::vector<cv::Point> * getCargoContour(){
    return nullptr;
}

int main() {
    std::string pipe_args = getGstreamerPipe(FPS, HEIGHT, WIDTH);
    cv::VideoCapture cap(0);

    if(!cap.isOpened()) {
        std::cerr << "[ERROR] Can't create gstreamer reader." << std::endl;
        return -1;
    }

    CameraFetcher cameraFetcher(&cap);
    cameraFetcher.start();

    Frame * frame;

    clock_t time0;
    clock_t time1;

    while (true) {
        time0 = clock();
        cameraFetcher.refresh_frame(&frame);
        if (!frame->mat.empty() && frame->lastFetcher != std::this_thread::get_id()) {
            filterCargo(&frame->mat);
        }
        time1 = clock();
        printf("inRange time is: %d\n", time1-time0); // for debugging
        if(!frame->mat.empty() && frame->lastFetcher != std::this_thread::get_id()){
            cv::imshow("filter", frame->mat);
        }
        int c = cv::waitKey(10);
        if(c == 27){
            break;
        }
    }
    cameraFetcher.stop();
    cap.release();
}

