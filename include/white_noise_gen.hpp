#ifndef __WHITE_NOISE_GEN_HPP__
#define __WHITE_NOISE_GEN_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class white_noise_gen
    {
    public:
        white_noise_gen(cv::Mat& src);
        virtual ~white_noise_gen();

        cv::Mat& result();

    protected:

    private:
        cv::Mat _result;
    };
}

#endif