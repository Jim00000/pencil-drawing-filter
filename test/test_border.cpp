#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int
main(int argc, char* argv[])
{
    Mat src = imread("/home/jim/Github/pencil-drawing-filter/sample.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    float ksize = 5;
    GaussianBlur(src, src, Size(ksize, ksize), 0, 0);

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(src, grad_x, CV_16S, 1, 0, 1, ksize, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    Sobel(src, grad_y, CV_16S, 0, 1, 1, ksize, 0, BORDER_DEFAULT );
    convertScaleAbs(grad_y, abs_grad_y);

    Mat dst1, dst2;
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst1);
    threshold(dst1, dst2, 80, 255, THRESH_BINARY | THRESH_OTSU);
    cv::bitwise_not(dst2, dst2);
    imshow("Sobel_2", dst2);
    waitKey(0);

    return 0;
}