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
            cv::Mat& vector_field();
            
        protected:

        private:
            cv::Mat2f _vector_field;
    };
}

#endif