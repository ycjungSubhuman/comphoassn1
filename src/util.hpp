#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace compho {
cv::Mat1f normalize_image(const cv::Mat &img);

std::vector<cv::Mat1f> split_single_column_image(const cv::Mat1f &img);

std::vector<cv::Mat1f> crop_images(const std::vector<cv::Mat1f> &imgs,
                                   int padding);

void write_displacement_file(const std::string &filename,
                             const std::vector<cv::Vec2i> &disps);
void read_displacement_file(const std::string &filename,
                            std::vector<cv::Vec2i> &disps);

void write_merged_image(const std::string &filename,
                        const std::vector<cv::Mat1f> &imgs,
                        const std::vector<cv::Vec2i> &disps);
} // namespace compho
