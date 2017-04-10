#include "HoughTransform.h"

HoughTransform::HoughTransform() : filter(100)
{
	initializedImg = false;
	initializedAccu = false;
}

HoughTransform::HoughTransform(cv::Mat image) : filter(100)
{
	initializedImg = false;
	initializedAccu = false;
	setImage(image);
}

HoughTransform::HoughTransform(int filter)
{
	initializedImg = false;
	initializedAccu = false;
	this->filter = filter;
}

HoughTransform::HoughTransform(int filter, int thetaMax, int accuracy)
{
	initializedImg = false;
	initializedAccu = false;
	this->filter = filter;
	initAccumulator(thetaMax, accuracy);
}

HoughTransform::HoughTransform(int thetaMax, int accuracy) : filter(100)
{
	initializedImg = false;
	initializedAccu = false;
	initAccumulator(thetaMax, accuracy);
}

		/*SETTERS*/
/*-------------------------------*/
void HoughTransform::setFilter(int new_filter) {
	
	this->filter = new_filter;
}

void HoughTransform::setImage(cv::Mat frame) {

	this->image = frame;
	cv::Point point;
	points.clear();

	getWhitePoints(frame);
	initializedImg = true;
}

void HoughTransform::initAccumulator(int thetaMax, double accuracy) {

	if (!initializedImg)
		throw new Hought_exception("Image not initialized");

	this->accuracy = accuracy;
	this->thetaMax = thetaMax / accuracy;
	
	accuHeight = sqrt(pow(image.rows, 2.0) + pow(image.cols, 2.0)) + 1;

	accumulator.resize(thetaMax / accuracy);
	for (int i = 0; i < accumulator.size(); i++)
		accumulator[i].resize(accuHeight);
	
	initializedAccu = true;
}
/*-------------------------------*/


		/*Main algoritmus*/
/*-------------------------------*/
std::vector<std::vector<cv::Point> > HoughTransform::findLines() {

	if (!initializedAccu)
		initAccumulator(180, 1);

	/*-----------------------------------------*/
	/*----------Fill the Accumulator------------*/

	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < thetaMax; j++) {
			double theta = j * accuracy;
			double r = (double)points[i].x * cos((double)theta*DEG2RAD) + (double)points[i].y * sin((double)theta*DEG2RAD);		
			accumulator[j][round(fabs(r))]++;
		}
	}

	/*-----------------------------------------*/
	/*----------------Find lines---------------*/

	int maxValue = 0;

	std::vector<int> finalR, finalTheta;
	for (int i = 0; i < thetaMax; i++) {
		for (int j = 0; j < accuHeight; j++) {

			if (accumulator[i][j] > filter) {
				finalR.push_back(j);
				finalTheta.push_back(i);
			}
		}
	}

	/*-----------------------------------------*/
	/*-----------Compute points----------------*/
	std::vector<std::vector<cv::Point> > lines(finalR.size());
	
	for (int i = 0; i < finalR.size(); i++) {
		
		cv::Point pt1, pt2;
		double a = cos(finalTheta[i]*DEG2RAD), b = sin(finalTheta[i]* DEG2RAD);
		double x0 = a*finalR[i], y0 = b*finalR[i];
		pt1.y = cvRound(x0 + 1000 * (-b));
		pt1.x = cvRound(y0 + 1000 * (a));
		pt2.y = cvRound(x0 - 1000 * (-b));
		pt2.x = cvRound(y0 - 1000 * (a));

		lines[i].push_back(pt1);
		lines[i].push_back(pt2);
	}
	return lines;
}

/*-------------------------------*/

		/*Debug functions*/
/*-------------------------------*/

void HoughTransform::printWhitePoints() {

	printf("\n FOUND WHITE POINTS\n");
	for (int i = 0; i < points.size(); i++)
	{
		printf("x=%d , y=%d \n", points[i].x, points[i].y);
	}
}

void HoughTransform::drawAccumulator() {

	cv::Mat finalAcumulator = cv::Mat(accuHeight, thetaMax, CV_8UC1, cv::Scalar(255));

	for (int i = 0; i < thetaMax; i++) {
		for (int j = 0; j < accuHeight; j++) {
			finalAcumulator.at<uchar>(j,i) = accumulator[i][j];
		}
	}
	imshow("ACCUMULATOR", finalAcumulator);
}
/*-------------------------------*/

		/*PRIVATE FUNCTION*/
void HoughTransform::getWhitePoints(cv::Mat inputImg) {

	cv::Scalar colour;
	points.clear();
	for (int i = 0; i < inputImg.rows; i++) {
		for (int j = 0; j < inputImg.cols; j++) {
			colour = inputImg.at<uchar>(i, j);
			if (colour.val[0] == 255) {
				points.push_back(cv::Point(i, j));
			}
		}
	}
}