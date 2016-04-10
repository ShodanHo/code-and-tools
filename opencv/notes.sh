dnf install cmake python-devel numpy gcc gcc-c++ \
    gtk2-devel libdc1394-devel libv4l-devel ffmpeg-devel \
    gstreamer-plugins-base-devel libpng-devel \
    libjpeg-turbo-devel jasper-devel openexr-devel \
    libtiff-devel libwebp-devel

dnf install tbb-devel eigen3-devel python-sphinx texlive


git clone https://github.com/Itseez/opencv.git
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
-D BUILD_EXAMPLES=ON
-D PLANTUML_JAR=ON
