/*
    pencil-drawing-filter project implements an pencil drawing filter
    Copyright (C) 2018  Jim00000

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    Mat texture = imread("/home/jim/Github/pencil-drawing-filter/build/output/sample5.jpg");
    cvtColor(texture, texture, CV_BGR2GRAY);

    // Generate white noise image
    white_noise_gen wh_gen(texture, 0.8);

    // Generate kernel vector
    std::vector<float> kernel(8);
    std::fill(kernel.begin(), kernel.end(), 1);

    // Generate vector field from an image
    vector_field_gen vf(texture);

    LIC lic(wh_gen.result(), vf.vector_field(), kernel);

    border edge(texture);

    Mat drawing, output;
    combiner::combine_foreground_and_background(edge.result() ,lic.result(), drawing);
    drawing.convertTo(drawing, CV_8UC1);

    Mat paper = imread("/home/jim/Github/pencil-drawing-filter/resources/paper.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat ext_paper;
    combiner::extend_file(paper, ext_paper, drawing.size());
    combiner::combine_image_by_weight(drawing, ext_paper, output, 0.75);

    imshow("test", output);
    waitKey(0);


    return 0;
}