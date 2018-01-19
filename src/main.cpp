#include <iostream>
#include <limits>
#include "lic/lic.hpp"

using namespace std;
using namespace pencil;
using namespace cv;

int
main(int argc, char* argv[])
{
    // Load texture
    Mat texture = imread("/home/jim/Github/pencil-drawing-filter/white-noise.jpg");
    Mat gray;
    cv::cvtColor(texture, gray, CV_BGR2GRAY);

    // Generate vector matrix
    Mat2f vector = Mat2f(gray.rows, gray.cols);
    float x = 50;
    vector.setTo(Vec2f(1, -1));

    // Generate kernel vector
    std::vector<float> kernel(50);
    std::fill(kernel.begin(), kernel.end(), 1);

    LIC lic(gray, vector, kernel);

    imshow("test", lic.result());
    waitKey(0);


    return 0;
}