#include "stdafx.h"
#include "Core.h"
#include "UseOrb.h"
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cv;

static constexpr int count = 6;

UseOrb::UseOrb()
{
}

UseOrb::~UseOrb()
{
}

void UseOrb::run()
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
		Ptr<Feature2D> extractor = ORB::create(500, 1.2f, 8, 31, 0, wta_k);
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
