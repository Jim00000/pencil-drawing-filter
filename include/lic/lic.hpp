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

#ifndef __LIC_HPP__
#define __LIC_HPP__

#include <vector>
#include <opencv2/opencv.hpp>

namespace pencil
{
    /**
     * Line integral convolution
     */
    class LIC
    {
    public:
        LIC(cv::Mat& texture, cv::Mat2f& vector, std::vector<float>& kernel);
        virtual ~LIC();

        cv::Mat& result();

    protected:

    private:
        cv::Mat _texture;
        cv::Mat2f _vector;
        std::vector<float> _kernel;
        cv::Mat _result;
    };
}

#endif