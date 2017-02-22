#pragma once
#include "ResultAnalyse.h"

class UseResize
{
	const int size_ = 3;

	cv::Mat _makeIcon(int img_index) const;
	double _compareIcons(const cv::Mat& left, const cv::Mat& right) const;
public:
	UseResize();
	~UseResize();

	std::vector<AlgDev::Result> run();
	static void testMethod();
};

