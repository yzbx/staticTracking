#ifndef MOVINGLINEDETECTOR_H
#define MOVINGLINEDETECTOR_H
#include <opencv2/opencv.hpp>
#include <QtCore>
#include <iostream>
#include <queue>

#include "../lib/yzbx_cdnetbenchmark.h"
#include "../lib/yzbx_frameinput.h"
#include "../lib/yzbx_utility.h"

using namespace std;
using namespace cv;

class movingLineDetector
{
public:
    movingLineDetector();
    void movingPointDetector(const Mat &inputImg);
    void point2line();
    void process(const Mat &inputImg);
    void overturn();
    void getExtreamImage();

    QString configFilePath="/home/yzbx/config/staticTracking.ini";
    int frameNum=0;
    //corner response img
    Mat img1,img2,img3;
    //extream corner response img
    Mat img_extream1,img_extream2;
    float noiseThreshold=0;
};

#endif // MOVINGLINEDETECTOR_H
