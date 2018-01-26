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
#include "combiner.hpp"

using namespace cv;
using namespace pencil;

int main()
{
    Mat paper = imread("/home/jim/Github/pencil-drawing-filter/resources/paper.png");
    cvtColor(paper, paper, CV_BGR2GRAY);
    Mat dst;
    combiner::extend_file(paper, dst, Size(600, 600));

    imshow("test", dst);
    waitKey();
    
    return 0;
}