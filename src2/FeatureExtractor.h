#ifndef __FeatureExtractor__
#define __FeatureExtractor__

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>

class FeatureExtractor
{
public:

    FeatureExtractor()
    {

    }
    ~FeatureExtractor();

    void Compute(cv::Mat& image, std::vector<cv::KeyPoint>& keypoints,
                  cv::Mat& descriptors);
    
    cv::Mat getRoi8X8(cv::Mat& roi40X40);
    
    cv::Mat getRoi40X40(cv::Mat& image, int x, int y);
    
    cv::Mat getBlurredRoi(cv::Mat& patch);

protected:


};

#endif