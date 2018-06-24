if [ -d "opencv/build" ];
then
    # We're using a cached version of our OpenCV build
    echo "Cache found - using that."
    cd opencv
    git init
    cd build
    make
    sudo make install
else
    # No OpenCV cache – clone and make the files
    echo "No cache found - cloning and making files."
    rm -r opencv
    git clone https://github.com/opencv/opencv.git
    cd opencv
    git fetch origin --tags
    git checkout 3.1.0
    mkdir build
    cd build
    cmake ..
    make
	sudo make install
fi
