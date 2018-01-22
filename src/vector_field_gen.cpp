#include "vector_field_gen.hpp"

using namespace std;
using namespace cv;
using namespace pencil;

inline float _m(Mat& gray, size_t p, size_t q) __attribute__((always_inline));

vector_field_gen::vector_field_gen(Mat& src)
{

}

vector_field_gen::~vector_field_gen()
{

}

cv::Mat&
vector_field_gen::vector_field()
{
    return _vector_field;
}

float _m(Mat& gray, uint p, uint q)
{
    float m = 0.0f;
    for(size_t i = 0; i < gray.rows; i++) { 
        for(size_t j = 0; j < gray.cols; j++) {
            m += 
        }
    }
}