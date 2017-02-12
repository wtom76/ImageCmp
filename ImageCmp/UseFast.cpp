#include "stdafx.h"
#include "UseFast.h"
#include <iostream>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

static const char* g_filenames[] =
{
	R"(.\img\larger1.jpg)",
	R"(.\img\larger2.jpg)",
	R"(.\img\larger3.jpg)",
	R"(.\img\larger4.jpg)",
	R"(.\img\larger5.png)",
	R"(.\img\larger6.jpg)"
};

static constexpr int count = sizeof(g_filenames) / sizeof(g_filenames[0]);
static constexpr int threshold = 65;

int image_index = 1;

UseFast::UseFast()
{
}

UseFast::~UseFast()
{
}

void UseFast::_run(int image_index)
{
	std::vector<Mat> images(count);
	images[image_index] = imread(g_filenames[image_index], IMREAD_GRAYSCALE);
	if (images[image_index].empty())
	{
		std::cout << "Image isn't loaded" << std::endl;
		return;
	}

	std::vector<KeyPoint> keypoints;

	FAST(images[image_index], keypoints, threshold, true, FastFeatureDetector::TYPE_9_16);

	if (keypoints.empty())
	{
		std::cout << "No keypoints found" << std::endl;
		return;
	}
	Mat img_kp;
	drawKeypoints(images[image_index], keypoints, img_kp);

	imshow("FAST", img_kp);
	cv::waitKey();
}

void UseFast::_runGftt(int image_index)
{
	std::vector<Mat> images(count);
	images[image_index] = imread(g_filenames[image_index], IMREAD_GRAYSCALE);
	if (images[image_index].empty())
	{
		std::cout << "Image isn't loaded" << std::endl;
		return;
	}

	std::vector<KeyPoint> keypoints;

	Ptr<GFTTDetector> detector = GFTTDetector::create();
	detector->detect(images[image_index], keypoints);

	if (keypoints.empty())
	{
		std::cout << "No keypoints found" << std::endl;
		return;
	}
	Mat img_kp;
	drawKeypoints(images[image_index], keypoints, img_kp);

	imshow("FAST", img_kp);
	cv::waitKey();
}

void UseFast::run()
{
	for (int i = 0; i < count; ++i)
	{
		_runGftt(i);
	}
}
