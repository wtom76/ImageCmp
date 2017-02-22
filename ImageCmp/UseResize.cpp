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

UseResize::UseResize()
{
}

UseResize::~UseResize()
{
}

Mat UseResize::_makeIcon(int img_index) const
{
	Mat src, hsv;
	src = imread(g_filenames[img_index], IMREAD_COLOR);
	cvtColor(src, hsv, CV_BGR2HSV);

	Mat icon;
	resize(hsv, icon, Size(size_, size_));
	return icon;
}

double UseResize::_compareIcons(const Mat& left, const Mat& right) const
{
	return 0.0;
}

std::vector<AlgDev::Result> UseResize::run()
{
	using Result = AlgDev::Result;
	using namespace std;

	const size_t count = g_filenames.size();
	vector<shared_future<Mat>> icons;
	icons.reserve(count);
	auto make_hist = [this](int i) { return _makeIcon(i); };
	for (int i = 0; i < count; ++i)
	{
		icons.push_back(async(make_hist, i));
	}

	vector<future<Result>> rfuts;
	rfuts.reserve(count * (count + 1) / 2);
	auto cmp_hist = [this, &icons](int i, int j)
	{ return Result(pair<int, int>(i, j), _compareIcons(icons[i].get(), icons[j].get())); };
	for (int i = 0; i < count; ++i)
	{
		for (int j = i; j < count; ++j)
		{
			rfuts.push_back(async(cmp_hist, i, j));
		}
	}
	vector<Result> result(count * (count + 1) / 2);
	transform(begin(rfuts), end(rfuts), begin(result), mem_fun_ref_t<Result, future<Result>>(&future<Result>::get));
	return result;
}

void UseResize::testMethod()
{
	AlgDev::Analyse(UseResize().run(), std::greater<AlgDev::Result>());
}
