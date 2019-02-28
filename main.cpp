#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
//MAYBE: Cant using with double cvtColor??
int main()
{
    VideoCapture video(0);
    vector<vector<Point> > contours;
//MAYBE: Want use CreateTrackerbar? Must have namedWindow!
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    while (1)
    {
        Mat frame;
        Mat hsv;
        video >> frame;
        cvtColor(frame, hsv, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat mask;

        inRange(hsv, Scalar(68, 119, 139), Scalar(179, 255, 255), mask); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        findContours( mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
        for( int i = 0; i< contours.size(); i++ )
           {
             drawContours( frame, contours,i,Scalar(0,0,255),4,LINE_8 );
           }
        imshow("Detect", mask);

        imshow("Original", frame);

        if ( waitKey(1) == 27)break;
    }

}
