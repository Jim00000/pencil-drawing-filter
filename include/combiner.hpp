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

#ifndef __COMBINER_HPP__
#define __COMBINER_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class combiner
    {
        public:
            combiner();
            virtual ~combiner();

            static void combine_foreground_and_background(const cv::Mat& foreground, const cv::Mat& background, cv::Mat& result);
            static void combine_image_by_weight(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, const float weight = 0.5);
            static void extend_file(const cv::Mat& src, cv::Mat& dst, const cv::Size size);

        protected:

        private:
    };
}

#endif