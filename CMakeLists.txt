cmake_minimum_required(VERSION 3.5)
project(vision)

find_package( OpenCV REQUIRED )
set(WITH_CUDA ON)

include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB vision_SRC
        "src/main/cpp/*.hpp"
        "src/main/cpp/*.cpp"
        "src/main/cpp/*.h"
        "src/main/cpp/**/*.hpp"
        "src/main/cpp/**/*.cpp"
        "src/main/cpp/**/*.h"
        )

add_executable(vision ${vision_SRC})

target_link_libraries( vision opencv_core opencv_highgui opencv_videoio ws2_32)