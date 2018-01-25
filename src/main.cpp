#include <iostream>
#include <limits>
#include <random>
#include "lic/lic.hpp"
#include "white_noise_gen.hpp"
#include "vector_field_gen.hpp"

using namespace std;
using namespace pencil;
using namespace cv;

int
main(int argc, char* argv[])
{

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);

    // Load texture
    // Mat texture = Mat(500, 500, CV_8U);
    
    // for(size_t i = 0; i < texture.rows; i++) {
    //     for(size_t j = 0; j < texture.cols; j++) {
    //         int rand = distribution(generator);
    //         texture.at<uchar>(i, j) = rand * 255;
    //     }
    // }

    // Load image
    Mat texture = imread("/home/jim/Github/pencil-drawing-filter/build/sample3.jpg");
    cvtColor(texture, texture, CV_BGR2GRAY);

    // Generate white noise image
    white_noise_gen wh_gen(texture, 0.85);

    // Generate vector matrix
    // Mat2f vector = Mat2f(texture.size());
    // vector.setTo(cv::normalize(Vec2f(0, 50)));

    // Generate kernel vector
    std::vector<float> kernel(20);
    std::fill(kernel.begin(), kernel.end(), 1);

    // Generate vector field from an image
    vector_field_gen vf(texture);

    LIC lic(wh_gen.result(), vf.vector_field(), kernel);

    imshow("test", lic.result());
    waitKey(0);


    return 0;
}