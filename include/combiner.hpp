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

        protected:

        private:
    };
}

#endif