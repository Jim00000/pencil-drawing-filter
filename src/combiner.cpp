#include <exception>
#include "combiner.hpp"

using namespace cv;
using namespace pencil;

combiner::combiner()
{

}

combiner::~combiner()
{

}

void
combiner::combine_foreground_and_background(const Mat& foreground,
                                            const Mat& background, Mat& result)
{
    if(foreground.size() != background.size()) {
        throw std::runtime_error("Argument `foreground` and `background` has different size");
    }

    result = Mat(foreground.size(), CV_8U);

    cv::bitwise_not(foreground, foreground);
    cv::bitwise_not(background, background);
    
    for(size_t i = 0; i < foreground.rows; i++) {
        for(size_t j = 0; j < foreground.cols; j++) {
            result.at<uchar>(i, j) = saturate_cast<uchar>(background.at<uchar>(i, j) + foreground.at<uchar>(i, j));
        }
    }

    cv::bitwise_not(foreground, foreground);
    cv::bitwise_not(background, background);
    cv::bitwise_not(result, result);

}