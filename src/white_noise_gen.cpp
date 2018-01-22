#include <iostream>
#include <limits>
#include <random>
#include "white_noise_gen.hpp"

using namespace pencil;
using namespace cv;

inline void _process(Mat& src, Mat& dst, float k) __attribute__((always_inline));

white_noise_gen::white_noise_gen(Mat& src, float k)
{
    _result = Mat(src.size(), src.type());
    _k = k;
    _process(src, _result, _k);
}

white_noise_gen::~white_noise_gen()
{

}

Mat&
white_noise_gen::result()
{
    return _result;
}

void
_process(Mat& src, Mat& dst, float k)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0, 99.5 * k);

    for(uint i = 0; i < src.rows; i++) {
        for(uint j = 0; j < src.cols; j++) {
            uchar gray = src.at<uchar>(i, j);
            const float threshold = static_cast<float>(gray) / static_cast<float>
                                    (std::numeric_limits<uchar>().max()) * 100.0f;
            const float toss = distribution(generator);
            const bool hit = (toss <= threshold) ? true : false;
            dst.at<uchar>(i, j) = hit ? std::numeric_limits<uchar>().max() :
                                  std::numeric_limits<uchar>().min();
        }
    }
}