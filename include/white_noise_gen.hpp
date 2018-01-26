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

#ifndef __WHITE_NOISE_GEN_HPP__
#define __WHITE_NOISE_GEN_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class white_noise_gen
    {
    public:
        white_noise_gen(cv::Mat& src, float k = 1.0);
        virtual ~white_noise_gen();

        cv::Mat& result();

    protected:

    private:
        cv::Mat _result;
        float _k;
    };
}

#endif