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

inline float _get_delta(cv::Point2f& point,
                        cv::Vec2f& vec) __attribute__((always_inline));

inline float _get_delta_inv(cv::Point2f& point,
                        cv::Vec2f& vec) __attribute__((always_inline));


LIC::LIC(cv::Mat& texture, cv::Mat2f& vector, std::vector<float>& kernel)
{
    _texture = texture.clone();
    _vector = vector.clone();
    _kernel = kernel;
    _result = cv::Mat::zeros(texture.size(), texture.type());
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

    #pragma omp parallel for collapse(2)
    for(uint i = 0; i < height; i++) {
        for (uint j = 0; j < width; j++) {
            const uint x = j;
            const uint y = i;
            int last_x = -1, last_y = -1;
            float forward_h = 0, backward_h = 0;
            float foward_val = 0, backward_val = 0;
            cv::Point2f point;
            cv::Vec2f vec;

            // Calculate forward stream line
            point = cv::Point2f(x + 0.5f, y + 0.5f);
            vec = vector.at<cv::Vec2f>(x, y);
            for(uint k = 0; k < kernel.size(); k++) {
                float delta = _get_delta(point, vec);
                // cv::Vec2f norm_v2 = cv::normalize(vec);
                point.x = point.x + delta * vec.val[0] / cv::norm(vec);
                point.y = point.y + delta * vec.val[1] / cv::norm(vec);

                if(point.x >= width) {
                    point.x = width - 1;
                    break;
                } else if(point.x < 0) {
                    point.x = 0;
                    break;
                }

                if(point.y >= height) {
                    point.y = height - 1;
                    break;
                } else if(point.y < 0) {
                    point.y = 0;
                    break;
                }

                if(last_x == floor(point.x) && last_y == floor(point.y)) {
                    // Do nothing
                } else {
                    foward_val = foward_val + texture.at<uchar>(floor(point.x),
                                                                floor(point.y)) * kernel[k];
                    last_x = floor(point.x);
                    last_y = floor(point.y);
                    vec = vector.at<cv::Vec2f>(floor(point.x), floor(point.y));
                    forward_h += kernel[k];
                }

                
            }

            // Calculate backward stream line
            point = cv::Point2f(x + 0.5f, y + 0.5f);
            vec = vector.at<cv::Vec2f>(x, y);
            last_x = -1, last_y = -1;
            for(uint k = 0; k < kernel.size(); k++) {
                float delta = _get_delta_inv(point, vec);
                // cv::Vec2f norm_v2 = cv::normalize(vec);
                point.x = point.x - delta * vec.val[0] / cv::norm(vec);
                point.y = point.y - delta * vec.val[1] / cv::norm(vec);

                if(point.x >= width) {
                    point.x = width - 1;
                    break;
                } else if(point.x < 0) {
                    point.x = 0;
                    break;
                }

                if(point.y >= height) {
                    point.y = height - 1;
                    break;
                } else if(point.y < 0) {
                    point.y = 0;
                    break;
                }

                if(last_x == floor(point.x) && last_y == floor(point.y)) {
                    // Do nothing
                } else {
                    backward_val = backward_val + texture.at<uchar>(floor(point.x),
                                                                floor(point.y)) * kernel[k];
                    last_x = floor(point.x);
                    last_y = floor(point.y);
                    vec = vector.at<cv::Vec2f>(floor(point.x), floor(point.y));
                    backward_h += kernel[k];
                }
            }

            // Computation
            float new_pixel_value = (foward_val + backward_val) / (forward_h + backward_h);
            result.at<uchar>(j, i) = floor(new_pixel_value);
        }
    }
}

float
_get_delta(cv::Point2f& point, cv::Vec2f& vec)
{
    const float px = point.x;
    const float py = point.y;
    const float left_bound = floor(px);
    const float right_bound = left_bound + 1;
    const float bottom_bound = floor(py);
    const float top_bound = bottom_bound + 1;

    const float dtop = fabs(top_bound - py);
    const float dbottom = fabs(py - bottom_bound);
    const float dleft = fabs(px - left_bound);
    const float dright = fabs(right_bound - px);

    const float vx = vec.val[0];
    const float vy = vec.val[1];

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

float
_get_delta_inv(cv::Point2f& point, cv::Vec2f& vec)
{
    const float px = point.x;
    const float py = point.y;
    const float left_bound = floor(px);
    const float right_bound = left_bound + 1;
    const float bottom_bound = floor(py);
    const float top_bound = bottom_bound + 1;

    const float dtop = fabs(top_bound - py);
    const float dbottom = fabs(py - bottom_bound);
    const float dleft = fabs(px - left_bound);
    const float dright = fabs(right_bound - px);

    const float vx = vec.val[0];
    const float vy = vec.val[1];

    if(vx > 0) {
        // Move right
        const float horizontal_moving_time = fabs(dleft / vx);
        if(vy > 0) {
            // Move right and up
            const float vertical_moving_time = fabs(dbottom / vy);
            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);
            } else {
                return horizontal_moving_time * cv::norm(vec);
            }
        } else if(vy < 0) {
            // Move down and right
            const float vertical_moving_time = fabs(dtop / vy);
            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);
            } else {
                return horizontal_moving_time * cv::norm(vec);
            }
        } else {
            // Move right directly
            return dleft;
        }
    } else if(vx < 0) {
        // Move left
        const float horizontal_moving_time = fabs(dright / vx);
        if(vy > 0) {
            // Move left and up
            const float vertical_moving_time = fabs(dbottom / vy);
            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);
            } else {
                return horizontal_moving_time * cv::norm(vec);
            }
        } else if(vy < 0) {
            // Move left and down
            const float vertical_moving_time = fabs(dtop / vy);
            if(horizontal_moving_time > vertical_moving_time) {
                // choose vertical_moving_time
                return vertical_moving_time * cv::norm(vec);
            } else {
                return horizontal_moving_time * cv::norm(vec);
            }
        } else {
            // Move left directly
            return dright;
        }
    } else {
        // Can not move horizontal direction
        if(vy > 0) {
            // Move up
            return dbottom;
        } else if(vy < 0) {
            // Move down
            return dtop;
        } else {
            // Cannot not move anyway
            return std::numeric_limits<float>::infinity();
        }
    }

    throw std::runtime_error("Critical error occurred (info : control flow cannot reach here)");
}
