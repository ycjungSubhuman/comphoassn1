#include <cmath>

#include "pyramid.hpp"

namespace compho {
Pyramid::Pyramid(cv::Mat1f image, size_t num_level, size_t factor_scale)
    : m_num_level(num_level), m_factor_scale(factor_scale) {

    m_imgs.emplace_back(image.clone());
    for (size_t i = 1; i < num_level; i++) {
        cv::Mat1f img_down;
        cv::pyrDown(m_imgs[i - 1], img_down,
                    cv::Size2i(m_imgs[i - 1].size().width / factor_scale,
                               m_imgs[i - 1].size().height / factor_scale));
        m_imgs.emplace_back(img_down);
    }
}

cv::Mat1f Pyramid::image(size_t level) const { return m_imgs.at(level); }

size_t Pyramid::levels() const { return m_num_level; }
} // namespace compho
