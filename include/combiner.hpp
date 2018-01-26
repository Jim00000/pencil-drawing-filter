#ifndef __COMBINER_HPP__
#define __COMBINER_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class combiner
    {
        public:
            combiner();
            virtual ~combiner();

            static void combine_foreground_and_background(const cv::Mat& foreground, const cv::Mat& background, cv::Mat& result);
            static void combine_image_by_weight(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, const float weight = 0.5);
            static void extend_file(const cv::Mat& src, cv::Mat& dst, const cv::Size size);

        protected:

        private:
    };
}

#endif