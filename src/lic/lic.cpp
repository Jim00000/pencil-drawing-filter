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
#include <cmath>
#include <limits>
#include <exception>
#include <omp.h>
#include "lic/lic.hpp"

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

using namespace pencil;

void _process(const cv::Mat& texture, const cv::Mat2f& vector,
              const std::vector<float>& kernel,
              cv::Mat& result);

float _get_delta(const cv::Point2f& point, const cv::Vec2f& vec);
float _get_delta(const float px, const float py, const float vx,
                 const float vy);

LIC::LIC(cv::Mat& texture, cv::Mat2f& vector, std::vector<float>& kernel)
{
    _texture = texture.clone();
    _vector = vector.clone();
    _kernel = kernel;
    _result = cv::Mat::zeros(texture.size(), texture.type());

    // Tweak the vector (This is a bug need to be fixed)
    // for(size_t i = 0; i < _vector.rows; i++) {
    //     for(size_t j = 0; j < _vector.cols; j++) {
    //         float x = _vector.at<cv::Vec2f>(i, j).val[0];
    //         float y = _vector.at<cv::Vec2f>(i, j).val[1];
    //         _vector.at<cv::Vec2f>(i, j).val[0] = y;
    //         _vector.at<cv::Vec2f>(i, j).val[1] = x;
    //     }
    // }

    _process(_texture, _vector, _kernel, _result);
}

LIC::~LIC()
{

}

cv::Mat&
LIC::result()
{
    return _result;
}

void
_process(const cv::Mat& texture, const cv::Mat2f& vector,
         const std::vector<float>& kernel,
         cv::Mat& result)
{
    uint height = static_cast<uint>(texture.cols);
    uint width = static_cast<uint>(texture.rows);

    // #pragma omp parallel for collapse(2)
    for(uint i = 0; i < height; i++) {
        for (uint j = 0; j < width; j++) {
            const uint x = j;
            const uint y = i;
            float forward_h = 0, backward_h = 0;
            float foward_val = 0, backward_val = 0;
            cv::Point2f point;
            cv::Vec2f vec;

            // Calculate forward stream line
            point = cv::Point2f(x + 0.5f, y + 0.5f);
            vec = vector.at<cv::Vec2f>(x, y);

            for(uint k = 0; k < kernel.size(); k++) {
                float delta = _get_delta(point, vec);
                if(delta == std::numeric_limits<float>::infinity()) {
                    break;
                }
                cv::Vec2f norm_v2 = cv::normalize(vec);
                cv::Vec2f mov = norm_v2 * delta;
                point.x = point.x + mov.val[0];
                point.y = point.y + mov.val[1];

                if(point.x >= width) {
                    break;
                } else if(point.x < 0) {
                    break;
                }

                if(point.y >= height) {
                    break;
                } else if(point.y < 0) {
                    break;
                }


                foward_val = foward_val + texture.at<uchar>(floor(point.x),
                                                            floor(point.y)) * delta;
                vec = vector.at<cv::Vec2f>(floor(point.x), floor(point.y));
                forward_h += delta;


            }

            // Calculate backward stream line
            point = cv::Point2f(x + 0.5f, y + 0.5f);
            vec = vector.at<cv::Vec2f>(x, y) * -1;

            for(uint k = 0; k < kernel.size(); k++) {
                float delta = _get_delta(point, vec);
                if(delta == std::numeric_limits<float>::infinity()) {
                    break;
                }
                cv::Vec2f norm_v2 = cv::normalize(vec);
                cv::Vec2f mov = norm_v2 * delta;
                point.x = point.x + mov.val[0];
                point.y = point.y + mov.val[1];

                if(point.x >= width) {
                    break;
                } else if(point.x < 0) {
                    break;
                }

                if(point.y >= height) {
                    break;
                } else if(point.y < 0) {
                    break;
                }

                // std::cout << floor(point.x) << ", " << floor(point.y) << std::endl;
                backward_val = backward_val + texture.at<uchar>(floor(point.x),
                                                                floor(point.y)) * delta;
                vec = vector.at<cv::Vec2f>(floor(point.x), floor(point.y)) * -1;
                backward_h += delta;

            }

            // Computation
            float h = forward_h + backward_h;
            if(h == 0) {
                result.at<uchar>(j, i) = 255;
            } else {
                float new_pixel_value = (foward_val + backward_val) / h;
                result.at<uchar>(j, i) = round(new_pixel_value);
            }
        }
    }
}

float
_get_delta(const cv::Point2f& point, const cv::Vec2f& vec)
{
    return _get_delta(point.x, point.y, vec.val[0], vec.val[1]);
}

float
_get_delta(const float px, const float py, const float vx, const float vy)
{
    const float left_bound = floor(px);
    const float right_bound = left_bound + 1;
    const float bottom_bound = floor(py);
    const float top_bound = bottom_bound + 1;

    float dtop = fabs(top_bound - py);
    float dbottom = fabs(py - bottom_bound);
    float dleft = fabs(px - left_bound);
    float dright = fabs(right_bound - px);

    dtop = (dtop == 0) ? 1 : dtop;
    dbottom = (dbottom == 0) ? 1 : dbottom;
    dleft = (dleft == 0) ? 1 : dleft;
    dright = (dright == 0) ? 1 : dright;

    const cv::Vec2f vec = cv::Vec2f(vx, vy);

    if(vx > 0) {
        // Move right
        const float horizontal_moving_time = fabs(dright / vx);

        if(vy > 0) {
            // Move right and up
            const float vertical_moving_time = fabs(dtop / vy);

            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);

            } else {
                return horizontal_moving_time * cv::norm(vec);
            }

        } else if(vy < 0) {
            // Move down and right
            const float vertical_moving_time = fabs(dbottom / vy);

            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);

            } else {
                return horizontal_moving_time * cv::norm(vec);
            }

        } else {
            // Move right directly
            return dright;
        }

    } else if(vx < 0) {
        // Move left
        const float horizontal_moving_time = fabs(dleft / vx);

        if(vy > 0) {
            // Move left and up
            const float vertical_moving_time = fabs(dtop / vy);

            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);

            } else {
                return horizontal_moving_time * cv::norm(vec);
            }

        } else if(vy < 0) {
            // Move left and down
            const float vertical_moving_time = fabs(dbottom / vy);

            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);

            } else {
                return horizontal_moving_time * cv::norm(vec);
            }

        } else {
            // Move left directly
            return dleft;
        }

    } else {
        // Can not move horizontal direction
        if(vy > 0) {
            // Move up
            return dtop;

        } else if(vy < 0) {
            // Move down
            return dbottom;

        } else {
            // Cannot not move anyway
            return std::numeric_limits<float>::infinity();
        }
    }

    throw std::runtime_error("Critical error occurred (info : control flow cannot reach here)");
}