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

#ifndef __VECTOR_FIELD_GEN_HPP__
#define __VECTOR_FIELD_GEN_HPP__

#include <opencv2/opencv.hpp>

namespace pencil
{
    class vector_field_gen
    {
        public:
            vector_field_gen(cv::Mat& src);
            virtual ~vector_field_gen();
            cv::Mat2f& vector_field();
            
        protected:
            virtual void _process(const cv::Mat& src, const cv::Mat& seg, cv::Mat2f& vector_field);
            virtual void _fill_vector(const cv::Mat& src, const cv::Mat& seg, cv::Mat2f& vector_field, const uint idx);
            virtual void _tweak_frequency_spectrum(cv::Mat& freq);

        private:
            cv::Mat2f _vector_field;
    };
}

#endif