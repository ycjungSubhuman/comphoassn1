#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace compho {
/**
 * Normalize image elementsinto 0~1
 */
cv::Mat1f normalize_image(const cv::Mat &img);

/**
 * Split vertically-stacked image into three pieces
 */
std::vector<cv::Mat1f> split_single_column_image(const cv::Mat1f &img);

/**
 * Crop center with padding
 */
cv::Mat1f crop_image(const cv::Mat1f &img, int padding);
std::vector<cv::Mat1f> crop_images(const std::vector<cv::Mat1f> &imgs,
                                   int padding);

/**
 * Get overlapping portion of fixed and moving images given global translation
 */
void overlap_translated(int delta_x, int delta_y, const cv::Mat1f &fixed,
                        const cv::Mat1f &moving, cv::Mat1f &fixed_overlapped,
                        cv::Mat1f &moving_overlapped);

/**
 * Write displace vectors into a file
 */
void write_displacement_file(const std::string &filename,
                             const std::vector<cv::Vec2i> &disps);
/**
 * Write displace vectors from a file
 */
void read_displacement_file(const std::string &filename,
                            std::vector<cv::Vec2i> &disps);
/**
 * Write RGB image from a list of single-channel images and displacement vectors
 * for each image
 */
void write_merged_image(const std::string &filename,
                        const std::vector<cv::Mat1f> &imgs,
                        const std::vector<cv::Vec2i> &disps);
} // namespace compho
