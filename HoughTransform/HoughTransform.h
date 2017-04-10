#pragma once
#include <stdio.h>
#include <windows.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

class HoughTransform
{
public:
	HoughTransform();
	HoughTransform(cv::Mat image);
	HoughTransform(int filter);
	HoughTransform(int thetaMax, int accuracy);
	HoughTransform(int filter, int thetaMax, int accuracy);

	void setImage(cv::Mat frame);
	void setFilter(int new_filter);
	void printWhitePoints();
	std::vector<std::vector<cv::Point> > findLines();
	void initAccumulator(int maxTheta, double accuracy);
	void drawAccumulator();

	class Hought_exception : public std::exception {
		
	public:
		Hought_exception(std::string err) {
			this->err = err;
		}
		virtual const char* what() const {
			return ("HOUGH EXCEPTION: " + err).c_str();
		}
	private: 
		std::string err;
	};


private:

	const double DEG2RAD = 0.01745329;
	const double RAD2DEG = 57.2957796;

	cv::Mat image;
	std::vector<cv::Point> points;
	std::vector<std::vector<int> > accumulator;

	int filter;

	int thetaMax;
	int accuHeight;

	double center_x;
	double center_y;
	double accuracy;

	bool initializedAccu;
	bool initializedImg;

	void getWhitePoints(cv::Mat inputImg);

};

