#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
	// 3x3 単位行列 (= cv::Mat m(3,3, CV_64F) )
	cv::Mat m = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);

	std::cout << "m=" << m << std::endl << std::endl;

}