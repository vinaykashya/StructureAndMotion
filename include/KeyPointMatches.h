#pragma once
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <cassert>
#include <Image.h>

using namespace cv;
using namespace std;
class KeyPointMatches
{
    public:
        vector<KeyPoint> mKeyPoints;
        Mat mDescriptors;
        map<int, map<int,int>> mKeyPointMatches;
        map<int, int> mKPtoLandMarkMatches;
};