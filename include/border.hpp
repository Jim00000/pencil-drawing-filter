#ifndef __BORDER_HPP__
#define __BORDER_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class border
    {
    public:
        border(const cv::Mat& src);
        virtual ~border();
        cv::Mat& result();
    protected:
        virtual void _process(const cv::Mat& src, cv::Mat& result);
    private:
        cv::Mat _result;
    };
}

#endif