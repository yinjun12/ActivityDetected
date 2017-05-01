#include "monitor.h"
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard;

char recordName[128]="D:\\OpenCV\\video1.avi";

void help();

int main(int argc, char* argv[]) {
	cout << "Starts After 2s..." << endl;
	Sleep(2000);
	namedWindow("Monitor");
	pMOG = createBackgroundSubtractorMOG2(); //MOG approach
	pMOG2 = createBackgroundSubtractorKNN(); //MOG2 approach
	processCamera(true, 0);
	destroyAllWindows();
	return EXIT_SUCCESS;
}


void help() {
	cout
		<< "----------------------------------------------------------------------------\n"
		<< "Usage:                                                                      \n"
		<< " ./MonitorRecorder.exe [VIS] [MODE] [FPS] [THRESHOLD] [OUTPUTFILE]          \n"
		<< "   [VIS]  : use -vis to show the monitor window, or it will run background. \n"
		<< "   [MODE] : -src   shows the original frame;                                \n"
		<< "            -mog1       shows the MOG frame;                                \n"
		<< "            -mog2      shows the MOG2 frame.                                \n"
		<< "   [FPS]  : set the fps of record file, default is 24.                      \n"
		<< "   [THRESHOLD]                                                              \n"
		<< "          : set the number x that the monitor will start recording after    \n"
		<< "            x unnormal frames passed.                                       \n"
		<< "   [OUTPUTFILE]                                                             \n"
		<< "          : assign the output recording file. It must be .avi format.       \n"
		<< "                                                   designed by Forec        \n"
		<< "----------------------------------------------------------------------------\n";
}