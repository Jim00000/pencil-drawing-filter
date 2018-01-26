#include "border.hpp"

using namespace std;
using namespace cv;
using namespace pencil;

border::border(const Mat& src)
{
    _result = Mat(src.size(), CV_8U);
    _process(src, _result);
}

border::~border()
{

}

void
border::_process(const Mat& src, Mat& result)
{
    float ksize = 5;
    GaussianBlur(src, src, Size(ksize, ksize), 0, 0);
    
    Mat grad_x, grad_y;
    Sobel(src, grad_x, CV_16S, 1, 0, 1, ksize, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_x, grad_x);
    Sobel(src, grad_y, CV_16S, 0, 1, 1, ksize, 0, BORDER_DEFAULT );
    convertScaleAbs(grad_y, grad_y);

    addWeighted(grad_x, 0.5, grad_y, 0.5, 0, result);
    threshold(result, result, 80, 255, THRESH_BINARY | THRESH_OTSU);

    result *= 0.1;
    // Reverse color (=> color = 255 - color for each pixel)
    cv::bitwise_not(result, result);
}

Mat&
border::result()
{
    return _result;
}