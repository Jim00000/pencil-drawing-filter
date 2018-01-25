#ifndef __VECTOR_FIELD_GEN_HPP__
#define __VECTOR_FIELD_GEN_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class vector_field_gen
    {
        public:
            vector_field_gen(cv::Mat& src);
            virtual ~vector_field_gen();
            cv::Mat2f& vector_field();
            
        protected:
            virtual void _process(const cv::Mat& src, const cv::Mat& seg, cv::Mat2f& vector_field);
            virtual void _fill_vector(const cv::Mat& src, const cv::Mat& seg, cv::Mat2f& vector_field, const uint idx);
            virtual void _tweak_frequency_spectrum(cv::Mat& freq);

        private:
            cv::Mat2f _vector_field;
    };
}

#endif