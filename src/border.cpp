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

#include "border.hpp"
#include "edge_feature_filter.hpp"

using namespace std;
using namespace cv;
using namespace pencil;

border::border(Mat& src)
{
    _result = Mat(src.size(), CV_8U);
    _process(src, _result);
}

border::~border()
{

}

void
border::_process(Mat& src, Mat& result)
{
    sketch::edge_feature_filter edge(3, 210);
    edge.process(src, result);
    cv::bitwise_not(result, result);
    result *= 0.2;
    cv::bitwise_not(result, result);

    // float ksize = 5;
    // GaussianBlur(src, src, Size(ksize, ksize), 0, 0);
    
    // Mat grad_x, grad_y;
    // Sobel(src, grad_x, CV_16S, 1, 0, 1, ksize, 0, BORDER_DEFAULT);
    // convertScaleAbs(grad_x, grad_x);
    // Sobel(src, grad_y, CV_16S, 0, 1, 1, ksize, 0, BORDER_DEFAULT );
    // convertScaleAbs(grad_y, grad_y);

    // addWeighted(grad_x, 0.5, grad_y, 0.5, 0, result);
    // threshold(result, result, 80, 255, THRESH_BINARY | THRESH_OTSU);

    // result *= 0.1;
    // // Reverse color (=> color = 255 - color for each pixel)
    // cv::bitwise_not(result, result);
}

Mat&
border::result()
{
    return _result;
}