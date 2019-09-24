#include <cassert>
#include <iostream>

#include "multi_scale.hpp"
#include "util.hpp"

namespace compho {
MultiScalePairAlignAlgorithm::MultiScalePairAlignAlgorithm(
    cv::Vec2i pert, std::shared_ptr<MatchingScore> fn_score, size_t num_level,
    size_t factor_scale)
    : m_pert(std::move(pert)), m_fn_score(std::move(fn_score)),
      m_num_level(num_level), m_factor_scale(factor_scale) {}

cv::Vec2i MultiScalePairAlignAlgorithm::align_pair(const cv::Mat1f &fixed,
                                                   const cv::Mat1f &moving) {
    Pyramid pyr_fixed(fixed, m_num_level, m_factor_scale);
    Pyramid pyr_moving(moving, m_num_level, m_factor_scale);

    assert(m_num_level == pyr_fixed.levels() &&
           m_num_level == pyr_moving.levels());

    cv::Vec2i offset(0, 0); // displacement offset to be used in the next level
    for (size_t i = 0; i < m_num_level; i++) {
        offset = pyr_fixed.scale_up_value(1, offset);
        cv::Vec2i disp_i =
            SingleScalePairAlignAlgorithm(m_pert, m_fn_score, offset)
                .align_pair(pyr_fixed.image(m_num_level - i - 1),
                            pyr_moving.image(m_num_level - i - 1));
        offset += disp_i;
    }

    return offset;
}
} // namespace compho
