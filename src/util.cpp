#include <algorithm>
#include <fstream>

#include "util.hpp"
#include <stdexcept>

namespace compho {

cv::Mat1f normalize_image(const cv::Mat &img) {
    cv::Mat1f result;

    if (img.type() == CV_8UC1) {
        img.convertTo(result, CV_32F, 1. / 255.);
    } else {
        img.convertTo(result, CV_32F, 1. / 65535.);
    }

    return result;
}

std::vector<cv::Mat1f> split_single_column_image(const cv::Mat1f &img) {
    int height = img.rows / 3;

    cv::Mat1f B = img(cv::Rect(0, 0, img.cols, height));
    cv::Mat1f G = img(cv::Rect(0, height, img.cols, height));
    cv::Mat1f R = img(cv::Rect(0, height * 2, img.cols, height));

    return std::vector<cv::Mat1f>{B, G, R};
}

std::vector<cv::Mat1f> crop_images(const std::vector<cv::Mat1f> &imgs,
                                   int padding) {
    std::vector<cv::Mat1f> result;

    const int c = padding;
    for (size_t i = 0; i < imgs.size(); i++) {
        result.push_back(imgs[i](
            cv::Rect(c, c, imgs[i].cols - 2 * c, imgs[i].rows - 2 * c)));
    }
    return result;
}

void write_displacement_file(const std::string &filename,
                             const std::vector<cv::Vec2i> &disps) {
    std::ofstream of(filename);
    for (size_t i = 0; i < disps.size(); i++) {
        of << disps[i][0] << " " << disps[i][1] << std::endl;
    }
}

void read_displacement_file(const std::string &filename,
                            std::vector<cv::Vec2i> &disps) {
    std::ifstream f(filename);
    if (f.fail()) {
        throw std::runtime_error("File does not exist: " + filename);
    }

    int delta_x, delta_y;
    while (f >> delta_x >> delta_y) {
        disps.emplace_back(cv::Vec2i(delta_x, delta_y));
    }
}

void write_merged_image(const std::string &filename,
                        const std::vector<cv::Mat1f> &imgs,
                        const std::vector<cv::Vec2i> &disps) {
    cv::Mat1f B = imgs[0];
    cv::Mat1f G = imgs[1];
    cv::Mat1f R = imgs[2];
    cv::Vec2i aG = disps[1];
    cv::Vec2i aR = disps[2];

    int crop_x = std::max(std::abs(aG[0]), std::abs(aR[0]));
    int crop_y = std::max(std::abs(aG[1]), std::abs(aR[1]));
    B = B(cv::Rect(crop_x, crop_y, B.cols - 2 * crop_x, B.rows - 2 * crop_y));
    G = G(cv::Rect(crop_x - aG[0], crop_y - aG[1], G.cols - 2 * crop_x,
                   G.rows - 2 * crop_y));
    R = R(cv::Rect(crop_x - aR[0], crop_y - aR[1], R.cols - 2 * crop_x,
                   R.rows - 2 * crop_y));
    cv::Mat1f BGR[] = {B, G, R};
    cv::Mat3f colorim;
    cv::merge(BGR, 3, colorim);

    cv::Mat tmp;
    if (B.type() == CV_8UC1) {
        colorim.convertTo(tmp, CV_8U, 255.);
    } else {
        colorim.convertTo(tmp, CV_16U, 65535.);
    }
    cv::imwrite(filename, tmp);
}
} // namespace compho
