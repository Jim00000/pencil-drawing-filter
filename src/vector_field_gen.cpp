#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>
#include "opencv2/ximgproc/segmentation.hpp"
#include "vector_field_gen.hpp"

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;
using namespace pencil;

// definition of get_delta comes from lic.cpp
float _get_delta(const cv::Point2f& point, const cv::Vec2f& vec);
void _get_frequency_spectrum(const Mat& src, Mat& dst);
bool _analyze_maximal_energy(const Mat src, double* angle);
void _fill(const float vx, const float vy, Mat2f& vector_field, const Mat& seg,
           const uint idx);
float _m_pq(const Mat& gray, const Mat& seq, const size_t k, const size_t p,
            const size_t q);
float _u_pq(const Mat& gray, const Mat& seq, const size_t k, const size_t p,
            const size_t q);

vector_field_gen::vector_field_gen(Mat& src)
{
    setUseOptimized(true);
    setNumThreads(4);
    _vector_field = Mat2f(src.size());
    Mat segmentation;

    // Ptr<GraphSegmentation> gs = createGraphSegmentation(0.6, 3000.0f, 150);
    Ptr<GraphSegmentation> gs = createGraphSegmentation(0.4, 3000.0f, 150);
    gs->processImage(src, segmentation);

    _process(src, segmentation, _vector_field);
}

vector_field_gen::~vector_field_gen()
{

}

cv::Mat2f&
vector_field_gen::vector_field()
{
    return _vector_field;
}

void
vector_field_gen::_process(const Mat& src, const Mat& seg, Mat2f& vector_field)
{
    double maxvalue_d;
    cv::minMaxLoc(seg, nullptr, &maxvalue_d);
    size_t maxvalue = static_cast<size_t>(maxvalue_d);

    #pragma omp parallel for shared(src, seg, vector_field)
    for(size_t k = 0; k <= maxvalue; k++) {
        Mat buffer = src.clone();

        for(size_t i = 0; i < src.rows; i++) {
            for(size_t j = 0; j < src.cols; j++) {
                if(seg.at<int>(i, j) != k) {
                    buffer.at<uchar>(i, j) = 0;
                }
            }
        }

        _fill_vector(buffer, seg, vector_field, k);
    }
}

void
vector_field_gen::_fill_vector(const cv::Mat& src, const cv::Mat& seg,
                               cv::Mat2f& vector_field, const uint idx)
{
    // Get the image's frequency spectrum
    Mat freq;
    _get_frequency_spectrum(src, freq);
    // Convert from double type to uchar type
    freq.convertTo(freq, CV_8UC1, 255);
    // imshow("test", freq);
    // waitKey();
    _tweak_frequency_spectrum(freq);
    double angle = 0;
    bool explicit_direction = _analyze_maximal_energy(freq, &angle);
    // imshow("test", freq);
    // waitKey();
    if(explicit_direction == true) {
        float theta = angle * M_PI / 180;
        float vx = cos(theta);
        float vy = sin(theta);
        _fill(vx, vy, vector_field, seg, idx);
    } else {
        float theta = 0.5f * atan(2 * _u_pq(src, seg, idx, 1, 1) / ( _u_pq(src, seg,
                                                                           idx, 2, 0) - _u_pq(src, seg, idx, 0, 2) ) );
        float vx = cos(theta);
        float vy = sin(theta);
        _fill(vx, vy, vector_field, seg, idx);
    }
}


void
vector_field_gen::_tweak_frequency_spectrum(cv::Mat& freq)
{
    cv::fastNlMeansDenoising(freq, freq, 14);
    Mat newimg(freq.size(), freq.type());
    float alpha = 12;
    float beta = -1500;

    for(int i = 0 ; i < freq.rows; i++) {
        for(int j = 0; j < freq.cols; j++) {
            freq.at<uchar>(i, j) = saturate_cast<uchar>(freq.at<uchar>(i,
                                                                       j) * alpha + beta);
        }
    }
}

