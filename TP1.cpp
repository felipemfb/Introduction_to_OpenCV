//basic C and C++ libraries
#include <stdio.h>
#include <iostream>

#include <opencv2/core.hpp> //declare the data image structure

//declare graphical interface functions that interact with the image
#include <opencv2/highgui.hpp> //react to mouse and keyboard events, draw shapes, write text on the image and more
#include <opencv2/imgproc.hpp>  // <- circle, putText, fontes, etc.
#include <opencv2/imgcodecs.hpp> //declare image read and write functions

//draws the RedLine at the image
void redLine(cv::Mat& image, int height) {
	using namespace cv;
	int width = image.cols;
	for (int k = 0; k < width; k++) {
		//defines the red color at the image
		if (image.type() == CV_8UC1) {
			cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
		}
		else if (image.type() == CV_8UC3) {
			image.at<Vec3b>(height, k)[0] = 0;
			image.at<Vec3b>(height, k)[1] = 0;
			image.at<Vec3b>(height, k)[2] = 255;
		}
	}
}

//Plots the histogram with the intensity profile
void intensityProfile(cv::Mat& image, int height) {
	using namespace cv;
	int width = image.cols, plotHeight = 400;
	Mat imageIntensity(plotHeight + 100, width, CV_8UC1, Scalar(255));
	float prevX = -1, prevY = -1;
	//normalize the data into the Window size
	for (int k = 0; k < width; k++){
		float bitValue = image.at<uchar>(height, k);
		float tmp = ((bitValue * (plotHeight - 1)) / 255) + 1;
		float finalBit = plotHeight - tmp + 50;
		imageIntensity.at<uchar>(finalBit, k) = 0;
		//connects each point, forming a line
		if (prevX >= 0 && prevY >= 0) {
			cv::line(imageIntensity, Point(prevX, prevY), Point(k, finalBit), Scalar(0), 1);
		}
		prevX = k;
		prevY = finalBit;
	}
	imshow("Intensity Profile", imageIntensity);
	waitKey(0);
}

//controls the mouse click
void onMouse(int event, int x, int y, int flags, void* param) {
	using namespace cv;
	using namespace std;
	Mat* im = reinterpret_cast<Mat*>(param);
	Mat copy;
	(*im).copyTo(copy);
	switch (event) {
		case EVENT_LBUTTONDOWN:
			//helps the main function to execute the program
			redLine(copy, y);
			imshow("RedLine Image", copy);
			intensityProfile(*im, y);
			waitKey(0);
			break;
	}
}

int main() {
	using namespace std;
	using namespace cv;

	//read and display a gray level image
	Mat image;
	image = imread("IIA_images/images/chessboards/chessboard01.jpg", IMREAD_GRAYSCALE);
	if (image.empty()) {
		cout << "Could not open or find the image" << endl;
		return 0;
	}

	//Select an image line(mouse click in the image window)
	//Draw this line in red color
	Mat imageCopy;
	image.copyTo(imageCopy);
	namedWindow("RedLine Image");
	imshow("RedLine Image", imageCopy);
	//Draw the image intensity profil in a new window
	setMouseCallback("RedLine Image", onMouse, reinterpret_cast<void*>(&imageCopy));

	waitKey(0);
	return 0;
}