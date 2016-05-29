#include "yzbx_frameinput.h"

yzbx_frameInput::yzbx_frameInput(int deviceNum)
{
    cap.open(deviceNum);
    CV_Assert(cap.isOpened());
}

yzbx_frameInput::yzbx_frameInput(string videopath){
    cap.open(videopath);
    CV_Assert(cap.isOpened());
}

yzbx_frameInput::yzbx_frameInput(string frameFloderPath, int startFrameNum, int lastFrameNum){
    rootPath=frameFloderPath;
    frameNum=startFrameNum;
    this->lastFrameNum=lastFrameNum;
}

void yzbx_frameInput::getNextFrame (Mat &frame, int method){
    switch (method) {
    case FromDevice:
    {
        cap>>frame;
        break;
    }
    case FromVideoFile:
    {
        cap>>frame;
        break;
    }
    case FromCDNet:
    {
        stringstream ss;
        ss<<frameNum;
        char cstr[10];
        sprintf (cstr,"%06d",frameNum);
        string numstr(cstr);

        if(frameNum<=lastFrameNum||lastFrameNum==-1){
            string filename=rootPath+"/in"+numstr+".jpg";
            QString qfilename=QString::fromStdString (filename);
            QFile file(qfilename);
            if(file.exists ())
                frame=imread(rootPath+"/in"+numstr+".jpg");
            else
                frame.release ();
        }
        else{
            cout<<"reach last frame num "<<lastFrameNum<<endl;
        }
        frameNum++;
        updateFrameNumInGetNextFrame=true;
        break;
    }
    default:
    {
        CV_Assert(false);
        break;
    }
    }
}

void yzbx_frameInput::getNextGroundTruth(Mat &gt){
    stringstream ss;
    ss<<frameNum;
    char cstr[10];
    sprintf (cstr,"%06d",frameNum);
    string numstr(cstr);

    string gtpath=rootPath;
    int pos=gtpath.find("/input");
    CV_Assert(pos>0);
    gtpath.replace(pos,6,"/groundtruth");
    if(frameNum<=lastFrameNum||lastFrameNum==-1){
        string filename=gtpath+"/gt"+numstr+".png";
        QString qfilename=QString::fromStdString (filename);
        QFile file(qfilename);
        if(file.exists ())
            gt=imread(filename);
        else{
            cout<<"file "<<filename<<" do not exist"<<endl;
            gt.release ();
        }
    }
    else{
        cout<<"reach last frame num "<<lastFrameNum<<endl;
    }

    if(!updateFrameNumInGetNextFrame) frameNum++;
}
