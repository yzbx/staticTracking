#ifndef YZBX_UTILITY_H
#define YZBX_UTILITY_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include "lib/lbp/lbp.hpp"
#include "lib/lbp/histogram.hpp"
//#include "lib/LBP1/LBP1.hpp"
#include "yzbx_config.h"
#include <QString>
#include <QStringList>
#include <fstream>
#include <QDebug>
#include <QSettings>
//set qt sync with file.

#define yzbx_match_BF 0
#define yzbx_match_KNN 1

using namespace std;
using namespace cv;
/*
    version: 1.0
    author: hellogiser
    blog: http://www.cnblogs.com/hellogiser
    date: 2014/5/30
*/
// hamming distance of two integer 0-1 bits
unsigned yzbx_hamdist(unsigned x, unsigned y);
unsigned yzbx_d1(unsigned x,unsigned y);
unsigned yzbx_d2(unsigned x,unsigned y);

//distance_type 0 hamdist, 1 d1, 2 d2,...
unsigned yzbx_distance(unsigned x,unsigned y,int distance_type=2);
//color_space BGR 0,Lab 1, ab 2,L*ab 3,LBP 4,5,6, ...
unsigned yzbx_distance_Vec3b(Vec3b x,Vec3b y,int distance_type=2,int color_space=0);
void showImgInLoop(const cv::Mat img,int i,string title="");
void cvt2CV_8U(const cv::Mat input,cv::Mat &output);
void showMat8U(const string windowName, const cv::Mat input);

//bool separatedMatCheck(const Mat &a,const Mat& b);
//bool separatedMatsCheck(vector<Mat> &va, vector<Mat> &vb);

void yzbx_imfill(Mat &input);

void yzbx_lbp(const Mat &input, Mat &output, int lbp_operator=1);

void yzbx_match(Mat &descriptor1, Mat &descriptor2, vector<DMatch> &matches, int matchType=yzbx_match_BF);

void icvprCcaByTwoPass(const Mat &_binImg, Mat &_lableImg);
void compressMat(Mat &mCV32SC1, Mat &mCV8UC1);
Scalar icvprGetRandomColor();
void icvprLabelColor(const Mat &_labelImg, Mat &_colorLabelImg);
void connectedComponentSplit(Mat &nextFGMask, Mat &labelImg8UC1);

string getImgType(int imgTypeInt);
void CDNet_GetRoiNum(QString filename, int &a, int &b);

size_t getDirections(float dx,float dy);
void filterBinaryImageByArea(Mat &binImage,double minarea);
void yzbx_generateErrorMap(const Mat& truthGround,const Mat & FGMask,Mat &errorMat);

//double threshold connect for binary image.
void doubleThresholdConnect(const Mat &highThresholdMask,const Mat &lowThresholdMask,Mat &outputMask);

#define _DEBUG 1

#if _DEBUG
//#define LOG_MESSAGE(x) std::cout << __FILE__ <<__FUNCTION__<< " (" << __LINE__ << "): " << x << std::endl;
#define LOG_MESSAGE(x) std::cout <<__FUNCTION__<< " (" << __LINE__ << "): " << x << std::endl;
#else
#define LOG_MESSAGE(x)
#endif
#endif // UTILITY_HPP
