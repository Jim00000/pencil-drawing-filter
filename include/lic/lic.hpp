#ifndef __LIC_HPP__
#define __LIC_HPP__

#include <vector>
#include <opencv2/opencv.hpp>

namespace pencil
{
    /**
     * Line integral convolution
     */
    class LIC
    {
    public:
        LIC(cv::Mat& texture, cv::Mat2f& vector, std::vector<float>& kernel);
        virtual ~LIC();

        cv::Mat& result();

    protected:

    private:
        cv::Mat _texture;
        cv::Mat2f _vector;
        std::vector<float> _kernel;
        cv::Mat _result;
    };
}

#endif