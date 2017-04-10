#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "String.h"
#include "Windows.h"
#include "Math.h"

#include "HoughTransform.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	Mat inputImg;
	Mat grayImg;

	inputImg = imread("C:/Users/Michal/Documents/Visual Studio 2015/Projects/HoughTransform/image.jpg");
	if (inputImg.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cvtColor(inputImg, grayImg, CV_BGR2GRAY);
	Canny(grayImg, grayImg, 50, 200, 3);
	
	HoughTransform h_transform;
	h_transform.setImage(grayImg);
	h_transform.setFilter(105);

	try {
		h_transform.initAccumulator(180, 1);
		auto lines = h_transform.findLines();

		for (auto i : lines)
			line(inputImg, i[0], i[1], cv::Scalar(255, 0, 0), 3, CV_AA);
		imshow("HOUGH TRANSFORM", inputImg);
		h_transform.drawAccumulator();
	}
	catch (HoughTransform::Hought_exception &e) {
		printf("%s", e.what());
	}
	
	while (1) {
		if (waitKey()) {
			break; // stop 
		}
	}

	return 0;

}

