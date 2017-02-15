#pragma once
#include <opencv2\core\mat.hpp>

class UseHist
{
	void _createHist(int img_index, cv::Mat& hist) const;
public:
	UseHist();
	~UseHist();

	void run();
};

