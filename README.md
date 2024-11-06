# INSTRUCTIONS

Working on Ubuntu 24.04

Download and install [CUDA 12.6](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=WSL-Ubuntu&target_version=2.0&target_type=deb_local)

Download and install OpenCV:

```
mkdir ~/opencv && cd ~/opencv
```
```
git clone -b 4.x https://github.com/opencv/opencv.git
git clone -b 4.x https://github.com/opencv/opencv_contrib.git
```
```
cd opencv
mkdir build && cd build
```
```
cmake -D CUDA_nppicom_LIBRARY=/usr/local/cuda/lib64/libnppicom.so -D CUDA_nppist_LIBRARY=/usr/local/cuda/lib64/libnppist.so -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -DWITH_CUDA=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_opencv_cudacodec=ON -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

Install PCL:

```
sudo apt-get update
sudo apt-get install libpcl-dev
```

Install Abseil

```
cd
git clone https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
git checkout 20240722.0
```


Install Ceres:

``` 
sudo apt-get install cmake libgoogle-glog-dev libgflags-dev libeigen3-dev libsuitesparse-dev
```
```
git clone https://github.com/ceres-solver/ceres-solver.git
cd ceres-solver
mkdir build && cd build
cmake .. -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF
```


# MarkerMapper
Tracking a dodecahedron with aruco markers. (Version 2)  
It's part of project [PressPen](https://github.com/neconeconeco/PressPen).  
  
In this version, I use markermapper of aruco library to solve the least square problem.
