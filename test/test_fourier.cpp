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

#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

float _get_delta(const cv::Point2f& point, const cv::Vec2f& vec);
double _calcMedian(std::vector<uint> scores);

int
main(int argc, char** argv)
{
    Mat src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    Mat padded;

    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT,
                   Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    split(complexImg, planes);

    ///DEBUG

    Mat mag, angle;
    cv::cartToPolar(planes[0], planes[1], mag, angle);
    // cout << mag.t() << endl;
    // cout << "\nangle: " << angle.t() * 180.0 / CV_PI << endl;

    std::map<size_t, double> magmap;

    for(size_t i = 0; i < 360; i++) {
        magmap[i] = 0.0;
    }

    Mat ccc = angle.t() * 180.0 / CV_PI;

    for(int i = 0; i < 720; i++) {
        for(int j = 0; j < 720; j++) {
            // cout << std::round(ccc.at<float>(i, j)) << endl;
            size_t ang = std::round(ccc.at<float>(i, j));
            magmap[ang] += mag.at<float>(i, j); 
        }
    }

    for(size_t i = 0; i < 360; i++) {
        cout << i << " : " << magmap[i] << endl;
    }

    ///DEBUG
    return 0;
    

    magnitude(planes[0], planes[1],
              planes[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    Mat magI = planes[0];
    magI += Scalar::all(1); //magI = log(1+planes[0])
    log(magI, magI);

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    const int cx = magI.cols / 2;
    const int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX);

    // imshow("test", magI);
    // waitKey();

    imwrite("tmp.png", magI * 255);
    magI = imread("tmp.png", CV_LOAD_IMAGE_GRAYSCALE);


    cv::fastNlMeansDenoising(magI, magI, 14);
    Mat newimg(magI.size(), magI.type());
    float alpha = 12;
    float beta = -1500;

    for(int i = 0 ; i < magI.rows; i++) {
        for(int j = 0; j < magI.cols; j++) {
            magI.at<uchar>(i, j) = saturate_cast<uchar>(magI.at<uchar>(i,
                                                                       j) * alpha + beta);
        }
    }

    // cv::fastNlMeansDenoising(magI, magI, 15.0);

    imshow("spectrum", magI);
    waitKey();

    /* experiment */

    Mat veri = Mat(magI.rows, magI.cols, CV_8UC3, Scalar(0));
    const float max_length = std::min(magI.rows / 2, magI.cols / 2);
    size_t total = 180;
    vector<uint> angles(total);
    const float uint_inc = 180.0 / static_cast<float>(total);

    for(float i = 0; i < total; i += uint_inc) {
        Point2f point(cx + 0.5, cy + 0.5);
        Point2f basic(cx + 0.5, cy + 0.5);
        Vec2f vec(cos(uint_inc * i * M_PI / 180), sin(uint_inc * i * M_PI / 180));
        uint energy = magI.at<uchar>(cx, cy);

        // Add forward streamline energy
        while(true) {
            float delta = _get_delta(point, vec);
            point.x += vec.val[0] * delta;
            point.y += vec.val[1] * delta;

            float dst = cv::norm(basic - point);
            if(dst > max_length)
                break;

            if(point.x < magI.cols && point.x >= 0 && point.y < magI.rows && point.y >= 0) {
                energy += magI.at<uchar>(floor(point.x), floor(point.y));
                Point _p = Point(floor(point.x), floor(point.y));
                cv::line(veri, _p, _p, Scalar(255, 0, 0), 1);
            }
            else {
                break;
            }
        }

        point = Point2f(cx + 0.5, cy + 0.5);
        vec *= -1;
        // Add backward streamline energy
        while(true) {
            float delta = _get_delta(point, vec);
            point.x += vec.val[0] * delta;
            point.y += vec.val[1] * delta;

            float dst = cv::norm(basic - point);
            if(dst > max_length)
                break;

            if(point.x < magI.cols && point.x >= 0 && point.y < magI.rows && point.y >= 0) {
                energy += magI.at<uchar>(floor(point.x), floor(point.y));
                Point _p = Point(floor(point.x), floor(point.y));
                cv::line(veri, _p, _p, Scalar(255, 0, 0), 1);
            }
            else {
                break;
            }
        }

        angles[i] = energy;
    }

    double avg = 0;
    double max = 0;
    double sel_angle = 0;
    for(size_t i = 0; i < total; i++) {
        printf("angle %.2f : %-u\n", 180 - i * uint_inc, angles[i]);
        avg += angles[i];
        if(angles[i] > max) {
            max = angles[i];
            sel_angle = 180 - i * uint_inc;
        }
    }

    avg /= total;
    // avg = _calcMedian(angles);
    cout << "Average : " << avg << endl;
    cout << "Max : " << max << endl;
    cout << "Angle : " << sel_angle << endl;
    cout << "Threshold factor : " << 1 - avg / max  << endl;

    imshow("spectrum", veri);
    waitKey();

    /* experiment */

    return 0;
}

double
_calcMedian(std::vector<uint> scores)
{
    double median;
    size_t size = scores.size();

    sort(scores.begin(), scores.end());

    if (size  % 2 == 0) {
        median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
    }
    else {
        median = scores[size / 2];
    }

    return median;
}