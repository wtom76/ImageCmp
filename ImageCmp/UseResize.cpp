#include "stdafx.h"
#include "Core.h"
#include "UseResize.h"
#include "ResultAnalyse.h"
#include <iostream>
#include <string>
#include <future>
#include <algorithm>
#include <functional>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

UseResize::UseResize(int size)
	: size_(size)
	, diff_kernel_(_createKernel())
	, max_diff_(_maxDiff())
{
}

UseResize::~UseResize()
{
}

cv::Mat UseResize::_createKernel() const
{
	assert(size_ > 0);
	Mat kernel(size_, size_, 0);
	for (int y = 0; y < kernel.rows; ++y)
	{
		uchar* row = kernel.ptr<uchar>(y);
		for (int x = 0; x < kernel.cols; ++x)
		{
			row[x] = 1;
		}
	}
	return kernel;
}

decltype(UseResize::max_diff_) UseResize::_maxDiff() const
{
	assert(!diff_kernel_.empty());
	using DiffT = remove_const<decltype(max_diff_)>::type;
	DiffT max_diff = 0;
	const DiffT max_diff_elem = (static_cast<DiffT>(numeric_limits<uchar>::max()) + 1) >> 1;
	for (int y = 0; y < diff_kernel_.rows; ++y)
	{
		const uchar* row = diff_kernel_.ptr<uchar>(y);
		for (int x = 0; x < diff_kernel_.cols; ++x)
		{
			max_diff += max_diff_elem * row[x];
		}
	}
	return max_diff;
}

Mat UseResize::_makeIcon(int img_index) const
{
	Mat hsv;
	{
		Mat src;
		src = imread(g_filenames[img_index], IMREAD_COLOR);
		if (src.empty())
		{
			throw exception(string(baseFileName(g_filenames[img_index]) + " isn't loaded").c_str());
		}
		cvtColor(src, hsv, CV_BGR2HSV);
	}

	Mat icon;
	resize(hsv, icon, Size(size_, size_));
	return icon;
}

cv::Mat UseResize::_makeHueFPrint(int img_index) const
{
	Mat hue;
	{
		Mat icon;
		{
			Mat hsv;
			{
				Mat src;
				src = imread(g_filenames[img_index], IMREAD_COLOR);
				if (src.empty())
				{
					throw exception(string(baseFileName(g_filenames[img_index]) + " isn't loaded").c_str());
				}
				cvtColor(src, hsv, CV_BGR2HSV);
			}

			resize(hsv, icon, Size(size_, size_));
		}

		vector<Mat> channels;
		split(icon, channels);
		hue = channels.front();
	}

	return hue;
}

double UseResize::_compare(const Mat& left, const Mat& right) const
{
	long diff = 0;
	for (int y = 0; y < left.rows; ++y)
	{
		const uchar* kernel_row = diff_kernel_.ptr<uchar>(y);
		const uchar* left_row = left.ptr<uchar>(y);
		const uchar* right_row = right.ptr<uchar>(y);
		for (int x = 0; x < left.cols; ++x)
		{
			diff += kernel_row[x] * _hueDiff(left_row[x], right_row[x]);
		}
	}
	return static_cast<double>(diff) / max_diff_;
}

std::vector<AlgDev::Result> UseResize::run()
{
	using Result = AlgDev::Result;
	using namespace std;

	const size_t count = g_filenames.size();
	vector<shared_future<Mat>> icons;
	icons.reserve(count);
	auto make_hist = [this](int i) { return _makeHueFPrint(i); };
	for (int i = 0; i < count; ++i)
	{
		icons.push_back(async(launch::deferred, make_hist, i));
	}

	vector<future<Result>> rfuts;
	rfuts.reserve(count * (count + 1) / 2);
	auto cmp_hist = [this, &icons](int i, int j)
	{ return Result(pair<int, int>(i, j), _compare(icons[i].get(), icons[j].get())); };
	for (int i = 0; i < count; ++i)
	{
		for (int j = i; j < count; ++j)
		{
			rfuts.push_back(async(launch::deferred, cmp_hist, i, j));
		}
	}
	vector<Result> result(count * (count + 1) / 2);
	transform(begin(rfuts), end(rfuts), begin(result), mem_fun_ref_t<Result, future<Result>>(&future<Result>::get));
	return result;
}

void UseResize::testMethod(int size, double thresh)
{
	cout << endl << "size = " << size << ", thresh = " << thresh << endl;
	AlgDev::Analyse(UseResize(size).run(), std::less<AlgDev::Result>(), thresh);
}

void UseResize::testMethod()
{
	testMethod(3, 0.25);
	testMethod(3, 0.30);
	testMethod(4, 0.25);
	testMethod(5, 0.28);
	testMethod(6, 0.27);
}
