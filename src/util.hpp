#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace compho {
std::vector<cv::Mat> split_single_column_image(const cv::Mat &img, int split);
void write_displacement_file(const std::string &filename,
                             const std::vector<cv::Vec2i> &images);
} // namespace compho
