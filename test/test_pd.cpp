#include <iostream>
#include <limits>
#include <random>
#include "lic/lic.hpp"
#include "white_noise_gen.hpp"
#include "vector_field_gen.hpp"
#include "border.hpp"
#include "combiner.hpp"

using namespace std;
using namespace pencil;
using namespace cv;

int
main(int argc, char* argv[])
{
    // Load image
    Mat texture = imread("/home/jim/Github/pencil-drawing-filter/sample.jpg");
    cvtColor(texture, texture, CV_BGR2GRAY);

    // Generate white noise image
    white_noise_gen wh_gen(texture, 0.8);

    // Generate kernel vector
    std::vector<float> kernel(5);
    std::fill(kernel.begin(), kernel.end(), 1);

    // Generate vector field from an image
    vector_field_gen vf(texture);

    LIC lic(wh_gen.result(), vf.vector_field(), kernel);

    border edge(texture);

    Mat drawing;
    combiner::combine_foreground_and_background(edge.result() ,lic.result(), drawing);

    imshow("test", drawing);
    waitKey(0);


    return 0;
}