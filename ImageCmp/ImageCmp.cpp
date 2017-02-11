// ImageCmp.cpp : Defines the entry point for the console application.
//

// http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_matching/akaze_matching.html
// http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_tracking/akaze_tracking.html

#include "stdafx.h"
#include <opencv2\stitching\detail\matchers.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cv;

const char* g_filenames[] =
{
	R"(..\img\larger1.jpg)",
	R"(..\img\larger2.jpg)",
	R"(..\img\larger3.jpg)",
	R"(..\img\larger4.jpg)",
	R"(..\img\larger5.png)",
	R"(..\img\larger6.jpg)"
};

//const char* g_filenames[] =
//{
//	R"(.\img\larger1.jpg)",
//	R"(.\img\larger2.jpg)",
//	R"(.\img\larger3.jpg)",
//	R"(.\img\larger4.jpg)"
//};

constexpr int count = 6;
//constexpr int mcount = 3;

int main()
{
	try
	{
		//DescriptorMatcher 
		std::vector<Mat> images(count);
		for (int i = 0; i < count; ++i)
		{
			images[i] = imread(g_filenames[i], 1);
		}

		constexpr int wta_k = 2;
		//Ptr<Feature2D> extractor = ORB::create(500, 1.2f, 8, 31, 0, wta_k);
		Ptr<AKAZE> extractor = AKAZE::create();
		extractor->setThreshold(0.001);
		//extractor->setDescriptorType(AKAZE::DESCRIPTOR_KAZE);
		std::vector<std::vector<KeyPoint>> kpoints(count);
		std::vector<Mat> descr(count);
		for (int i = 0; i < count; ++i)
		{
			extractor->detectAndCompute(images[i], noArray(), kpoints[i], descr[i]);
		}
		//FlannBasedMatcher matcher;
		BFMatcher matcher(wta_k > 2 ? NORM_HAMMING2 : NORM_HAMMING, true);
		int m = 0;
		for (int i = 0; i < count - 1; ++i)
		{
			for (int j = i + 1; j < count; ++j)
			{
				std::vector<DMatch> matches;
				matcher.match(descr[i], descr[j], matches);
				auto minmax = std::minmax_element(std::begin(matches), std::end(matches),
					[](const DMatch& left, const DMatch& right)
				{
					return left.distance < right.distance;
				});
				float average = 0.;
				if (!matches.empty())
				{
					for (auto& m : matches)
					{
						average += m.distance;
					}
					average /= matches.size();
				}
				std::cout << g_filenames[i] << ' ' << g_filenames[j] << ' '
					<< minmax.first->distance << ' ' << minmax.second->distance << ' '
					<< average << std::endl;
			}
		}

		for (int i = 0; i < images.size(); ++i)
		{
			Mat image_kp;
			drawKeypoints(images[i], kpoints[i], image_kp, Scalar(-1), DrawMatchesFlags::DEFAULT);
			imshow(std::to_string(i), image_kp);
			waitKey();
		}
	}
	catch (cv::Exception& ex)
	{
		std::cout << ex.err << std::endl;
	}
	waitKey();
}

