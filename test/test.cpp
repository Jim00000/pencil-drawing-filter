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

#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc/segmentation.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

static void
help()
{
    std::cout << std::endl <<
              "Usage:" << std::endl <<
              "./ssearch input_image (f|q)" << std::endl <<
              "f=fast, q=quality" << std::endl <<
              "Use l to display less rects, m to display more rects, q to quit" << std::endl;
}


int
main(int argc, char** argv)
{
    // speed-up using multithreads
    setUseOptimized(true);
    setNumThreads(4);

    // read image
    Mat src = imread(argv[1]);
    cvtColor(src, src, CV_BGR2GRAY);
    Mat dst;
    Mat output(src.size(), CV_8UC3);

    // Ptr<GraphSegmentation> gs = createGraphSegmentation(0.6, 3000.0f, 150);
    Ptr<GraphSegmentation> gs = createGraphSegmentation(0.3, 3000.0f, 150);
    gs->processImage(src, dst);

    double min = 0, max = 0;
    cv::minMaxLoc(dst, &min, &max);
    vector<Vec3b> colors(max);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,255);

    for(size_t i = 0; i < max ; i++) {
        int r = distribution(generator);
        int g = distribution(generator);
        int b = distribution(generator);
        colors[i] = Vec3b(r, g, b);
    }

    for(uint i = 0; i < dst.rows; i++) {
        for(uint j = 0; j < dst.cols; j++) {
            int color_idx = dst.at<int>(i, j);
            Vec3b color = colors[color_idx];
            output.at<Vec3b>(i, j) = color;
        }
    }

    imshow("test", output);
    waitKey(0);

    return 0;
}