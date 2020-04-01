#pragma once
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <cassert>
#include "Image.h"
#include "KeyPointMatches.h"
using namespace cv;
using namespace std;

class Camera{
    public:
        KeyPointMatches mKPMatches;
        size_t mCameraID;
        Mat mCalibrationMatrix;
        Mat mProjectionMatrix;
        Mat mTransformationMatrix;
        Image mImg;
};