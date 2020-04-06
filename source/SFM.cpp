#include "PoseEstimator.h"
using namespace std;
using namespace cv;
int main()
{
    PoseEstimator sfmTest("/home/awm/Documents/SFM/Input_Images");
    sfmTest.fPopulateCameras();
    cout<<"cameras populated"<<endl;
    sfmTest.fCalculateMatchFeaturePoints();
    sfmTest.fShowMatches();
    return 0;
}