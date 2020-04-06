#include "PoseEstimator.h"
#include "dirent.h"
using namespace std;
using namespace cv;
bool PoseEstimator::fPopulateCameras()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (mFolderName.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL ) 
        {
            if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;
            mImageNames.push_back(mFolderName+"/"+ ent->d_name);
        }
        closedir (dir);
    } 
    else
    {
        cout<<"could not open directory"<<endl;;
        return 0;
    }
    mNumCameraPoses = mImageNames.size();
    mCameraPose.resize(mNumCameraPoses);
    for(int i = 0 ; i < mImageNames.size(); i++)
    {
        cout<<"File:"<<mImageNames[i]<<endl;
        bool sts = mCameraPose[i].mImg.fReadImage(mImageNames[i]);
        if(!sts)
        {
            cout<<"Cannot read Images"<<endl;;
            return 0;
        }
        else
        {
            mCameraPose[i].mCameraID = i;
        }
        //mCameraPose[i].mCalibrationMatrix ;        
    }
    
    return 1;
}

bool PoseEstimator::fCalculateMatchFeaturePoints()
{
    Ptr<AKAZE> feature = AKAZE::create();
    BFMatcher matcher(NORM_HAMMING);
    for(int i = 0; i < mNumCameraPoses; i++)
    {
        feature->detectAndCompute(mCameraPose[i].mImg.mImage,noArray(), mCameraPose[i].mKPMatches.mKeyPoints, mCameraPose[i].mKPMatches.mDescriptors);
    }
    cout<<"features calculated:"<<endl;
    for(int i = 0; i < mNumCameraPoses-1; i++)
    {
        for(int j = i+1; j < mNumCameraPoses; j++)
        {
            vector<vector<DMatch>> matches;
            vector<DMatch> good_matches;
            vector<Point2f> src, dst;
            vector<KeyPoint> src_inlier, dst_inlier;
            vector<uchar> mask;
            vector<int> i_kp, j_kp;
            matcher.knnMatch(mCameraPose[i].mKPMatches.mDescriptors, mCameraPose[j].mKPMatches.mDescriptors, matches, 2);
            for(auto &m:matches)
            {
                if(m[0].distance < 0.8*m[1].distance)
                {
                    src.push_back(mCameraPose[i].mKPMatches.mKeyPoints[m[0].queryIdx].pt);
                    dst.push_back(mCameraPose[j].mKPMatches.mKeyPoints[m[0].trainIdx].pt);
                    i_kp.push_back(m[0].queryIdx);
                    j_kp.push_back(m[0].trainIdx);

                }
            }
            findFundamentalMat(src,dst,FM_RANSAC, .0, 0.99, mask);

            for(size_t k = 0; k < mask.size(); k++)
            {
                if(mask[k])
                {
                    mCameraPose[i].mKPMatches.mKeyPointMatches[i_kp[k]][j] = j_kp[k];
                    mCameraPose[j].mKPMatches.mKeyPointMatches[j_kp[k]][i] = i_kp[k];
                }
            }
        }
    }
    cout<<"features matches calculated:"<<endl;
}

void PoseEstimator::fShowMatches()
{
    namedWindow("Image Matches", WINDOW_NORMAL);
    for(int i = 0 ; i < mNumCameraPoses-1 ; i++)
    {
        for(int j = i+1; j < mNumCameraPoses; j++)
        {
            vector<KeyPoint> src, dst;
            vector<DMatch> matches;
            for(int k = 0; k < mCameraPose[i].mKPMatches.mKeyPoints.size(); k++)
            {
                if(mCameraPose[i].mKPMatches.fKeyPointMatchExists(k,j))
                {
                    matches.push_back(DMatch(k,mCameraPose[i].mKPMatches.fReturnMatchIndex(k,j), 0.0));
                }
            }
            Mat res;
            drawMatches(mCameraPose[i].mImg.mImage, mCameraPose[i].mKPMatches.mKeyPoints, mCameraPose[j].mImg.mImage, mCameraPose[j].mKPMatches.mKeyPoints,matches, res); 
            imshow("Image Matches", res);
            waitKey();
        }
    }
}

bool PoseEstimator::fCalculateCameraPoses()
{
    
}