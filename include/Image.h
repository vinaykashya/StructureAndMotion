#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <cassert>

using namespace cv;
using namespace std;

class Image
{
    public:
        Image(){}
        Image(string _imagePath):mImagePath(_imagePath)
        {
            mImage = imread(mImagePath);
            if(! mImage.data )
            {
                cout <<  "Could not open or find the image" << std::endl ;
                return;
            }
            if(mImage.channels() > 1)
                cvtColor(mImage,mImage, COLOR_BGR2GRAY);
            resize(mImage, mImage, mImage.size()/4);
            mWidth = mImage.cols;
            mHeight = mImage.rows;
        }
        bool fReadImage(string _imagePath)
        {
            mImage = imread(_imagePath);
            if(! mImage.data )
            {
                cout <<  "Could not open or find the image" << std::endl ;
                return -1;
            }
            if(mImage.channels() > 1)
                cvtColor(mImage,mImage, COLOR_BGR2GRAY);
            resize(mImage, mImage, mImage.size()/4);
            mWidth = mImage.cols;
            mHeight = mImage.rows;
            return 1;
        }
        
        Mat mImage;
        int mWidth = 0, mHeight = 0;
        string mImagePath;
};