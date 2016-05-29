#include "movinglinedetector.h"

movingLineDetector::movingLineDetector()
{

}

void movingLineDetector::movingPointDetector(const Mat &inputImg){
    CV_Assert(!inputImg.empty());

    Mat src;
    if(inputImg.channels()==3)
        cvtColor(inputImg,src,CV_BGR2GRAY);
    else
        src=inputImg.clone();

    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( src.size(), CV_32FC1 );
    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    /// Detecting corners
    cornerHarris( src, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

    /// Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    if(frameNum==0){
        img3=dst_norm;
    }
    else{
        if(frameNum==1){
            img2=img3;
            img3=dst_norm;
        }
        else{
            img1=img2;
            img2=img3;
            img3=dst_norm;
        }
    }

    convertScaleAbs( dst_norm, dst_norm_scaled );

    imshow("dst_norm_scaled",dst_norm_scaled);
}

void movingLineDetector::overturn(){
    namedWindow("input",WINDOW_NORMAL);
    namedWindow("point",WINDOW_NORMAL);
    namedWindow("fgMask",WINDOW_NORMAL);
    namedWindow("extream",WINDOW_NORMAL);

    //step 1: config file
    QSettings *set=new QSettings(configFilePath, QSettings::IniFormat);

    //step 2: set up path
    string inputRoot=set->value("inputRoot").toString().toStdString();
    string outputRoot=set->value("outputRoot").toString().toStdString();

    CV_Assert(!inputRoot.empty());
    CV_Assert(!outputRoot.empty());

    yzbx_CDNetBenchMark benchmark(QString::fromStdString (inputRoot),QString::fromStdString (outputRoot));

    CV_Assert(benchmark.InputPathList.size ()==benchmark.OutputPathList.size ());

    int startBenchmarkNum=set->value("staticTracking/startBenchmarkNum").toInt();
    CV_Assert(startBenchmarkNum>=0);
    for(int benchmarkNum=startBenchmarkNum;benchmarkNum<benchmark.InputPathList.size ();benchmarkNum++){
        qDebug()<<"input="<<benchmark.InputPathList.at(benchmarkNum);
        qDebug()<<"output="<<benchmark.OutputPathList.at(benchmarkNum);

        QString qrootin=benchmark.InputPathList.at(benchmarkNum);
        QString qrootout=benchmark.OutputPathList.at(benchmarkNum);
        string rootin=qrootin.toStdString ()+"/input";
        string rootout=qrootout.toStdString ();
        yzbx_frameInput yfInput(rootin,1,-1);

        Mat input,fgMask,output;
        namedWindow ("input");
        for(int i=1;;i++){
            cout<<"i="<<i<<" *****************"<<endl;
            yfInput.getNextFrame (input,FromCDNet);
            if(!input.empty ()){
                imshow("input",input);
            }
            else{
                break;
            }

            yfInput.getNextGroundTruth(fgMask);

            if(!fgMask.empty ()){
                imshow("fgMask",fgMask);
                process(input);
            }

            int key=waitKey (30);
            if(key=='n'){
                qDebug()<<"exit to next filepath";
                break;
            }
            else if(key=='q'){
                qDebug()<<"exit the programe!";
                return;
            }
            else if(key=='s'){
                qDebug()<<"wait next keyboard";
                waitKey(0);
            }
        }
    }
}

void movingLineDetector::process(const Mat &inputImg){
    movingPointDetector(inputImg);
    if(frameNum>=2){
        getExtreamImage();
    }
    frameNum++;
}

void movingLineDetector::getExtreamImage(){
    CV_Assert(frameNum>=2);
    CV_Assert(!img1.empty());
    CV_Assert(!img3.empty());

    vector<Mat> vec;
    vec.push_back(img1);
    vec.push_back(img2);
    vec.push_back(img3);

    int img_rows=img1.rows;
    int img_cols=img1.cols;

    Mat extrameMat(img1.size(),CV_8UC1);
    for(int i=0;i<img_rows;i++){
        for(int j=0;j<img_cols;j++){
            //p should big than some value.
            uchar p=img2.at<float>(i,j);

            bool maxFlag=true;

//            if(p<noiseThreshold){
//                maxFlag=false;
//            }

            for(int c=0;c<3&&maxFlag;c++){
               for(int x=-1;x<=1&&maxFlag;x++){
                   for(int y=-1;y<=1&&maxFlag;y++){
                       if(p<vec[c].at<float>(i+x,j+y)){
                           maxFlag=false;
                       }
                   }
               }
            }

            if(maxFlag){
                extrameMat.at<uchar>(i,j)=255;
            }
            else{
                extrameMat.at<uchar>(i,j)=0;
            }
        }
    }

    if(frameNum==2){
        img_extream1=extrameMat;
    }
    else if(frameNum>2){
        img_extream2=img_extream1;
        img_extream1=extrameMat;
    }

    imshow("extream",img_extream1);
}
