#include <iostream>
#include <string>
#include <vector>
#include <condition_variable>
#include "networking/Server.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "CameraFetcher.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
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

std::thread processingThread;
char currentTarget = NULL_TARGET_CODE;
char currentMethod = NO_RETURN_METHOD;
char isRunning = false;

int process(CameraFetcher & cameraFetcher, Frame ** frame){
    cameraFetcher.refresh_frame(frame);
    if((*frame)->mat.empty() || (*frame)->lastFetcher == std::this_thread::get_id()){
        return 0;
    }
    filterCargo(&(*frame)->mat);
    return  1;
}

void processLogic(CameraFetcher & cameraFetcher, Frame ** frame){
    while(isRunning){
        if(currentMethod == INFINITE_RETURN_METHOD){
            process(cameraFetcher, frame);
            // TODO send data
        }
        else if(currentMethod == SINGLE_RETURN_METHOD){
            while(!process(cameraFetcher, frame)){};
            // TODO send data
            isRunning = false;
            break;
        } else{
            std::cerr << "Processing Error: invalid process method, doing nothing." << std::endl;
            isRunning = false;
            break;
        }
    }
}

void startProcessLogic(CameraFetcher & cameraFetcher, Frame ** frame){
    isRunning = true;
    processingThread = std::thread(processLogic, std::ref(cameraFetcher), frame);
}

void stopProcessLogic(){
    isRunning = false;
    if(processingThread.joinable()){
        processingThread.join();
    }
}

int main() {
    Server server(2679);
    server.start();
    server.waitForClient();

    std::string pipe_args = getGstreamerPipe(FPS, HEIGHT, WIDTH);
    cv::VideoCapture cap(0);

    if(!cap.isOpened()) {
        std::cerr << "Gstreamer Error: Can't create gstreamer reader." << std::endl;
        return -1;
    }

    CameraFetcher cameraFetcher(&cap);
    cameraFetcher.start();

    Frame * frame;

    while (true) {
        Command command = server.recvMessage();
        if(command.is_valid) {
            currentTarget = command.target;
            currentMethod = command.method;
            if (!isRunning) {
                stopProcessLogic();
                startProcessLogic(cameraFetcher, &frame);
            }
        }
    }

    stopProcessLogic();
    cameraFetcher.stop();
    cap.release();
}


#pragma clang diagnostic pop