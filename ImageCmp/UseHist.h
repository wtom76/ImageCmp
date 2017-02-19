#pragma once
#include <opencv2\core\mat.hpp>
#include <vector>
#include "ResultAnalyse.h"

class UseHist
{
	int compare_method_;
	void _createHist(int img_index, cv::Mat& hist) const;
public:
	explicit UseHist(int compare_method) : compare_method_(compare_method) {}
	~UseHist() {}

	void run();
	std::vector<AlgDev::Result> runResult();
	void testMethod();
};

