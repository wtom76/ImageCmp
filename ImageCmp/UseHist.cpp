#include "stdafx.h"
#include "UseHist.h"
#include "ResultAnalyse.h"
#include <iostream>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;

static const char* g_filenames[] =
{
	R"(.\img\larger1.jpg)",
	R"(.\img\larger2.jpg)",
	R"(.\img\larger3.jpg)",
	R"(.\img\larger4.jpg)",
	R"(.\img\larger5.png)",
	R"(.\img\larger6.jpg)",
	R"(.\img\graf1.png)",
	R"(.\img\graf3.png)"
};

void UseHist::run()
{
	constexpr int count = sizeof(g_filenames) / sizeof(g_filenames[0]);
	std::vector<Mat> histograms(count);
	for (int i = 0; i < count; ++i)
	{
		_createHist(i, histograms[i]);
	}

	for (int i = 0; i < count; ++i)
	{
		for (int j = i; j < count; ++j)
		{
			const double cmprd = compareHist(histograms[i], histograms[j], compare_method_);
			std::wcout << i << L'\t' << j << L'\t' << cmprd << std::endl;
		}
	}
}

std::vector<AlgDev::Result> UseHist::runResult()
{
	constexpr int count = sizeof(g_filenames) / sizeof(g_filenames[0]);
	std::vector<AlgDev::Result> result;
	result.reserve((count + 1) * count / 2);
	std::vector<Mat> histograms(count);
	for (int i = 0; i < count; ++i)
	{
		_createHist(i, histograms[i]);
	}

	for (int i = 0; i < count; ++i)
	{
		for (int j = i; j < count; ++j)
		{
			const double cmprd = compareHist(histograms[i], histograms[j], compare_method_);
			result.push_back(AlgDev::Result(std::pair<int, int>(i, j), cmprd));
		}
	}
	return result;
}

void UseHist::_createHist(int img_index, Mat& hist) const
{
	Mat src, hsv;
	src = imread(g_filenames[img_index], IMREAD_COLOR);

	cvtColor(src, hsv, CV_BGR2HSV);

	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	//MatND hist; - placed to params
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };

	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins * 10, CV_8UC3);

	for (int h = 0; h < hbins; h++)
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, Point(h*scale, s*scale),
				Point((h + 1)*scale - 1, (s + 1)*scale - 1),
				Scalar::all(intensity),
				CV_FILLED);
		}

	imwrite(std::string(".\\hist\\") + std::to_string(img_index) + "_hist.png", histImg);

	//namedWindow("Source", 1);
	//imshow("Source", src);

	//namedWindow("H-S Histogram", 1);
	//imshow("H-S Histogram", histImg);
	//waitKey();
}

void UseHist::testMethod()
{
	std::wcout << std::endl << L"CV_COMP_CORREL" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_CORREL).runResult());
	std::wcout << std::endl << L"CV_COMP_CHISQR" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_CHISQR).runResult());
	std::wcout << std::endl << L"CV_COMP_INTERSECT" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_INTERSECT).runResult());
	std::wcout << std::endl << L"CV_COMP_BHATTACHARYYA = CV_COMP_HELLINGER" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_BHATTACHARYYA).runResult());
	std::wcout << std::endl << L"CV_COMP_CHISQR_ALT" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_CHISQR_ALT).runResult());
	std::wcout << std::endl << L"CV_COMP_KL_DIV" << std::endl;
	AlgDev::Analyse(UseHist(CV_COMP_KL_DIV).runResult());
}
