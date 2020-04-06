#pragma once
#include "Camera.h"
using namespace std;
using namespace cv;
struct Pnt3D
{
    Point3f pt;
    int seen = 0; // how many cameras have seen this point
};
class PoseEstimator
{
    public:
        PoseEstimator(string foldername):mFolderName(foldername){}
        vector<Camera> mCameraPose;
        vector<Pnt3D> mPointCloud;
        string mFolderName;
        vector<string> mImageNames;
        size_t mNumCameraPoses;
        bool fPopulateCameras();
        bool fCalculateMatchFeaturePoints();
        void fShowMatches();
        bool fCalculateCameraPoses();
        /* bool fTriangulateFeaturePoints();
        bool fBundleAdjust(); */
};