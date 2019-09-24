#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

namespace compho {
/**
 * Image pyramid for a single-channel image.
 *
 * Level 0 is the largest image
 */
class Pyramid {
  public:
    /**
     * @param image           original image
     * @param num_level       number of levels in this pyramid
     * @param factor_scale    scale multiplier for each adjacent pairs of levels
     */
    Pyramid(cv::Mat1f image, size_t num_level = 4, size_t factor_scale = 2);

    /**
     * Get an image at level 'level'
     */
    cv::Mat1f image(size_t level) const;

    /**
     * Scale down image-space value 'value' 'delta_level' times
     */
    template <typename T> T scale_down_value(size_t delta_level, T value) const;

    /**
     * Scale up image-space value 'value' 'delta_level' times
     */
    template <typename T> T scale_up_value(size_t delta_level, T value) const;

    /**
     * Get the total number of levels in this pyramid
     */
    size_t levels() const;

  private:
    std::vector<cv::Mat1f> m_imgs;
    size_t m_num_level;
    size_t m_factor_scale;
};
} // namespace compho

#include "pyramid_impl.hpp"
