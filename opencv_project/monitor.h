#pragma once
//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//C++
#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_world320d.lib")
using namespace cv;
using namespace std;
extern Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
extern Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor,extern表示在别的文件中定义
extern int keyboard;
const float h_ranges[] = { 0, 256 };
const float s_ranges[] = { 0, 180 };
const float* ranges[] = { h_ranges, s_ranges };
const int h_bins = 50, s_bins = 60;
const int histSize[] = { h_bins, s_bins };
extern char recordName[128];
void processCamera(bool showWindow,
	unsigned int method,
	unsigned int unnormal = 10,
	unsigned int fps = 24) {
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cerr << "Unable to open camera " << endl;
		exit(EXIT_FAILURE);
	}
	namedWindow("Monitor");//create window named by "Monitor"
	namedWindow("fgmask");
	bool backGroundFlag = true, recordFlag = false;
	Mat frame, fgMask;					// current frame, fg mask
	Mat HSV;							// HSV format 临时变量
	MatND base, cur;					// histogram
	unsigned int UnnormalFrames = 0;//up to the normal
	int channels[] = { 0, 1 };
	CvSize size = cvSize(
		(int)capture.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)capture.get( CV_CAP_PROP_FRAME_HEIGHT)
	);
	VideoWriter recorder(recordName, CV_FOURCC('M', 'J', 'P', 'G'), 32, size);
	// ESC or 'q' for quitting
	while ((char)keyboard != 'q' && (char)keyboard != 27) {
		if (!capture.isOpened()) {
			cerr << "Unable to read camera" << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}
		capture.read(frame);
		if (method == 0)
			pMOG2->apply(frame, fgMask);
		else if (method == 1)
			pMOG->apply(frame, fgMask);
		else if (method == 2)
			fgMask = frame;
		// set background
		if (backGroundFlag) {
			cvtColor(frame, HSV, CV_BGR2HSV);
			calcHist(&HSV, 1, channels, Mat(), base, 2, histSize, ranges, true, false);
			normalize(base, base, 0, 1, NORM_MINMAX, -1, Mat());
			backGroundFlag = false;
		}
		cvtColor(frame, HSV, CV_BGR2HSV);
		calcHist(&HSV, 1, channels, Mat(), cur, 2, histSize, ranges, true, false);
		normalize(cur, cur, 0, 1, NORM_MINMAX, -1, Mat());
		double comp = compareHist(base, cur, 0);
		if (comp < 0.5)
			UnnormalFrames += 1;
		else if (UnnormalFrames > 0)
			UnnormalFrames--;
		if (UnnormalFrames > unnormal)
			recordFlag = true;
		else if (UnnormalFrames <= 0) {
			UnnormalFrames = 0;
			recordFlag = false;
		}
		// DO SOMETHING WARNING
		// Here We Starting Recoding
		if (recordFlag)
		{
			recorder.write(frame);
		}
		if (showWindow && !frame.empty())
		{
			imshow("Monitor", frame);
			imshow("fgmask", fgMask);
		} 
		keyboard = waitKey(30);
	}
	recorder.release();
	capture.release();
}