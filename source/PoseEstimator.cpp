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
        //namedWindow("Test", 0);
        /* imshow("Test", mCameraPose[i].mImg.mImage);
        waitKey(); */
        //vector<KeyPoint> kp;
        //Mat desc;
        feature->detectAndCompute(mCameraPose[i].mImg.mImage,noArray(), mCameraPose[i].mKPMatches.mKeyPoints, mCameraPose[i].mKPMatches.mDescriptors);
        cout<<"features for image calculated before copying:"<<i<<endl;
        //mCameraPose[i].mKPMatches.mKeyPoints = kp;
        //mCameraPose[i].mKPMatches.mDescriptors = desc;
        //feature->compute(mCameraPose[i].mKPMatches.mKeyPoints, mCameraPose[i].mKPMatches.mDescriptors);
    }
    cout<<"features calculated:"<<endl;
    namedWindow("img", WINDOW_NORMAL);
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
                if(m[0].distance < 0.7*m[1].distance)
                {
                    src.push_back(mCameraPose[i].mKPMatches.mKeyPoints[m[0].queryIdx].pt);
                    dst.push_back(mCameraPose[j].mKPMatches.mKeyPoints[m[0].trainIdx].pt);
                    i_kp.push_back(m[0].queryIdx);
                    j_kp.push_back(m[0].trainIdx);

                }
            }
            findFundamentalMat(src,dst,FM_RANSAC, 3.0, 0.99, mask);
            /* Mat canvas = mCameraPose[i].mImg.mImage.clone();
            canvas.push_back(mCameraPose[j].mImg.mImage.clone()); */

            for(size_t k = 0; k < mask.size(); k++)
            {
                if(mask[k])
                {
                    mCameraPose[i].mKPMatches.mKeyPointMatches[i_kp[k]][j] = j_kp[k];
                    mCameraPose[j].mKPMatches.mKeyPointMatches[j_kp[k]][i] = i_kp[k];
                    //KeyPoint a, b;
                    
                    //src_inlier.push_back(mCameraPose[i].mKPMatches.mKeyPoints[i_kp[k]]);
                    //dst_inlier.push_back(mCameraPose[j].mKPMatches.mKeyPoints[j_kp[k]]);
                    //int new_i = static_cast<int>(src_inlier.size());
                    //good_matches.push_back(DMatch(new_i, new_i, 0));
                    /* line(canvas, src[k], dst[k] + Point2f(0, mCameraPose[i].mImg.mHeight), Scalar(0, 0, 255), 2);
                    resize(canvas, canvas, canvas.size()/2);
                    imshow("img", canvas);
                    waitKey(); */
                }
            }
            //Mat res;
            //drawMatches(mCameraPose[i].mImg.mImage, src_inlier, mCameraPose[j].mImg.mImage, dst_inlier, good_matches, res);
            //imshow("img", res);
            //waitKey();
        }
    }
    cout<<"features matches calculated:"<<endl;

}