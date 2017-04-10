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
	//Constructors
	HoughTransform();										
	HoughTransform(cv::Mat image);							
	HoughTransform(int filter);								
	HoughTransform(int thetaMax, int accuracy);				
	HoughTransform(int filter, int thetaMax, int accuracy);

	//Seters
	void setImage(cv::Mat frame);
	void setFilter(int new_filter);
	void initAccumulator(int maxTheta, double accuracy);

	//Main algorithm - returns vector of lines (2x point)
	std::vector<std::vector<cv::Point> > findLines();
	
	//Debug functions
	void printWhitePoints();
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

	//Minimum number of points from accumulator
	int filter;

	//Accumulator params
	std::vector<std::vector<int> > accumulator;
	int thetaMax;
	int accuHeight;
	double accuracy;

	//Flags
	bool initializedAccu;
	bool initializedImg;

	//Get the white points from cv::Mat
	std::vector<cv::Point> points;
	void getWhitePoints(cv::Mat inputImg);

};

