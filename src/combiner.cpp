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
#include <exception>
#include "combiner.hpp"

using namespace cv;
using namespace pencil;

inline void _matrix_combine(const Mat& src, Mat& dst, const size_t start_x,
                            const size_t start_y, const size_t width,
                            const size_t height) __attribute__((always_inline));

combiner::combiner()
{

}

combiner::~combiner()
{

}

void
combiner::combine_foreground_and_background(const Mat& foreground,
                                            const Mat& background, Mat& result)
{
    if(foreground.size() != background.size()) {
        throw std::runtime_error("Argument `foreground` and `background` has different size");
    }

    result = Mat(foreground.size(), CV_8U);

    cv::bitwise_not(foreground, foreground);
    cv::bitwise_not(background, background);

    for(size_t i = 0; i < foreground.rows; i++) {
        for(size_t j = 0; j < foreground.cols; j++) {
            result.at<uchar>(i, j) = saturate_cast<uchar>(background.at<uchar>(i,
                                                                               j) + foreground.at<uchar>(i, j));
        }
    }

    cv::bitwise_not(foreground, foreground);
    cv::bitwise_not(background, background);
    cv::bitwise_not(result, result);

}

void
combiner::combine_image_by_weight(const cv::Mat& src1, const cv::Mat& src2,
                                  cv::Mat& dst, const float weight)
{
    if(src1.size() != src2.size()) {
        throw std::runtime_error("Argument `src1` and `src2` has different size");
    }

    dst = Mat(src1.size(), CV_8U);
    dst = src1 * weight + src2 * (1.0f - weight);
}

void
combiner::extend_file(const Mat& src, Mat& dst, const Size size)
{
    dst = Mat(size, src.type());
    Size unit_size = src.size();
    float horiz_scale = static_cast<float>(size.width) / static_cast<float>
                        (unit_size.width);
    float verti_scale = static_cast<float>(size.height) / static_cast<float>
                        (unit_size.height);
    if(verti_scale <= 1.0 && horiz_scale <= 1.0) {
        dst = Mat(src, Rect(0, 0, size.width, size.height));
    } else if(verti_scale <= 1.0 && horiz_scale >= 1.0) {
        const float horiz_step = unit_size.width;
        float x = 0;
        for(float i = horiz_scale; i > 1; i -= 1) {
            _matrix_combine(src, dst, x, 0, unit_size.width, size.height);
            x += horiz_step;
        }

        // Add the last part
        const float horiz_left = size.width - x;
        _matrix_combine(src, dst, x, 0, horiz_left, size.height);

    } else if(verti_scale >= 1.0 && horiz_scale <= 1.0) {
        const float verti_step = unit_size.height;
        float y = 0;
        for(float j = verti_scale; j > 1; j -= 1) {
            _matrix_combine(src, dst, 0, y, size.width, unit_size.height);
            y += verti_step;
        }

        // Add the last part
        const float verti_left = size.height - y;
        _matrix_combine(src, dst, 0, y, size.width, verti_left);

    } else {
        const float horiz_step = unit_size.width;
        const float verti_step = unit_size.height;
        float x = 0, y = 0;
        for(float i = horiz_scale; i > 1; i -= 1) {
            for(float j = verti_scale, yy = 0; j > 1; j -= 1, yy += verti_step) {
                _matrix_combine(src, dst, x, yy, unit_size.width, unit_size.height);
            }
            x += horiz_step;
        }

        // Add the last part
        y = verti_step * floor(verti_scale);
        const float horiz_left = size.width - x;
        const float verti_left = size.height - y;

        for(float i = horiz_scale, xx = 0; i > 1; i -= 1, xx += horiz_step) {
            _matrix_combine(src, dst, xx, y, unit_size.width, verti_left);
        }

        for(float j = verti_scale, yy = 0; j > 1; j -= 1, yy += verti_step) {
            _matrix_combine(src, dst, x, yy, horiz_left, unit_size.height);
        }

        _matrix_combine(src, dst, x, y, horiz_left, verti_left);

    }
}

void
_matrix_combine(const Mat& src, Mat& dst, const size_t start_x,
                const size_t start_y, const size_t width, const size_t height)
{
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            dst.at<uchar>(start_y + i, start_x + j) = src.at<uchar>(i, j);
        }
    }
}