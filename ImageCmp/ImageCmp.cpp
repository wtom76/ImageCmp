// ImageCmp.cpp : Defines the entry point for the console application.
//

// http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_matching/akaze_matching.html
// http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_tracking/akaze_tracking.html
// https://www.learnopencv.com/image-recognition-and-object-detection-part1/
// http://akaifi.github.io/MultiObjectTrackingBasedOnColor/

// Scalable near identical image and shot detection
// O.Chum, J.Philbin, M.Isard, A.Zisserman, CIVR 2007

// http://docs.opencv.org/2.4/modules/imgproc/doc/histograms.html

#include "stdafx.h"
#include "UseOrb.h"
#include "UzeAkaze.h"
#include "UseFast.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Object.h"
#include "UseMoments.h"
#include "UseHist.h"

int main()
{
	try
	{
		UseHist().run();
	}
	catch (cv::Exception& ex)
	{
		std::cout << ex.err << std::endl;
	}
}

