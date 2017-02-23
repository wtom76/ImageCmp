#pragma once
#include "ResultAnalyse.h"
#include <limits>

class UseResize
{
	const int size_;
	cv::Mat diff_kernel_;
	const long max_diff_;

	cv::Mat _createKernel() const;
	decltype(max_diff_) _maxDiff() const;
	cv::Mat _makeIcon(int img_index) const;
	cv::Mat _makeHueFPrint(int img_index) const;

	template <typename T>
	T _hueDiff(T hue1, T hue2) const;
	double _compare(const cv::Mat& left, const cv::Mat& right) const;
public:
	UseResize(int size);
	~UseResize();

	std::vector<AlgDev::Result> run();
	static void testMethod(int size, double thresh);
	static void testMethod();
};

template <typename T>
T UseResize::_hueDiff(T hue1, T hue2) const
{
	return std::min(
		std::abs(hue1 - hue2),
		std::numeric_limits<T>::max() - std::abs(hue1 - hue2));
}
