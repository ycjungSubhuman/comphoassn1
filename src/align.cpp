#include <cassert>
#include <opencv2/core/matx.hpp>

#include "align.hpp"

namespace compho {
std::vector<cv::Vec2i>
PairwiseAlignAlgorithm::align(const std::vector<cv::Mat1f> &images) {
    assert(images.size() >= 2);

    std::vector<cv::Vec2i> result;

    result.push_back(cv::Vec2i(0.0f, 0.0f)); // zero vector for the reference
    for (size_t i = 1; i < images.size(); i++) {
        result.emplace_back(this->align_pair(images[0], images[i]));
    }

    return result;
}
} // namespace compho