void
_get_frequency_spectrum(const Mat& src, Mat& dst)
{
    Mat padded;
    const int m = getOptimalDFTSize(src.rows);
    const int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT,
                   Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    split(complexImg, planes);

    magnitude(planes[0], planes[1], planes[0]);
    dst = planes[0];
    dst += Scalar::all(1);
    log(dst, dst);

    dst = dst(Rect(0, 0, dst.cols & -2, dst.rows & -2));

    const int cx = dst.cols / 2;
    const int cy = dst.rows / 2;

    Mat q0(dst, Rect(0, 0, cx, cy));
    Mat q1(dst, Rect(cx, 0, cx, cy));
    Mat q2(dst, Rect(0, cy, cx, cy));
    Mat q3(dst, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(dst, dst, 0, 1, CV_MINMAX);
}

float
_m_pq(const Mat& gray, const Mat& seq, const size_t k, const size_t p,
      const size_t q)
{
    float sum = 0;
    for(size_t i = 0; i < gray.rows; i++) {
        for(size_t j = 0; j < gray.cols; j++) {
            if(seq.at<int>(i, j) == k) {
                sum += pow(j, p) * pow(i, q) * gray.at<uchar>(i, j);
            }
        }
    }

    return sum;
}

float
_u_pq(const Mat& gray, const Mat& seq, const size_t k, const size_t p,
      const size_t q)
{
    const float denum = _m_pq(gray, seq, k, 0, 0);
    const float xc = _m_pq(gray, seq, k, 1, 0) / denum;
    const float yc = _m_pq(gray, seq, k, 0, 1) / denum;
    float sum = 0;
    for(size_t i = 0; i < gray.rows; i++) {
        for(size_t j = 0; j < gray.cols; j++) {
            if(seq.at<int>(i, j) == k) {
                sum += pow(j - xc, p) * pow(i - yc, q) * gray.at<uchar>(i, j);
            }
        }
    }

    return sum;
}

bool
_analyze_maximal_energy(const Mat src, double* angle)
{
    // Prepare data
    const float max_length = std::min(src.rows / 2, src.cols / 2);
    size_t total = 180;
    vector<uint> angles(total);
    const float uint_inc = 180.0 / static_cast<float>(total);

    const int cx = src.cols / 2;
    const int cy = src.rows / 2;

    for(float i = 0; i < total; i += uint_inc) {
        Point2f point(cx + 0.5, cy + 0.5);
        Point2f basic(cx + 0.5, cy + 0.5);
        Vec2f vec(cos(uint_inc * i * M_PI / 180), sin(uint_inc * i * M_PI / 180));
        uint energy = src.at<uchar>(cx, cy);

        // Add forward streamline energy
        while(true) {
            float delta = _get_delta(point, vec);
            point.x += vec.val[0] * delta;
            point.y += vec.val[1] * delta;

            float dst = cv::norm(basic - point);
            if(dst > max_length)
                break;

            if(point.x < src.cols && point.x >= 0 && point.y < src.rows && point.y >= 0) {
                energy += src.at<uchar>(floor(point.x), floor(point.y));
            } else {
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

            if(point.x < src.cols && point.x >= 0 && point.y < src.rows && point.y >= 0) {
                energy += src.at<uchar>(floor(point.x), floor(point.y));
            } else {
                break;
            }
        }

        angles[i] = energy;

    }

    // Compute score
    double avg = 0;
    double max = 0;
    double sel_angle = 0;
    for(size_t i = 0; i < total; i++) {
        avg += angles[i];
        if(angles[i] > max) {
            max = angles[i];
            sel_angle = 180 - i * uint_inc;
        }
    }

    avg /= total;
    double factor = 1 - avg / max;
    cout << "Average : " << avg << endl;
    cout << "Max : " << max << endl;
    cout << "Angle : " << sel_angle << endl;
    cout << "Threshold factor : " << factor << endl;

    *angle = sel_angle + 90;
    *angle = (*angle >= 360) ? *angle - 360 : *angle;

    return (factor >= 0.5) ? true : false;
}

void
_fill(const float vx, const float vy, Mat2f& vector_field, const Mat& seg,
      const uint idx)
{
    for(size_t i = 0; i < seg.rows; i++) {
        for(size_t j = 0; j < seg.cols; j++) {
            if(seg.at<int>(i, j) == idx) {
                vector_field.at<Vec2f>(i, j).val[0] = vx;
                vector_field.at<Vec2f>(i, j).val[1] = vy;
            }
        }
    }
}