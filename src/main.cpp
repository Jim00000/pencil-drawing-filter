#include <iostream>
#include <limits>
#include <random>
#include "lic/lic.hpp"

using namespace std;
using namespace pencil;
using namespace cv;

int
main(int argc, char* argv[])
{

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);

    // Load texture
    Mat texture = Mat(500, 300, CV_8U);
    
    for(size_t i = 0; i < texture.rows; i++) {
        for(size_t j = 0; j < texture.cols; j++) {
            int rand = distribution(generator);
            texture.at<uchar>(i, j) = rand * 255;
        }
    }

    // Load image
    // Mat texture = imread("/home/jim/Github/pencil-drawing-filter/sample.jpg");
    // cvtColor(texture, texture, CV_BGR2GRAY);

    // Generate vector matrix
    Mat2f vector = Mat2f(texture.size());
    vector.setTo(cv::normalize(Vec2f(50, 50)));

    // Generate kernel vector
    std::vector<float> kernel(20);
    std::fill(kernel.begin(), kernel.end(), 1);

    LIC lic(texture, vector, kernel);

    imshow("test", lic.result());
    waitKey(0);
    // imwrite ("test.jpg", lic.result());


    return 0;
}