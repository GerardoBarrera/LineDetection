#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>

#include <iostream>

using namespace cv;
using namespace std;

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{
	// Create a VideoCapture object and open the input file
	// If the input is the web camera, pass 0 instead of the video file name
	VideoCapture cap(0);

	// Check if camera opened successfully
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	while (1) {

		Mat frame;
		// Capture frame-by-frame
		cap >> frame;


		// If the frame is empty, break immediately
		if (frame.empty())
			break;
		Mat dst, cdst;
		Canny(frame, dst, 50, 200, 3);
		cvtColor(dst, cdst, COLOR_GRAY2BGR);
#if 0
		vector<Vec2f> lines;
		HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		}
#else
		vector<Vec4i> lines;
		HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 50);
		float vWeight = 0;
		float hWeight = 0;
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);

			double distance = sqrt(pow((l[2]-l[0]), 2) + pow((l[3]-l[2]), 2));
			//cout << distance;
			//calculate angle in radian,  if you need it in degrees just do angle * 180 / PI
			float angle = atan2(l[1] - l[3], l[0] - l[2]) * 180 / 3.14;
			if (angle <-70 && angle >-120 || angle < 120 && angle > 70) {
				vWeight += distance;
			}
			else {
				hWeight += distance;
			}

		}
		if (hWeight > vWeight) {
			cout << "MOVE HORIZONTAL" << endl;
		}
		else {
			cout << "MOVE VERTICAL" << endl;
		}

#endif
		// Display the resulting frame
		imshow("Frame", frame);
		imshow("detected lines", cdst);


		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25);
		if (c == 27)
			break;
	}

	// When everything done, release the video capture object
	cap.release();

	// Closes all the frames
	destroyAllWindows();

	return 0;
}
